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
    
    // Constants - OPTIMIZED FOR SPEED AND STABILITY
    static constexpr unsigned long INITIAL_SCAN_WAIT = 3000;         // 3 seconds (faster retry)
    static constexpr unsigned long AP_SCAN_INTERVAL_EARLY = 30000;   // 30 seconds (first 5 min)
    static constexpr unsigned long AP_SCAN_INTERVAL_NORMAL = 120000; // 2 minutes (after 5 min)
    static constexpr unsigned long AP_MODE_EARLY_PERIOD = 300000;    // 5 minutes
    static constexpr unsigned long CONNECTION_TIMEOUT = 6000;        // 6 seconds (fast fail for quick retry)
    static constexpr unsigned long CONNECTION_CHECK_INTERVAL = 5000; // 5 seconds stability check
    static constexpr unsigned long RADIO_STABILIZATION_DELAY = 500;  // 500ms for WiFi radio to stabilize
    static constexpr unsigned long QUICK_CONNECTION_CHECK = 3000;    // 3 seconds initial check in begin()
    
    String currentConnectingMDNS = "";
    String currentConnectingSSID = "";
    
    // Scan for saved networks and return which one to connect - OPTIMIZED
    int scanForBestNetwork() {
        Serial.println("[WiFi] Scanning for networks...");
        unsigned long scanStart = millis();
        
        // Fast scan since radio is pre-warmed
        WiFi.scanNetworks(false, false, false, 120); // async=false, show_hidden=false, passive=false, max_ms=120
        int n = WiFi.scanComplete();
        
        if (n == WIFI_SCAN_RUNNING) {
            // Wait for scan to complete (shouldn't happen with sync scan)
            while ((n = WiFi.scanComplete()) == WIFI_SCAN_RUNNING) {
                delay(10);
            }
        }
        
        Serial.printf("[WiFi] Scan completed in %lums, found %d networks\n", 
                     millis() - scanStart, n);
        
        if (n == 0) {
            return 0; // No networks found
        }
        
        bool primaryFound = false;
        bool backupFound = false;
        int primaryRSSI = -100;
        int backupRSSI = -100;
        
        for (int i = 0; i < n; i++) {
            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            
            if (!primarySSID.isEmpty() && ssid == primarySSID) {
                primaryFound = true;
                primaryRSSI = rssi;
                Serial.printf("[WiFi] Primary network '%s' found (RSSI: %d)\n", 
                             ssid.c_str(), rssi);
            }
            
            if (!backupSSID.isEmpty() && ssid == backupSSID) {
                backupFound = true;
                backupRSSI = rssi;
                Serial.printf("[WiFi] Backup network '%s' found (RSSI: %d)\n", 
                             ssid.c_str(), rssi);
            }
        }
        
        WiFi.scanDelete();
        
        // Return priority: Primary (1) > Backup (2) > None (0)
        if (primaryFound) {
            Serial.printf("[WiFi] Selected: PRIMARY (signal: %d dBm)\n", primaryRSSI);
            return 1;
        }
        if (backupFound) {
            Serial.printf("[WiFi] Selected: BACKUP (signal: %d dBm)\n", backupRSSI);
            return 2;
        }
        
        Serial.println("[WiFi] No saved networks found");
        return 0;
    }
    
    // Try to connect to WiFi - OPTIMIZED with pre-warmed radio
    void tryConnect(const String& ssid, const String& password, const String& ip, const String& mdns) {
        if (ssid.isEmpty()) return;
        
        Serial.printf("[WiFi] Connecting to: %s\n", ssid.c_str());
        unsigned long startTime = millis();
        
        if (isAPMode) {
            WiFi.softAPdisconnect(true);
            isAPMode = false;
            delay(50);
            WiFi.mode(WIFI_STA);  // Switch back to STA mode
            delay(100);
        }
        
        // Radio is already pre-initialized in setup(), no need for OFF/ON cycle
        // Just ensure we're in STA mode (minimal delay)
        if (WiFi.getMode() != WIFI_STA) {
            WiFi.mode(WIFI_STA);
            delay(50); // Quick mode verification
        }
        
        if (!ip.isEmpty() && ip != "0.0.0.0") {
            IPAddress staticIP, gateway, subnet, dns;
            if (staticIP.fromString(ip)) {
                gateway.fromString(ip);
                gateway[3] = 1; // x.x.x.1
                subnet.fromString("255.255.255.0");
                dns = gateway; // Use gateway as DNS
                
                if (!WiFi.config(staticIP, gateway, subnet, dns)) {
                    Serial.println("[WiFi] ⚠ Static IP config failed, trying DHCP");
                    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
                } else {
                    Serial.printf("[WiFi] Using static IP: %s (GW: %s)\n", 
                                 ip.c_str(), gateway.toString().c_str());
                }
            }
        } else {
            WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
            Serial.println("[WiFi] Using DHCP");
        }
        
        WiFi.begin(ssid.c_str(), password.c_str());
        
        // Radio already warmed up, just brief stabilization for DHCP/config
        Serial.println("[WiFi] Waiting for connection establishment...");
        delay(RADIO_STABILIZATION_DELAY); // 500ms for DHCP to start
        
        tryingToConnect = true;
        connectionStartTime = millis();
        currentConnectingSSID = ssid;
        currentConnectingMDNS = mdns;
        
        Serial.printf("[WiFi] Connection init took %lums\n", millis() - startTime);
    }
    
    // Switch to AP Mode (ONLY if not connected AND network not available)
    void switchToAPMode() {
        if (isConnected && WiFi.status() == WL_CONNECTED) {
            return;
        }
        
        Serial.println("[WiFi] Switching to AP Mode...");
        
        // Skip re-scanning if we just came from begin() - we already scanned twice
        // Only scan when called from loop() connection loss scenarios
        if (!tryingToConnect && (!primarySSID.isEmpty() || !backupSSID.isEmpty())) {
            Serial.println("[WiFi] Quick check for available networks before AP Mode...");
            int availableNetwork = scanForBestNetwork();
            if (availableNetwork > 0) {
                Serial.println("[WiFi] Network found, connecting instead of AP Mode");
                startConnectionSequence(availableNetwork);
                return;
            }
        }
        
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID.c_str());
        delay(100);
        
        IPAddress IP = WiFi.softAPIP();
        
        isAPMode = true;
        isConnected = false;
        tryingToConnect = false;
        apModeStartTime = millis();
        lastScanTime = millis();
        
        Serial.println("[WiFi] ✓ AP Mode started");
        Serial.printf("[WiFi] SSID: %s (Open - No password)\n", apSSID.c_str());
        Serial.printf("[WiFi] IP: %s\n", IP.toString().c_str());
        
        if (MDNS.begin(apMDNS.c_str())) {
            Serial.printf("[WiFi] mDNS: http://%s.local\n", apMDNS.c_str());
        }
        
        Serial.println("[WiFi] Ready!");
    }
    
    // Start connection sequence based on available network
    void startConnectionSequence(int networkType = 0) {
        if (networkType == 1 && !primarySSID.isEmpty()) {
            // Primary network available
            tryConnect(primarySSID, primaryPassword, primaryIP, primaryMDNS);
        } else if (networkType == 2 && !backupSSID.isEmpty()) {
            // Backup network available
            tryConnect(backupSSID, backupPassword, backupIP, backupMDNS);
        } else if (networkType == 0) {
            // No specific network, try primary first then backup
            if (!primarySSID.isEmpty()) {
                tryConnect(primarySSID, primaryPassword, primaryIP, primaryMDNS);
            } else if (!backupSSID.isEmpty()) {
                tryConnect(backupSSID, backupPassword, backupIP, backupMDNS);
            } else {
                switchToAPMode();
            }
        } else {
            switchToAPMode();
        }
    }
    
