#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_task_wdt.h>  // Watchdog timer for safety

// Pin Definitions
#define OLED_SDA 22        // D4
#define OLED_SCL 23        // D5
#define BUTTON_PIN 2       // D2

// Display Settings - 0.91" OLED is usually 128x32
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

// Button Settings
#define DEBOUNCE_DELAY 50  // ms

class DisplayManager {
private:
  Adafruit_SSD1306 display;
  uint8_t currentPage;
  unsigned long lastButtonPress;
  bool lastButtonState;
  uint8_t oledAddress;
  bool displayFound;
  
  // Auto-recovery system
  unsigned long lastRecoveryAttempt;
  uint8_t recoveryAttemptCount;
  static constexpr unsigned long RECOVERY_INTERVAL = 10000; // Try recovery every 10 seconds
  static constexpr uint8_t MAX_RECOVERY_ATTEMPTS = 5;       // Max 5 attempts before giving up
  
  // Task counts
  int todayCount;
  int tomorrowCount;
  int weekCount;
  
  // App title
  String appTitle;
  
  // Network info
  String networkSSID;
  String networkIP;
  String networkLocal;
  
  // System ready flag
  bool systemReady;
  
public:
  DisplayManager() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      currentPage(0),
      lastButtonPress(0),
      lastButtonState(HIGH),
      oledAddress(0x3C),
      displayFound(false),
      lastRecoveryAttempt(0),
      recoveryAttemptCount(0),
      todayCount(0),
      tomorrowCount(0),
      weekCount(0),
      appTitle("To2Do-SmartKraft"),
      networkSSID(""),
      networkIP(""),
      networkLocal(""),
      systemReady(false) {
  }
  
