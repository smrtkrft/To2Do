#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include "Persistence_Manager.h"

class WiFiManager {
private:
    PersistenceManager* persistence;
    
    // AP Mode configuration (loaded from persistence)
    String apSSID = "SmartKraft-To2Do";
    String apMDNS = "to2do";
    
    // Network credentials (loaded from persistence)
    String primarySSID = "";
    String primaryPassword = "";
    String primaryIP = "";
    String primaryMDNS = "smartkraft-todo";
    
    String backupSSID = "";
    String backupPassword = "";
    String backupIP = "";
    String backupMDNS = "smartkraft-todo-backup";
    
    // State flags
    bool isAPMode = false;
    bool isConnected = false;
    bool tryingToConnect = false;
    
    // Timing
    unsigned long apModeStartTime = 0;
    unsigned long lastScanTime = 0;
    unsigned long connectionStartTime = 0;
    unsigned long lastConnectionCheck = 0;
    int connectionFailCount = 0;  // Count consecutive failures
    
    // Constants
    static constexpr unsigned long INITIAL_SCAN_WAIT = 15000;        // 15 seconds (changed from 30)
    static constexpr unsigned long AP_SCAN_INTERVAL_EARLY = 30000;   // 30 seconds (first 5 min)
    static constexpr unsigned long AP_SCAN_INTERVAL_NORMAL = 120000; // 2 minutes (after 5 min)
    static constexpr unsigned long AP_MODE_EARLY_PERIOD = 300000;    // 5 minutes
    static constexpr unsigned long CONNECTION_TIMEOUT = 15000;       // 15 seconds
    static constexpr unsigned long CONNECTION_CHECK_INTERVAL = 5000; // 5 seconds stability check
    
    String currentConnectingMDNS = "";
    String currentConnectingSSID = "";
    
    // Scan for saved networks
    bool scanForSavedNetworks() {
        Serial.println("[WiFi] Scanning networks...");
        int n = WiFi.scanNetworks();
        
        if (n == 0) {
            Serial.println("[WiFi] No networks found");
            return false;
        }
        
        Serial.printf("[WiFi] Found %d networks\n", n);
        
        bool primaryFound = false;
        bool backupFound = false;
        
        for (int i = 0; i < n; i++) {
            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            
            if (!primarySSID.isEmpty() && ssid == primarySSID) {
                primaryFound = true;
                Serial.printf("[WiFi] ✓ Primary found: %s (RSSI: %d)\n", ssid.c_str(), rssi);
            }
            
            if (!backupSSID.isEmpty() && ssid == backupSSID) {
                backupFound = true;
                Serial.printf("[WiFi] ✓ Backup found: %s (RSSI: %d)\n", ssid.c_str(), rssi);
            }
        }
        
        WiFi.scanDelete();
        return primaryFound || backupFound;
    }
    
    // Try to connect to WiFi
    void tryConnect(const String& ssid, const String& password, const String& ip, const String& mdns) {
        if (ssid.isEmpty()) return;
        
        Serial.printf("[WiFi] Connecting to: %s\n", ssid.c_str());
        
        // Disable AP mode if active
        if (isAPMode) {
            Serial.println("[WiFi] Disabling AP mode...");
            WiFi.softAPdisconnect(true);
            isAPMode = false;
        }
        
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);
        delay(100);
        
        // Configure static IP if provided
        if (!ip.isEmpty() && ip != "0.0.0.0") {
            IPAddress staticIP, gateway, subnet;
            if (staticIP.fromString(ip)) {
                gateway.fromString(ip);
                gateway[3] = 1;
                subnet.fromString("255.255.255.0");
                
                if (WiFi.config(staticIP, gateway, subnet)) {
                    Serial.printf("[WiFi] Static IP configured: %s\n", ip.c_str());
                }
            }
        } else {
            WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
        }
        
        WiFi.begin(ssid.c_str(), password.c_str());
        
