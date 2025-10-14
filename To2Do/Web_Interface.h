#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char* getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>To2Do - SmartKraft</title>
<style>
*{margin:0;padding:0;box-sizing:border-box;}
:root{
--bg-primary:#0a0a0a;
--bg-secondary:#151515;
--bg-tertiary:#1f1f1f;
--text-primary:#e8e8e8;
--text-secondary:#a8a8a8;
--accent-blue:#3b82f6;
--accent-green:#10b981;
--accent-orange:#f59e0b;
--accent-red:#ef4444;
--border-color:#2a2a2a;
--shadow:0 4px 6px rgba(0,0,0,0.3);
}
body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:var(--bg-primary);color:var(--text-primary);overflow-x:hidden;}
.workspace{display:flex;height:100vh;}
.sidebar{width:320px;background:var(--bg-secondary);border-right:1px solid var(--border-color);display:flex;flex-direction:column;overflow:hidden;}
.sidebar-header{padding:20px;background:var(--bg-tertiary);border-bottom:1px solid var(--border-color);}
.sidebar-title{font-size:24px;font-weight:700;margin-bottom:15px;color:var(--accent-blue);}
.sidebar-stats{display:flex;gap:10px;margin-top:15px;}
.stat{flex:1;text-align:center;padding:10px;background:var(--bg-secondary);border-radius:8px;}
.stat-value{font-size:20px;font-weight:700;color:var(--accent-blue);}
.stat-label{font-size:11px;color:var(--text-secondary);margin-top:4px;}
.sidebar-tabs{display:flex;padding:15px;gap:8px;background:var(--bg-tertiary);border-bottom:1px solid var(--border-color);}
.sidebar-tab{flex:1;padding:10px;background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:6px;cursor:pointer;text-align:center;font-size:12px;font-weight:600;color:var(--text-secondary);transition:all 0.2s;}
.sidebar-tab:hover{background:var(--bg-tertiary);color:var(--text-primary);}
.sidebar-tab.active{background:var(--accent-blue);color:#fff;border-color:var(--accent-blue);}
.sidebar-search{padding:15px;background:var(--bg-tertiary);border-bottom:1px solid var(--border-color);}
.search-input{width:100%;padding:10px 12px;background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:6px;color:var(--text-primary);font-size:13px;}
.search-input:focus{outline:none;border-color:var(--accent-blue);}
.sidebar-projects{flex:1;overflow-y:auto;padding:10px;}
.project-item{padding:12px;margin-bottom:8px;background:var(--bg-tertiary);border:1px solid var(--border-color);border-radius:8px;cursor:pointer;transition:all 0.2s;}
.project-item:hover{background:var(--bg-secondary);border-color:var(--accent-blue);}
.project-item.active{border-color:var(--accent-blue);background:var(--bg-secondary);}
.project-name{font-size:14px;font-weight:600;margin-bottom:4px;}
.project-meta{display:flex;gap:10px;margin-top:6px;font-size:11px;color:var(--text-secondary);}
.priority-badge{padding:3px 8px;border-radius:4px;font-size:10px;font-weight:600;}
.priority-high{background:#ef444433;color:#ef4444;}
.priority-medium{background:#f59e0b33;color:#f59e0b;}
.priority-low{background:#10b98133;color:#10b981;}
.sidebar-footer{padding:15px;background:var(--bg-tertiary);border-top:1px solid var(--border-color);display:flex;gap:8px;}
.footer-btn{flex:1;padding:10px;background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:6px;cursor:pointer;font-size:13px;font-weight:600;color:var(--text-primary);transition:all 0.2s;text-align:center;}
.footer-btn:hover{background:var(--accent-blue);border-color:var(--accent-blue);color:#fff;}
.main-content{flex:1;display:flex;flex-direction:column;overflow:hidden;}
.content-header{padding:20px 30px;background:var(--bg-secondary);border-bottom:1px solid var(--border-color);display:flex;justify-content:space-between;align-items:center;}
.project-title{font-size:28px;font-weight:700;}
.header-actions{display:flex;gap:12px;}
.action-btn{padding:10px 16px;background:var(--bg-tertiary);border:1px solid var(--border-color);border-radius:6px;cursor:pointer;font-size:13px;font-weight:600;color:var(--text-primary);transition:all 0.2s;}
.action-btn:hover{background:var(--accent-blue);border-color:var(--accent-blue);color:#fff;}
.content-filters{padding:15px 30px;background:var(--bg-tertiary);border-bottom:1px solid var(--border-color);display:flex;gap:10px;}
.filter-btn{padding:8px 14px;background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:6px;cursor:pointer;font-size:12px;font-weight:600;color:var(--text-secondary);transition:all 0.2s;}
.filter-btn:hover{background:var(--bg-tertiary);color:var(--text-primary);}
.filter-btn.active{background:var(--accent-blue);color:#fff;border-color:var(--accent-blue);}
.content-body{flex:1;overflow-y:auto;padding:20px 30px;}
.task-list{display:flex;flex-direction:column;gap:10px;}
.task-item{padding:16px;background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:8px;display:flex;align-items:center;gap:15px;transition:all 0.2s;}
.task-item:hover{background:var(--bg-tertiary);border-color:var(--accent-blue);}
.task-checkbox{width:20px;height:20px;border:2px solid var(--border-color);border-radius:4px;cursor:pointer;flex-shrink:0;}
.task-checkbox.checked{background:var(--accent-green);border-color:var(--accent-green);}
.task-content{flex:1;}
.task-name{font-size:15px;font-weight:600;margin-bottom:4px;}
.task-desc{font-size:12px;color:var(--text-secondary);margin-top:4px;}
.task-actions{display:flex;gap:8px;}
.task-action-btn{padding:6px 12px;background:var(--bg-tertiary);border:1px solid var(--border-color);border-radius:4px;cursor:pointer;font-size:11px;color:var(--text-secondary);transition:all 0.2s;}
.task-action-btn:hover{background:var(--accent-blue);border-color:var(--accent-blue);color:#fff;}
.fab{position:fixed;bottom:30px;right:30px;width:60px;height:60px;background:var(--accent-blue);border-radius:50%;display:flex;align-items:center;justify-content:center;cursor:pointer;box-shadow:var(--shadow);font-size:28px;color:#fff;transition:all 0.2s;}
.fab:hover{transform:scale(1.1);background:#2563eb;}
.modal{position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,0.7);display:none;align-items:center;justify-content:center;z-index:1000;}
.modal.active{display:flex;}
.modal-content{background:var(--bg-secondary);border:1px solid var(--border-color);border-radius:12px;width:90%;max-width:600px;max-height:90vh;overflow-y:auto;padding:30px;}
.modal-header{font-size:24px;font-weight:700;margin-bottom:20px;padding-bottom:15px;border-bottom:1px solid var(--border-color);}
.modal-body{display:flex;flex-direction:column;gap:15px;}
.form-group{display:flex;flex-direction:column;gap:6px;}
.form-label{font-size:13px;font-weight:600;color:var(--text-secondary);}
.form-input,.form-textarea,.form-select{padding:10px 12px;background:var(--bg-tertiary);border:1px solid var(--border-color);border-radius:6px;color:var(--text-primary);font-size:13px;}
.form-input:focus,.form-textarea:focus,.form-select:focus{outline:none;border-color:var(--accent-blue);}
.form-textarea{resize:vertical;min-height:80px;}
.modal-actions{display:flex;gap:10px;margin-top:20px;padding-top:20px;border-top:1px solid var(--border-color);}
.modal-btn{flex:1;padding:12px;border:1px solid var(--border-color);border-radius:6px;cursor:pointer;font-size:14px;font-weight:600;transition:all 0.2s;}
.btn-primary{background:var(--accent-blue);color:#fff;border-color:var(--accent-blue);}
.btn-primary:hover{background:#2563eb;}
.btn-secondary{background:var(--bg-tertiary);color:var(--text-primary);}
.btn-secondary:hover{background:var(--bg-secondary);}
.settings-modal .modal-content{max-width:800px;}
.settings-tabs{display:flex;gap:10px;margin-bottom:20px;border-bottom:1px solid var(--border-color);}
.settings-tab{padding:12px 20px;cursor:pointer;font-size:14px;font-weight:600;color:var(--text-secondary);border-bottom:2px solid transparent;transition:all 0.2s;}
.settings-tab:hover{color:var(--text-primary);}
.settings-tab.active{color:var(--accent-blue);border-bottom-color:var(--accent-blue);}
.settings-tab-content{display:none;}
.settings-tab-content.active{display:block;}
.settings-section{margin-bottom:20px;}
.settings-label{display:block;font-size:13px;font-weight:600;color:var(--text-secondary);margin-bottom:8px;}
.settings-input{width:100%;padding:10px 12px;background:var(--bg-tertiary);border:1px solid var(--border-color);border-radius:6px;color:var(--text-primary);font-size:13px;}
.settings-hint{font-size:11px;color:var(--text-secondary);margin-top:4px;}
.theme-toggle{display:flex;align-items:center;gap:12px;padding:15px;background:var(--bg-tertiary);border-radius:8px;}
.toggle-switch{position:relative;width:50px;height:26px;}
.toggle-switch input{opacity:0;width:0;height:0;}
.toggle-slider{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background:#666;border-radius:26px;transition:0.3s;}
.toggle-slider:before{position:absolute;content:"";height:20px;width:20px;left:3px;bottom:3px;background:white;border-radius:50%;transition:0.3s;}
.toggle-switch input:checked + .toggle-slider{background:var(--accent-blue);}
.toggle-switch input:checked + .toggle-slider:before{transform:translateX(24px);}
.theme-label{font-size:13px;font-weight:600;color:var(--text-secondary);}
.accordion-item{margin-bottom:10px;border:1px solid var(--border-color);border-radius:8px;overflow:hidden;}
.accordion-header{padding:15px;background:var(--bg-tertiary);cursor:pointer;display:flex;justify-content:space-between;align-items:center;}
.accordion-title{font-size:14px;font-weight:600;}
.accordion-status{font-size:12px;color:var(--text-secondary);}
.accordion-content{max-height:0;overflow:hidden;transition:max-height 0.3s ease;}
.accordion-content.active{max-height:1000px;padding:15px;}
.settings-checkbox{display:flex;align-items:center;gap:10px;padding:12px;background:var(--bg-tertiary);border-radius:6px;cursor:pointer;}
.settings-checkbox input{width:18px;height:18px;}
.settings-btn{padding:10px 20px;background:var(--accent-blue);color:#fff;border:none;border-radius:6px;cursor:pointer;font-size:13px;font-weight:600;transition:all 0.2s;}
.settings-btn:hover{background:#2563eb;}
.info-status-bar{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:15px;margin-bottom:20px;}
.info-status-item{padding:15px;background:var(--bg-tertiary);border-radius:8px;}
.info-status-label{font-size:11px;color:var(--text-secondary);margin-bottom:6px;display:block;}
.info-status-value{font-size:16px;font-weight:700;color:var(--text-primary);}
.backup-actions{display:flex;gap:10px;margin-top:10px;}
</style>
</head>
<body>
<div class="workspace">
<div class="sidebar">
<div class="sidebar-header">
<div class="sidebar-title" id="app-title">ToDo - SmartKraft</div>
<div class="sidebar-stats">
<div class="stat"><div class="stat-value" id="total-tasks">0</div><div class="stat-label">TOPLAM</div></div>
<div class="stat"><div class="stat-value" id="active-tasks">0</div><div class="stat-label">AKTIF</div></div>
<div class="stat"><div class="stat-value" id="total-projects">0</div><div class="stat-label">PROJE</div></div>
</div>
</div>
<div class="sidebar-tabs">
<button class="sidebar-tab" data-category="Work" id="tab-cat1">WORK</button>
<button class="sidebar-tab active" data-category="Personal" id="tab-cat2">PERSONAL</button>
<button class="sidebar-tab" data-category="Projects" id="tab-cat3">PROJECTS</button>
</div>
<div class="sidebar-search">
<input type="text" class="search-input" id="project-search" placeholder="Search projects...">
</div>
<div class="sidebar-projects" id="projects-list"></div>
<div class="sidebar-footer">
<button class="footer-btn" id="new-project-btn">+ PROJECT</button>
<button class="footer-btn" id="settings-btn">⚙ SETTINGS</button>
</div>
</div>
<div class="main-content">
<div class="content-header">
<div class="project-title" id="current-project-title">Select a project</div>
<div class="header-actions">
<button class="action-btn" id="edit-project-btn">✎ Edit</button>
<button class="action-btn" id="archive-project-btn">📦 Archive</button>
</div>
</div>
<div class="content-filters">
<button class="filter-btn active" data-filter="all">All</button>
<button class="filter-btn" data-filter="active">Active</button>
<button class="filter-btn" data-filter="completed">Completed</button>
<button class="filter-btn" data-filter="high">High Priority</button>
</div>
<div class="content-body">
<div class="task-list" id="tasks-list"></div>
</div>
</div>
</div>
<div class="fab" id="fab-new-task">+</div>

<!-- Task Modal -->
<div class="modal" id="task-modal">
<div class="modal-content">
<div class="modal-header" id="task-modal-title">New Task</div>
<div class="modal-body">
<div class="form-group">
<label class="form-label">Task Name</label>
<input type="text" class="form-input" id="task-name" placeholder="Enter task name">
</div>
<div class="form-group">
<label class="form-label">Description</label>
<textarea class="form-textarea" id="task-desc" placeholder="Task description"></textarea>
</div>
<div class="form-group">
<label class="form-label">Priority</label>
<select class="form-select" id="task-priority">
<option value="low">Low</option>
<option value="medium" selected>Medium</option>
<option value="high">High</option>
</select>
</div>
<div class="form-group">
<label class="form-label">Due Date</label>
<input type="date" class="form-input" id="task-due-date">
</div>
</div>
<div class="modal-actions">
<button class="modal-btn btn-secondary" onclick="closeTaskModal()">Cancel</button>
<button class="modal-btn btn-primary" id="save-task-btn">Save Task</button>
</div>
</div>
</div>

<!-- Project Modal -->
<div class="modal" id="project-modal">
<div class="modal-content">
<div class="modal-header" id="project-modal-title">New Project</div>
<div class="modal-body">
<div class="form-group">
<label class="form-label">Project Name</label>
<input type="text" class="form-input" id="project-name" placeholder="Enter project name">
</div>
<div class="form-group">
<label class="form-label">Description</label>
<textarea class="form-textarea" id="project-desc" placeholder="Project description"></textarea>
</div>
<div class="form-group">
<label class="form-label">Category</label>
<select class="form-select" id="project-category">
<option value="Work">Work</option>
<option value="Personal" selected>Personal</option>
<option value="Projects">Projects</option>
</select>
</div>
<div class="form-group">
<label class="form-label">Priority</label>
<select class="form-select" id="project-priority">
<option value="low">Low</option>
<option value="medium" selected>Medium</option>
<option value="high">High</option>
</select>
</div>
</div>
<div class="modal-actions">
<button class="modal-btn btn-secondary" onclick="closeProjectModal()">Cancel</button>
<button class="modal-btn btn-primary" id="save-project-btn">Save Project</button>
</div>
</div>
</div>

<!-- Settings Modal -->
<div class="modal settings-modal" id="settings-modal">
<div class="modal-content">
<div class="modal-header">Settings</div>
<div class="settings-tabs">
<div class="settings-tab active" data-tab="general">General</div>
<div class="settings-tab" data-tab="connection">Connection</div>
<div class="settings-tab" data-tab="mail">Mail</div>
<div class="settings-tab" data-tab="backup">Backup</div>
<div class="settings-tab" data-tab="info">Info</div>
</div>
<div class="settings-tab-content active" data-content="general">
<div class="settings-section">
<label class="settings-label">TEMA</label>
<div class="theme-toggle" style="justify-content:center;">
<span class="theme-label">AYDINLIK</span>
<label class="toggle-switch">
<input type="checkbox" id="theme-toggle" checked>
<span class="toggle-slider"></span>
</label>
<span class="theme-label">KOYU</span>
</div>
</div>
<div class="settings-section">
<label class="settings-label">UYGULAMA BASLIGI</label>
<input type="text" class="settings-input" id="setting-app-title" placeholder="ToDo - SmartKraft">
</div>
<div class="settings-section">
<label class="settings-label">KATEGORI ISIMLERI</label>
<input type="text" class="settings-input" id="setting-cat1" placeholder="Kategori 1">
<input type="text" class="settings-input" id="setting-cat2" placeholder="Kategori 2" style="margin-top:8px;">
<input type="text" class="settings-input" id="setting-cat3" placeholder="Kategori 3" style="margin-top:8px;">
</div>
</div>
<div class="settings-tab-content" data-content="connection">
<div class="accordion">
<div class="accordion-item">
<div class="accordion-header" data-accordion="ap-mode">
<span class="accordion-title">▼ AP MODE SETTINGS</span>
<span class="accordion-status" id="ap-status">Active</span>
</div>
<div class="accordion-content active" data-content="ap-mode">
<div class="settings-section">
<label class="settings-label">AP SSID</label>
<input type="text" class="settings-input" id="ap-ssid" placeholder="SmartKraft-To2Do">
<p class="settings-hint">WiFi access point name</p>
</div>
<div class="settings-section">
<label class="settings-label">mDNS ADDRESS</label>
<input type="text" class="settings-input" id="ap-mdns" placeholder="to2do">
<p class="settings-hint">Access via http://to2do.local</p>
</div>
</div>
</div>
<div class="accordion-item">
<div class="accordion-header" data-accordion="wifi-primary">
<span class="accordion-title">▶ PRIMARY WIFI</span>
<span class="accordion-status" id="wifi-primary-status">Not Configured</span>
</div>
<div class="accordion-content" data-content="wifi-primary">
<div class="settings-section">
<label class="settings-label">WIFI SSID</label>
<input type="text" class="settings-input" id="wifi-primary-ssid" placeholder="Your WiFi name">
</div>
<div class="settings-section">
<label class="settings-label">WIFI PASSWORD</label>
<input type="password" class="settings-input" id="wifi-primary-pass" placeholder="WiFi password">
</div>
<div class="settings-section">
<label class="settings-label">STATIC IP (optional)</label>
<input type="text" class="settings-input" id="wifi-primary-ip" placeholder="192.168.1.100">
<p class="settings-hint">Leave empty for DHCP</p>
</div>
<div class="settings-section">
<label class="settings-label">mDNS ADDRESS</label>
<input type="text" class="settings-input" id="wifi-primary-mdns" placeholder="to2do">
</div>
</div>
</div>
<div class="accordion-item">
<div class="accordion-header" data-accordion="wifi-backup">
<span class="accordion-title">▶ BACKUP WIFI</span>
<span class="accordion-status" id="wifi-backup-status">Not Configured</span>
</div>
<div class="accordion-content" data-content="wifi-backup">
<div class="settings-section">
<label class="settings-label">WIFI SSID</label>
<input type="text" class="settings-input" id="wifi-backup-ssid" placeholder="Backup WiFi name">
</div>
<div class="settings-section">
<label class="settings-label">WIFI PASSWORD</label>
<input type="password" class="settings-input" id="wifi-backup-pass" placeholder="WiFi password">
</div>
<div class="settings-section">
<label class="settings-label">STATIC IP (optional)</label>
<input type="text" class="settings-input" id="wifi-backup-ip" placeholder="192.168.1.101">
</div>
<div class="settings-section">
<label class="settings-label">mDNS ADDRESS</label>
<input type="text" class="settings-input" id="wifi-backup-mdns" placeholder="to2do-backup">
</div>
</div>
</div>
</div>
<button class="settings-btn" onclick="saveNetworkSettings()" style="margin-top:20px;">SAVE NETWORK SETTINGS</button>
</div>
<div class="settings-tab-content" data-content="mail">
<div class="settings-section">
<label class="settings-label">EMAIL NOTIFICATIONS</label>
<div class="theme-toggle" style="justify-content:center;">
<span class="theme-label">DISABLED</span>
<label class="toggle-switch">
<input type="checkbox" id="mail-enabled-toggle">
<span class="toggle-slider"></span>
</label>
<span class="theme-label">ENABLED</span>
</div>
<p class="settings-hint" style="margin-top:10px;">When disabled, all SMTP operations are skipped (no RAM/time wasted)</p>
</div>
<div class="accordion">
<div class="accordion-item">
<div class="accordion-header" data-accordion="smtp-config">
<span class="accordion-title">▶ SMTP CONFIGURATION</span>
<span class="accordion-status" id="smtp-status">Not Configured</span>
</div>
<div class="accordion-content" data-content="smtp-config">
<div class="settings-section">
<label class="settings-label">SMTP SERVER</label>
<input type="text" class="settings-input" id="smtp-server" placeholder="smtp.gmail.com">
<p class="settings-hint">Example: smtp.gmail.com, smtp.office365.com</p>
</div>
<div class="settings-section">
<label class="settings-label">SMTP PORT</label>
<input type="number" class="settings-input" id="smtp-port" placeholder="587" value="587">
<p class="settings-hint">Common: 587 (TLS), 465 (SSL), 25 (unsecure)</p>
</div>
<div class="settings-section">
<label class="settings-label">USERNAME</label>
<input type="text" class="settings-input" id="smtp-user" placeholder="your-email@gmail.com">
</div>
<div class="settings-section">
<label class="settings-label">PASSWORD</label>
<div style="display:flex;gap:10px;align-items:center;">
<input type="password" class="settings-input" id="smtp-password" placeholder="App password" style="flex:1;">
<label class="settings-checkbox" style="width:auto;padding:8px;">
<input type="checkbox" id="show-smtp-password">
<span style="font-size:12px;">Show</span>
</label>
</div>
<p class="settings-hint">For Gmail: Use App Password (not your main password)</p>
</div>
<div class="settings-section">
<label class="settings-label">FROM EMAIL</label>
<input type="text" class="settings-input" id="smtp-from" placeholder="your-email@gmail.com">
<p class="settings-hint">Email address to send notifications from</p>
</div>
<div class="settings-section">
<label class="settings-label">USE SSL/TLS</label>
<label class="settings-checkbox">
<input type="checkbox" id="smtp-ssl" checked>
<span>Enable SSL/TLS encryption</span>
</label>
</div>
</div>
</div>
</div>
<button class="settings-btn" onclick="saveMailSettings()" style="margin-top:20px;">SAVE MAIL SETTINGS</button>
</div>
<div class="settings-tab-content" data-content="backup">
<div class="settings-section">
<label class="settings-label">BACKUP & RESTORE</label>
<p class="settings-hint">Export all your data to a JSON file or import from a backup</p>
<div class="backup-actions">
<button class="settings-btn" onclick="exportBackup()" style="margin:0;">EXPORT</button>
<button class="settings-btn" onclick="document.getElementById('backup-upload').click()" style="margin:0;">IMPORT</button>
</div>
<input type="file" id="backup-upload" accept=".json" style="display:none;" onchange="importBackup(event)">
</div>
<div class="settings-section" style="margin-top:30px;">
<label class="settings-label">FACTORY RESET</label>
<p class="settings-hint" style="color:var(--accent-red);">⚠️ This will delete all your data and restore demo content!</p>
<button class="settings-btn" onclick="factoryReset()" style="background:var(--accent-red);margin-top:10px;">FACTORY RESET</button>
</div>
</div>
<div class="settings-tab-content" data-content="info">
<div class="info-status-bar">
<div class="info-status-item">
<span class="info-status-label">Versiyon:</span>
<span class="info-status-value">SmartKraft-ToDo V1.1</span>
</div>
<div class="info-status-item">
<span class="info-status-label">WiFi:</span>
<span class="info-status-value" id="info-wifi-status">AP Mode</span>
</div>
<div class="info-status-item">
<span class="info-status-label">IP:</span>
<span class="info-status-value" id="info-ip">192.168.4.1</span>
</div>
<div class="info-status-item">
<span class="info-status-label">mDNS:</span>
<span class="info-status-value" id="info-mdns">to2do.local</span>
</div>
<div class="info-status-item">
<span class="info-status-label">Free Heap:</span>
<span class="info-status-value" id="info-heap">0 KB</span>
</div>
<div class="info-status-item">
<span class="info-status-label">SPIFFS:</span>
<span class="info-status-value" id="info-spiffs">0 / 0 KB</span>
</div>
</div>
</div>
<div class="modal-actions">
<button class="modal-btn btn-secondary" onclick="closeSettingsModal()">Close</button>
<button class="modal-btn btn-primary" onclick="saveGeneralSettings()">Save Settings</button>
</div>
</div>
</div>

<script src="/js/core.js"></script>
</body>
</html>
)rawliteral";
}

#endif
