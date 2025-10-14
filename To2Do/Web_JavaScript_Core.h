#ifndef WEB_JAVASCRIPT_CORE_H
#define WEB_JAVASCRIPT_CORE_H

const char* getJavaScriptCore() {
  return R"rawliteral(
class ToDoApp {
    constructor() {
        this.projects = [];
        this.tasks = [];
        this.currentProject = null;
        this.currentCategory = 'Personal';
        this.currentFilter = 'all';
        this.editingProject = null;
        this.editingTask = null;
    }

    async init() {
        console.log('[ToDoApp] Initializing...');
        await this.loadData();
        this.setupEventListeners();
        this.loadGUISettings();
        this.loadNetworkStatus();
        this.renderProjects();
        console.log('[ToDoApp] Ready');
    }

    async loadData() {
        try {
            const response = await fetch('/api/todos');
            const data = await response.json();
            this.projects = data.projects || [];
            this.tasks = data.tasks || [];
            console.log(`[ToDoApp] Loaded ${this.projects.length} projects, ${this.tasks.length} tasks`);
            this.updateStats();
        } catch (error) {
            console.error('[ToDoApp] Load error:', error);
        }
    }

    async saveData() {
        try {
            const data = {
                projects: this.projects,
                tasks: this.tasks
            };
            const response = await fetch('/api/todos', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(data)
            });
            if (response.ok) {
                console.log('[ToDoApp] Data saved');
                return true;
            }
        } catch (error) {
            console.error('[ToDoApp] Save error:', error);
        }
        return false;
    }

    setupEventListeners() {
        // FAB
        document.getElementById('fab-new-task').addEventListener('click', () => {
            this.showTaskModal();
        });

        // Category tabs
        document.querySelectorAll('.sidebar-tab').forEach(tab => {
            tab.addEventListener('click', (e) => {
                this.currentCategory = e.target.dataset.category;
                document.querySelectorAll('.sidebar-tab').forEach(t => t.classList.remove('active'));
                e.target.classList.add('active');
                this.renderProjects();
            });
        });

        // Project search
        document.getElementById('project-search').addEventListener('input', (e) => {
            this.renderProjects(e.target.value);
        });

        // New project
        document.getElementById('new-project-btn').addEventListener('click', () => {
            this.showProjectModal();
        });

        // Settings button
        document.getElementById('settings-btn').addEventListener('click', () => {
            this.showSettingsModal();
        });

        // Filters
        document.querySelectorAll('.filter-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                this.currentFilter = e.target.dataset.filter;
                document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
                e.target.classList.add('active');
                this.renderTasks();
            });
        });

        // Save project
        document.getElementById('save-project-btn').addEventListener('click', () => {
            this.saveProject();
        });

        // Save task
        document.getElementById('save-task-btn').addEventListener('click', () => {
            this.saveTask();
        });

        // Settings tabs
        document.querySelectorAll('.settings-tab').forEach(tab => {
            tab.addEventListener('click', (e) => {
                const tabName = e.target.dataset.tab;
                document.querySelectorAll('.settings-tab').forEach(t => t.classList.remove('active'));
                e.target.classList.add('active');
                document.querySelectorAll('.settings-tab-content').forEach(c => c.classList.remove('active'));
                document.querySelector(`[data-content="${tabName}"]`).classList.add('active');
            });
        });

        // Accordion
        document.querySelectorAll('.accordion-header').forEach(header => {
            header.addEventListener('click', (e) => {
                const accordion = e.currentTarget.dataset.accordion;
                const content = document.querySelector(`[data-content="${accordion}"]`);
                content.classList.toggle('active');
                const title = e.currentTarget.querySelector('.accordion-title');
                if (content.classList.contains('active')) {
                    title.textContent = title.textContent.replace('▶', '▼');
                } else {
                    title.textContent = title.textContent.replace('▼', '▶');
                }
            });
        });
    }

    updateStats() {
        const totalTasks = this.tasks.length;
        const activeTasks = this.tasks.filter(t => !t.completed).length;
        const totalProjects = this.projects.length;

        document.getElementById('total-tasks').textContent = totalTasks;
        document.getElementById('active-tasks').textContent = activeTasks;
        document.getElementById('total-projects').textContent = totalProjects;
    }

    renderProjects(searchQuery = '') {
        const projectsList = document.getElementById('projects-list');
        projectsList.innerHTML = '';

        let filtered = this.projects.filter(p => 
            p.category === this.currentCategory && !p.archived
        );

        if (searchQuery) {
            const query = searchQuery.toLowerCase();
            filtered = filtered.filter(p => 
                p.name.toLowerCase().includes(query) ||
                p.description.toLowerCase().includes(query)
            );
        }

        filtered.forEach(project => {
            const projectEl = document.createElement('div');
            projectEl.className = 'project-item';
            if (this.currentProject && this.currentProject.id === project.id) {
                projectEl.classList.add('active');
            }

            const taskCount = this.tasks.filter(t => t.projectId === project.id).length;

            projectEl.innerHTML = `
                <div class="project-name">${project.name}</div>
                <div class="project-meta">
                    <span class="priority-badge priority-${project.priority}">${project.priority.toUpperCase()}</span>
                    <span>${taskCount} tasks</span>
                </div>
            `;

            projectEl.addEventListener('click', () => {
                this.selectProject(project);
            });

            projectsList.appendChild(projectEl);
        });

        if (filtered.length === 0) {
            projectsList.innerHTML = '<div style="padding:20px;text-align:center;color:#666;">No projects found</div>';
        }
    }

    selectProject(project) {
        this.currentProject = project;
        document.getElementById('current-project-title').textContent = project.name;
        this.renderProjects();
        this.renderTasks();
    }

    renderTasks() {
        const tasksList = document.getElementById('tasks-list');
        tasksList.innerHTML = '';

        if (!this.currentProject) {
            tasksList.innerHTML = '<div style="padding:40px;text-align:center;color:#666;">Select a project to view tasks</div>';
            return;
        }

        let filtered = this.tasks.filter(t => t.projectId === this.currentProject.id);

        // Apply filter
        if (this.currentFilter === 'active') {
            filtered = filtered.filter(t => !t.completed);
        } else if (this.currentFilter === 'completed') {
            filtered = filtered.filter(t => t.completed);
        } else if (this.currentFilter === 'high') {
            filtered = filtered.filter(t => t.priority === 'high');
        }

        filtered.forEach(task => {
            const taskEl = document.createElement('div');
            taskEl.className = 'task-item';

            taskEl.innerHTML = `
                <div class="task-checkbox ${task.completed ? 'checked' : ''}" onclick="app.toggleTask(${task.id})"></div>
                <div class="task-content">
                    <div class="task-name" style="${task.completed ? 'text-decoration:line-through;opacity:0.6;' : ''}">${task.name}</div>
                    ${task.description ? `<div class="task-desc">${task.description}</div>` : ''}
                    <div class="project-meta">
                        <span class="priority-badge priority-${task.priority}">${task.priority.toUpperCase()}</span>
                        ${task.dueDate ? `<span>📅 ${task.dueDate}</span>` : ''}
                    </div>
                </div>
                <div class="task-actions">
                    <button class="task-action-btn" onclick="app.editTask(${task.id})">✎</button>
                    <button class="task-action-btn" onclick="app.deleteTask(${task.id})">🗑</button>
                </div>
            `;

            tasksList.appendChild(taskEl);
        });

        if (filtered.length === 0) {
            tasksList.innerHTML = '<div style="padding:40px;text-align:center;color:#666;">No tasks found</div>';
        }
    }

    showProjectModal(project = null) {
        this.editingProject = project;
        const modal = document.getElementById('project-modal');
        const title = document.getElementById('project-modal-title');

        if (project) {
            title.textContent = 'Edit Project';
            document.getElementById('project-name').value = project.name;
            document.getElementById('project-desc').value = project.description || '';
            document.getElementById('project-category').value = project.category;
            document.getElementById('project-priority').value = project.priority;
        } else {
            title.textContent = 'New Project';
            document.getElementById('project-name').value = '';
            document.getElementById('project-desc').value = '';
            document.getElementById('project-category').value = this.currentCategory;
            document.getElementById('project-priority').value = 'medium';
        }

        modal.classList.add('active');
    }

    closeProjectModal() {
        document.getElementById('project-modal').classList.remove('active');
        this.editingProject = null;
    }

    async saveProject() {
        const name = document.getElementById('project-name').value.trim();
        if (!name) {
            alert('Please enter a project name');
            return;
        }

        const projectData = {
            name: name,
            description: document.getElementById('project-desc').value.trim(),
            category: document.getElementById('project-category').value,
            priority: document.getElementById('project-priority').value,
            archived: false
        };

        if (this.editingProject) {
            // Update existing
            Object.assign(this.editingProject, projectData);
        } else {
            // Create new
            const newId = Math.max(0, ...this.projects.map(p => p.id)) + 1;
            this.projects.push({ id: newId, ...projectData });
        }

        await this.saveData();
        this.renderProjects();
        this.updateStats();
        this.closeProjectModal();
    }

    showTaskModal(task = null) {
        if (!this.currentProject) {
            alert('Please select a project first');
            return;
        }

        this.editingTask = task;
        const modal = document.getElementById('task-modal');
        const title = document.getElementById('task-modal-title');

        if (task) {
            title.textContent = 'Edit Task';
            document.getElementById('task-name').value = task.name;
            document.getElementById('task-desc').value = task.description || '';
            document.getElementById('task-priority').value = task.priority;
            document.getElementById('task-due-date').value = task.dueDate || '';
        } else {
            title.textContent = 'New Task';
            document.getElementById('task-name').value = '';
            document.getElementById('task-desc').value = '';
            document.getElementById('task-priority').value = 'medium';
            document.getElementById('task-due-date').value = '';
        }

        modal.classList.add('active');
    }

    closeTaskModal() {
        document.getElementById('task-modal').classList.remove('active');
        this.editingTask = null;
    }

    async saveTask() {
        const name = document.getElementById('task-name').value.trim();
        if (!name) {
            alert('Please enter a task name');
            return;
        }

        const taskData = {
            name: name,
            description: document.getElementById('task-desc').value.trim(),
            priority: document.getElementById('task-priority').value,
            dueDate: document.getElementById('task-due-date').value,
            completed: false,
            projectId: this.currentProject.id
        };

        if (this.editingTask) {
            // Update existing
            Object.assign(this.editingTask, taskData);
        } else {
            // Create new
            const newId = Math.max(0, ...this.tasks.map(t => t.id)) + 1;
            this.tasks.push({ id: newId, ...taskData });
        }

        await this.saveData();
        this.renderTasks();
        this.updateStats();
        this.closeTaskModal();
    }

    async toggleTask(taskId) {
        const task = this.tasks.find(t => t.id === taskId);
        if (task) {
            task.completed = !task.completed;
            await this.saveData();
            this.renderTasks();
            this.updateStats();
        }
    }

    editTask(taskId) {
        const task = this.tasks.find(t => t.id === taskId);
        if (task) {
            this.showTaskModal(task);
        }
    }

    async deleteTask(taskId) {
        if (!confirm('Delete this task?')) return;
        
        this.tasks = this.tasks.filter(t => t.id !== taskId);
        await this.saveData();
        this.renderTasks();
        this.updateStats();
    }

    showSettingsModal() {
        document.getElementById('settings-modal').classList.add('active');
        this.loadSettings();
    }

    closeSettingsModal() {
        document.getElementById('settings-modal').classList.remove('active');
    }

    async loadGUISettings() {
        try {
            const response = await fetch('/api/settings');
            const settings = await response.json();
            
            document.getElementById('app-title').textContent = settings.appTitle || 'ToDo - SmartKraft';
            
            const cat1 = settings.cat1 || 'Work';
            const cat2 = settings.cat2 || 'Personal';
            const cat3 = settings.cat3 || 'Projects';
            
            document.getElementById('tab-cat1').textContent = cat1.toUpperCase();
            document.getElementById('tab-cat2').textContent = cat2.toUpperCase();
            document.getElementById('tab-cat3').textContent = cat3.toUpperCase();
            
            document.getElementById('tab-cat1').dataset.category = cat1;
            document.getElementById('tab-cat2').dataset.category = cat2;
            document.getElementById('tab-cat3').dataset.category = cat3;
        } catch (error) {
            console.error('[ToDoApp] Load GUI settings error:', error);
        }
    }

    async loadSettings() {
        try {
            const response = await fetch('/api/settings');
            const settings = await response.json();
            
            document.getElementById('setting-app-title').value = settings.appTitle || '';
            document.getElementById('setting-cat1').value = settings.cat1 || '';
            document.getElementById('setting-cat2').value = settings.cat2 || '';
            document.getElementById('setting-cat3').value = settings.cat3 || '';
            
            document.getElementById('theme-toggle').checked = settings.theme === 'dark';
        } catch (error) {
            console.error('[ToDoApp] Load settings error:', error);
        }

        // Load network settings
        try {
            const response = await fetch('/api/network/settings');
            const settings = await response.json();
            
            document.getElementById('ap-ssid').value = settings.apSSID || '';
            document.getElementById('ap-mdns').value = settings.apMDNS || '';
            document.getElementById('wifi-primary-ssid').value = settings.primarySSID || '';
            document.getElementById('wifi-primary-pass').value = settings.primaryPassword || '';
            document.getElementById('wifi-primary-ip').value = settings.primaryIP || '';
            document.getElementById('wifi-primary-mdns').value = settings.primaryMDNS || '';
            document.getElementById('wifi-backup-ssid').value = settings.backupSSID || '';
            document.getElementById('wifi-backup-pass').value = settings.backupPassword || '';
            document.getElementById('wifi-backup-ip').value = settings.backupIP || '';
            document.getElementById('wifi-backup-mdns').value = settings.backupMDNS || '';
        } catch (error) {
            console.error('[ToDoApp] Load network settings error:', error);
        }

        // Load mail settings
        try {
            const response = await fetch('/api/mail/config');
            const config = await response.json();
            
            document.getElementById('mail-enabled-toggle').checked = config.enabled || false;
            document.getElementById('smtp-server').value = config.smtpServer || '';
            document.getElementById('smtp-port').value = config.smtpPort || 587;
            document.getElementById('smtp-user').value = config.smtpUser || '';
            document.getElementById('smtp-from').value = config.smtpFrom || '';
            document.getElementById('smtp-ssl').checked = config.smtpSSL !== false;
        } catch (error) {
            console.error('[ToDoApp] Load mail config error:', error);
        }
    }

    async loadNetworkStatus() {
        try {
            const response = await fetch('/api/network/status');
            const status = await response.json();
            
            document.getElementById('info-wifi-status').textContent = status.mode || 'Unknown';
            document.getElementById('info-ip').textContent = status.ip || '0.0.0.0';
            document.getElementById('info-mdns').textContent = status.mdns || 'unknown.local';
        } catch (error) {
            console.error('[ToDoApp] Load network status error:', error);
        }

        // Load system info
        try {
            const response = await fetch('/api/system/info');
            const info = await response.json();
            
            document.getElementById('info-heap').textContent = Math.floor(info.freeHeap / 1024) + ' KB';
            document.getElementById('info-spiffs').textContent = 
                Math.floor(info.spiffsUsed / 1024) + ' / ' + Math.floor(info.spiffsTotal / 1024) + ' KB';
        } catch (error) {
            console.error('[ToDoApp] Load system info error:', error);
        }
    }
}

