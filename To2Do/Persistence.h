#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "Data_Manager.h"

/*
 * PERSISTENCE LAYER
 * Wrapper around DataManager for backward compatibility
 */

class Persistence {
private:
    DataManager dataManager;
    
public:
    bool begin() {
        return dataManager.begin();
    }
    
    DataManager* getDataManager() {
        return &dataManager;
    }
    
    // Network settings helpers
    bool saveNetworkSettings(const String& jsonString) {
        return dataManager.setNetworkSettings(jsonString);
    }
    
    String getNetworkSettings() {
        return dataManager.getNetworkSettings();
    }
    
    // GUI settings helpers
    bool saveGUISettings(const String& jsonString) {
        return dataManager.setGUISettings(jsonString);
    }
    
    String getGUISettings() {
        return dataManager.getGUISettings();
    }
    
    // Factory reset
    void factoryReset() {
        dataManager.factoryReset();
    }
};

#endif
