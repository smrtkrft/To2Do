# API Reference

This document describes the REST API endpoints provided by the To2Do ESP32 web server.

## Base URL

When running on the device:
- AP Mode: `http://192.168.4.1`
- WiFi Mode: `http://[device-ip]` or `http://[mdns-name].local`

Example: `http://to2do.local`

## Response Format

All responses are in JSON format unless otherwise specified.

### Success Response
```json
{
  "success": true,
  "message": "Optional success message"
}
```

### Error Response
```json
{
  "error": "Error description"
}
```

## Endpoints

### Web Interface

#### `GET /`
Serves the main HTML interface.

**Response**: HTML document

---

#### `GET /js/core.js`
Serves the JavaScript core application.

**Response**: JavaScript code

---

### Todos API

#### `GET /api/todos`
Retrieve all projects and tasks.

**Response**:
```json
{
  "projects": [
    {
      "id": 1,
      "name": "Website Redesign",
      "category": "Work",
      "description": "Company website modernization project",
      "priority": "high",
      "archived": false
    }
  ],
  "tasks": [
    {
      "id": 1,
      "name": "Design homepage mockup",
      "description": "Create wireframes and mockups",
      "priority": "high",
      "dueDate": "2024-10-20",
      "completed": false,
      "projectId": 1
    }
  ]
}
```

---

#### `POST /api/todos`
Save all projects and tasks.

**Request Body**:
```json
{
  "projects": [...],
  "tasks": [...]
}
```

**Response**:
```json
{
  "success": true
}
```

---

### Settings API

#### `GET /api/settings`
Retrieve GUI settings.

**Response**:
```json
{
  "theme": "dark",
  "appTitle": "ToDo - SmartKraft",
  "cat1": "Work",
  "cat2": "Personal",
  "cat3": "Projects"
}
```

---

#### `POST /api/settings`
Save GUI settings.

**Request Body**:
```json
{
  "theme": "dark",
  "appTitle": "My Tasks",
  "cat1": "Work",
  "cat2": "Personal",
  "cat3": "Projects"
}
```

**Response**:
```json
{
  "success": true
}
```

---

### Network API

#### `GET /api/network/status`
Get current network status.

**Response**:
```json
{
  "mode": "AP",
  "ip": "192.168.4.1",
  "mdns": "to2do.local",
  "ssid": "SmartKraft-To2Do"
}
```

---

#### `GET /api/network/settings`
Get network configuration settings.

**Response**:
```json
{
  "apSSID": "SmartKraft-To2Do",
  "apMDNS": "to2do",
  "primarySSID": "HomeWiFi",
  "primaryPassword": "***",
  "primaryIP": "",
  "primaryMDNS": "to2do",
  "backupSSID": "",
  "backupPassword": "",
  "backupIP": "",
  "backupMDNS": ""
}
```

---

#### `POST /api/network/config`
Update network configuration. Device will restart after applying.

**Request Body**:
```json
{
  "apSSID": "SmartKraft-To2Do",
  "apMDNS": "to2do",
  "primarySSID": "MyWiFi",
  "primaryPassword": "password123",
  "primaryIP": "",
  "primaryMDNS": "mytodo",
  "backupSSID": "",
  "backupPassword": "",
  "backupIP": "",
  "backupMDNS": ""
}
```

**Response**:
```json
{
  "success": true,
  "message": "Settings saved and connecting..."
}
```

**Note**: Device will restart to apply new WiFi settings.

---

### System API

#### `GET /api/system/info`
Get system information.

**Response**:
```json
{
  "version": "SmartKraft-ToDo V1.1",
  "chipModel": "ESP32-D0WDQ6",
  "chipCores": 2,
  "cpuFreq": 240,
  "freeHeap": 153264,
  "heapSize": 327680,
  "flashSize": 4194304,
  "spiffsTotal": 1441792,
  "spiffsUsed": 45056,
  "spiffsFree": 1396736,
  "uptime": 3600
}
```

