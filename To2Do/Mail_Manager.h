#ifndef MAIL_MANAGER_H
#define MAIL_MANAGER_H

#include <ArduinoJson.h>
#include "Data_Manager.h"

class MailManager {
private:
    DataManager* dataManager;
    
    struct MailConfig {
        bool enabled;
        String smtpServer;
        int smtpPort;
        String smtpUser;
        String smtpPassword;
        String fromEmail;
        bool useSSL;
    };
    
    MailConfig config;
    
public:
    MailManager(DataManager* dm) : dataManager(dm) {
        config.enabled = false;
        config.smtpPort = 587;
        config.useSSL = true;
        loadConfig();
    }
    
    bool loadConfig() {
        if (!dataManager) return false;
        
        String settingsData = dataManager->getSettings();
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, settingsData);
        
        if (error) return false;
        
        if (doc.containsKey("mailEnabled")) {
            config.enabled = doc["mailEnabled"];
            config.smtpServer = doc["smtpServer"].as<String>();
            config.smtpPort = doc["smtpPort"] | 587;
            config.smtpUser = doc["smtpUser"].as<String>();
            config.smtpPassword = doc["smtpPassword"].as<String>();
            config.fromEmail = doc["fromEmail"].as<String>();
            config.useSSL = doc["useSSL"] | true;
        }
        
        return true;
    }
    
    bool saveConfig(const String& configJson) {
        if (!dataManager) return false;
        
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, configJson);
        
        if (error) return false;
        
        config.enabled = doc["enabled"] | false;
        config.smtpServer = doc["smtpServer"].as<String>();
        config.smtpPort = doc["smtpPort"] | 587;
        config.smtpUser = doc["smtpUser"].as<String>();
        config.smtpPassword = doc["smtpPassword"].as<String>();
        config.fromEmail = doc["fromEmail"].as<String>();
        config.useSSL = doc["useSSL"] | true;
        
        String currentSettings = dataManager->getSettings();
        DynamicJsonDocument settingsDoc(4096);
        deserializeJson(settingsDoc, currentSettings);
        
        settingsDoc["mailEnabled"] = config.enabled;
        settingsDoc["smtpServer"] = config.smtpServer;
        settingsDoc["smtpPort"] = config.smtpPort;
        settingsDoc["smtpUser"] = config.smtpUser;
        settingsDoc["smtpPassword"] = config.smtpPassword;
        settingsDoc["fromEmail"] = config.fromEmail;
        settingsDoc["useSSL"] = config.useSSL;
        
        String updatedSettings;
        serializeJson(settingsDoc, updatedSettings);
        
        return dataManager->setSettings(updatedSettings);
    }
    
    String getConfigJSON() {
        DynamicJsonDocument doc(2048);
        
        doc["enabled"] = config.enabled;
        doc["smtpServer"] = config.smtpServer;
        doc["smtpPort"] = config.smtpPort;
        doc["smtpUser"] = config.smtpUser;
        doc["smtpPassword"] = config.smtpPassword;
        doc["fromEmail"] = config.fromEmail;
        doc["useSSL"] = config.useSSL;
        
        String output;
        serializeJson(doc, output);
        return output;
    }
    
    bool isEnabled() {
        return config.enabled;
    }
};

#endif