        tryingToConnect = true;
        connectionStartTime = millis();
        currentConnectingSSID = ssid;
        currentConnectingMDNS = mdns;
    }
    
    // Switch to AP Mode (ONLY if not connected AND network not available)
    void switchToAPMode() {
        // Don't switch to AP if already connected to WiFi
        if (isConnected && WiFi.status() == WL_CONNECTED) {
            Serial.println("[WiFi] Already connected to WiFi, AP mode not needed");
            return;
        }
        
        // CRITICAL: Do final scan before opening AP mode
        // If saved SSID is available, DO NOT open AP mode
        if (!primarySSID.isEmpty() || !backupSSID.isEmpty()) {
            Serial.println("[WiFi] Final scan before AP mode...");
            if (scanForSavedNetworks()) {
                Serial.println("[WiFi] ✓ Network available! Connecting instead of AP mode...");
                startConnectionSequence();
                return;
            }
            Serial.println("[WiFi] ✗ Network not available, proceeding with AP mode");
        }
        
        Serial.println("[WiFi] ========================================");
        Serial.println("[WiFi] SWITCHING TO AP MODE");
        Serial.println("[WiFi] ========================================");
        
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID.c_str());  // No password
        delay(100);
        
        IPAddress IP = WiFi.softAPIP();
        Serial.printf("[WiFi] AP SSID: %s\n", apSSID.c_str());
        Serial.printf("[WiFi] AP IP: %s\n", IP.toString().c_str());
        
        // Setup mDNS for AP mode
        if (MDNS.begin(apMDNS.c_str())) {
            Serial.printf("[WiFi] AP mDNS: http://%s.local\n", apMDNS.c_str());
        }
        
        isAPMode = true;
        isConnected = false;
        tryingToConnect = false;
        apModeStartTime = millis();
        lastScanTime = millis();
        
        Serial.println("[WiFi] ========================================");
    }
    
    // Start connection sequence
    void startConnectionSequence() {
        if (!primarySSID.isEmpty()) {
            tryConnect(primarySSID, primaryPassword, primaryIP, primaryMDNS);
        } else if (!backupSSID.isEmpty()) {
            tryConnect(backupSSID, backupPassword, backupIP, backupMDNS);
        } else {
            Serial.println("[WiFi] No saved credentials, switching to AP mode");
            switchToAPMode();
        }
    }
    