**Fields**:
- `version`: Software version string
- `chipModel`: ESP32 chip model
- `chipCores`: Number of CPU cores
- `cpuFreq`: CPU frequency in MHz
- `freeHeap`: Free RAM in bytes
- `heapSize`: Total RAM in bytes
- `flashSize`: Flash memory size in bytes
- `spiffsTotal`: SPIFFS filesystem size in bytes
- `spiffsUsed`: SPIFFS used space in bytes
- `spiffsFree`: SPIFFS free space in bytes
- `uptime`: Uptime in seconds

---

#### `POST /api/system/factory-reset`
Perform factory reset. Deletes all user data and restores demo content.

**Request Body**: None (empty POST)

**Response**:
```json
{
  "success": true
}
```

**Note**: Device will restart after reset.

---

### Backup API

#### `GET /api/backup/export`
Export all user data as JSON.

**Response**: Complete JSON dump of all data
```json
{
  "projects": [...],
  "tasks": [...],
  "guiSettings": {...},
  "networkSettings": {...},
  "mailSettings": {...}
}
```

**Content-Type**: `application/json`

**Note**: This is the complete backup file. Save it for restoration.

---

#### `POST /api/backup/import`
Import user data from backup.

**Request Body**: Complete JSON backup (as exported)

**Response**:
```json
{
  "success": true
}
```

**Note**: Overwrites all existing data.

---

### Mail API

#### `GET /api/mail/config`
Get email configuration (password hidden).

**Response**:
```json
{
  "enabled": false,
  "smtpServer": "smtp.gmail.com",
  "smtpPort": 587,
  "smtpUser": "user@gmail.com",
  "smtpPassword": "",
  "smtpFrom": "user@gmail.com",
  "smtpSSL": true
}
```

**Note**: Password field is always empty for security.

---

#### `POST /api/mail/config`
Save email configuration.

**Request Body**:
```json
{
  "enabled": true,
  "smtpServer": "smtp.gmail.com",
  "smtpPort": 587,
  "smtpUser": "user@gmail.com",
  "smtpPassword": "app-password-here",
  "smtpFrom": "user@gmail.com",
  "smtpSSL": true
}
```

**Response**:
```json
{
  "success": true
}
```

---

### Notification API

#### `GET /api/notifications/today`
Get tasks due today.

**Response**:
```json
{
  "count": 3,
  "categories": {
    "Work": [
      {
        "id": 1,
        "name": "Task name",
        "dueDate": "2024-10-14"
      }
    ]
  }
}
```

---

#### `GET /api/notifications/tomorrow`
Get tasks due tomorrow.

**Response**: Same format as `/api/notifications/today`

---

#### `GET /api/notifications/week`
Get tasks due this week.

**Response**: Same format as `/api/notifications/today`

---

#### `GET /api/notifications/overdue`
Get overdue tasks.

**Response**: Same format as `/api/notifications/today`

---

#### `POST /api/timezone`
Set timezone for notifications.

**Request Body**:
```json
{
  "timezone": "America/New_York"
}
```

**Response**:
```json
{
  "success": true
}
```

**Valid Timezones**:
- `UTC`
- `America/New_York`
- `America/Los_Angeles`
- `Europe/London`
- `Europe/Berlin`
- `Asia/Tokyo`
- etc. (standard IANA timezone identifiers)

---

## Data Models

### Project Object

```json
{
  "id": 1,
  "name": "Project Name",
  "category": "Work",
  "description": "Project description",
  "priority": "high",
  "archived": false
}
```

**Fields**:
- `id` (integer): Unique identifier
- `name` (string): Project name
- `category` (string): One of the three categories
- `description` (string): Optional description
- `priority` (string): "low", "medium", or "high"
- `archived` (boolean): Archived status

### Task Object

```json
{
  "id": 1,
  "name": "Task Name",
  "description": "Task description",
  "priority": "medium",
  "dueDate": "2024-10-20",
  "completed": false,
  "projectId": 1
}
```

**Fields**:
- `id` (integer): Unique identifier
- `name` (string): Task name
- `description` (string): Optional description
- `priority` (string): "low", "medium", or "high"
- `dueDate` (string): ISO date format (YYYY-MM-DD)
- `completed` (boolean): Completion status
- `projectId` (integer): ID of parent project

