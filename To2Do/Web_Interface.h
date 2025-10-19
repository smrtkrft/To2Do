#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char* getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>ToDo - SmartKraft</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}body{font-family:'Courier New',monospace;background:#000;color:#fff;overflow:hidden;font-size:14px}body[data-theme="light"]{background:#f5f5f5;color:#000}body[data-theme="light"] .workspace{background:#f5f5f5}body[data-theme="light"] .sidebar{background:#fff;border-right:1px solid #ddd}body[data-theme="light"] .sidebar-header{border-bottom:1px solid #ddd}body[data-theme="light"] .stat-label{color:#999}body[data-theme="light"] .sidebar-tab{background:#fff;border-right:1px solid #ddd;color:#999}body[data-theme="light"] .sidebar-tab.active{background:#f5f5f5;color:#000;border-bottom:2px solid #000}body[data-theme="light"] .project-search{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .sidebar-project{background:#fff;border:1px solid #ddd}body[data-theme="light"] .sidebar-project:hover{background:#f9f9f9;border-color:#ccc}body[data-theme="light"] .sidebar-project.active{background:#f0f0f0;border-color:#000}body[data-theme="light"] .project-desc{color:#666}body[data-theme="light"] .project-count{color:#999}body[data-theme="light"] .sidebar-btn{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .sidebar-btn:hover{background:#e5e5e5;border-color:#000}body[data-theme="light"] .panel{background:#f5f5f5}body[data-theme="light"] .panel-header{border-bottom:1px solid #ddd}body[data-theme="light"] .panel-subtitle{color:#666}body[data-theme="light"] .panel-filter-btn{background:#fff;border:1px solid #ddd;color:#999}body[data-theme="light"] .panel-filter-btn.active{background:#000;color:#fff;border-color:#000}body[data-theme="light"] .panel-filter-btn:hover:not(.active){background:#f9f9f9}body[data-theme="light"] .task-item{background:#fff;border:1px solid #ddd}body[data-theme="light"] .task-item:hover{background:#f9f9f9;border-color:#ccc}body[data-theme="light"] .task-checkbox{border:2px solid #999}body[data-theme="light"] .task-checkbox.checked{background:#000}body[data-theme="light"] .task-checkbox.checked::after{border-color:#fff}body[data-theme="light"] .task-type-badge{background:#f5f5f5;border:1px solid #ddd;color:#666}body[data-theme="light"] .task-description{color:#555}body[data-theme="light"] .task-dependencies{background:#f9f9f9;border:1px solid #ddd}body[data-theme="light"] .dependency-label{color:#999}body[data-theme="light"] .dependency-item{color:#666;border-bottom:1px solid #eee}body[data-theme="light"] .dependency-remove{background:#f5f5f5;border:1px solid #ddd;color:#666}body[data-theme="light"] .dependency-remove:hover{background:#f00;border-color:#f00;color:#fff}body[data-theme="light"] .checklist-checkbox{border:1px solid #999}body[data-theme="light"] .checklist-checkbox.checked{background:#999}body[data-theme="light"] .checklist-text{color:#555}body[data-theme="light"] .checklist-text.completed{color:#aaa}body[data-theme="light"] .checklist-remove{background:#f5f5f5;border:1px solid #ddd;color:#999}body[data-theme="light"] .checklist-remove:hover{background:#f00;border-color:#f00;color:#fff}body[data-theme="light"] .checklist-input{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .task-meta{color:#999}body[data-theme="light"] .task-action-btn{background:#fff;border:1px solid #ddd;color:#666}body[data-theme="light"] .task-action-btn:hover{background:#f9f9f9;border-color:#000;color:#000}body[data-theme="light"] .task-action-btn.delete:hover{background:#f00;border-color:#f00;color:#fff}body[data-theme="light"] .empty-state{color:#bbb}body[data-theme="light"] .fab{background:#000;color:#fff;box-shadow:0 4px 12px rgba(0,0,0,.3)}body[data-theme="light"] .fab:hover{box-shadow:0 6px 16px rgba(0,0,0,.5)}body[data-theme="light"] .modal{background:rgba(0,0,0,.7)}body[data-theme="light"] .modal-content{background:#fff;border:2px solid #ddd}body[data-theme="light"] .modal-input,body[data-theme="light"] .modal-textarea,body[data-theme="light"] .modal-select,body[data-theme="light"] .modal-date{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .modal-input:focus,body[data-theme="light"] .modal-textarea:focus,body[data-theme="light"] .modal-select:focus,body[data-theme="light"] .modal-date:focus{border-color:#000}body[data-theme="light"] .modal-btn{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .modal-btn.primary{background:#000;color:#fff}body[data-theme="light"] .modal-btn:hover{background:#e5e5e5;border-color:#000}body[data-theme="light"] .modal-btn.primary:hover{background:#333}body[data-theme="light"] .settings-tab{background:#fff;color:#999}body[data-theme="light"] .settings-tab.active{color:#000;border-bottom-color:#000}body[data-theme="light"] .settings-tab:hover:not(.active){background:#f9f9f9}body[data-theme="light"] .settings-label{color:#666}body[data-theme="light"] .settings-input{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .settings-input:focus{border-color:#000}body[data-theme="light"] .theme-label{color:#666}body[data-theme="light"] .toggle-slider{background:#ddd}body[data-theme="light"] .toggle-slider:before{background:#000}body[data-theme="light"] .toggle-switch input:checked+.toggle-slider{background:#000}body[data-theme="light"] .toggle-switch input:checked+.toggle-slider:before{background:#fff}body[data-theme="light"] .context-menu{background:#fff;border:1px solid #ddd}body[data-theme="light"] .context-menu-item{border-bottom:1px solid #eee}body[data-theme="light"] .context-menu-item:hover{background:#f9f9f9}body[data-theme="light"] .context-menu-item.danger:hover{background:#f00;color:#fff}body[data-theme="light"] .toast{background:#000;color:#fff;border:2px solid #000}body[data-theme="light"] .toast.error{background:#f00;color:#fff}body[data-theme="light"] ::-webkit-scrollbar-track{background:#f5f5f5}body[data-theme="light"] ::-webkit-scrollbar-thumb{background:#ddd}body[data-theme="light"] ::-webkit-scrollbar-thumb:hover{background:#ccc}.workspace{display:flex;height:100vh}.sidebar{width:450px;background:#0a0a0a;border-right:1px solid #222;display:flex;flex-direction:column}.sidebar-header{padding:20px;border-bottom:1px solid #222}.sidebar-title{font-size:20px;letter-spacing:3px;margin-bottom:15px}.sidebar-stats{display:flex;justify-content:space-between}.stat{text-align:center}.stat-value{font-size:28px;font-weight:bold}.stat-label{font-size:12px;color:#666;margin-top:5px}.sidebar-tabs{display:flex;border-bottom:1px solid #222}.sidebar-tab{flex:1;padding:12px;text-align:center;background:#0a0a0a;border:none;border-right:1px solid #222;color:#666;cursor:pointer;transition:all .2s;font-family:'Courier New',monospace;font-size:14px;font-weight:bold}.sidebar-tab:last-child{border-right:none}.sidebar-tab.active{background:#000;color:#fff;border-bottom:2px solid #fff}.sidebar-search{padding:15px;border-bottom:1px solid #222}.project-search{width:100%;padding:10px;background:#111;border:1px solid #333;color:#fff;font-family:'Courier New',monospace;font-size:14px}.sidebar-body{flex:1;overflow-y:auto}.projects-list{padding:15px}.project-group-label{font-size:11px;color:#444;margin-bottom:10px;letter-spacing:2px}.sidebar-project{display:flex;justify-content:space-between;align-items:center;padding:12px;margin-bottom:8px;background:#0a0a0a;border:1px solid #222;cursor:pointer;transition:all .2s}.sidebar-project:hover{background:#111;border-color:#333}.sidebar-project.active{background:#111;border-color:#fff}.project-info{flex:1}.project-name{font-size:15px;font-weight:bold;margin-bottom:4px}.project-desc{font-size:12px;color:#666}.project-meta{display:flex;align-items:center;gap:10px}.project-priority{width:8px;height:8px;border-radius:50%}.priority-high{background:#f00}.priority-medium{background:#ff0}.priority-low{background:#0f0}.project-count{font-size:12px;color:#666}.sidebar-footer{padding:15px;border-top:1px solid #222;display:flex;gap:10px}.sidebar-btn{flex:1;padding:12px;background:#111;border:1px solid #333;color:#fff;cursor:pointer;transition:all .2s;font-family:'Courier New',monospace;font-size:13px}.sidebar-btn:hover{background:#222;border-color:#fff}.panel{flex:1;display:flex;flex-direction:column;overflow:hidden}.panel-header{padding:20px;border-bottom:1px solid #222}.panel-title-row{display:flex;justify-content:space-between;align-items:flex-start;margin-bottom:5px}.panel-title{font-size:22px}.panel-actions{display:flex;gap:8px}.panel-btn{padding:8px 12px;background:#111;border:1px solid #333;color:#fff;cursor:pointer;font-family:'Courier New',monospace;font-size:12px;transition:all .2s}.panel-btn:hover{background:#222;border-color:#fff}.panel-subtitle{font-size:13px;color:#888;font-style:italic;margin-bottom:15px}.panel-filters{display:flex;gap:6px;flex-wrap:wrap}.panel-filter-btn{padding:6px 10px;background:#0a0a0a;border:1px solid #333;color:#666;cursor:pointer;transition:all .2s;font-family:'Courier New',monospace;font-size:12px}.panel-filter-btn.active{background:#fff;color:#000;border-color:#fff}.panel-filter-btn:hover:not(.active){background:#111}.panel-body{flex:1;overflow-y:auto;padding:20px}.tasks-container{max-width:1400px}.task-item{margin-bottom:15px;padding:15px;background:#0a0a0a;border:1px solid #222;border-left:3px solid #444;transition:all .2s}.task-item:hover{background:#111;border-color:#333}.task-item.completed{opacity:.6}.task-item.priority-high{border-left-color:#f00}.task-item.priority-medium{border-left-color:#ff0}.task-item.priority-low{border-left-color:#0f0}.task-header{display:flex;gap:15px}.task-checkbox{width:20px;height:20px;border:2px solid #666;cursor:pointer;flex-shrink:0;margin-top:2px}.task-checkbox.checked{background:#fff;position:relative}.task-checkbox.checked::after{content:'';position:absolute;left:5px;top:2px;width:5px;height:10px;border:solid #000;border-width:0 2px 2px 0;transform:rotate(45deg)}.task-content{flex:1}.task-title-row{display:flex;justify-content:space-between;align-items:flex-start;margin-bottom:8px}.task-title{font-size:15px;font-weight:bold;flex:1}.task-title.editing,.task-description.editing{border:1px solid #fff;padding:4px;background:#111;outline:none;display:block;width:100%}.task-type-badge{padding:5px 10px;background:#111;border:1px solid #333;font-size:11px;color:#888}.task-description{font-size:13px;color:#aaa;margin-bottom:10px}.task-dependencies{margin:10px 0;padding:10px;background:#050505;border:1px solid #222;font-size:12px}.dependency-label{color:#666;margin-bottom:8px;font-size:11px;letter-spacing:1px}.dependency-item{display:flex;justify-content:space-between;align-items:center;padding:6px 0;color:#888;border-bottom:1px solid #111}.dependency-item:last-child{border-bottom:none}.dependency-text{flex:1}.dependency-text::before{content:'→ ';color:#f00;font-weight:bold}.dependency-remove{padding:4px 8px;background:#111;border:1px solid #333;color:#888;cursor:pointer;font-size:10px;transition:all .2s}.dependency-remove:hover{background:#f00;border-color:#f00;color:#000}.task-checklist{margin:10px 0;padding-left:10px}.checklist-item{display:flex;align-items:flex-start;gap:8px;margin-bottom:6px;font-size:13px}.checklist-checkbox{width:14px;height:14px;border:1px solid #666;cursor:pointer;flex-shrink:0;margin-top:2px}.checklist-checkbox.checked{background:#666}.checklist-text{flex:1;color:#aaa}.checklist-text.completed{text-decoration:line-through;color:#555}.checklist-remove{padding:2px 6px;background:#111;border:1px solid #333;color:#666;cursor:pointer;font-size:10px;margin-left:8px;transition:all .2s}.checklist-remove:hover{background:#f00;border-color:#f00;color:#000}.checklist-input{width:100%;padding:6px;background:#111;border:1px solid #333;color:#fff;font-family:'Courier New',monospace;font-size:12px}.task-footer{display:flex;justify-content:space-between;align-items:center;margin-top:10px}.task-meta{display:flex;gap:10px;font-size:12px;color:#666}.task-actions{display:flex;gap:8px}.task-action-btn{padding:6px 10px;background:#0a0a0a;border:1px solid #333;color:#888;cursor:pointer;transition:all .2s;font-family:'Courier New',monospace;font-size:12px}.task-action-btn:hover{background:#111;border-color:#fff;color:#fff}.task-action-btn.delete:hover{background:#f00;border-color:#f00;color:#000}.empty-state{text-align:center;padding:60px 20px;color:#444}.fab{position:fixed;bottom:30px;right:30px;width:60px;height:60px;background:#fff;border:none;border-radius:50%;color:#000;font-size:28px;cursor:pointer;box-shadow:0 4px 12px rgba(255,255,255,.3);transition:all .2s;z-index:100}.fab:hover{transform:scale(1.1);box-shadow:0 6px 16px rgba(255,255,255,.5)}.fab:active{transform:scale(.95)}.modal{display:none;position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,.85);align-items:center;justify-content:center;z-index:1000}.modal.active{display:flex}.modal-content{background:#0a0a0a;border:2px solid #333;width:600px;max-width:90%;max-height:90vh;display:flex;flex-direction:column}.modal-title{font-size:20px;margin-bottom:20px;letter-spacing:2px;padding:30px 30px 0 30px}.modal-form-row{display:grid;grid-template-columns:2fr 1fr 1fr;gap:10px;margin-bottom:15px;padding:0 30px}.modal-input,.modal-textarea,.modal-select,.modal-date{width:100%;padding:10px;background:#111;border:1px solid #333;color:#fff;font-family:'Courier New',monospace;font-size:14px}.modal-input:focus,.modal-textarea:focus,.modal-select:focus,.modal-date:focus{outline:none;border-color:#fff}.modal-textarea{min-height:80px;resize:vertical;margin-bottom:15px}.modal-date::-webkit-calendar-picker-indicator{filter:invert(1)}.modal-actions{display:flex;gap:10px;padding:20px 30px;border-top:1px solid #333;margin-top:auto}.modal-btn{flex:1;padding:12px;background:#111;border:1px solid #333;color:#fff;cursor:pointer;font-family:'Courier New',monospace;font-size:14px;transition:all .2s}.modal-btn.primary{background:#fff;color:#000}.modal-btn:hover{background:#222;border-color:#fff}.modal-btn.primary:hover{background:#ddd}.settings-tabs{display:flex;gap:5px;margin-bottom:20px;border-bottom:1px solid #333;padding:30px 30px 0 30px}.settings-tab{flex:1;padding:12px;background:#0a0a0a;border:none;border-bottom:2px solid transparent;color:#666;cursor:pointer;transition:all .2s;font-family:'Courier New',monospace;font-size:13px;font-weight:bold}.settings-tab.active{color:#fff;border-bottom-color:#fff}.settings-tab:hover:not(.active){background:#111}.settings-content{flex:1;overflow-y:auto;padding:20px 30px}.settings-tab-content{display:none}.settings-tab-content.active{display:block}.settings-tab-content p{margin-bottom:10px;line-height:1.6;color:#aaa}.settings-section{margin-bottom:25px}.settings-label{display:block;font-size:12px;color:#888;margin-bottom:8px;letter-spacing:1px}.settings-input{width:100%;padding:10px;background:#111;border:1px solid #333;color:#fff;font-family:'Courier New',monospace;font-size:14px;margin-bottom:8px}.settings-input:focus{outline:none;border-color:#fff}.theme-toggle{display:flex;align-items:center;gap:15px;padding:10px 0}.theme-label{font-size:13px;color:#888;min-width:50px}.toggle-switch{position:relative;width:50px;height:24px;display:inline-block}.toggle-switch input{opacity:0;width:0;height:0}.toggle-slider{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background:#333;transition:.3s;border-radius:24px}.toggle-slider:before{position:absolute;content:"";height:18px;width:18px;left:3px;bottom:3px;background:#fff;transition:.3s;border-radius:50%}.toggle-switch input:checked+.toggle-slider{background:#fff}.toggle-switch input:checked+.toggle-slider:before{transform:translateX(26px);background:#000}.accordion{margin-bottom:20px}.accordion-item{margin-bottom:10px;border:1px solid #333;background:#0a0a0a}.accordion-header{display:flex;justify-content:space-between;align-items:center;padding:15px;cursor:pointer;transition:all .2s;border-bottom:1px solid transparent}.accordion-header:hover{background:#111}.accordion-header.active{background:#111;border-bottom-color:#333}.accordion-title{font-size:14px;font-weight:bold;color:#fff;letter-spacing:1px}.accordion-status{font-size:11px;padding:4px 8px;background:#222;border:1px solid #333;border-radius:3px;color:#888}.accordion-status.active{background:#0f0;border-color:#0f0;color:#000}.accordion-status.connected{background:#0ff;border-color:#0ff;color:#000}.accordion-status.error{background:#f00;border-color:#f00;color:#fff}.accordion-content{display:none;padding:20px;border-top:1px solid #333}.accordion-content.active{display:block}.settings-hint{font-size:11px;color:#666;margin-top:5px;font-style:italic}.settings-hint span{color:#fff;font-weight:bold}.settings-checkbox{display:flex;align-items:center;gap:8px;margin-top:8px;cursor:pointer;font-size:13px;color:#888}.settings-checkbox input{cursor:pointer}.settings-btn{width:100%;padding:12px;background:#111;border:1px solid #333;color:#fff;cursor:pointer;font-family:'Courier New',monospace;font-size:13px;font-weight:bold;transition:all .2s;margin-top:10px}.settings-btn:hover{background:#222;border-color:#fff}.connection-info{margin-top:25px;padding:20px;background:#0a0a0a;border:2px solid #333}.connection-info-title{font-size:13px;font-weight:bold;color:#fff;margin-bottom:15px;letter-spacing:1px}.connection-status-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:15px}.connection-status-item{display:flex;flex-direction:column;gap:5px}.status-label{font-size:11px;color:#666}.status-value{font-size:13px;color:#fff;font-weight:bold}body[data-theme="light"] .accordion-item{background:#fff;border:1px solid #ddd}body[data-theme="light"] .accordion-header:hover{background:#f9f9f9}body[data-theme="light"] .accordion-header.active{background:#f9f9f9;border-bottom-color:#ddd}body[data-theme="light"] .accordion-title{color:#000}body[data-theme="light"] .accordion-status{background:#f5f5f5;border-color:#ddd;color:#666}body[data-theme="light"] .accordion-status.active{background:#0f0;border-color:#0f0;color:#000}body[data-theme="light"] .accordion-status.connected{background:#0ff;border-color:#0ff;color:#000}body[data-theme="light"] .accordion-status.error{background:#f00;border-color:#f00;color:#fff}body[data-theme="light"] .accordion-content{border-top:1px solid #ddd}body[data-theme="light"] .settings-hint{color:#999}body[data-theme="light"] .settings-hint span{color:#000}body[data-theme="light"] .settings-checkbox{color:#666}body[data-theme="light"] .settings-btn{background:#f5f5f5;border-color:#ddd;color:#000}body[data-theme="light"] .settings-btn:hover{background:#e5e5e5;border-color:#000}body[data-theme="light"] .connection-info{background:#f9f9f9;border-color:#ddd}body[data-theme="light"] .connection-info-title{color:#000}body[data-theme="light"] .status-label{color:#999}body[data-theme="light"] .status-value{color:#000}.info-status-bar{display:grid;grid-template-columns:repeat(4,1fr);gap:10px;margin-bottom:20px;padding:15px;background:#0a0a0a;border:1px solid #333}.info-status-item{text-align:center;padding:8px;background:#111;border:1px solid #222}.info-status-label{display:block;font-size:10px;color:#666;margin-bottom:4px;letter-spacing:1px}.info-status-value{display:block;font-size:12px;color:#fff;font-weight:bold}.info-section{margin-bottom:20px;padding:15px;background:#0a0a0a;border:1px solid #222;border-left:3px solid #444}.info-section span{color:#fff;font-weight:bold}.info-title{font-size:14px;font-weight:bold;color:#fff;margin-bottom:10px;letter-spacing:1px}.info-text{font-size:13px;line-height:1.8;color:#aaa;margin-bottom:6px;padding-left:10px}.info-text strong{color:#fff}.info-footer{margin-top:20px;padding:15px;text-align:center;background:#0a0a0a;border:1px solid #333}.info-footer-text{font-size:12px;color:#888;margin-bottom:8px}.info-footer-link{display:inline-block;padding:8px 16px;background:#111;border:1px solid #333;color:#fff;text-decoration:none;font-size:12px;transition:all .2s}.info-footer-link:hover{background:#222;border-color:#fff;transform:translateX(3px)}body[data-theme="light"] .info-status-bar{background:#fff;border:1px solid #ddd}body[data-theme="light"] .info-status-item{background:#f9f9f9;border:1px solid #ddd}body[data-theme="light"] .info-status-label{color:#999}body[data-theme="light"] .info-status-value{color:#000}body[data-theme="light"] .info-section{background:#fff;border:1px solid #ddd;border-left:3px solid #999}body[data-theme="light"] .info-section span{color:#000}body[data-theme="light"] .info-title{color:#000}body[data-theme="light"] .info-text{color:#555}body[data-theme="light"] .info-text strong{color:#000}body[data-theme="light"] .info-footer{background:#f9f9f9;border:1px solid #ddd}body[data-theme="light"] .info-footer-text{color:#666}body[data-theme="light"] .info-footer-link{background:#f5f5f5;border:1px solid #ddd;color:#000}body[data-theme="light"] .info-footer-link:hover{background:#e5e5e5;border-color:#000}.context-menu{display:none;position:fixed;background:#0a0a0a;border:1px solid #333;min-width:200px;z-index:2000}.context-menu.active{display:block}.context-menu-item{padding:12px 15px;cursor:pointer;border-bottom:1px solid #222;font-size:13px}.context-menu-item:hover{background:#111}.context-menu-item.danger:hover{background:#f00;color:#000}.toast{position:fixed;bottom:30px;left:50%;transform:translateX(-50%);padding:15px 25px;background:#fff;color:#000;font-size:13px;border:2px solid #fff;z-index:3000;animation:slideUp .3s ease}.toast.error{background:#f00;color:#fff}@keyframes slideUp{from{transform:translateX(-50%) translateY(100px);opacity:0}to{transform:translateX(-50%) translateY(0);opacity:1}}::-webkit-scrollbar{width:8px}::-webkit-scrollbar-track{background:#0a0a0a}::-webkit-scrollbar-thumb{background:#333}::-webkit-scrollbar-thumb:hover{background:#555}@media(max-width:768px){body{overflow:auto}.workspace{flex-direction:column;height:auto}.sidebar{width:100%;border-right:none;border-bottom:1px solid #222}.sidebar-header{padding:15px}.sidebar-title{font-size:18px}.sidebar-stats{gap:10px}.stat-value{font-size:20px}.sidebar-tabs{flex-wrap:nowrap;overflow-x:auto}.sidebar-tab{font-size:12px;padding:10px}.sidebar-body{max-height:40vh}.panel{width:100%}.panel-header{padding:15px}.panel-title{font-size:18px}.panel-filters{overflow-x:auto;flex-wrap:nowrap}.panel-body{padding:10px}.task-item{padding:10px}.task-title{font-size:14px}.modal-content{padding:20px;width:95%}.modal-form-row{grid-template-columns:1fr;gap:8px}.fab{bottom:20px;right:20px;width:50px;height:50px;font-size:24px}.info-status-bar{grid-template-columns:repeat(2,1fr)}.info-status-item{font-size:11px}}
.fab-notification{right:100px}.notification-popup{display:none;position:fixed;bottom:110px;right:30px;width:280px;background:#0a0a0a;border:1px solid #333;border-radius:12px;box-shadow:0 12px 30px rgba(0,0,0,.6);overflow:hidden;z-index:120}.notification-popup.active{display:block}.notification-tabs{display:flex;border-bottom:1px solid #333;background:#050505}.notification-tab{flex:1;padding:10px;font-size:12px;font-weight:bold;text-transform:uppercase;background:transparent;border:none;color:#666;cursor:pointer;transition:all .2s}.notification-tab:hover{color:#fff}.notification-tab.active{color:#fff;background:#111}.notification-content{padding:15px;max-height:220px;overflow-y:auto}.notification-panel{display:none;font-size:12px;line-height:1.5;color:#bbb}.notification-panel.active{display:block}.notification-empty{font-size:12px;color:#555;text-align:center}.notification-list{list-style:none;padding-left:0;margin:0}.notification-list li{margin-bottom:8px;color:#ddd;font-size:12px}@media(max-width:768px){.fab-notification{right:85px;bottom:20px}.notification-popup{right:20px;width:90%;bottom:80px}}</style>
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
<button class="fab fab-notification" id="fab-notifications">!</button>
<button class="fab" id="fab-new-task">+</button>
<div class="notification-popup" id="notification-popup">
<div class="notification-tabs">
<button class="notification-tab active" data-tab="today">BUGÜN</button>
<button class="notification-tab" data-tab="tomorrow">YARIN</button>
<button class="notification-tab" data-tab="week">BU HAFTA</button>
<button class="notification-tab" data-tab="past">GEÇMİŞ</button>
</div>
<div class="notification-content">
<div class="notification-panel active" data-content="today"><p class="notification-empty">Bugün için bildirim yok.</p></div>
<div class="notification-panel" data-content="tomorrow"><p class="notification-empty">Yarın için bildirim yok.</p></div>
<div class="notification-panel" data-content="week"><p class="notification-empty">Bu hafta için bildirim yok.</p></div>
<div class="notification-panel" data-content="past"><p class="notification-empty">Geçmişte bildirim yok.</p></div>
</div>
</div>
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
<div class="modal" id="task-edit-modal">
<div class="modal-content">
<div class="modal-title">GÖREVI DÜZENLE</div>
<input type="text" class="modal-input" id="edit-modal-task-title" placeholder="Baslik...">
<textarea class="modal-textarea" id="edit-modal-task-desc" placeholder="Aciklama, detaylar, notlar..."></textarea>
<div class="modal-form-row">
<select class="modal-select" id="edit-modal-task-type"><option value="task">Gorev</option><option value="plan">Plan</option><option value="note">Not</option><option value="reminder">Hatirlatma</option></select>
<select class="modal-select" id="edit-modal-task-priority"><option value="low">Dusuk</option><option value="medium" selected>Orta</option><option value="high">Yuksek</option></select>
<input type="date" class="modal-date" id="edit-modal-task-date">
</div>
<div class="modal-actions">
<button class="modal-btn" id="edit-modal-task-cancel">IPTAL</button>
<button class="modal-btn primary" id="edit-modal-task-update">GUNCELLE</button>
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
<p>E-posta bildirimleri ve mail sunucu ayarlari.</p>
<p>SMTP yapilandirmasi ve bildirim tercihleri.</p>
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
<p class="info-text">1. Sol taraftan bir kategori secin (<span id="info-cat1">KISISEL</span>, <span id="info-cat2">SMARTKRAFT</span>, <span id="info-cat3">GENEL</span>)</p>
<p class="info-text">2. "+ YENI PROJE" butonu ile proje olusturun</p>
<p class="info-text">3. Proje ustune tiklayarak secin</p>
<p class="info-text">4. Sag alttaki "+" butonu ile gorev ekleyin</p>
</div>
<div class="info-section">
<h3 class="info-title">PROJE YONETIMI</h3>
<p class="info-text"><strong>Proje Olusturma:</strong> Sol alttaki "+ YENI PROJE" butonunu kullanin</p>
<p class="info-text"><strong>Proje Duzenleme:</strong> Proje uzerine sag tiklayip "DUZENLE" secin</p>
<p class="info-text"><strong>Proje Kopyalama:</strong> Sag tikla > "KOPYALA" ile ayni projeden kopya olusturun</p>
<p class="info-text"><strong>Proje Arsivleme:</strong> Sag tikla > "ARSIVLE" ile projeyi arsive tasiyin</p>
<p class="info-text"><strong>Proje Silme:</strong> Sag tikla > "SIL" ile projeyi kalici olarak silin</p>
</div>
<div class="info-section">
<h3 class="info-title">GOREV YONETIMI</h3>
<p class="info-text"><strong>Gorev Ekleme:</strong> Sag alttaki beyaz "+" butonuna tiklayin</p>
<p class="info-text"><strong>Gorev Tamamlama:</strong> Gorevin solundaki kutuya tiklayarak tamamlandi isareti koyun</p>
<p class="info-text"><strong>Gorev Turleri:</strong> Gorev, Plan, Not, Hatirlatma olarak 4 turde olusturabilirsiniz</p>
<p class="info-text"><strong>Oncelik Seviyeleri:</strong> Dusuk (Yesil), Orta (Sari), Yuksek (Kirmizi)</p>
<p class="info-text"><strong>Alt Gorevler:</strong> Her gorevin icine checklist ekleyebilirsiniz</p>
<p class="info-text"><strong>Bagimlilik:</strong> Gorevler arasi bagimlilik ekleyerek is akisini yonetin</p>
</div>
<div class="info-section">
<h3 class="info-title">FILTRELEME</h3>
<p class="info-text"><strong>TUMU:</strong> Tum gorevleri gosterir</p>
<p class="info-text"><strong>AKTIF:</strong> Sadece tamamlanmamis gorevler</p>
<p class="info-text"><strong>TAMAMLANAN:</strong> Sadece tamamlanmis gorevler</p>
<p class="info-text"><strong>GOREV/PLAN/NOT/HATIRLATMA:</strong> Turune gore filtreleme</p>
</div>
<div class="info-section">
<h3 class="info-title">ARAMA VE NAVIGASYON</h3>
<p class="info-text"><strong>Proje Arama:</strong> Sol ust kosedeki arama kutusunu kullanin</p>
<p class="info-text"><strong>Kategori Degistirme:</strong> Ust sekmelerden (<span id="info-cat1-nav">KISISEL</span>, <span id="info-cat2-nav">SMARTKRAFT</span>, <span id="info-cat3-nav">GENEL</span>) birini secin</p>
</div>
<div class="info-section">
<h3 class="info-title">KISAYOLLAR</h3>
<p class="info-text"><strong>Sag Tik:</strong> Projeler uzerinde sag tik menusu</p>
<p class="info-text"><strong>Cift Tik:</strong> Gorev baslik veya aciklama uzerinde duzenlemek icin</p>
<p class="info-text"><strong>FAB Butonu:</strong> Her zaman gorunen "+" butonu ile hizli gorev ekleme</p>
</div>
<div class="info-section">
<h3 class="info-title">KISISELLESTIRME</h3>
<p class="info-text"><strong>GUI Sekmesi:</strong> Uygulama basligini, kategori isimlerini ve temayi degistirin</p>
<p class="info-text"><strong>Tema:</strong> Acik/Koyu tema arasi gecis yapabilirsiniz</p>
<p class="info-text"><strong>Kayit:</strong> Tum ayarlar otomatik olarak tarayicida saklanir</p>
</div>
<div class="info-section">
<h3 class="info-title">VERI SAKLAMA</h3>
<p class="info-text">Tum proje, gorev ve ayarlar ESP32 cihazinda SPIFFS hafizasinda saklanir.</p>
<p class="info-text">Elektrik kesilse ve firmware tekrar yuklenme yapilsa bile verileriniz korunur.</p>
<p class="info-text"><strong>Kalici Saklama:</strong> WiFi ayarlari, tema, kategoriler her zaman korunur.</p>
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
<a href="https://smartkraft.ch/todo" target="_blank" class="info-footer-link">smartkraft.ch/todo →</a>
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
document.addEventListener('DOMContentLoaded',function(){
  const notifBtn=document.getElementById('fab-notifications');
  const popup=document.getElementById('notification-popup');
  if(!notifBtn||!popup)return;
  const tabs=popup.querySelectorAll('.notification-tab');
  const panels=popup.querySelectorAll('.notification-panel');
  const setActive=function(target){
    tabs.forEach(function(tab){tab.classList.toggle('active',tab.dataset.tab===target);});
    panels.forEach(function(panel){panel.classList.toggle('active',panel.dataset.content===target);});
  };
  notifBtn.addEventListener('click',function(ev){
    ev.stopPropagation();
    const willOpen=!popup.classList.contains('active');
    popup.classList.toggle('active');
    if(willOpen){
      const current=popup.querySelector('.notification-tab.active');
      setActive(current?current.dataset.tab:'today');
    }
  });
  popup.addEventListener('click',function(ev){ev.stopPropagation();});
  document.addEventListener('click',function(ev){
    if(!popup.contains(ev.target)&&ev.target!==notifBtn){popup.classList.remove('active');}
  });
  tabs.forEach(function(tab){
    tab.addEventListener('click',function(){
      setActive(tab.dataset.tab);
    });
  });
});
</script>
<script src="/app.js"></script>
</body>
</html>
)rawliteral";
}

#endif
