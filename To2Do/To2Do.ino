/*
 * SmartKraft To2Do
 * ESP32-based To-Do List Manager with OLED Display
 * 
 * Features:
 * - Web-based interface for managing projects and tasks
 * - Categorized organization (Work, Personal, Projects)
 * - Import/Export functionality for data backup
 * - Email notifications via SMTP
 * - WiFi and AP mode support
 * - OLED display support (coming soon)
 * - Data security - all data stored locally on SPIFFS
 * - Offline operation capability
 */

#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// Core managers
#include "Persistence.h"
#include "WiFi_Manager.h"
#include "Backup_Manager.h"
#include "Mail_Manager.h"
#include "Notification_Manager.h"

// Web interface
#include "Web_Interface.h"
#include "Web_JavaScript_Core.h"

// Global instances
WebServer server(80);
Persistence persistence;
WiFiManager* wifiManager = nullptr;
BackupManager* backupManager = nullptr;
MailManager* mailManager = nullptr;
NotificationManager* notificationManager = nullptr;

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n=== SmartKraft-ToDo Starting ===");
  
  // Initialize persistence (SPIFFS)
  if (!persistence.begin()) {
    Serial.println("ERROR: Persistence initialization failed!");
    return;
  }
  
  // Initialize WiFi Manager
  wifiManager = new WiFiManager(&persistence);
  wifiManager->begin();
  
  // Initialize Backup Manager
  backupManager = new BackupManager(persistence.getDataManager());
  
  // Initialize Mail Manager
  mailManager = new MailManager(persistence.getDataManager());
  
  // Initialize Notification Manager
  notificationManager = new NotificationManager(persistence.getDataManager());
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
  // Setup web server
  setupServerRoutes();
  server.begin();
  
  Serial.println("\n=== SmartKraft-ToDo READY ===");
  Serial.printf("Mode: %s\n", wifiManager->isAP() ? "AP" : "WiFi Connected");
  Serial.printf("IP: %s\n", wifiManager->getIP().c_str());
  Serial.printf("Access: http://%s\n", wifiManager->getMDNS().c_str());
  Serial.println();
}

// ==================== LOOP ====================

void loop() {
  wifiManager->loop();  // WiFi management
  server.handleClient();
  delay(2);
}

// ==================== SERVER ROUTES ====================

void setupServerRoutes() {
  // Main page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });
  
  // JavaScript core
  server.on("/js/core.js", HTTP_GET, []() {
    server.send(200, "application/javascript", getJavaScriptCore());
  });
  
  // Todos API endpoints
  server.on("/api/todos", HTTP_GET, handleGetTodos);
  server.on("/api/todos", HTTP_POST, handleSaveTodos);
  
  // Settings API endpoints
  server.on("/api/settings", HTTP_GET, handleGetSettings);
  server.on("/api/settings", HTTP_POST, handleSaveSettings);
  
  // Network API endpoints
  server.on("/api/network/status", HTTP_GET, handleNetworkStatus);
  server.on("/api/network/settings", HTTP_GET, handleGetNetworkSettings);
  server.on("/api/network/config", HTTP_POST, handleNetworkConfig);
  
  // System API endpoints
  server.on("/api/system/factory-reset", HTTP_POST, handleFactoryReset);
  server.on("/api/system/info", HTTP_GET, handleSystemInfo);
  
  // Backup API endpoints
  server.on("/api/backup/export", HTTP_GET, handleBackupExport);
  server.on("/api/backup/import", HTTP_POST, handleBackupImport);
  
  // Mail API endpoints
  server.on("/api/mail/config", HTTP_GET, handleGetMailConfig);
  server.on("/api/mail/config", HTTP_POST, handleSaveMailConfig);
  
  // Notification API endpoints
  server.on("/api/notifications/today", HTTP_GET, []() {
    handleNotifications("today");
  });
  server.on("/api/notifications/tomorrow", HTTP_GET, []() {
    handleNotifications("tomorrow");
  });
  server.on("/api/notifications/week", HTTP_GET, []() {
    handleNotifications("week");
  });
  server.on("/api/notifications/overdue", HTTP_GET, []() {
    handleNotifications("overdue");
  });
  
  // Timezone setting
  server.on("/api/timezone", HTTP_POST, handleSetTimezone);
}

// ==================== TODOS API ====================

void handleGetTodos() {
  DataManager* dm = persistence.getDataManager();
  
  DynamicJsonDocument doc(24576);
  doc["projects"] = dm->getProjects();
  doc["tasks"] = dm->getTasks();
  
  // Parse as proper objects
  DynamicJsonDocument result(24576);
  result["projects"] = serialized(dm->getProjects());
  result["tasks"] = serialized(dm->getTasks());
  
  String output;
  serializeJson(result, output);
  server.send(200, "application/json", output);
}

void handleSaveTodos() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String body = server.arg("plain");
  DataManager* dm = persistence.getDataManager();
  
  bool success = dm->setTodosData(body);
  
  if (success) {
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Save failed\"}");
  }
}

