#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

#include "Data_Manager.h"

/*
 * BACKUP MANAGER
 * Handles import/export of user data
 */

class BackupManager {
private:
    DataManager* dataManager;
    
public:
    BackupManager(DataManager* dm) : dataManager(dm) {}
    
    String exportData() {
        Serial.println("[BackupManager] Exporting data...");
        String data = dataManager->getAllData();
        Serial.printf("[BackupManager] ✓ Exported %d bytes\n", data.length());
        return data;
    }
    
    bool importData(const String& jsonData) {
        Serial.println("[BackupManager] Importing data...");
        Serial.printf("[BackupManager] Data size: %d bytes\n", jsonData.length());
        
        bool success = dataManager->setAllData(jsonData);
        
        if (success) {
            Serial.println("[BackupManager] ✓ Import successful");
        } else {
            Serial.println("[BackupManager] ✗ Import failed");
        }
        
        return success;
    }
};

#endif
