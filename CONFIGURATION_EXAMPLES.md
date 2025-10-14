# Configuration Examples for To2Do

This file contains example configurations for common email providers and network setups.

## Email (SMTP) Configuration Examples

### Gmail

```
SMTP SERVER: smtp.gmail.com
SMTP PORT: 587
USERNAME: your-email@gmail.com
PASSWORD: [16-character App Password]
FROM EMAIL: your-email@gmail.com
SSL/TLS: Enabled
```

**Important for Gmail:**
1. Enable 2-Factor Authentication on your Google Account
2. Generate App Password at: https://myaccount.google.com/apppasswords
3. Use the 16-character App Password (not your regular Gmail password)
4. Select "Mail" as the app when generating

### Outlook/Office 365

```
SMTP SERVER: smtp-mail.outlook.com
SMTP PORT: 587
USERNAME: your-email@outlook.com
PASSWORD: [Your Outlook password]
FROM EMAIL: your-email@outlook.com
SSL/TLS: Enabled
```

**Note:** You may need to enable "Less secure app access" in Outlook settings

### Yahoo Mail

```
SMTP SERVER: smtp.mail.yahoo.com
SMTP PORT: 587
USERNAME: your-email@yahoo.com
PASSWORD: [App Password]
FROM EMAIL: your-email@yahoo.com
SSL/TLS: Enabled
```

**Important for Yahoo:**
1. Generate App Password at: https://login.yahoo.com/account/security
2. Use the App Password (not your regular Yahoo password)

### Custom SMTP Server

```
SMTP SERVER: mail.yourdomain.com
SMTP PORT: 587 (or 465 for SSL, 25 for no encryption)
USERNAME: your-email@yourdomain.com
PASSWORD: [Your email password]
FROM EMAIL: your-email@yourdomain.com
SSL/TLS: Enabled (recommended)
```

**Port Selection Guide:**
- Port 587: STARTTLS (most common, recommended)
- Port 465: SSL/TLS
- Port 25: No encryption (not recommended)

## Network Configuration Examples

### Example 1: Simple Home WiFi

**AP Mode Settings:**
```
AP SSID: SmartKraft-To2Do
mDNS ADDRESS: to2do
```

**Primary WiFi:**
```
WIFI SSID: MyHomeWiFi
WIFI PASSWORD: MySecurePassword123
STATIC IP: (leave empty for DHCP)
mDNS ADDRESS: to2do
```

**Access:**
- AP Mode: http://192.168.4.1 or http://to2do.local
- WiFi Mode: http://to2do.local or check router for IP

### Example 2: Office Network with Static IP

**Primary WiFi:**
```
WIFI SSID: OfficeNetwork
WIFI PASSWORD: OfficePass2024
STATIC IP: 192.168.1.100
mDNS ADDRESS: office-todo
```

**Access:**
- http://192.168.1.100
- http://office-todo.local

### Example 3: Home with Backup WiFi

**Primary WiFi (Main Router):**
```
WIFI SSID: HomeWiFi-5G
WIFI PASSWORD: MainPassword123
STATIC IP: (empty - use DHCP)
mDNS ADDRESS: home-todo
```

**Backup WiFi (Guest Network):**
```
WIFI SSID: HomeWiFi-Guest
WIFI PASSWORD: GuestPass456
STATIC IP: (empty - use DHCP)
mDNS ADDRESS: home-todo-backup
```

**Behavior:** Device tries Primary first, falls back to Backup if Primary unavailable

### Example 4: Mobile Hotspot

**Primary WiFi:**
```
WIFI SSID: iPhone-Hotspot
WIFI PASSWORD: hotspot123
STATIC IP: (empty - use DHCP)
mDNS ADDRESS: mobile-todo
```

**Good for:** Portable use, working on the go

## GUI Customization Examples

### Example 1: Work-Focused Setup

```
APP TITLE: TaskMaster Pro
CATEGORY 1: Projects
CATEGORY 2: Meetings
CATEGORY 3: Research
THEME: Dark
```

### Example 2: Personal Life Organization

```
APP TITLE: My Life Manager
CATEGORY 1: Home
CATEGORY 2: Health
CATEGORY 3: Hobbies
THEME: Light
```

### Example 3: Student Setup

```
APP TITLE: Study Planner
CATEGORY 1: Classes
CATEGORY 2: Homework
CATEGORY 3: Projects
THEME: Dark
```

### Example 4: Small Business

```
APP TITLE: Business Tasks
CATEGORY 1: Clients
CATEGORY 2: Operations
CATEGORY 3: Marketing
THEME: Dark
```

## Sample Project Structures

### Software Development Team

**Category: Projects**
- Projects:
  - Website Redesign
  - Mobile App Update
  - API Integration
  - Bug Fixes Sprint

