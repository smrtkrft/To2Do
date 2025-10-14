#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include "Persistence.h"

/*
 * WIFI MANAGER
 * Handles WiFi connection modes: AP Mode or Client Mode
 * Supports primary and backup WiFi networks
 */

class WiFiManager {
private:
    Persistence* persistence;
    
    String apSSID;
    String apMDNS;
    String currentIP;
    String currentMDNS;
    bool isAPMode;
    
public:
    WiFiManager(Persistence* p) : persistence(p), isAPMode(true) {}
    
    void begin() {
        // Load network settings
        String settings = persistence->getNetworkSettings();
        
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, settings);
        
        apSSID = doc["apSSID"] | "SmartKraft-To2Do";
        apMDNS = doc["apMDNS"] | "to2do";
        
        String priSSID = doc["primarySSID"] | "";
        String priPass = doc["primaryPassword"] | "";
        String priMDNS = doc["primaryMDNS"] | "";
        
        Serial.println("[WiFiManager] Starting...");
        
        // Try to connect to primary WiFi first
        if (priSSID.length() > 0) {
            Serial.printf("[WiFiManager] Attempting to connect to: %s\n", priSSID.c_str());
            WiFi.mode(WIFI_STA);
            WiFi.begin(priSSID.c_str(), priPass.c_str());
            
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 20) {
                delay(500);
                Serial.print(".");
                attempts++;
            }
            
            if (WiFi.status() == WL_CONNECTED) {
                isAPMode = false;
                currentIP = WiFi.localIP().toString();
                currentMDNS = priMDNS.length() > 0 ? priMDNS : apMDNS;
                
                // Start mDNS
                if (MDNS.begin(currentMDNS.c_str())) {
                    Serial.printf("\n[WiFiManager] ✓ Connected to WiFi: %s\n", priSSID.c_str());
                    Serial.printf("[WiFiManager] IP: %s\n", currentIP.c_str());
                    Serial.printf("[WiFiManager] mDNS: %s.local\n", currentMDNS.c_str());
                } else {
                    Serial.println("\n[WiFiManager] mDNS failed to start");
                }
                return;
            }
        }
        
        // Fall back to AP mode
        Serial.println("\n[WiFiManager] Starting AP Mode");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID.c_str());
        
        currentIP = WiFi.softAPIP().toString();
        currentMDNS = apMDNS;
        isAPMode = true;
        
        // Start mDNS in AP mode
        if (MDNS.begin(currentMDNS.c_str())) {
            Serial.printf("[WiFiManager] ✓ AP Started: %s\n", apSSID.c_str());
            Serial.printf("[WiFiManager] IP: %s\n", currentIP.c_str());
            Serial.printf("[WiFiManager] mDNS: %s.local\n", currentMDNS.c_str());
        } else {
            Serial.println("[WiFiManager] mDNS failed to start");
        }
    }
    
    void loop() {
        // Keep mDNS running
        MDNS.update();
    }
    
    void applyNewSettings(String newApSSID, String newApMDNS,
                          String priSSID, String priPass, String priIP, String priMDNS,
                          String bkpSSID, String bkpPass, String bkpIP, String bkpMDNS) {
        Serial.println("[WiFiManager] Applying new settings...");
        
        // Settings are already saved by caller
        // We need to restart to apply them
        Serial.println("[WiFiManager] Restarting to apply new settings...");
        delay(1000);
        ESP.restart();
    }
    
    String getIP() {
        return currentIP;
    }
    
    String getMDNS() {
        return currentMDNS + ".local";
    }
    
    bool isAP() {
        return isAPMode;
    }
    
    String getStatus() {
        DynamicJsonDocument doc(512);
        doc["mode"] = isAPMode ? "AP" : "WiFi";
        doc["ip"] = currentIP;
        doc["mdns"] = currentMDNS + ".local";
        doc["ssid"] = isAPMode ? apSSID : WiFi.SSID();
        
        String output;
        serializeJson(doc, output);
        return output;
    }
};

#endif
