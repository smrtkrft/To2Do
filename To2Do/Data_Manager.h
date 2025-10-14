#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <SPIFFS.h>
#include <ArduinoJson.h>

/*
 * UNIFIED DATA MANAGER
 * Tüm kullanıcı verileri tek bir JSON dosyasında (/userdata.json)
 * - Projects & Tasks (Projeler ve Todo'lar)
 * - GUI Settings (Tema, kategori isimleri, app başlığı)
 * - Network Settings (WiFi credentials - Primary & Backup)
 * 
 * Factory Reset: Tüm kullanıcı verilerini siler, demo içeriği yükler
 */

class DataManager {
private:
    const char* DATA_FILE = "/userdata.json";
    const size_t JSON_CAPACITY = 24576; // 24KB buffer (reduced from 32KB)
    
    DynamicJsonDocument userData;
    
public:
    DataManager() : userData(JSON_CAPACITY) {
        Serial.println("[DataManager] Initialized with 24KB buffer");
    }
    
    // ==================== INITIALIZATION ====================
    
    bool begin() {
        if (!SPIFFS.begin(true)) {
            Serial.println("[DataManager] ERROR: SPIFFS mount failed");
            return false;
        }
        
        Serial.printf("[DataManager] SPIFFS: %d/%d bytes used\n", 
                      SPIFFS.usedBytes(), SPIFFS.totalBytes());
        
        if (!loadFromFile()) {
            Serial.println("[DataManager] No existing data, initializing with demo content");
            initializeDemoData();
            saveToFile();
        }
        
        return true;
    }
    
    // ==================== CORE PERSISTENCE ====================
    
    bool loadFromFile() {
        if (!SPIFFS.exists(DATA_FILE)) {
            Serial.println("[DataManager] Data file does not exist");
            return false;
        }
        
        File file = SPIFFS.open(DATA_FILE, "r");
        if (!file) {
            Serial.println("[DataManager] ERROR: Cannot open data file");
            return false;
        }
        
        DeserializationError error = deserializeJson(userData, file);
        file.close();
        
        if (error) {
            Serial.printf("[DataManager] ERROR: JSON parse failed: %s\n", error.c_str());
            return false;
        }
        
        Serial.printf("[DataManager] ✓ Loaded data from %s\n", DATA_FILE);
        return true;
    }
    
    bool saveToFile() {
        // Count current data before saving
        int projectCount = userData["projects"].size();
        int taskCount = userData["tasks"].size();
        
        Serial.printf("[DataManager] saveToFile: Attempting to save %d projects, %d tasks\n", 
                      projectCount, taskCount);
        
        File file = SPIFFS.open(DATA_FILE, "w");
        if (!file) {
            Serial.println("[DataManager] ERROR: Cannot open data file for writing");
            return false;
        }
        
        size_t bytesWritten = serializeJson(userData, file);
        file.close();
        
        if (bytesWritten == 0) {
            Serial.println("[DataManager] ERROR: Failed to write data");
            return false;
        }
        
        Serial.printf("[DataManager] ✓ Saved %d bytes to %s\n", bytesWritten, DATA_FILE);
        return true;
    }
    
    // ==================== DATA ACCESS ====================
    
    // Get complete user data as JSON string
    String getAllData() {
        String output;
        serializeJson(userData, output);
        return output;
    }
    
    // Update complete user data from JSON string
    bool setAllData(const String& jsonString) {
        DynamicJsonDocument doc(JSON_CAPACITY);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            Serial.printf("[DataManager] ERROR: setAllData parse failed: %s\n", error.c_str());
            return false;
        }
        