// Global functions
window.app = new ToDoApp();

window.closeProjectModal = () => app.closeProjectModal();
window.closeTaskModal = () => app.closeTaskModal();
window.closeSettingsModal = () => app.closeSettingsModal();

window.saveGeneralSettings = async () => {
    const settings = {
        theme: document.getElementById('theme-toggle').checked ? 'dark' : 'light',
        appTitle: document.getElementById('setting-app-title').value,
        cat1: document.getElementById('setting-cat1').value,
        cat2: document.getElementById('setting-cat2').value,
        cat3: document.getElementById('setting-cat3').value
    };

    try {
        const response = await fetch('/api/settings', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(settings)
        });
        
        if (response.ok) {
            alert('Settings saved! Reloading...');
            location.reload();
        } else {
            alert('Failed to save settings');
        }
    } catch (error) {
        console.error('Save settings error:', error);
        alert('Error saving settings');
    }
};

window.saveNetworkSettings = async () => {
    const settings = {
        apSSID: document.getElementById('ap-ssid').value,
        apMDNS: document.getElementById('ap-mdns').value,
        primarySSID: document.getElementById('wifi-primary-ssid').value,
        primaryPassword: document.getElementById('wifi-primary-pass').value,
        primaryIP: document.getElementById('wifi-primary-ip').value,
        primaryMDNS: document.getElementById('wifi-primary-mdns').value,
        backupSSID: document.getElementById('wifi-backup-ssid').value,
        backupPassword: document.getElementById('wifi-backup-pass').value,
        backupIP: document.getElementById('wifi-backup-ip').value,
        backupMDNS: document.getElementById('wifi-backup-mdns').value
    };

    try {
        const response = await fetch('/api/network/config', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(settings)
        });
        
        if (response.ok) {
            alert('Network settings saved! Device will restart...');
        } else {
            alert('Failed to save network settings');
        }
    } catch (error) {
        console.error('Save network settings error:', error);
        alert('Error saving network settings');
    }
};

