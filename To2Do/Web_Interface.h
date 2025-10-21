#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include "Web_CSS.h"

const char* getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title id="page-title">ToDo - SmartKraft</title>
<link rel="stylesheet" href="/app.css">
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
<div class="sidebar-search"><input type="text" class="project-search" id="project-search" placeholder="Proje ara..."></div>
<div class="sidebar-body"><div class="projects-list" id="projects-list"></div></div>
<div class="sidebar-footer">
<button class="sidebar-btn" id="new-project-btn">+ YENI PROJE</button>
<button class="sidebar-btn" id="settings-btn">AYARLAR</button>
</div>
</div>
<div class="panel">
<div class="panel-header">
<div class="panel-title-row">
<div style="text-align:center;width:100%;"><div class="panel-title" id="current-project">PROJE SEC</div><div class="panel-subtitle" id="current-project-desc"></div></div>
</div>
<div class="panel-filters" style="justify-content:center;">
<button class="panel-filter-btn active" data-filter="all">TUMU</button>
<button class="panel-filter-btn" data-filter="active">AKTIF</button>
<button class="panel-filter-btn" data-filter="completed">TAMAMLANAN</button>
<button class="panel-filter-btn" data-filter="task">GOREV</button>
<button class="panel-filter-btn" data-filter="plan">PLAN</button>
<button class="panel-filter-btn" data-filter="note">NOT</button>
<button class="panel-filter-btn" data-filter="reminder">HATIRLATMA</button>
</div>
</div>
<div class="panel-body"><div class="tasks-container" id="tasks-container"><div class="empty-state"><h3>PROJE SECILMEDI</h3><p>Sol taraftan bir proje secin</p></div></div></div>
</div>
</div>
<button class="fab notification-fab" id="notification-btn" style="right:110px;">!<span id="notification-badge" class="notification-badge"></span></button>
<button class="fab" id="fab-new-task">+</button>
<div class="modal" id="task-modal">
<div class="modal-content">
<div class="modal-title">YENI GOREV</div>
<input type="text" class="modal-input" id="modal-task-title" placeholder="Baslik...">
<textarea class="modal-textarea" id="modal-task-desc" placeholder="Aciklama, detaylar, notlar..."></textarea>
<div class="modal-form-row">
<select class="modal-select" id="modal-task-type"><option value="task">Gorev</option><option value="plan">Plan</option><option value="note">Not</option><option value="reminder">Hatirlatma</option></select>
<select class="modal-select" id="modal-task-priority"><option value="low">Dusuk</option><option value="medium" selected>Orta</option><option value="high">Yuksek</option></select>
<input type="date" class="modal-date" id="modal-task-date">
</div>
<div class="modal-actions">
<button class="modal-btn" id="modal-task-cancel">IPTAL</button>
<button class="modal-btn primary" id="modal-task-create">OLUSTUR</button>
</div>
</div>
</div>
<div class="modal" id="edit-task-modal">
<div class="modal-content">
<div class="modal-title">GOREVI DUZENLE</div>
<input type="text" class="modal-input" id="edit-task-title" placeholder="Baslik...">
<textarea class="modal-textarea" id="edit-task-desc" placeholder="Aciklama, detaylar, notlar..."></textarea>
<div class="modal-form-row">
<select class="modal-select" id="edit-task-type"><option value="task">Gorev</option><option value="plan">Plan</option><option value="note">Not</option><option value="reminder">Hatirlatma</option></select>
<select class="modal-select" id="edit-task-priority"><option value="low">Dusuk</option><option value="medium">Orta</option><option value="high">Yuksek</option></select>
<input type="date" class="modal-date" id="edit-task-date">
</div>
<div class="modal-actions">
<button class="modal-btn" id="edit-task-cancel">IPTAL</button>
<button class="modal-btn primary" id="edit-task-save">KAYDET</button>
</div>
</div>
</div>
<div class="modal" id="project-modal">
<div class="modal-content">
<div class="modal-title">YENI PROJE</div>
<input type="text" class="modal-input" id="modal-project-name" placeholder="Proje adi...">
<textarea class="modal-textarea" id="modal-project-desc" placeholder="Proje aciklamasi..."></textarea>
<select class="modal-select" id="modal-project-priority"><option value="low">Dusuk Oncelik</option><option value="medium" selected>Orta Oncelik</option><option value="high">Yuksek Oncelik</option></select>
<div class="modal-actions">
<button class="modal-btn" id="modal-project-cancel">IPTAL</button>
<button class="modal-btn primary" id="modal-project-create">KAYDET</button>
<button class="modal-btn primary" id="modal-project-update" style="display:none;">GUNCELLE</button>
</div>
</div>
</div>
<div class="modal" id="notification-modal">
<div class="modal-content">
<div class="modal-title">BILDIRIMLER</div>
<div class="settings-tabs">
<button class="settings-tab active" data-tab="notif-today">BUGUN</button>
<button class="settings-tab" data-tab="notif-tomorrow">YARIN</button>
<button class="settings-tab" data-tab="notif-week">BU HAFTA</button>
<button class="settings-tab" data-tab="notif-overdue">GECMIS</button>
</div>
<div class="settings-content">
<div class="settings-tab-content active" data-content="notif-today" id="notif-today-content">
<p style="text-align:center;color:#666;">Loading...</p>
</div>
<div class="settings-tab-content" data-content="notif-tomorrow" id="notif-tomorrow-content">
<p style="text-align:center;color:#666;">Loading...</p>
</div>
<div class="settings-tab-content" data-content="notif-week" id="notif-week-content">
<p style="text-align:center;color:#666;">Loading...</p>
</div>
<div class="settings-tab-content" data-content="notif-overdue" id="notif-overdue-content">
<p style="text-align:center;color:#666;">Loading...</p>
</div>
</div>
<div class="modal-actions">
<button class="modal-btn" onclick="closeNotificationModal()">KAPAT</button>
</div>
</div>
</div>
<div class="modal" id="settings-modal">
<div class="modal-content">
<div class="modal-title">AYARLAR</div>
<div class="settings-tabs">
<button class="settings-tab active" data-tab="gui">GUI</button>
<button class="settings-tab" data-tab="connection">BAGLANTI</button>
<button class="settings-tab" data-tab="mail">MAIL</button>
<button class="settings-tab" data-tab="info">INFO</button>
</div>
<div class="settings-content">
<div class="settings-tab-content active" data-content="gui">
<div class="settings-section" style="text-align:center;">
<label class="settings-label">TEMA</label>
<div class="theme-toggle" style="justify-content:center;">
<span class="theme-label">ACIK</span>
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
<input type="text" class="settings-input" id="setting-cat2" placeholder="Kategori 2">
<input type="text" class="settings-input" id="setting-cat3" placeholder="Kategori 3">
</div>
<div class="settings-section">
<label class="settings-label">TARIH / SAAT</label>
<div style="display:grid;grid-template-columns:1fr 1fr;gap:15px;align-items:center;padding:20px;background:#0a0a0a;border:1px solid #333;border-radius:8px;">
<div style="text-align:center;">
<span style="display:block;font-size:11px;color:#666;margin-bottom:8px;letter-spacing:1px;">TARIH</span>
<span id="current-date" style="display:block;font-size:18px;color:#fff;font-weight:bold;font-family:'Courier New',monospace;">--.--.----</span>
</div>
<div style="text-align:center;">
<span style="display:block;font-size:11px;color:#666;margin-bottom:8px;letter-spacing:1px;">SAAT</span>
<span id="current-time" style="display:block;font-size:18px;color:#fff;font-weight:bold;font-family:'Courier New',monospace;">--:--</span>
</div>
</div>
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
<label class="settings-label">AP SSID (Access Point Name)</label>
<input type="text" class="settings-input" id="setting-ap-ssid" placeholder="SmartKraft-ToDo" value="SmartKraft-ToDo">
<p class="settings-hint">No password required. Open network for easy first-time setup.</p>
</div>
<div class="settings-section">
<label class="settings-label">mDNS HOSTNAME (AP Mode)</label>
<input type="text" class="settings-input" id="setting-ap-mdns" placeholder="smartkraft-todo" value="smartkraft-todo">
<p class="settings-hint">Access via: http://<span id="ap-mdns-preview">smartkraft-todo</span>.local</p>
</div>
</div>
</div>
<div class="accordion-item">
<div class="accordion-header" data-accordion="primary-wifi">
<span class="accordion-title">▶ PRIMARY WIFI</span>
<span class="accordion-status" id="primary-status">Not Connected</span>
</div>
<div class="accordion-content" data-content="primary-wifi">
<div class="settings-section">
<label class="settings-label">WiFi SSID (Network Name)</label>
<input type="text" class="settings-input" id="setting-primary-ssid" placeholder="Your WiFi Name">
</div>
<div class="settings-section">
<label class="settings-label">WiFi PASSWORD</label>
<input type="password" class="settings-input" id="setting-primary-password" placeholder="••••••••">
<label class="settings-checkbox">
<input type="checkbox" id="show-primary-password">
<span>Show Password</span>
</label>
</div>
<div class="settings-section">
<label class="settings-label">STATIC IP ADDRESS</label>
<input type="text" class="settings-input" id="setting-primary-ip" placeholder="192.168.1.100">
<p class="settings-hint">Leave empty for DHCP (automatic)</p>
</div>
<div class="settings-section">
<label class="settings-label">mDNS HOSTNAME</label>
<input type="text" class="settings-input" id="setting-primary-mdns" placeholder="smartkraft-todo">
<p class="settings-hint">Access via: http://<span id="primary-mdns-preview">smartkraft-todo</span>.local</p>
</div>
<button class="settings-btn" id="test-primary-wifi">TEST CONNECTION</button>
</div>
</div>
<div class="accordion-item">
<div class="accordion-header" data-accordion="backup-wifi">
<span class="accordion-title">▶ BACKUP WIFI</span>
<span class="accordion-status" id="backup-status">Not Configured</span>
</div>
<div class="accordion-content" data-content="backup-wifi">
<div class="settings-section">
<label class="settings-label">WiFi SSID (Network Name)</label>
<input type="text" class="settings-input" id="setting-backup-ssid" placeholder="Backup WiFi Name">
</div>
<div class="settings-section">
<label class="settings-label">WiFi PASSWORD</label>
<input type="password" class="settings-input" id="setting-backup-password" placeholder="••••••••">
<label class="settings-checkbox">
<input type="checkbox" id="show-backup-password">
<span>Show Password</span>
</label>
</div>
<div class="settings-section">
<label class="settings-label">STATIC IP ADDRESS</label>
<input type="text" class="settings-input" id="setting-backup-ip" placeholder="192.168.2.100">
<p class="settings-hint">Leave empty for DHCP (automatic)</p>
</div>
<div class="settings-section">
<label class="settings-label">mDNS HOSTNAME</label>
<input type="text" class="settings-input" id="setting-backup-mdns" placeholder="smartkraft-todo-backup">
<p class="settings-hint">Access via: http://<span id="backup-mdns-preview">smartkraft-todo-backup</span>.local</p>
</div>
<button class="settings-btn" id="test-backup-wifi">TEST CONNECTION</button>
</div>
</div>
</div>
<div class="connection-info">
<h4 class="connection-info-title">CURRENT CONNECTION STATUS</h4>
<div class="connection-status-grid">
<div class="connection-status-item">
<span class="status-label">Mode:</span>
<span class="status-value" id="current-mode">AP Mode</span>
</div>
<div class="connection-status-item">
<span class="status-label">Connected To:</span>
<span class="status-value" id="current-ssid">-</span>
</div>
<div class="connection-status-item">
<span class="status-label">IP Address:</span>
<span class="status-value" id="current-ip">192.168.4.1</span>
</div>
<div class="connection-status-item">
<span class="status-label">Signal Strength:</span>
<span class="status-value" id="current-signal">-</span>
</div>
</div>
</div>
</div>
<div class="settings-tab-content" data-content="mail">
<div class="settings-section" style="text-align:center;">
<label class="settings-label">SMTP MAIL INTEGRATION</label>
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
<p class="settings-hint">Common: 587 (TLS), 465 (SSL), 25 (No encryption)</p>
</div>
<div class="settings-section">
<label class="settings-label">SMTP USERNAME</label>
<input type="text" class="settings-input" id="smtp-user" placeholder="your@email.com">
</div>
<div class="settings-section">
<label class="settings-label">SMTP PASSWORD</label>
<input type="password" class="settings-input" id="smtp-password" placeholder="••••••••">
<label class="settings-checkbox">
<input type="checkbox" id="show-smtp-password">
<span>Show Password</span>
</label>
</div>
<div class="settings-section">
<label class="settings-label">FROM EMAIL</label>
<input type="email" class="settings-input" id="from-email" placeholder="sender@example.com">
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
<span class="info-status-value" id="info-mdns">smartkraft-todo.local</span>
</div>
</div>
<div class="info-section">
<h3 class="info-title">HIZLI BASLANGIC</h3>
<p class="info-text">1. Kategori seçin (<span id="info-cat1">KISISEL</span>, <span id="info-cat2">SMARTKRAFT</span>, <span id="info-cat3">GENEL</span>)</p>
<p class="info-text">2. "+ YENI PROJE" ile proje oluşturun</p>
<p class="info-text">3. Sağ alttaki "+" butonu ile görev ekleyin</p>
<p class="info-text">4. Görev kutusuna tıklayarak tamamlayın</p>
</div>
<div class="info-section">
<h3 class="info-title">OLED EKRAN KULLANIMI</h3>
<p class="info-text">Arkadaki butona basarak bilgi ekranları arasında geçiş yapabilir, görev takibi yapabilir, ağ bağlantı bilgilerine erişebilirsiniz.</p>
<p class="info-text"><strong>Güncelleme:</strong> Her 5 saniyede otomatik yenilenir</p>
</div>
<div class="info-section">
<h3 class="info-title">PROJE YONETIMI</h3>
<p class="info-text"><strong>Oluşturma:</strong> Sol alttaki "+ YENI PROJE" butonu</p>
<p class="info-text"><strong>Düzenleme:</strong> Proje üzerine sağ tıkla > "DUZENLE"</p>
<p class="info-text"><strong>Kopyalama:</strong> Sağ tıkla > "KOPYALA" (aynı görevlerle yeni proje)</p>
<p class="info-text"><strong>Arşivleme:</strong> Sağ tıkla > "ARSIVLE" (gizlenir ama silinmez)</p>
<p class="info-text"><strong>Silme:</strong> Sağ tıkla > "SIL" (kalıcı silme)</p>
</div>
<div class="info-section">
<h3 class="info-title">GOREV YONETIMI</h3>
<p class="info-text"><strong>Ekleme:</strong> Sağ alttaki "+" butonu</p>
<p class="info-text"><strong>Türler:</strong> Görev, Plan, Not, Hatırlatma</p>
<p class="info-text"><strong>Öncelik:</strong> Düşük (Yeşil), Orta (Sarı), Yüksek (Kırmızı)</p>
<p class="info-text"><strong>Alt Görevler:</strong> Her görevin içine checklist eklenebilir</p>
<p class="info-text"><strong>Tarih:</strong> Görevlere tarih atayın, bildirimlerde görünsün</p>
<p class="info-text"><strong>Bağımlılık:</strong> Görevler arası ilişki tanımlayın</p>
</div>
<div class="info-section">
<h3 class="info-title">AYARLAR</h3>
<p class="info-text"><strong>GUI:</strong> Başlık, kategori isimleri, tema (açık/koyu)</p>
<p class="info-text"><strong>Network:</strong> WiFi ayarları, mDNS adresi, test bağlantısı</p>
<p class="info-text"><strong>Mail:</strong> E-posta bildirimleri için SMTP ayarları</p>
<p class="info-text"><strong>Info:</strong> Sistem bilgileri ve kullanım kılavuzu</p>
</div>
<div class="info-section">
<h3 class="info-title">VERI SAKLAMA</h3>
<p class="info-text"><strong>Kalıcı Depolama:</strong> Tüm veriler ESP32 SPIFFS hafızasında saklanır</p>
<p class="info-text"><strong>Güvenli:</strong> Elektrik kesilse veya firmware güncellense bile veriler korunur</p>
<p class="info-text"><strong>Tarayıcı Senkronizasyonu:</strong> Tarih/saat otomatik senkronize edilir</p>
<p class="info-text"><strong>Yedekleme:</strong> BACKUP/RESTORE bölümünden JSON olarak yedekleyin</p>
</div>
<div class="accordion">
<div class="accordion-item">
<div class="accordion-header" data-accordion="backup">
<span class="accordion-title">▶ BACKUP / RESTORE</span>
<span class="accordion-status">Safe</span>
</div>
<div class="accordion-content" data-content="backup">
<div class="info-section" style="border-left:3px solid #444;margin:0;">
<p class="info-text"><strong>Backup your data (WiFi settings excluded)</strong></p>
<p class="info-text">Included: Projects, Tasks, GUI Settings</p>
<p class="info-text">Excluded: WiFi credentials (for security)</p>
<div style="display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-top:15px;">
<button class="settings-btn" onclick="fetch('/api/backup/export').then(r=>r.text()).then(data=>{const blob=new Blob([data],{type:'application/json'});const url=window.URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download='ToDo_Backup.json';document.body.appendChild(a);a.click();document.body.removeChild(a);window.URL.revokeObjectURL(url);})" style="margin:0;">EXPORT</button>
<button class="settings-btn" onclick="document.getElementById('backup-upload').click()" style="margin:0;">IMPORT</button>
</div>
<input type="file" id="backup-upload" accept=".json" style="display:none" onchange="if(this.files[0]){const f=new FileReader();f.onload=e=>{if(confirm('Restore backup? Current data will be overwritten.')){fetch('/api/backup/import',{method:'POST',headers:{'Content-Type':'application/json'},body:e.target.result}).then(r=>r.json()).then(d=>{alert(d.success?'Backup restored!':'Failed: '+d.error);if(d.success)location.reload();})}};f.readAsText(this.files[0])}">
</div>
</div>
</div>
<div class="accordion-item">
<div class="accordion-header" data-accordion="factory-reset">
<span class="accordion-title">▶ FACTORY RESET</span>
<span class="accordion-status error">Tehlikeli</span>
</div>
<div class="accordion-content" data-content="factory-reset">
<div class="info-section" style="border-left:3px solid #dc3545;margin:0;">
<p class="info-text">Bu islem <strong>TUM verileri</strong> kalici olarak silecektir:</p>
<p class="info-text">✗ Tum projeler ve gorevler</p>
<p class="info-text">✗ GUI ayarlari (tema, kategoriler)</p>
<p class="info-text">✗ WiFi network ayarlari</p>
<p class="info-text" style="margin-top:15px;color:#dc3545;"><strong>Bu islem geri alinamaz!</strong></p>
<button onclick="app.confirmFactoryReset()" style="margin-top:15px;padding:12px 24px;background:#dc3545;color:#fff;border:2px solid #dc3545;border-radius:4px;cursor:pointer;font-weight:bold;font-size:14px;width:100%;">FACTORY RESET</button>
</div>
</div>
</div>
</div>
<div class="info-footer">
<p class="info-footer-text">Daha fazla bilgi icin:</p>
<a href="https://smartkraft.ch/to2do" target="_blank" class="info-footer-link">smartkraft.ch/to2do →</a>
</div>
</div>
</div>
<div class="modal-actions">
<button class="modal-btn" id="modal-settings-cancel">IPTAL</button>
<button class="modal-btn primary" id="modal-settings-save">KAYDET</button>
</div>
</div>
</div>
<div class="context-menu" id="context-menu">
<div class="context-menu-item" data-action="edit">DUZENLE</div>
<div class="context-menu-item" data-action="duplicate">KOPYALA</div>
<div class="context-menu-item" data-action="archive">ARSIVLE</div>
<div class="context-menu-item danger" data-action="delete">SIL</div>
</div>
<script>
function saveMailConfig(){
const config={
enabled:document.getElementById('mail-enabled-toggle').checked,
smtpServer:document.getElementById('smtp-server').value,
smtpPort:parseInt(document.getElementById('smtp-port').value)||587,
smtpUser:document.getElementById('smtp-user').value,
smtpPassword:document.getElementById('smtp-password').value,
fromEmail:document.getElementById('from-email').value,
useSSL:document.getElementById('smtp-ssl').checked
};
fetch('/api/mail/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(config)})
.then(r=>r.json())
.then(d=>{alert(d.success?'Mail settings saved!':'Failed: '+(d.error||'Unknown'));})
.catch(e=>alert('Error saving mail settings'));
}
function loadMailConfig(){
fetch('/api/mail/config')
.then(r=>r.json())
.then(d=>{
document.getElementById('mail-enabled-toggle').checked=d.enabled||false;
document.getElementById('smtp-server').value=d.smtpServer||'';
document.getElementById('smtp-port').value=d.smtpPort||587;
document.getElementById('smtp-user').value=d.smtpUser||'';
document.getElementById('smtp-password').value=d.smtpPassword||'';
document.getElementById('from-email').value=d.fromEmail||'';
document.getElementById('smtp-ssl').checked=d.useSSL!==false;
document.getElementById('smtp-status').textContent=d.enabled?'Enabled':'Disabled';
})
.catch(e=>console.error('Load mail config error:',e));
}
document.getElementById('show-smtp-password')?.addEventListener('change',function(){
const pwd=document.getElementById('smtp-password');
pwd.type=this.checked?'text':'password';
});
setTimeout(loadMailConfig,1000);
function openNotificationModal(){
const modal=document.getElementById('notification-modal');
modal.classList.add('active');

// Sync browser date before loading notifications to ensure accuracy
console.log('[Notification] Syncing browser date before loading...');
syncBrowserDateToDevice();

// Small delay to ensure date is synced before loading notifications
setTimeout(() => {
  document.querySelectorAll('[data-tab^="notif-"]').forEach(t=>t.classList.remove('active'));
  document.querySelector('[data-tab="notif-today"]')?.classList.add('active');
  document.querySelectorAll('[id^="notif-"][id$="-content"]').forEach(c=>c.style.display='none');
  document.getElementById('notif-today-content').style.display='block';
  loadNotifications('today');
}, 300);
}
function closeNotificationModal(){
document.getElementById('notification-modal').classList.remove('active');
}
function loadNotifications(type){
const contentId='notif-'+type+'-content';
const contentEl=document.getElementById(contentId);
if(!contentEl)return;
contentEl.innerHTML='<p style="text-align:center;color:#666;">Loading...</p>';
fetch('/api/notifications/'+type)
.then(r=>r.json())
.then(d=>{
console.log('Notification response:',d);
if(d.error){
contentEl.innerHTML='<p style="text-align:center;color:#f00;">'+d.error+'</p>';
return;
}
if(d.count===0){
contentEl.innerHTML='<p style="text-align:center;color:#888;padding:40px;">No tasks found</p>';
return;
}

// Category mapping: Convert internal category names to user-defined display names
// Internal categories: "Work", "Personal", "Projects"
// User can customize these in settings as category1, category2, category3
const categoryMapping = {
  'Work': app.settings.category1,
  'Personal': app.settings.category2,
  'Projects': app.settings.category3
};

let html='';
const cats=d.categories||{};
console.log('Categories:',cats);
console.log('Category settings:', app.settings);
for(const catName in cats){
const projects=cats[catName];
if(!projects||typeof projects!=='object'){
console.warn('Invalid projects for category:',catName,projects);
continue;
}
const projectCount=Object.keys(projects).length;

// Calculate total task count for this category
let totalTaskCount = 0;
for(const projName in projects){
const tasks=projects[projName];
if(tasks && Array.isArray(tasks)){
totalTaskCount += tasks.length;
}
}

// Map category name to user-defined name
const displayCatName = categoryMapping[catName] || catName;
console.log('Category mapping:', catName, '->', displayCatName);

html+='<div class="accordion-item"><div class="accordion-header" data-accordion="notif-'+catName+'"><span class="accordion-title">▶ '+displayCatName+'</span><span class="accordion-status">'+projectCount+' proje · '+totalTaskCount+' görev</span></div><div class="accordion-content" data-content="notif-'+catName+'">';
for(const projName in projects){
const tasks=projects[projName];
if(!tasks||!Array.isArray(tasks)){
console.warn('Invalid tasks for project:',projName,tasks);
continue;
}
html+='<div style="margin-bottom:15px;"><strong>'+projName+'</strong> ('+tasks.length+')<ul style="margin:5px 0 0 20px;padding:0;">';
tasks.forEach(t=>{
html+='<li style="margin:5px 0;color:#aaa;">'+t.title+' <span style="font-size:11px;color:#666;">('+t.date+')</span></li>';
});
html+='</ul></div>';
}
html+='</div></div>';
}
contentEl.innerHTML='<div class="accordion">'+html+'</div>';
setTimeout(()=>{
contentEl.querySelectorAll('.accordion-header').forEach(header=>{
header.addEventListener('click',e=>{
const accordionName=e.currentTarget.dataset.accordion;
const content=contentEl.querySelector('.accordion-content[data-content="'+accordionName+'"]');
if(!content)return;
const isActive=content.classList.contains('active');
if(isActive){
content.classList.remove('active');
header.classList.remove('active');
const titleEl=header.querySelector('.accordion-title');
if(titleEl)titleEl.textContent=titleEl.textContent.replace('▼','▶');
}else{
content.classList.add('active');
header.classList.add('active');
const titleEl=header.querySelector('.accordion-title');
if(titleEl)titleEl.textContent=titleEl.textContent.replace('▶','▼');
}
});
});
},100);
updateNotificationBadge();
})
.catch(e=>{
console.error('Notification error:',e);
contentEl.innerHTML='<p style="text-align:center;color:#f00;">Error loading notifications</p>';
});
}
function updateNotificationBadge(){
fetch('/api/notifications/today').then(r=>r.json()).then(d=>{
const badge=document.getElementById('notification-badge');
if(d.count>0){
badge.textContent=d.count;
badge.style.display='flex';
}else{
badge.style.display='none';
}
});
}
document.getElementById('notification-btn')?.addEventListener('click',openNotificationModal);
document.querySelectorAll('[data-tab^="notif-"]').forEach(tab=>{
tab.addEventListener('click',e=>{
const type=e.target.dataset.tab.replace('notif-','');
document.querySelectorAll('[data-tab^="notif-"]').forEach(t=>t.classList.remove('active'));
e.target.classList.add('active');
document.querySelectorAll('[id^="notif-"][id$="-content"]').forEach(c=>c.style.display='none');
document.getElementById('notif-'+type+'-content').style.display='block';
loadNotifications(type);
});
});
setTimeout(()=>{
updateNotificationBadge();
const offset=new Date().getTimezoneOffset()/-60;
fetch('/api/notifications/timezone',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({offset:offset})});

// Sync browser date/time to device on every page load
syncBrowserDateToDevice();

loadDateTime();
},2000);

