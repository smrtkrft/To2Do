/*
 * ESP32-C6 TODO App - SmartKraft Edition
 * All data persists across firmware updates (stored in SPIFFS)
 * Smart WiFi Manager with AP Mode fallback
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Persistence_Manager.h"
#include "WiFi_Manager.h"
#include "Web_Interface.h"
#include "Web_JavaScript.h"
#include "Backup_Manager.h"
#include "Mail_Manager.h"
#include "Time_Manager.h"
#include "Notification_Manager.h"

WebServer server(80);
PersistenceManager persistence;
WiFiManager* wifiManager;
BackupManager* backupManager;
MailManager* mailManager;
NotificationManager* notificationManager;
TimeManager* timeManager;

const size_t JSON_BUFFER_SIZE = 16384;

// No more time sync tracking - browser provides all time data



void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n========================================");
  Serial.println("SmartKraft To2Do Starting...");
  Serial.println("========================================");
  unsigned long setupStart = millis();
  
  // Pre-initialize WiFi radio immediately - this "warms up" the radio
  // so it's ready when WiFiManager calls begin()
  Serial.println("[Setup] Pre-initializing WiFi radio...");
  WiFi.mode(WIFI_STA);  // Wake up WiFi radio early
  WiFi.disconnect();     // Ensure clean state
  delay(200);            // Let radio stabilize during SPIFFS init
  Serial.println("[Setup] WiFi radio pre-initialized and ready");
  
  if (!persistence.begin()) {
    Serial.println("[ERROR] Persistence init failed!");
    return;
  }
  
  Serial.println("[Setup] Starting WiFi Manager...");
  wifiManager = new WiFiManager(&persistence);
  wifiManager->begin();
  
  backupManager = new BackupManager(persistence.getDataManager());
  mailManager = new MailManager(persistence.getDataManager());
  notificationManager = new NotificationManager(persistence.getDataManager());
  timeManager = new TimeManager();
  
  notificationManager->setTimeManager(timeManager);
  
  // Load existing time from SPIFFS first
  if (timeManager->loadDateFromSPIFFS()) {
    if (timeManager->isDateValid()) {
      Serial.printf("[Setup] Loaded valid time from SPIFFS: %s %s\n", 
                   timeManager->getFormattedDate().c_str(), 
                   timeManager->getFormattedTime().c_str());
    }
  }
  
  // If no valid time, set a fallback time
  if (!timeManager->isDateValid()) {
    Serial.println("[Setup] No valid time found, setting fallback time");
    timeManager->setManualDate(2025, 10, 20, 12, 0); // Fallback time
  }
  
  // Time is managed by browser sync - no NTP sync needed
  Serial.println("[Setup] Time will be synced from browser");
  
  setupServerRoutes();
  server.begin();
  
  Serial.println("========================================");
  Serial.printf("[Setup] ✓ Setup completed in %lums\n", millis() - setupStart);
  Serial.println("[Setup] Web server ready!");
  Serial.println("========================================\n");
}

void loop() {
  wifiManager->loop();
  server.handleClient();
  
  // No more internet time sync - browser provides time via /api/time endpoint
  
  delay(2); // Small delay for stability
}

void setupServerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/app.js", HTTP_GET, []() {
    server.send(200, "application/javascript", getJavaScriptCombined());
  });
  server.on("/api/todos", HTTP_GET, handleGetTodos);
  server.on("/api/todos", HTTP_POST, handleCreateTodo);
  server.on("/api/todos", HTTP_PUT, handleUpdateTodo);
  server.on("/api/todos", HTTP_DELETE, handleDeleteTodo);
  
  // Settings API endpoints
  server.on("/api/settings", HTTP_GET, handleGetSettings);
  server.on("/api/settings", HTTP_POST, handleSaveSettings);
  
  // Network API endpoints
  server.on("/api/network/status", HTTP_GET, handleNetworkStatus);
  server.on("/api/network/settings", HTTP_GET, handleGetNetworkSettings);
  server.on("/api/network/config", HTTP_POST, handleNetworkConfig);
  
  // System API endpoints
  server.on("/api/factory-reset", HTTP_POST, handleFactoryReset);
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
  server.on("/api/notifications/timezone", HTTP_POST, handleSetTimezone);
  
  // Time API endpoints
  server.on("/api/time", HTTP_GET, handleGetTime);
  server.on("/api/time", HTTP_POST, handleSetManualTime);
  server.on("/api/time/sync", HTTP_POST, handleTimeSyncNow);
  
  server.on("/api/health", HTTP_GET, []() {
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "404");
  });
}

void handleRoot() {
  Serial.println("[Root] Root page requested");
  Serial.println("[Root] Serving main page");
  server.send(200, "text/html", getHTML());
}

void handleGetTodos() {
  Serial.println("[API] GET /api/todos - Loading todos...");
  String content = persistence.loadTodos();
  Serial.printf("[API] Sending %d bytes\n", content.length());
  server.send(200, "application/json", content);
}

void handleCreateTodo() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String body = server.arg("plain");
  
  Serial.println("[Todos] Received save request");
  Serial.printf("[Todos] Data size: %d bytes\n", body.length());
  
  if (persistence.saveTodos(body)) {
    Serial.println("[Todos] ✓ Saved successfully to SPIFFS");
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    Serial.println("[Todos] ✗ Save failed!");
    server.send(500, "application/json", "{\"error\":\"Write failed\"}");
  }
}

void handleUpdateTodo() {
  server.send(200, "application/json", "{\"success\":true}");
}

void handleDeleteTodo() {
  server.send(200, "application/json", "{\"success\":true}");
}

// ==================== NETWORK API ====================

void handleNetworkStatus() {
  String json = wifiManager->getStatusJSON();
  server.send(200, "application/json", json);
}

void handleGetNetworkSettings() {
  String networkSettings = persistence.loadNetworkSettings();
  server.send(200, "application/json", networkSettings);
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

// ==================== SETTINGS API ====================

void handleGetSettings() {
  String settings = persistence.loadSettings();
  server.send(200, "application/json", settings);
}

void handleSaveSettings() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String body = server.arg("plain");
  
  if (persistence.saveSettings(body)) {
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Settings saved\"}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Failed to save settings\"}");
  }
}

// ==================== SYSTEM API ====================

void handleFactoryReset() {
  Serial.println("\n!!! FACTORY RESET REQUESTED !!!");
  
  // Reset all user data (projects, tasks, settings)
  if (persistence.factoryReset()) {
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Factory reset completed. Restarting...\"}");
    delay(1000);
    ESP.restart();
  } else {
    server.send(500, "application/json", "{\"error\":\"Factory reset failed\"}");
  }
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
  
  String backupData = backupManager->exportBackup();
  server.sendHeader("Content-Disposition", "attachment; filename=backup.json");
  server.send(200, "application/json", backupData);
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
  
  String backupData = server.arg("plain");
  
  if (backupManager->importBackup(backupData)) {
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
  
  Serial.printf("[API] GET /api/notifications/%s\n", filterType.c_str());
  
  String notifications = notificationManager->getNotifications(filterType);
  
  Serial.printf("[API] Notification response length: %d bytes\n", notifications.length());
  
  server.send(200, "application/json", notifications);
}

void handleSetTimezone() {
  if (!notificationManager) {
    server.send(500, "application/json", "{\"error\":\"Notification manager not ready\"}");
    return;
  }
  
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, server.arg("plain"));
  
  int offset = doc["offset"] | 0;
  notificationManager->setTimezoneOffset(offset);
  
  server.send(200, "application/json", "{\"success\":true}");
}

// Time API handlers
void handleGetTime() {
  if (!timeManager) {
    server.send(500, "application/json", "{\"error\":\"Time manager not ready\"}");
    return;
  }
  
  DynamicJsonDocument doc(512);
  doc["date"] = timeManager->getFormattedDate();
  doc["time"] = timeManager->getFormattedTime();
  doc["lastSync"] = timeManager->getLastSyncTime();
  doc["isValid"] = timeManager->isDateValid();
  doc["wifiConnected"] = (WiFi.status() == WL_CONNECTED);
  
  String response;
  serializeJson(doc, response);
  
  Serial.printf("[API] GET /api/time - Date: %s, Time: %s, Valid: %s\n", 
                doc["date"].as<String>().c_str(), 
                doc["time"].as<String>().c_str(),
                doc["isValid"].as<bool>() ? "true" : "false");
  
  server.send(200, "application/json", response);
}

void handleSetManualTime() {
  if (!timeManager) {
    server.send(500, "application/json", "{\"error\":\"Time manager not ready\"}");
    return;
  }
  
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }
  
  String requestBody = server.arg("plain");
  Serial.printf("[API] POST /api/time - Request body: %s\n", requestBody.c_str());
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, requestBody);
  
  int year = doc["year"] | 0;
  int month = doc["month"] | 0;
  int day = doc["day"] | 0;
  int hour = doc["hour"] | 0;
  int minute = doc["minute"] | 0;
  
  Serial.printf("[API] Browser date received: %04d-%02d-%02d %02d:%02d\n", 
                year, month, day, hour, minute);
  
  if (year < 2024 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
    Serial.printf("[API] INVALID date/time values: %04d-%02d-%02d %02d:%02d\n",
                 year, month, day, hour, minute);
    server.send(400, "application/json", "{\"error\":\"Invalid date\"}");
    return;
  }
  
  Serial.println("[API] Calling timeManager->setManualDate()...");
  timeManager->setManualDate(year, month, day, hour, minute);
  Serial.println("[API] Date set and saved to SPIFFS successfully");
  
  DynamicJsonDocument response(256);
  response["success"] = true;
  response["date"] = timeManager->getFormattedDate();
  response["time"] = timeManager->getFormattedTime();
  response["saved"] = true;
  
  String responseStr;
  serializeJson(response, responseStr);
  Serial.printf("[API] Response: %s\n", responseStr.c_str());
  server.send(200, "application/json", responseStr);
}

void handleTimeSyncNow() {
  if (!timeManager) {
    server.send(500, "application/json", "{\"error\":\"Time manager not ready\"}");
    return;
  }
  
  // No more internet sync - just return current time from browser-synced data
  DynamicJsonDocument doc(256);
  doc["success"] = true;
  doc["date"] = timeManager->getFormattedDate();
  doc["time"] = timeManager->getFormattedTime();
  doc["source"] = "browser";
  doc["message"] = "Time is synced from browser, not from internet";
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}




