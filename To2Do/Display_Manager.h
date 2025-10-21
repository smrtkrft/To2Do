#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
      todayCount(0),
      tomorrowCount(0),
      weekCount(0),
      appTitle("ToDo-SmartKraft"),
      networkSSID(""),
      networkIP(""),
      networkLocal(""),
      systemReady(false) {
  }
  
  bool begin() {
    // Initialize I2C immediately
    Wire.begin(OLED_SDA, OLED_SCL);
    delay(100);
    
    // Try common OLED addresses
    uint8_t addresses[] = {0x3C, 0x3D};
    for (uint8_t addr : addresses) {
      if (display.begin(SSD1306_SWITCHCAPVCC, addr)) {
        oledAddress = addr;
        displayFound = true;
        break;
      }
    }
    
    if (!displayFound) {
      return false;
    }
    
    // Extra initialization for stability
    display.clearDisplay();
    display.display();
    delay(100);
    
    // Set max brightness
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(255);
    
    // Initialize button
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // Show SmartKraft splash screen immediately
    showSplashScreen();
    
    return true;
  }
  
  void showSplashScreen() {
    systemReady = false;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    // "SmartKraft" büyük (size 2) - üstte
    display.setTextSize(2);
    int16_t x1 = (SCREEN_WIDTH - (10 * 12)) / 2; // 10 karakter * 12 piksel (size 2)
    display.setCursor(x1, 4);
    display.println("SmartKraft");
    
    // "To2Do" küçük (size 1) - altta, ortalanmış
    display.setTextSize(1);
    int16_t x2 = (SCREEN_WIDTH - (5 * 6)) / 2; // 5 karakter * 6 piksel (size 1)
    display.setCursor(x2, 22);
    display.println("To2Do");
    
    display.display();
    Serial.println("[Display] Splash screen shown - waiting for system ready...");
  }
  
  void setAppTitle(const char* title) {
    appTitle = String(title); // Just save, don't display yet
  }
  
  void setSystemReady() {
    // Called when all data is loaded - switch to app title page
    systemReady = true;
    currentPage = 0; // Start at app title page
    updateDisplay();
    Serial.println("[Display] System ready! Showing app title page.");
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
    if (!displayFound) return;
    
    // Check button press
    checkButton();
  }
  
  void checkButton() {
    // Don't respond to button until system is ready
    if (!systemReady) return;
    
    bool buttonState = digitalRead(BUTTON_PIN);
    unsigned long currentTime = millis();
    
    // Detect button press (LOW = pressed with pull-up)
    if (buttonState == LOW && lastButtonState == HIGH) {
      // Debounce check
      if (currentTime - lastButtonPress > DEBOUNCE_DELAY) {
        lastButtonPress = currentTime;
        
        Serial.println("Buton basildi");
        Serial.println("\n");
        
        nextPage();
        
        // 500ms delay to prevent rapid switching
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
    // Clear display first
    display.clearDisplay();
    
    // Draw page content
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
    
    // Push to display
    display.display();
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