window.saveMailSettings = async () => {
    const config = {
        enabled: document.getElementById('mail-enabled-toggle').checked,
        smtpServer: document.getElementById('smtp-server').value,
        smtpPort: parseInt(document.getElementById('smtp-port').value) || 587,
        smtpUser: document.getElementById('smtp-user').value,
        smtpPassword: document.getElementById('smtp-password').value,
        smtpFrom: document.getElementById('smtp-from').value,
        smtpSSL: document.getElementById('smtp-ssl').checked
    };

    try {
        const response = await fetch('/api/mail/config', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(config)
        });
        
        if (response.ok) {
            alert('Mail settings saved!');
        } else {
            alert('Failed to save mail settings');
        }
    } catch (error) {
        console.error('Save mail settings error:', error);
        alert('Error saving mail settings');
    }
};

window.exportBackup = () => {
    fetch('/api/backup/export')
        .then(r => r.text())
        .then(data => {
            const blob = new Blob([data], { type: 'application/json' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = 'To2Do_Backup_' + new Date().toISOString().split('T')[0] + '.json';
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            window.URL.revokeObjectURL(url);
        })
        .catch(e => {
            console.error('Export error:', e);
            alert('Failed to export backup');
        });
};

window.importBackup = (event) => {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (e) => {
        const data = e.target.result;
        
        fetch('/api/backup/import', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: data
        })
        .then(r => r.json())
        .then(result => {
            if (result.success) {
                alert('Backup imported successfully! Reloading...');
                location.reload();
            } else {
                alert('Failed to import backup');
            }
        })
        .catch(e => {
            console.error('Import error:', e);
            alert('Error importing backup');
        });
    };
    reader.readAsText(file);
};

window.factoryReset = () => {
    if (!confirm('This will delete ALL your data! Are you sure?')) return;
    if (!confirm('Really? This cannot be undone!')) return;

    fetch('/api/system/factory-reset', { method: 'POST' })
        .then(r => r.json())
        .then(result => {
            if (result.success) {
                alert('Factory reset complete! Reloading...');
                location.reload();
            } else {
                alert('Factory reset failed');
            }
        })
        .catch(e => {
            console.error('Factory reset error:', e);
            alert('Error during factory reset');
        });
};

// Show/hide password toggle
document.addEventListener('DOMContentLoaded', () => {
    document.getElementById('show-smtp-password')?.addEventListener('change', function() {
        const pwd = document.getElementById('smtp-password');
        pwd.type = this.checked ? 'text' : 'password';
    });
});

// Initialize app when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    app.init();
});
)rawliteral";
}

#endif