        userData = doc;
        return saveToFile();
    }
    
    // Get projects array as JSON string
    String getProjects() {
        String output;
        serializeJson(userData["projects"], output);
        return output;
    }
    
    // Get tasks array as JSON string
    String getTasks() {
        String output;
        serializeJson(userData["tasks"], output);
        return output;
    }
    
    // Get GUI settings as JSON string
    String getGUISettings() {
        String output;
        serializeJson(userData["guiSettings"], output);
        return output;
    }
    
    // Update GUI settings
    bool setGUISettings(const String& jsonString) {
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            Serial.printf("[DataManager] ERROR: GUI settings parse failed: %s\n", error.c_str());
            return false;
        }
        
        userData["guiSettings"] = doc;
        return saveToFile();
    }
    
    // Get network settings as JSON string
    String getNetworkSettings() {
        String output;
        serializeJson(userData["networkSettings"], output);
        return output;
    }
    
    // Update network settings
    bool setNetworkSettings(const String& jsonString) {
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            Serial.printf("[DataManager] ERROR: Network settings parse failed: %s\n", error.c_str());
            return false;
        }
        
        userData["networkSettings"] = doc;
        return saveToFile();
    }
    
    // Get mail settings as JSON string
    String getMailSettings() {
        String output;
        serializeJson(userData["mailSettings"], output);
        return output;
    }
    
    // Update mail settings
    bool setMailSettings(const String& jsonString) {
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            Serial.printf("[DataManager] ERROR: Mail settings parse failed: %s\n", error.c_str());
            return false;
        }
        
        userData["mailSettings"] = doc;
        return saveToFile();
    }
    
    // Update projects and tasks
    bool setTodosData(const String& jsonString) {
        DynamicJsonDocument doc(JSON_CAPACITY);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            Serial.printf("[DataManager] ERROR: Todos parse failed: %s\n", error.c_str());
            return false;
        }
        
        userData["projects"] = doc["projects"];
        userData["tasks"] = doc["tasks"];
        
        bool success = saveToFile();
        if (success) {
            Serial.println("[DataManager] ✓ Todos saved to SPIFFS");
        } else {
            Serial.println("[DataManager] ✗ Failed to save todos to SPIFFS");
        }
        
        return success;
    }
    
    // ==================== FACTORY RESET ====================
    
    void factoryReset() {
        Serial.println("[DataManager] Factory reset initiated");
        userData.clear();
        initializeDemoData();
        saveToFile();
        Serial.println("[DataManager] ✓ Factory reset complete");
    }
    
private:
    void initializeDemoData() {
        // Initialize with demo projects
        JsonArray projects = userData.createNestedArray("projects");
        
        JsonObject p1 = projects.createNestedObject();
        p1["id"] = 1;
        p1["name"] = "Website Redesign";
        p1["category"] = "Work";
        p1["description"] = "Company website modernization project";
        p1["priority"] = "high";
        p1["archived"] = false;
        
        JsonObject p2 = projects.createNestedObject();
        p2["id"] = 2;
        p2["name"] = "Team Meeting Prep";
        p2["category"] = "Work";
        p2["description"] = "Prepare presentation and reports";
        p2["priority"] = "medium";
        p2["archived"] = false;
        
        JsonObject p3 = projects.createNestedObject();
        p3["id"] = 3;
        p3["name"] = "Home Improvement";
        p3["category"] = "Personal";
        p3["description"] = "Living room renovation tasks";
        p3["priority"] = "medium";
        p3["archived"] = false;
        
        JsonObject p4 = projects.createNestedObject();
        p4["id"] = 4;
        p4["name"] = "Fitness Goals";
        p4["category"] = "Personal";
        p4["description"] = "Weekly workout and diet plan";
        p4["priority"] = "high";
        p4["archived"] = false;
        
        JsonObject p5 = projects.createNestedObject();
        p5["id"] = 5;
        p5["name"] = "Smart Home System";
        p5["category"] = "Projects";
        p5["description"] = "IoT devices integration project";
        p5["priority"] = "high";
        p5["archived"] = false;
        
        JsonObject p6 = projects.createNestedObject();
        p6["id"] = 6;
        p6["name"] = "Learn New Skills";
        p6["category"] = "Projects";
        p6["description"] = "Online courses and certifications";
        p6["priority"] = "medium";
        p6["archived"] = false;
        
        JsonObject p7 = projects.createNestedObject();
        p7["id"] = 7;
        p7["name"] = "Shopping List";
        p7["category"] = "Personal";
        p7["description"] = "Weekly grocery and supplies";
        p7["priority"] = "low";
        p7["archived"] = false;
        
        // Initialize with demo tasks
        JsonArray tasks = userData.createNestedArray("tasks");
        
        // GUI Settings
        JsonObject gui = userData.createNestedObject("guiSettings");
        gui["theme"] = "dark";
        gui["appTitle"] = "ToDo - SmartKraft";
        gui["cat1"] = "Work";
        gui["cat2"] = "Personal";
        gui["cat3"] = "Projects";
        
        // Network Settings
        JsonObject network = userData.createNestedObject("networkSettings");
        network["apSSID"] = "SmartKraft-To2Do";
        network["apMDNS"] = "to2do";
        network["primarySSID"] = "";
        network["primaryPassword"] = "";
        network["primaryIP"] = "";
        network["primaryMDNS"] = "";
        network["backupSSID"] = "";
        network["backupPassword"] = "";
        network["backupIP"] = "";
        network["backupMDNS"] = "";
        
        // Mail Settings
        JsonObject mail = userData.createNestedObject("mailSettings");
        mail["enabled"] = false;
        mail["smtpServer"] = "";
        mail["smtpPort"] = 587;
        mail["smtpUser"] = "";
        mail["smtpPassword"] = "";
        mail["smtpFrom"] = "";
        mail["smtpSSL"] = true;
        
        Serial.println("[DataManager] Demo data initialized");
    }
};

#endif
