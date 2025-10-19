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
#include "Persistence_Manager.h"
#include "WiFi_Manager.h"
#include "Web_Interface.h"
#include "Web_JavaScript.h"
#include "Backup_Manager.h"

WebServer server(80);
PersistenceManager persistence;
WiFiManager* wifiManager;
BackupManager* backupManager;

const size_t JSON_BUFFER_SIZE = 24576;  // Standardized: matches Data_Manager.h and Backup_Manager.h

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n=== SmartKraft-ToDo Initializing ===");
  
  // Initialize persistence (SPIFFS)
  if (!persistence.begin()) {
    Serial.println("ERROR: Persistence initialization failed!");
    return;
  }
  
  // Initialize WiFi Manager
  wifiManager = new WiFiManager(&persistence);
  wifiManager->begin();  // Non-blocking - will connect in loop()
  
  // Initialize Backup Manager
  backupManager = new BackupManager(persistence.getDataManager());
  
  // Setup web server
  setupServerRoutes();
  server.begin();
  
  Serial.println("[System] Setup phase complete, WiFi connecting in background...");
  Serial.println();
}

void loop() {
  wifiManager->loop();  // WiFi management (includes connection logic)
  server.handleClient();
  
  // Print READY message once when WiFi is stabilized
  static bool readyPrinted = false;
  
  if (!readyPrinted) {
    // Check if system is ready (WiFi connected OR AP mode started and stable)
    if (wifiManager->isWiFiConnected() || (wifiManager->isAP() && (millis() > 2000))) {
      readyPrinted = true;
      
      Serial.println("\n=== SmartKraft-ToDo READY ===");
      Serial.printf("Mode: %s\n", wifiManager->isAP() ? "AP Mode" : "WiFi Connected");
      Serial.printf("IP: %s\n", wifiManager->getIP().c_str());
      Serial.printf("Access: http://%s\n", wifiManager->getMDNS().c_str());
      Serial.println();
    }
  }
  
  delay(2);
}

void setupServerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/app.js", HTTP_GET, []() {
    server.send(200, "application/javascript", getJavaScript().c_str());
  });
  server.on("/api/todos", HTTP_GET, handleGetTodos);
  server.on("/api/todos", HTTP_POST, handleCreateTodo);
  
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
  
  server.on("/api/health", HTTP_GET, []() {
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "404");
  });
}

void handleRoot() {
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