## Error Codes

### HTTP Status Codes

- `200 OK`: Request successful
- `400 Bad Request`: Invalid request data
- `500 Internal Server Error`: Server-side error

### Error Messages

```json
{
  "error": "No data"
}
```
Invalid or missing request body.

```json
{
  "error": "Invalid JSON"
}
```
Request body is not valid JSON.

```json
{
  "error": "Save failed"
}
```
Data could not be saved to SPIFFS.

```json
{
  "error": "Import failed"
}
```
Backup import failed (invalid format or corruption).

```json
{
  "error": "Mail manager not ready"
}
```
Mail manager not initialized.

```json
{
  "error": "Backup manager not ready"
}
```
Backup manager not initialized.

```json
{
  "error": "Notification manager not ready"
}
```
Notification manager not initialized.

## Rate Limiting

No rate limiting is currently implemented. Be considerate with API calls to avoid overwhelming the ESP32.

## Authentication

No authentication is currently required. All endpoints are publicly accessible.

**Security Note**: To2Do is designed for personal/local network use. Do not expose to the internet without additional security measures.

## CORS

Cross-Origin Resource Sharing (CORS) is not currently configured. Access the API from the same origin (device's web interface).

## WebSocket Support

Not currently implemented. All communication is via standard HTTP requests.

## Example Usage

### JavaScript (Fetch API)

```javascript
// Get all todos
fetch('/api/todos')
  .then(response => response.json())
  .then(data => {
    console.log('Projects:', data.projects);
    console.log('Tasks:', data.tasks);
  });

// Create a new project
const newProject = {
  id: Date.now(),
  name: "New Project",
  category: "Work",
  description: "Description",
  priority: "medium",
  archived: false
};

// Add to existing data and save
fetch('/api/todos')
  .then(r => r.json())
  .then(data => {
    data.projects.push(newProject);
    return fetch('/api/todos', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(data)
    });
  })
  .then(r => r.json())
  .then(result => {
    console.log('Saved:', result.success);
  });

// Export backup
fetch('/api/backup/export')
  .then(r => r.text())
  .then(data => {
    // Download as file
    const blob = new Blob([data], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'backup.json';
    a.click();
  });
```

### cURL

```bash
# Get todos
curl http://to2do.local/api/todos

# Get system info
curl http://to2do.local/api/system/info

# Export backup
curl http://to2do.local/api/backup/export > backup.json

# Import backup
curl -X POST http://to2do.local/api/backup/import \
  -H "Content-Type: application/json" \
  -d @backup.json

# Factory reset
curl -X POST http://to2do.local/api/system/factory-reset

# Save settings
curl -X POST http://to2do.local/api/settings \
  -H "Content-Type: application/json" \
  -d '{"theme":"dark","appTitle":"My Tasks","cat1":"Work","cat2":"Home","cat3":"Fun"}'
```

### Python

```python
import requests
import json

# Get todos
response = requests.get('http://to2do.local/api/todos')
data = response.json()
print(f"Projects: {len(data['projects'])}")
print(f"Tasks: {len(data['tasks'])}")

# Export backup
response = requests.get('http://to2do.local/api/backup/export')
with open('backup.json', 'w') as f:
    f.write(response.text)

# Get system info
response = requests.get('http://to2do.local/api/system/info')
info = response.json()
print(f"Free Heap: {info['freeHeap']} bytes")
print(f"Uptime: {info['uptime']} seconds")
```

## Notes

- All endpoints return JSON except `/` and `/js/core.js`
- POST requests require `Content-Type: application/json` header
- Device restart is automatic after network configuration changes
- Backup files can be large (up to 24KB)
- Mail password is never returned in GET requests for security

## Future API Additions

Planned endpoints:
- `POST /api/auth/login` - Authentication
- `POST /api/auth/logout` - Logout
- `GET /api/tasks/search` - Search tasks
- `GET /api/projects/archived` - Get archived projects
- `POST /api/tasks/{id}/complete` - Quick task completion
- `GET /api/stats` - Statistics and analytics
- `WebSocket /ws` - Real-time updates