public:
    WiFiManager(PersistenceManager* persistenceMgr) {
        persistence = persistenceMgr;
    }
    
    void begin() {
        Serial.println("[WiFi] ========================================");
        Serial.println("[WiFi] SMART WIFI MANAGER STARTING");
        Serial.println("[WiFi] ========================================");
        
        // Load network settings from persistence
        loadNetworkSettings();
        
        WiFi.setAutoReconnect(false);
        WiFi.persistent(false);
        
        // Check if we have any saved credentials
        if (primarySSID.isEmpty() && backupSSID.isEmpty()) {
            Serial.println("[WiFi] No saved credentials found");
            switchToAPMode();
            return;
        }
        
        Serial.println("[WiFi] Saved credentials found, scanning networks...");
        
        // First scan attempt
        if (scanForSavedNetworks()) {
            Serial.println("[WiFi] ✓ Saved network found on first scan, connecting...");
            startConnectionSequence();
            return;
        }
        
        // Wait 15 seconds and try again
        Serial.println("[WiFi] Saved networks not found, waiting 15 seconds...");
        delay(INITIAL_SCAN_WAIT);
        
        Serial.println("[WiFi] Second scan attempt...");
        if (scanForSavedNetworks()) {
            Serial.println("[WiFi] ✓ Saved network found on second scan, connecting...");
            startConnectionSequence();
        } else {
            Serial.println("[WiFi] ✗ Saved networks still not found, switching to AP mode");
            switchToAPMode();
        }
        
        Serial.println("[WiFi] ========================================");
    }
    
    void loop() {
        unsigned long now = millis();
        
        // === CONNECTED TO WIFI - Monitor connection health with stability check ===
        if (isConnected && WiFi.status() == WL_CONNECTED) {
            // Reset failure counter on successful connection check
            connectionFailCount = 0;
            lastConnectionCheck = now;
            // Connected and healthy - do nothing, save RAM
            // AP Mode is OFF, no scanning needed
            return;
        }
        
        // === CONNECTION LOST - Verify with stability check before recovery ===
        if (isConnected && WiFi.status() != WL_CONNECTED) {
            // Check periodically to avoid false positives
            if (now - lastConnectionCheck >= CONNECTION_CHECK_INTERVAL) {
                lastConnectionCheck = now;
                connectionFailCount++;
                
                Serial.printf("[WiFi] ⚠ Connection check failed (%d/3)\n", connectionFailCount);
                
                // Only switch to AP mode after 3 consecutive failures (15 seconds)
                if (connectionFailCount >= 3) {
                    Serial.println("[WiFi] ✗ Connection permanently lost! Attempting recovery...");
                    
                    // Try to scan and reconnect before switching to AP mode
                    if (scanForSavedNetworks()) {
                        Serial.println("[WiFi] ✓ Network still available, reconnecting...");
                        connectionFailCount = 0;
                        isConnected = false;
                        startConnectionSequence();
                    } else {
                        Serial.println("[WiFi] ✗ Network not found, switching to AP mode...");
                        connectionFailCount = 0;
                        isConnected = false;
                        switchToAPMode();
                    }
                }
            }
            return;
        }
        
        // === AP MODE LOGIC (Only when NOT connected) ===
        if (isAPMode) {
            // Determine scan interval based on how long we've been in AP mode
            unsigned long scanInterval;
            if (now - apModeStartTime < AP_MODE_EARLY_PERIOD) {
                scanInterval = AP_SCAN_INTERVAL_EARLY;  // 30 seconds for first 5 minutes
            } else {
                scanInterval = AP_SCAN_INTERVAL_NORMAL; // 2 minutes after that
            }
            
            // Periodic scan for saved networks (ONLY if we have saved credentials)
            if (!primarySSID.isEmpty() || !backupSSID.isEmpty()) {
                if (now - lastScanTime >= scanInterval) {
                    lastScanTime = now;
                    
                    Serial.println("[WiFi] [AP Mode] Periodic scan for saved networks...");
                    
                    if (scanForSavedNetworks()) {
                        Serial.println("[WiFi] [AP Mode] Saved network detected! Switching to STA mode...");
                        startConnectionSequence();
                    }
                }
            }
            return;
        }
        
        // === STA MODE LOGIC ===
        
        // Check connection attempt
        if (tryingToConnect) {
            wl_status_t status = WiFi.status();
            
            if (status == WL_CONNECTED) {
                Serial.println("[WiFi] ========================================");
                Serial.println("[WiFi] ✓ WIFI CONNECTED!");
                Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
                
                // Setup mDNS
                if (MDNS.begin(currentConnectingMDNS.c_str())) {
                    Serial.printf("[WiFi] mDNS: http://%s.local\n", currentConnectingMDNS.c_str());
                }
                
                Serial.println("[WiFi] ========================================");
                
                isAPMode = false;
                isConnected = true;
                tryingToConnect = false;
                connectionFailCount = 0;  // Reset failure counter
                lastConnectionCheck = now;
                return;
            }
            
            // Connection timeout
            if (now - connectionStartTime >= CONNECTION_TIMEOUT) {
                Serial.println("[WiFi] ✗ Connection timeout");
                WiFi.disconnect();
                tryingToConnect = false;
                
                // Try backup if primary failed
                if (currentConnectingSSID == primarySSID && !backupSSID.isEmpty()) {
                    Serial.println("[WiFi] Trying backup network...");
                    delay(1000);
                    tryConnect(backupSSID, backupPassword, backupIP, backupMDNS);
                } else {
                    Serial.println("[WiFi] All connection attempts failed, switching to AP mode");
                    switchToAPMode();
                }
            }
            return;
        }
    }
    
    // Apply new network settings from user
    void applyNewSettings(const String& apSSIDNew, const String& apMDNSNew,
                          const String& priSSID, const String& priPass, 
                          const String& priIP, const String& priMDNS,
                          const String& bkpSSID, const String& bkpPass,
                          const String& bkpIP, const String& bkpMDNS) {
        
        Serial.println("[WiFi] ========================================");
        Serial.println("[WiFi] APPLYING NEW NETWORK SETTINGS");
        Serial.println("[WiFi] ========================================");
        
        apSSID = apSSIDNew.isEmpty() ? "SmartKraft-To2Do" : apSSIDNew;
        apMDNS = apMDNSNew.isEmpty() ? "to2do" : apMDNSNew;
        
        primarySSID = priSSID;
        primaryPassword = priPass;
        primaryIP = priIP;
        primaryMDNS = priMDNS.isEmpty() ? "smartkraft-todo" : priMDNS;
        
        backupSSID = bkpSSID;
        backupPassword = bkpPass;
        backupIP = bkpIP;
        backupMDNS = bkpMDNS.isEmpty() ? "smartkraft-todo-backup" : bkpMDNS;
        
        Serial.printf("[WiFi] AP Mode: %s (mDNS: %s.local)\n", apSSID.c_str(), apMDNS.c_str());
        Serial.printf("[WiFi] Primary: %s\n", primarySSID.c_str());
        Serial.printf("[WiFi] Backup: %s\n", backupSSID.c_str());
        
        // Settings are already saved to persistence by To2Do.ino
        // Just attempt connection
        if (!primarySSID.isEmpty() || !backupSSID.isEmpty()) {
            Serial.println("[WiFi] New credentials received, attempting connection...");
            startConnectionSequence();
        }
        
        Serial.println("[WiFi] ========================================");
    }
    
    // Load network settings from persistence
    void loadNetworkSettings() {
        String networkJson = persistence->loadNetworkSettings();
        
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, networkJson);
        
        if (error) {
            Serial.println("[WiFi] No network settings found in persistence");
            return;
        }
        
        apSSID = doc["apSSID"] | "SmartKraft-To2Do";
        apMDNS = doc["apMDNS"] | "to2do";
        
        primarySSID = doc["primarySSID"] | "";
        primaryPassword = doc["primaryPassword"] | "";
        primaryIP = doc["primaryIP"] | "";
        primaryMDNS = doc["primaryMDNS"] | "smartkraft-todo";
        
        backupSSID = doc["backupSSID"] | "";
        backupPassword = doc["backupPassword"] | "";
        backupIP = doc["backupIP"] | "";
        backupMDNS = doc["backupMDNS"] | "smartkraft-todo-backup";
        
        Serial.println("[WiFi] Network settings loaded from persistence");
        Serial.printf("[WiFi] AP SSID: %s (mDNS: %s.local)\n", apSSID.c_str(), apMDNS.c_str());
        if (!primarySSID.isEmpty()) {
            Serial.printf("[WiFi] Primary SSID: %s\n", primarySSID.c_str());
        }
        if (!backupSSID.isEmpty()) {
            Serial.printf("[WiFi] Backup SSID: %s\n", backupSSID.c_str());
        }
    }
    
    // Get status as JSON
    String getStatusJSON() {
        String json = "{";
        json += "\"mode\":\"" + String(isAPMode ? "AP" : "STA") + "\",";
        json += "\"ssid\":\"" + String(isAPMode ? apSSID : WiFi.SSID()) + "\",";
        json += "\"ip\":\"" + (isAPMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString()) + "\",";
        json += "\"mdns\":\"" + String(isAPMode ? apMDNS : currentConnectingMDNS) + ".local\",";
        json += "\"rssi\":" + String(isAPMode ? 0 : WiFi.RSSI()) + ",";
        json += "\"connected\":" + String(isConnected ? "true" : "false") + ",";
        json += "\"apActive\":" + String(isAPMode ? "true" : "false");  // AP is OFF when WiFi connected
        json += "}";
        return json;
    }
    
    // Public getters
    bool isAP() const { return isAPMode; }
    bool isWiFiConnected() const { return isConnected; }
    
    String getIP() const {
        return isAPMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
    }
    
    String getMDNS() const {
        return String(isAPMode ? apMDNS : currentConnectingMDNS) + ".local";
    }
};

#endif