function syncBrowserDateToDevice(){
// Sync browser's current date/time to device and save to SPIFFS
const now = new Date();
const browserDate = {
  year: now.getFullYear(),
  month: now.getMonth() + 1,
  day: now.getDate(),
  hour: now.getHours(),
  minute: now.getMinutes()
};
console.log('[Time] Syncing browser date to device:', browserDate);
fetch('/api/time', {
  method: 'POST',
  headers: {'Content-Type': 'application/json'},
  body: JSON.stringify(browserDate)
}).then(response => response.json())
  .then(data => {
    console.log('[Time] Device date updated and saved to SPIFFS:', data);
  })
  .catch(error => {
    console.error('[Time] Failed to sync browser date:', error);
  });
}

function loadDateTime(){
// Simple browser date/time display
const now = new Date();
const day = now.getDate().toString().padStart(2, '0');
const month = (now.getMonth() + 1).toString().padStart(2, '0');
const year = now.getFullYear();
const hour = now.getHours().toString().padStart(2, '0');
const minute = now.getMinutes().toString().padStart(2, '0');

const dateStr = `${day}.${month}.${year}`;
const timeStr = `${hour}:${minute}`;

document.getElementById('current-date').textContent = dateStr;
document.getElementById('current-time').textContent = timeStr;

// Also sync to device periodically (every minute when time updates)
syncBrowserDateToDevice();
}

// Load time immediately on page load
setTimeout(() => {
  console.log('[Time] Loading browser date/time...');
  loadDateTime();
}, 100);

// Update time every minute (and sync to device)
setInterval(loadDateTime, 60000);
</script>
<script src="/app.js"></script>
</body>
</html>
)rawliteral";
}

#endif