// ==================== SETTINGS API ====================

void handleGetSettings() {
  DataManager* dm = persistence.getDataManager();
  String settings = dm->getGUISettings();
  server.send(200, "application/json", settings);
}

void handleSaveSettings() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String body = server.arg("plain");
  DataManager* dm = persistence.getDataManager();
  
  bool success = dm->setGUISettings(body);
  
  if (success) {
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Save failed\"}");
  }
}

// ==================== NETWORK API ====================

void handleNetworkStatus() {
  String status = wifiManager->getStatus();
  server.send(200, "application/json", status);
}

void handleGetNetworkSettings() {
  String settings = persistence.getNetworkSettings();
  server.send(200, "application/json", settings);
}

void handleNetworkConfig() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String body = server.arg("plain");
  Serial.println("[Network] Received network config:");
  Serial.println(body);
  
  // Parse JSON to extract WiFi settings
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, body);
  
  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }
  
  // Extract settings
  String apSSID = doc["apSSID"] | "SmartKraft-To2Do";
  String apMDNS = doc["apMDNS"] | "to2do";
  
  String priSSID = doc["primarySSID"] | "";
  String priPass = doc["primaryPassword"] | "";
  String priIP = doc["primaryIP"] | "";
  String priMDNS = doc["primaryMDNS"] | "";
  
  String bkpSSID = doc["backupSSID"] | "";
  String bkpPass = doc["backupPassword"] | "";
  String bkpIP = doc["backupIP"] | "";
  String bkpMDNS = doc["backupMDNS"] | "";
  
  Serial.printf("[Network] AP SSID: %s (mDNS: %s.local)\n", apSSID.c_str(), apMDNS.c_str());
  Serial.printf("[Network] Primary SSID: %s\n", priSSID.c_str());
  Serial.printf("[Network] Primary IP: %s\n", priIP.c_str());
  Serial.printf("[Network] Primary mDNS: %s\n", priMDNS.c_str());
  
  // Save network settings to persistence
  bool saved = persistence.saveNetworkSettings(body);
  Serial.printf("[Network] Data saved to SPIFFS: %s\n", saved ? "YES" : "NO");
  
  // Apply new settings to WiFi Manager
  wifiManager->applyNewSettings(apSSID, apMDNS,
                                 priSSID, priPass, priIP, priMDNS,
                                 bkpSSID, bkpPass, bkpIP, bkpMDNS);
  
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Settings saved and connecting...\"}");
}

// ==================== SYSTEM API ====================

void handleFactoryReset() {
  Serial.println("[System] Factory reset requested");
  persistence.factoryReset();
  server.send(200, "application/json", "{\"success\":true}");
  delay(1000);
  ESP.restart();
}

void handleSystemInfo() {
  DynamicJsonDocument doc(1024);
  
  doc["version"] = "SmartKraft-ToDo V1.1";
  doc["chipModel"] = ESP.getChipModel();
  doc["chipCores"] = ESP.getChipCores();
  doc["cpuFreq"] = ESP.getCpuFreqMHz();
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["heapSize"] = ESP.getHeapSize();
  doc["flashSize"] = ESP.getFlashChipSize();
  doc["spiffsTotal"] = SPIFFS.totalBytes();
  doc["spiffsUsed"] = SPIFFS.usedBytes();
  doc["spiffsFree"] = SPIFFS.totalBytes() - SPIFFS.usedBytes();
  doc["uptime"] = millis() / 1000;
  
  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void handleBackupExport() {
  if (!backupManager) {
    server.send(500, "application/json", "{\"error\":\"Backup manager not ready\"}");
    return;
  }
  
  String data = backupManager->exportData();
  server.send(200, "application/json", data);
}

void handleBackupImport() {
  if (!backupManager) {
    server.send(500, "application/json", "{\"error\":\"Backup manager not ready\"}");
    return;
  }
  
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String data = server.arg("plain");
  
  if (backupManager->importData(data)) {
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Import failed\"}");
  }
}

void handleGetMailConfig() {
  if (!mailManager) {
    server.send(500, "application/json", "{\"error\":\"Mail manager not ready\"}");
    return;
  }
  
  String configData = mailManager->getConfigJSON();
  server.send(200, "application/json", configData);
}

void handleSaveMailConfig() {
  if (!mailManager) {
    server.send(500, "application/json", "{\"error\":\"Mail manager not ready\"}");
    return;
  }
  
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String configData = server.arg("plain");
  
  if (mailManager->saveConfig(configData)) {
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Save failed\"}");
  }
}

void handleNotifications(String filterType) {
  if (!notificationManager) {
    server.send(500, "application/json", "{\"error\":\"Notification manager not ready\"}");
    return;
  }
  
  String data = notificationManager->getNotifications(filterType);
  server.send(200, "application/json", data);
}

void handleSetTimezone() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, server.arg("plain"));
  
  String timezone = doc["timezone"] | "UTC";
  notificationManager->setTimezone(timezone);
  
  server.send(200, "application/json", "{\"success\":true}");
}