public:
    WiFiManager(PersistenceManager* persistenceMgr) {
        persistence = persistenceMgr;
    }
    
    void begin() {
        Serial.println("[WiFi] WiFiManager starting...");
        unsigned long beginStart = millis();
        
        loadNetworkSettings();
        
        // Radio should already be pre-initialized in setup()
        Serial.printf("[WiFi] Radio status: mode=%d, status=%d\n", WiFi.getMode(), WiFi.status());
        
        WiFi.setAutoReconnect(false);
        WiFi.persistent(false);
        
        // Check if we have any saved networks
        if (primarySSID.isEmpty() && backupSSID.isEmpty()) {
            Serial.println("[WiFi] No saved networks, starting AP Mode");
            switchToAPMode();
            Serial.printf("[WiFi] Startup completed in %lums\n", millis() - beginStart);
            return;
        }
        
        Serial.printf("[WiFi] Saved networks - Primary: '%s', Backup: '%s'\n", 
                     primarySSID.c_str(), backupSSID.c_str());
        
        // First scan - try to find saved networks
        int availableNetwork = scanForBestNetwork();
        if (availableNetwork > 0) {
            Serial.println("[WiFi] Network found on first scan, connecting...");
            startConnectionSequence(availableNetwork);
            
            // Wait briefly to see if connection succeeds quickly
            Serial.println("[WiFi] Checking initial connection status...");
            unsigned long quickCheckStart = millis();
            while (millis() - quickCheckStart < QUICK_CONNECTION_CHECK) {
                if (WiFi.status() == WL_CONNECTED) {
                    delay(300); // Quick verification
                    
                    isAPMode = false;
                    isConnected = true;
                    tryingToConnect = false;
                    connectionFailCount = 0;
                    lastConnectionCheck = millis();
                    
                    Serial.println("[WiFi] ✓ Connected successfully during setup!");
                    Serial.printf("[WiFi] Connection took %lums\n", millis() - connectionStartTime);
                    Serial.printf("[WiFi] SSID: %s\n", WiFi.SSID().c_str());
                    Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
                    Serial.printf("[WiFi] Signal: %d dBm\n", WiFi.RSSI());
                    
                    if (MDNS.begin(currentConnectingMDNS.c_str())) {
                        Serial.printf("[WiFi] mDNS: http://%s.local\n", currentConnectingMDNS.c_str());
                    }
                    
                    Serial.println("[WiFi] Ready!");
                    Serial.printf("[WiFi] Startup completed in %lums\n", millis() - beginStart);
                    return;
                }
                delay(100); // Check every 100ms
            }
            
            // Connection didn't succeed in quick check time
            // loop() will handle timeout and retry
            Serial.printf("[WiFi] Initial connection pending, will complete in background (max %lums)\n", CONNECTION_TIMEOUT);
            Serial.printf("[WiFi] Startup completed in %lums\n", millis() - beginStart);
            return;
        }
        
        // Network not found, wait briefly and retry once
        Serial.printf("[WiFi] Network not found, waiting %lums before retry...\n", INITIAL_SCAN_WAIT);
        delay(INITIAL_SCAN_WAIT); // 3 seconds - quick retry
        
        availableNetwork = scanForBestNetwork();
        if (availableNetwork > 0) {
            Serial.println("[WiFi] Network found on second scan, connecting...");
            startConnectionSequence(availableNetwork);
            
            // Wait briefly for second attempt too
            Serial.println("[WiFi] Checking connection status (second attempt)...");
            unsigned long quickCheckStart = millis(); // Define variable here
            while (millis() - quickCheckStart < QUICK_CONNECTION_CHECK) {
                if (WiFi.status() == WL_CONNECTED) {
                    delay(300);
                    
                    isAPMode = false;
                    isConnected = true;
                    tryingToConnect = false;
                    connectionFailCount = 0;
                    lastConnectionCheck = millis();
                    
                    Serial.println("[WiFi] ✓ Connected successfully on retry!");
                    Serial.printf("[WiFi] Connection took %lums\n", millis() - connectionStartTime);
                    Serial.printf("[WiFi] SSID: %s\n", WiFi.SSID().c_str());
                    Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
                    Serial.printf("[WiFi] Signal: %d dBm\n", WiFi.RSSI());
                    
                    if (MDNS.begin(currentConnectingMDNS.c_str())) {
                        Serial.printf("[WiFi] mDNS: http://%s.local\n", currentConnectingMDNS.c_str());
                    }
                    
                    Serial.println("[WiFi] Ready!");
                    Serial.printf("[WiFi] Startup completed in %lums\n", millis() - beginStart);
                    return;
                }
                delay(100);
            }
            
            Serial.printf("[WiFi] Connection pending, will complete in background\n");
        } else {
            Serial.println("[WiFi] Network still not available, starting AP Mode");
            switchToAPMode();
        }
        
        Serial.printf("[WiFi] Startup completed in %lums\n", millis() - beginStart);
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
            if (now - lastConnectionCheck >= CONNECTION_CHECK_INTERVAL) {
                lastConnectionCheck = now;
                connectionFailCount++;
                
                Serial.printf("[WiFi] Connection check failed (%d/3)\n", connectionFailCount);
                
                if (connectionFailCount >= 3) {
                    Serial.println("[WiFi] Connection lost, attempting recovery...");
                    
                    int availableNetwork = scanForBestNetwork();
                    if (availableNetwork > 0) {
                        connectionFailCount = 0;
                        isConnected = false;
                        Serial.println("[WiFi] Network found, reconnecting...");
                        startConnectionSequence(availableNetwork);
                    } else {
                        connectionFailCount = 0;
                        isConnected = false;
                        Serial.println("[WiFi] Network not available, switching to AP Mode");
                        switchToAPMode();
                    }
                }
            }
            return;
        }
        
        // === AP MODE LOGIC (Only when NOT connected) ===
        if (isAPMode) {
            unsigned long scanInterval;
            if (now - apModeStartTime < AP_MODE_EARLY_PERIOD) {
                scanInterval = AP_SCAN_INTERVAL_EARLY;
            } else {
                scanInterval = AP_SCAN_INTERVAL_NORMAL;
            }
            
            if (!primarySSID.isEmpty() || !backupSSID.isEmpty()) {
                if (now - lastScanTime >= scanInterval) {
                    lastScanTime = now;
                    
                    Serial.println("[WiFi] AP Mode: Scanning for saved networks...");
                    int availableNetwork = scanForBestNetwork();
                    if (availableNetwork > 0) {
                        Serial.println("[WiFi] AP Mode: Network found, switching to STA mode");
                        startConnectionSequence(availableNetwork);
                    } else {
                        Serial.println("[WiFi] AP Mode: No saved networks found");
                    }
                }
            }
            return;
        }
        
        // === STA MODE LOGIC ===
        
        if (tryingToConnect) {
            wl_status_t status = WiFi.status();
            
            if (status == WL_CONNECTED) {
                // Quick verification - connection already stable due to radio stabilization
                delay(300); // 300ms verification delay
                
                unsigned long connectTime = millis() - connectionStartTime;
                
                isAPMode = false;
                isConnected = true;
                tryingToConnect = false;
                connectionFailCount = 0;
                lastConnectionCheck = now;
                
                Serial.println("[WiFi] ✓ Connected successfully!");
                Serial.printf("[WiFi] Connection took %lums\n", connectTime);
                Serial.printf("[WiFi] SSID: %s\n", WiFi.SSID().c_str());
                Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
                Serial.printf("[WiFi] Signal: %d dBm\n", WiFi.RSSI());
                
                if (MDNS.begin(currentConnectingMDNS.c_str())) {
                    Serial.printf("[WiFi] mDNS: http://%s.local\n", currentConnectingMDNS.c_str());
                }
                
                Serial.println("[WiFi] Ready!");
                
                return;
            }
            
            if (now - connectionStartTime >= CONNECTION_TIMEOUT) {
                unsigned long tryTime = millis() - connectionStartTime;
                Serial.printf("[WiFi] ✗ Connection timeout after %lums\n", tryTime);
                Serial.printf("[WiFi] Status: %d\n", status);
                
                WiFi.disconnect();
                tryingToConnect = false;
                switchToAPMode();
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
        
        if (!primarySSID.isEmpty() || !backupSSID.isEmpty()) {
            startConnectionSequence();
        }
    }
    
    // Load network settings from persistence
    void loadNetworkSettings() {
        String networkJson = persistence->loadNetworkSettings();
        
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, networkJson);
        
        if (error) {
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
