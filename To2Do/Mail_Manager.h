#ifndef MAIL_MANAGER_H
#define MAIL_MANAGER_H

#include <ESP_Mail_Client.h>
#include "Data_Manager.h"

/*
 * MAIL MANAGER
 * Handles SMTP email notifications
 */

class MailManager {
private:
    DataManager* dataManager;
    SMTPSession smtp;
    
    bool enabled;
    String smtpServer;
    int smtpPort;
    String smtpUser;
    String smtpPassword;
    String smtpFrom;
    bool smtpSSL;
    
public:
    MailManager(DataManager* dm) : dataManager(dm) {
        loadConfig();
    }
    
    void loadConfig() {
        String settings = dataManager->getMailSettings();
        
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, settings);
        
        enabled = doc["enabled"] | false;
        smtpServer = doc["smtpServer"] | "";
        smtpPort = doc["smtpPort"] | 587;
        smtpUser = doc["smtpUser"] | "";
        smtpPassword = doc["smtpPassword"] | "";
        smtpFrom = doc["smtpFrom"] | "";
        smtpSSL = doc["smtpSSL"] | true;
        
        Serial.printf("[MailManager] Loaded config - Enabled: %s\n", enabled ? "YES" : "NO");
    }
    
    String getConfigJSON() {
        String settings = dataManager->getMailSettings();
        
        // Remove password from response for security
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, settings);
        doc["smtpPassword"] = "";
        
        String output;
        serializeJson(doc, output);
        return output;
    }
    
    bool saveConfig(const String& jsonData) {
        Serial.println("[MailManager] Saving config...");
        
        bool success = dataManager->setMailSettings(jsonData);
        
        if (success) {
            loadConfig();
            Serial.println("[MailManager] ✓ Config saved");
        } else {
            Serial.println("[MailManager] ✗ Config save failed");
        }
        
        return success;
    }
    
    bool sendNotification(const String& subject, const String& body) {
        if (!enabled) {
            Serial.println("[MailManager] Email disabled, skipping send");
            return false;
        }
        
        if (smtpServer.length() == 0 || smtpUser.length() == 0) {
            Serial.println("[MailManager] SMTP not configured");
            return false;
        }
        
        Serial.printf("[MailManager] Sending email: %s\n", subject.c_str());
        
        // Configure SMTP
        Session_Config config;
        config.server.host_name = smtpServer.c_str();
        config.server.port = smtpPort;
        config.login.email = smtpUser.c_str();
        config.login.password = smtpPassword.c_str();
        config.login.user_domain = "";
        
        // Create message
        SMTP_Message message;
        message.sender.name = "SmartKraft To2Do";
        message.sender.email = smtpFrom.c_str();
        message.subject = subject.c_str();
        message.addRecipient("User", smtpUser.c_str());
        message.text.content = body.c_str();
        
        // Send
        if (!smtp.connect(&config)) {
            Serial.println("[MailManager] ✗ SMTP connection failed");
            return false;
        }
        
        if (!MailClient.sendMail(&smtp, &message)) {
            Serial.printf("[MailManager] ✗ Send failed: %s\n", smtp.errorReason().c_str());
            smtp.closeSession();
            return false;
        }
        
        smtp.closeSession();
        Serial.println("[MailManager] ✓ Email sent successfully");
        return true;
    }
};

#endif
