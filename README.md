# To2Do - SmartKraft To-Do Manager

To-Do list with an OLED screen, developed with ESP32, capable of sending email notifications

## Features

- ✅ **Categorized Organization**: Organize tasks into customizable categories (Work, Personal, Projects)
- 💾 **Import/Export**: Backup and restore your data anytime with JSON export/import
- 🔒 **Data Security**: All data stored locally on ESP32 SPIFFS - no cloud monitoring like Google
- 📡 **Offline Working**: Full functionality without internet connection
- 📧 **Email Notifications**: SMTP integration for task reminders and notifications
- 🌐 **Dual Mode**: WiFi Client mode or Access Point mode
- 🎨 **Modern Web UI**: Responsive dark-themed interface accessible from any device
- 📱 **mDNS Support**: Access via friendly URLs like `http://to2do.local`
- 🔧 **Factory Reset**: Easy restoration to default settings

## Hardware Requirements

- ESP32 Development Board
- OLED Display (optional, support coming soon)
- USB cable for programming and power

## Software Requirements

- Arduino IDE 1.8.x or 2.x
- ESP32 Board Support Package
- Required Libraries:
  - ArduinoJson (v6.x)
  - ESP32 Core Libraries (WiFi, WebServer, SPIFFS)
  - ESP_Mail_Client (for email notifications)

## Installation

1. **Install Arduino IDE**
   - Download from https://www.arduino.cc/en/software

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to Additional Board Manager URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools → Board → Board Manager
   - Search "ESP32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries**
   - Go to Sketch → Include Library → Manage Libraries
   - Install:
     - ArduinoJson by Benoit Blanchon (v6.x)
     - ESP Mail Client by Mobizt

4. **Upload the Sketch**
   - Open `To2Do/To2Do.ino` in Arduino IDE
   - Select your ESP32 board: Tools → Board → ESP32 Dev Module
   - Select the correct COM port: Tools → Port
   - Click Upload button

## First Run Setup

1. **Initial Access**
   - On first boot, the device starts in AP (Access Point) mode
   - Look for WiFi network: `SmartKraft-To2Do`
   - Connect to it (no password required)
   - Open browser and go to: `http://192.168.4.1` or `http://to2do.local`

2. **Configure WiFi (Optional)**
   - Click Settings (⚙) button
   - Go to Connection tab
   - Enter your WiFi credentials in "Primary WiFi" section
   - Save settings
   - Device will restart and connect to your WiFi
   - Find the new IP in your router, or use mDNS: `http://to2do.local`

3. **Start Using**
   - Create projects by clicking "+ PROJECT"
   - Select a project and add tasks with the "+" button
   - Organize by categories using the tabs (Work, Personal, Projects)

## Features Guide

### Projects & Tasks

- **Create Projects**: Click "+ PROJECT" to create a new project
- **Add Tasks**: Select a project and click the floating "+" button
- **Set Priority**: Mark tasks as High, Medium, or Low priority
- **Complete Tasks**: Click checkbox to mark tasks as complete
- **Filter Views**: Filter by All, Active, Completed, or High Priority
- **Search**: Use the search bar to find projects quickly

### Categories

- Three customizable categories: Work, Personal, Projects
- Change category names in Settings → General
- Each project belongs to one category
- Filter projects by selecting category tabs

### Data Backup

- **Export**: Settings → Backup → EXPORT
  - Downloads all data as JSON file
  - Includes projects, tasks, and settings
  - Named with date: `To2Do_Backup_YYYY-MM-DD.json`

- **Import**: Settings → Backup → IMPORT
  - Restore from previously exported JSON
  - Overwrites current data
  - Automatically reloads interface

### Email Notifications

1. **Enable Email**: Settings → Mail → Toggle "ENABLED"
2. **Configure SMTP**:
   - Server: e.g., `smtp.gmail.com`
   - Port: 587 (TLS) or 465 (SSL)
   - Username: Your email address
   - Password: App-specific password (for Gmail)
   - From Email: Sender address
3. **For Gmail Users**:
   - Enable 2-Factor Authentication
   - Generate App Password: https://myaccount.google.com/apppasswords
   - Use App Password instead of your regular password

### Network Configuration

- **AP Mode Settings**: Configure Access Point name and mDNS
- **Primary WiFi**: Main network to connect to
- **Backup WiFi**: Fallback network (optional)
- **Static IP**: Optional static IP address
- **mDNS**: Friendly URL access (e.g., `to2do.local`)

### General Settings

- **Theme**: Switch between Light and Dark modes
- **App Title**: Customize the application title
- **Category Names**: Rename the three category tabs

## System Information

Access real-time system info in Settings → Info:
- Version number
- WiFi connection status
- IP address and mDNS
- Free memory (heap)
- SPIFFS storage usage
- System uptime

## Factory Reset

If you need to reset everything to defaults:
1. Go to Settings → Backup
2. Click "FACTORY RESET" button
3. Confirm twice
4. Device will restore demo projects and reset all settings

## Data Security & Privacy

- ✅ **100% Local Storage**: All data stored on ESP32 SPIFFS
- ✅ **No Cloud Sync**: Your data never leaves the device
- ✅ **No Tracking**: No analytics or monitoring
- ✅ **Open Source**: Full code visibility
- ✅ **Offline Capable**: Works without internet
- ✅ **Your Control**: Export, backup, and own your data

## Technical Details

- **Storage**: SPIFFS filesystem on ESP32
- **Data Format**: JSON
- **Memory**: 24KB buffer for data operations
- **Web Server**: Lightweight ESP32 WebServer
- **Interface**: HTML5 + JavaScript (vanilla, no frameworks)
- **Styling**: CSS3 with dark theme
- **Demo Data**: 7 sample projects included on first boot

## Troubleshooting

### Cannot Access Web Interface
- Verify ESP32 is powered on
- Check WiFi connection to AP or network
- Try IP address directly: `http://192.168.4.1` (AP mode)
- Check serial monitor for IP address and mDNS name

### Email Not Sending
- Verify SMTP settings are correct
- For Gmail: Use App Password, not regular password
- Check SMTP server address and port
- Enable SSL/TLS if required by your email provider
- Verify ESP32 has internet access (not in AP-only mode)

### Data Not Saving
- Check SPIFFS initialization in serial monitor
- Verify sufficient SPIFFS space
- Try factory reset if corruption suspected

### WiFi Connection Issues
- Verify SSID and password are correct
- Check WiFi signal strength
- Try forgetting backup WiFi settings
- Reset to AP mode by clearing WiFi settings

## Serial Monitor Output

Connect to serial monitor (115200 baud) to see:
- Initialization status
- WiFi connection details
- IP address and mDNS name
- Data save/load operations
- Error messages and debugging info

## Future Enhancements

- 📺 OLED display integration with animations
- 🔔 Advanced notification scheduling
- 📊 Task statistics and analytics
- 🌍 Multi-language support (EN, DE, TR)
- 🔄 Cloud sync option (optional, privacy-focused)
- 📅 Calendar integration
- ⏰ Due date reminders
- 🏷️ Task tags and filters

## License

[Add your license here]

## Credits

Developed by SmartKraft
- Email: code@smartkraft.ch
- Version: 1.1

## Support

For issues, questions, or contributions, please visit the GitHub repository.