  bool begin() {
    // Aggressive I2C timeout to prevent system blocking
    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(100000);
    Wire.setTimeOut(50); // Very short timeout: 50ms to prevent NACK blocking
    delay(50);
    
    // Try to detect OLED with timeout protection
    unsigned long detectStart = millis();
    Wire.beginTransmission(0x3C);
    delayMicroseconds(100);
    uint8_t error3C = Wire.endTransmission();
    
    // If detection takes too long, abort
    if (millis() - detectStart > 100) {
      Serial.println("[Display] ✗ OLED detection timeout");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    detectStart = millis();
    Wire.beginTransmission(0x3D);
    delayMicroseconds(100);
    uint8_t error3D = Wire.endTransmission();
    
    // If detection takes too long, abort
    if (millis() - detectStart > 100) {
      Serial.println("[Display] ✗ OLED detection timeout");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    if (error3C != 0 && error3D != 0) {
      Serial.println("[Display] ✗ OLED not found (system continues)");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    uint8_t targetAddr = (error3C == 0) ? 0x3C : 0x3D;
    
    unsigned long initStart = millis();
    bool initSuccess = false;
    
    // Try init with strict timeout
    while (millis() - initStart < 500) { // Reduced from 1000ms
      if (display.begin(SSD1306_SWITCHCAPVCC, targetAddr, false)) {
        initSuccess = true;
        break;
      }
      delay(25); // Shorter delay
      
      // Additional timeout check
      if (millis() - initStart > 400) {
        break;
      }
    }
    
    if (!initSuccess) {
      Serial.println("[Display] ✗ Init timeout (system continues)");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    oledAddress = targetAddr;
    displayFound = true;
    
    // Test display operations with timeout
    unsigned long opStart = millis();
    display.clearDisplay();
    if (millis() - opStart > 100) { // Reduced from 300ms
      Serial.println("[Display] ✗ clearDisplay timeout (system continues)");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    opStart = millis();
    display.display();
    if (millis() - opStart > 100) { // Reduced from 300ms
      Serial.println("[Display] ✗ display timeout (system continues)");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return false;
    }
    
    delay(50);
    
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(255);
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    showSplashScreen();
    
    return true;
  }
  
  void showSplashScreen() {
    if (!displayFound) return;
    
    systemReady = false;
    unsigned long opStart;
    
    // Protected clearDisplay with timeout
    opStart = millis();
    display.clearDisplay();
    if (millis() - opStart > 100) { // Reduced timeout
      Serial.println("[Display] ✗ Splash clearDisplay timeout");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return;
    }
    
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    int16_t x1 = (SCREEN_WIDTH - (10 * 12)) / 2;
    display.setCursor(x1, 4);
    display.println("SmartKraft");
    
    display.setTextSize(1);
    int16_t x2 = (SCREEN_WIDTH - (5 * 6)) / 2;
    display.setCursor(x2, 22);
    display.println("To2Do");
    
    // Protected display with timeout
    opStart = millis();
    display.display();
    if (millis() - opStart > 100) { // Reduced timeout
      Serial.println("[Display] ✗ Splash display timeout");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return;
    }
  }
  
  void setAppTitle(const char* title) {
    appTitle = String(title); // Just save, don't display yet
  }
  
  void setSystemReady() {
    systemReady = true;
    currentPage = 0;
    updateDisplay();
  }
  
  void setTaskCounts(int today, int tomorrow, int week) {
    todayCount = today;
    tomorrowCount = tomorrow;
    weekCount = week;
  }
  
  void setNetworkInfo(String ssid, String ip, String local) {
    networkSSID = ssid;
    networkIP = ip;
    networkLocal = local;
  }
  
  void loop() {
    // Auto-recovery
    if (!displayFound && recoveryAttemptCount < MAX_RECOVERY_ATTEMPTS) {
      unsigned long now = millis();
      if (now - lastRecoveryAttempt >= RECOVERY_INTERVAL) {
        lastRecoveryAttempt = now;
        recoveryAttemptCount++;
        
        if (attemptRecovery()) {
          Serial.println("[Display] ✓ OLED recovered");
          recoveryAttemptCount = 0;
          
          if (systemReady) {
            updateDisplay();
          } else {
            showSplashScreen();
          }
        }
      }
      return;
    }
    
    if (!displayFound) return;
    checkButton();
  }
  
  bool attemptRecovery() {
    Serial.println("[Display] Attempting recovery...");
    
    // Reset I2C with shorter timeout
    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(100000);
    Wire.setTimeOut(50); // Aggressive timeout to prevent blocking
    delay(25);
    
    // Quick device detection with timeout
    unsigned long start = millis();
    Wire.beginTransmission(oledAddress);
    delayMicroseconds(100);
    uint8_t error = Wire.endTransmission();
    
    if (millis() - start > 100 || error != 0) {
      Serial.println("[Display] Recovery failed: device not responding");
      return false; // Timeout or device not responding
    }
    
    // Try to reinitialize display with timeout
    start = millis();
    bool initOk = display.begin(SSD1306_SWITCHCAPVCC, oledAddress, false);
    
    if (!initOk || (millis() - start > 300)) { // Reduced timeout
      Serial.println("[Display] Recovery failed: init timeout");
      return false; // Init failed or too slow
    }
    
    // Test display with timeout
    start = millis();
    display.clearDisplay();
    if (millis() - start > 100) {
      Serial.println("[Display] Recovery failed: clearDisplay timeout");
      return false;
    }
    
    start = millis();
    display.display();
    if (millis() - start > 100) {
      Serial.println("[Display] Recovery failed: display timeout");
      return false;
    }
    
    displayFound = true;
    Serial.println("[Display] Recovery successful!");
    return true;
  }
  
  void checkButton() {
    if (!systemReady) return;
    
    bool buttonState = digitalRead(BUTTON_PIN);
    unsigned long currentTime = millis();
    
    if (buttonState == LOW && lastButtonState == HIGH) {
      if (currentTime - lastButtonPress > DEBOUNCE_DELAY) {
        lastButtonPress = currentTime;
        nextPage();
        delay(500);
      }
    }
    
    lastButtonState = buttonState;
  }
  
  void nextPage() {
    currentPage++;
    if (currentPage > 4) {  // 5 pages: 0=title, 1=today, 2=tomorrow, 3=week, 4=network
      currentPage = 0;
    }
    updateDisplay();
  }
  
  void refreshCurrentPage() {
    // Refresh current page without changing page number
    updateDisplay();
  }
  
  void updateDisplay() {
    if (!displayFound) return;
    
    unsigned long opStart;
    
    // Protected clearDisplay with timeout
    opStart = millis();
    display.clearDisplay();
    if (millis() - opStart > 100) { // Reduced timeout
      Serial.println("[Display] ✗ updateDisplay clearDisplay timeout, starting recovery");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return;
    }
    
    switch (currentPage) {
      case 0:
        drawAppTitle();
        break;
      case 1:
        drawTodayPage();
        break;
      case 2:
        drawTomorrowPage();
        break;
      case 3:
        drawWeekPage();
        break;
      case 4:
        drawNetworkPage();
        break;
    }
    
    // Protected display with timeout
    opStart = millis();
    display.display();
    if (millis() - opStart > 100) { // Reduced timeout
      Serial.println("[Display] ✗ updateDisplay display timeout, starting recovery");
      displayFound = false;
      lastRecoveryAttempt = millis();
      recoveryAttemptCount = 0;
      return;
    }
  }
  
  void drawAppTitle() {
    // App title page - use saved title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Center text
    int16_t x = (SCREEN_WIDTH - (appTitle.length() * 6)) / 2;
    display.setCursor(x, 12);
    display.println(appTitle);
  }
  
  void drawTodayPage() {
    // Today's tasks - label on left, number on right (lower position)
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // "Bugun" text - sol tarafta, bir satır aşağıda
    display.setCursor(10, 14);
    display.print("Bugun");
    
    // Count - sağda (x=85), çok büyük rakam (size 3)
    display.setTextSize(3);
    display.setCursor(85, 6);
    display.print(todayCount);
  }
  
  void drawTomorrowPage() {
    // Tomorrow's tasks - label on left, number on right (lower position)
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // "Yarin" text - sol tarafta, bir satır aşağıda
    display.setCursor(10, 14);
    display.print("Yarin");
    
    // Count - sağda (x=85), çok büyük rakam (size 3)
    display.setTextSize(3);
    display.setCursor(85, 6);
    display.print(tomorrowCount);
  }
  
  void drawWeekPage() {
    // This week's tasks - label on left, number on right (lower position)
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // "Bu Hafta" text - sol tarafta, bir satır aşağıda
    display.setCursor(10, 14);
    display.print("Bu Hafta");
    
    // Count - sağda (x=85), çok büyük rakam (size 3)
    display.setTextSize(3);
    display.setCursor(85, 6);
    display.print(weekCount);
  }
  
  void drawNetworkPage() {
    // Network info - 3 lines centered horizontally
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Line 1: SSID (Y=2)
    int16_t x1 = (SCREEN_WIDTH - (networkSSID.length() * 6)) / 2;
    if (x1 < 0) x1 = 0; // Prevent negative
    display.setCursor(x1, 2);
    display.println(networkSSID);
    
    // Line 2: IP Address (Y=12)
    int16_t x2 = (SCREEN_WIDTH - (networkIP.length() * 6)) / 2;
    if (x2 < 0) x2 = 0;
    display.setCursor(x2, 12);
    display.println(networkIP);
    
    // Line 3: .local address (Y=22)
    int16_t x3 = (SCREEN_WIDTH - (networkLocal.length() * 6)) / 2;
    if (x3 < 0) x3 = 0;
    display.setCursor(x3, 22);
    display.println(networkLocal);
  }
  
  bool isDisplayFound() {
    return displayFound;
  }
  
  uint8_t getCurrentPage() {
    return currentPage;
  }
};

#endif