**Category: Meetings**
- Projects:
  - Daily Standups
  - Sprint Planning
  - Client Reviews
  - Team Retrospectives

**Category: Research**
- Projects:
  - New Technologies
  - Competitor Analysis
  - Documentation
  - Code Reviews

### Personal Productivity

**Category: Work**
- Projects:
  - Q1 Reports
  - Email Management
  - Team Coordination
  - Professional Development

**Category: Personal**
- Projects:
  - Home Improvement
  - Fitness Goals
  - Financial Planning
  - Shopping Lists

**Category: Hobbies**
- Projects:
  - Photography Projects
  - Learning Guitar
  - Garden Planning
  - Book Reading List

## Advanced Network Scenarios

### Scenario 1: Corporate Network with Proxy

**Note:** ESP32 doesn't support HTTP proxy natively. For corporate networks:
- Use guest WiFi if available
- Set up a bridge/gateway device
- Use AP mode and connect directly

### Scenario 2: Hidden SSID Network

**Primary WiFi:**
```
WIFI SSID: MyHiddenNetwork (type exact name)
WIFI PASSWORD: SecurePassword123
STATIC IP: (optional)
mDNS ADDRESS: todo
```

**Important:** Type the SSID exactly as configured on your router

### Scenario 3: Multi-Location Setup

**Location 1 (Home):**
```
PRIMARY WIFI:
  SSID: HomeNetwork
  PASSWORD: HomePass123
  mDNS: home-todo
```

**Location 2 (Office):**
```
BACKUP WIFI:
  SSID: OfficeNetwork
  PASSWORD: OfficePass456
  mDNS: office-todo
```

**Behavior:** Automatically connects to whichever network is available

## Timezone Configuration

**Note:** Timezone settings are for future email notification scheduling.

Common Timezone Examples:
```
UTC: UTC
Eastern Time: America/New_York
Central Europe: Europe/Berlin
UK: Europe/London
Pacific Time: America/Los_Angeles
India: Asia/Kolkata
Japan: Asia/Tokyo
Australia: Australia/Sydney
```

Set via API:
```javascript
POST /api/timezone
{
  "timezone": "America/New_York"
}
```

## Backup Configuration

### Backup Strategy Examples

**Daily User:**
- Export backup weekly
- Store in cloud (Google Drive, Dropbox)
- Name format: `To2Do_Backup_2024-10-14.json`

**Critical Data:**
- Export backup after each major change
- Keep multiple versions (rotate weekly)
- Store in 2+ locations

**Casual User:**
- Export backup monthly
- Keep one backup file
- Store locally on computer

## Factory Reset Scenarios

### When to Use Factory Reset:

1. **Corrupted Data:** Data file damaged, can't load
2. **Fresh Start:** Want to start over completely
3. **Testing:** Need demo data for demonstration
4. **Troubleshooting:** Eliminate user data as issue source

### After Factory Reset:

- All user projects/tasks deleted
- Demo content restored (7 sample projects)
- Settings reset to defaults
- Network settings preserved (won't need to reconnect WiFi)

## Security Best Practices

1. **WiFi Password:** Use strong, unique password
2. **SMTP Password:** Use app-specific passwords
3. **AP Mode:** Consider disabling when not needed
4. **Network Access:** Use on trusted networks
5. **Backups:** Encrypt backup files if they contain sensitive data
6. **Device Access:** Physical access = full data access
7. **Regular Updates:** Keep Arduino IDE and libraries updated

## Performance Tips

1. **Project Limit:** Keep under 50 projects for best performance
2. **Task Limit:** Under 200 tasks total recommended
3. **Description Length:** Keep descriptions concise
4. **Archive Old:** Archive completed projects regularly
5. **Regular Cleanup:** Delete obsolete tasks and projects
6. **Backup Before Bulk Changes:** Always export before major reorganization

## Troubleshooting Common Configurations

### Gmail "Authentication Failed"
- ✓ Using App Password, not regular password
- ✓ 2FA enabled on Google Account
- ✓ App Password is for "Mail" application
- ✓ No spaces in App Password
- ✓ Email address matches USERNAME and FROM EMAIL

### WiFi Won't Connect
- ✓ SSID spelled exactly correct (case-sensitive)
- ✓ Password correct (check for special characters)
- ✓ Router supports 2.4GHz (ESP32 can't use 5GHz)
- ✓ WiFi signal strong enough
- ✓ DHCP enabled on router (if not using static IP)

### mDNS Doesn't Work
- ✓ Device on same network as computer
- ✓ Firewall not blocking mDNS (port 5353)
- ✓ mDNS name is unique
- ✓ Router supports Bonjour/mDNS
- ✓ Try direct IP address instead

### Backup Import Fails
- ✓ File is valid JSON
- ✓ File from To2Do export (not manually edited)
- ✓ File size under 24KB
- ✓ All required fields present
- ✓ Data structure matches current version
