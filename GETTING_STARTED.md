# Getting Started with To2Do

This guide will help you get your To2Do ESP32 device up and running in minutes.

## What You'll Need

- ESP32 development board
- USB cable (for programming and power)
- Computer with Arduino IDE installed
- WiFi network (optional, but recommended)

## Quick Start Guide

### Step 1: Hardware Setup

1. Connect your ESP32 board to your computer using a USB cable
2. Make sure the board is recognized by your computer
   - Windows: Check Device Manager for COM port
   - Mac/Linux: Check `/dev/tty*` or `/dev/cu*` devices

### Step 2: Software Installation

1. **Install Arduino IDE**
   - Download from: https://www.arduino.cc/en/software
   - Version 1.8.x or 2.x both work

2. **Install ESP32 Support**
   ```
   - Open Arduino IDE
   - Go to: File → Preferences
   - In "Additional Board Manager URLs" add:
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   - Go to: Tools → Board → Board Manager
   - Search for "esp32"
   - Install "esp32 by Espressif Systems"
   ```

3. **Install Required Libraries**
   ```
   - Go to: Sketch → Include Library → Manage Libraries
   - Install these libraries:
     * ArduinoJson (by Benoit Blanchon) - Version 6.x
     * ESP Mail Client (by Mobizt)
   ```

### Step 3: Upload the Code

1. Open `To2Do/To2Do.ino` in Arduino IDE
2. Select your board:
   - Go to: Tools → Board → ESP32 Arduino → ESP32 Dev Module
3. Select the correct port:
   - Go to: Tools → Port → [Your ESP32 COM port]
4. Click the Upload button (→)
5. Wait for upload to complete (you'll see "Done uploading")

### Step 4: First Access

1. **Open Serial Monitor**
   - Click the magnifying glass icon (top right)
   - Set baud rate to: **115200**
   - You should see initialization messages

2. **Connect to the Device**
   - Look for WiFi network: `SmartKraft-To2Do`
   - Connect to it (no password needed)
   - Open browser to: http://192.168.4.1
   - OR try: http://to2do.local

3. **You should now see the To2Do interface!**

## First Time Configuration

### Configure WiFi (Recommended)

1. In the To2Do interface, click the **⚙ Settings** button
2. Click the **Connection** tab
3. Expand "PRIMARY WIFI" section
4. Enter your WiFi details:
   - WIFI SSID: Your network name
   - WIFI PASSWORD: Your network password
   - (Optional) mDNS ADDRESS: Custom name like "mytodo"
5. Click **SAVE NETWORK SETTINGS**
6. Device will restart and connect to your WiFi
7. Find the new IP in serial monitor or use mDNS (e.g., http://mytodo.local)

### Basic Usage

1. **Create Your First Project**
   - Click **+ PROJECT** button
   - Enter project name (e.g., "My First Project")
   - Select category (Work, Personal, or Projects)
   - Set priority level
   - Click **Save Project**

2. **Add Tasks**
   - Click on your new project to select it
   - Click the floating **+** button (bottom right)
   - Enter task details
   - Set priority and due date
   - Click **Save Task**

3. **Manage Tasks**
   - Click checkbox to mark as complete
   - Click ✎ to edit
   - Click 🗑 to delete
   - Use filters to view: All, Active, Completed, High Priority

## Advanced Configuration

### Email Notifications

To receive email reminders:

1. Go to Settings → Mail tab
2. Toggle to **ENABLED**
3. Expand "SMTP CONFIGURATION"
4. Enter your email settings:
   - **For Gmail users:**
     - SMTP SERVER: `smtp.gmail.com`
     - SMTP PORT: `587`
     - USERNAME: Your Gmail address
     - PASSWORD: App Password (not your regular password!)
     - FROM EMAIL: Your Gmail address
     - Enable SSL/TLS: ✓

5. **Getting Gmail App Password:**
   - Go to https://myaccount.google.com
   - Security → 2-Step Verification (enable if not already)
   - Search for "App passwords"
   - Generate password for "Mail"
   - Use this 16-character password in To2Do settings

### Backup Your Data

**Export:**
- Settings → Backup → EXPORT
- Saves all your data to a JSON file
- Keep this file safe!

**Import:**
- Settings → Backup → IMPORT
- Select your backup JSON file
- Restores all projects and tasks

### Customization

**Change Theme:**
- Settings → General → Toggle AYDINLIK/KOYU (Light/Dark)

**Rename Categories:**
- Settings → General → Category Names
- Change from default (Work, Personal, Projects) to your own

**Change App Title:**
- Settings → General → Application Title
- Customize the title shown in interface

## Troubleshooting

### Can't Connect to Web Interface

**Problem:** Browser can't reach http://192.168.4.1

**Solutions:**
- Check serial monitor - is device showing "READY"?
- Verify you're connected to `SmartKraft-To2Do` WiFi
- Try http://to2do.local instead
- Restart the ESP32 (unplug and replug USB)

### ESP32 Not Uploading

**Problem:** Upload fails or port not found

**Solutions:**
- Install ESP32 drivers (CP2102 or CH340, depending on your board)
- Try a different USB cable (some are power-only)
- Press and hold BOOT button while uploading
- Check board settings: ESP32 Dev Module, 115200 baud, 4MB flash

### Email Not Sending

**Problem:** SMTP notifications don't work

**Solutions:**
- For Gmail: Use App Password, NOT your regular password
- Verify SMTP server and port are correct
- Check ESP32 has internet access (not in AP-only mode)
- Enable "Less secure app access" if using older email providers

### Data Not Saving

**Problem:** Changes lost after restart

**Solutions:**
- Check serial monitor for SPIFFS errors
- Try factory reset: Settings → Backup → FACTORY RESET
- Re-upload the sketch with "Erase All Flash" option

## Serial Monitor Output

The serial monitor (115200 baud) shows useful information:

```
=== SmartKraft-ToDo Starting ===
[DataManager] Initialized with 24KB buffer
[DataManager] ✓ Loaded data from /userdata.json
[WiFiManager] Starting AP Mode
[WiFiManager] ✓ AP Started: SmartKraft-To2Do
[WiFiManager] IP: 192.168.4.1
[WiFiManager] mDNS: to2do.local

=== SmartKraft-ToDo READY ===
Mode: AP
IP: 192.168.4.1
Access: http://to2do.local
```

## Next Steps

- Explore the interface and add your real projects
- Set up email notifications for important tasks
- Create a backup of your data
- Customize categories to match your workflow
- Consider adding an OLED display (future feature)

## Getting Help

- Check the main README.md for detailed documentation
- Review the development notes in `eksik kalanlar.txt`
- Check serial monitor for error messages
- Verify all libraries are installed correctly

## Tips & Tricks

1. **Quick Access:** Bookmark http://to2do.local for easy access
2. **Multiple Devices:** Access from phone, tablet, computer simultaneously
3. **Offline Use:** Works perfectly without internet (except email)
4. **Regular Backups:** Export your data weekly
5. **Category Organization:** Use categories to separate work/life balance
6. **Priority System:** Use High for urgent, Medium for important, Low for someday
7. **Search Function:** Use project search to quickly find what you need

Enjoy your To2Do system! 🎉
