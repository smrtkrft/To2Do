# Web Interface Overview

This document describes the To2Do web interface and its features.

## Interface Layout

The To2Do web interface is a single-page application with a modern, dark-themed design.

### Main Layout Structure

```
┌────────────────────────────────────────────────────────────────┐
│  SIDEBAR (Left)            │  MAIN CONTENT (Right)             │
│                            │                                   │
│  ┌──────────────────────┐  │  ┌─────────────────────────────┐ │
│  │  ToDo - SmartKraft   │  │  │  Project Title              │ │
│  │  ──────────────────  │  │  │  [Edit] [Archive]           │ │
│  │  STATS:              │  │  └─────────────────────────────┘ │
│  │  Total: 0   Active:0 │  │                                   │
│  │  Projects: 0         │  │  ┌─────────────────────────────┐ │
│  └──────────────────────┘  │  │  [All] [Active] [Completed] │ │
│                            │  │  [High Priority]              │ │
│  ┌──────────────────────┐  │  └─────────────────────────────┘ │
│  │ [WORK] [PERSONAL]    │  │                                   │
│  │      [PROJECTS]      │  │  ┌─────────────────────────────┐ │
│  └──────────────────────┘  │  │  □ Task Name                │ │
│                            │  │     Description...          │ │
│  ┌──────────────────────┐  │  │     [HIGH] 📅 2024-10-15   │ │
│  │  [Search...]         │  │  │     [✎] [🗑]                │ │
│  └──────────────────────┘  │  └─────────────────────────────┘ │
│                            │                                   │
│  ┌──────────────────────┐  │  ┌─────────────────────────────┐ │
│  │  Project 1           │  │  │  □ Another Task             │ │
│  │  [MEDIUM] 3 tasks    │  │  │     Task details...         │ │
│  └──────────────────────┘  │  │     [MEDIUM] 📅 2024-10-20 │ │
│                            │  │     [✎] [🗑]                │ │
│  ┌──────────────────────┐  │  └─────────────────────────────┘ │
│  │  Project 2           │  │                                   │
│  │  [HIGH] 5 tasks      │  │                                   │
│  └──────────────────────┘  │                                   │
│                            │                                   │
│  [+ PROJECT] [⚙ SETTINGS]  │                                   │
└────────────────────────────┴───────────────────────────────────┘
                                                            [+] ← FAB
```

## Color Scheme (Dark Theme)

- **Background Primary**: #0a0a0a (Nearly black)
- **Background Secondary**: #151515 (Very dark gray)
- **Background Tertiary**: #1f1f1f (Dark gray)
- **Text Primary**: #e8e8e8 (Light gray)
- **Text Secondary**: #a8a8a8 (Medium gray)
- **Accent Blue**: #3b82f6 (Primary actions, active states)
- **Accent Green**: #10b981 (Completed tasks, success)
- **Accent Orange**: #f59e0b (Medium priority, warnings)
- **Accent Red**: #ef4444 (High priority, delete actions)
- **Border**: #2a2a2a (Subtle borders)

## Key UI Components

### 1. Sidebar Header
- **App Title**: Customizable (default: "ToDo - SmartKraft")
- **Statistics Cards**:
  - Total tasks
  - Active tasks
  - Total projects

### 2. Category Tabs
- Three customizable categories
- Default: WORK, PERSONAL, PROJECTS
- Active tab highlighted in blue
- Clicking switches project view

### 3. Project Search
- Real-time search
- Filters projects by name or description
- Case-insensitive

### 4. Projects List
- Scrollable list of projects
- Shows:
  - Project name
  - Priority badge (colored)
  - Task count
- Click to select and view tasks

### 5. Action Buttons
- **+ PROJECT**: Create new project
- **⚙ SETTINGS**: Open settings modal

### 6. Content Header
- Current project title
- **Edit Button**: Modify project details
- **Archive Button**: Archive/unarchive project

### 7. Task Filters
- **All**: Show all tasks
- **Active**: Show incomplete tasks only
- **Completed**: Show completed tasks only
- **High Priority**: Show high-priority tasks only

### 8. Tasks List
- Scrollable list of tasks
- Each task shows:
  - Checkbox (click to complete)
  - Task name (crossed out when completed)
  - Description (optional)
  - Priority badge
  - Due date (if set)
  - Edit button (✎)
  - Delete button (🗑)

### 9. Floating Action Button (FAB)
- Fixed bottom-right position
- Blue circular button with "+"
- Click to create new task
- Only active when project selected

## Modals

### Task Modal

```
┌─────────────────────────────────────┐
│  New Task / Edit Task               │
├─────────────────────────────────────┤
│                                     │
│  Task Name:                         │
│  [________________________]         │
│                                     │
│  Description:                       │
│  [________________________]         │
│  [________________________]         │
│  [________________________]         │
│                                     │
│  Priority:                          │
│  [▼ Medium      ]                   │
│                                     │
│  Due Date:                          │
│  [📅 2024-10-14 ]                   │
│                                     │
├─────────────────────────────────────┤
│              [Cancel] [Save Task]   │
└─────────────────────────────────────┘
```

### Project Modal

```
┌─────────────────────────────────────┐
│  New Project / Edit Project         │
├─────────────────────────────────────┤
│                                     │
│  Project Name:                      │
│  [________________________]         │
│                                     │
│  Description:                       │
│  [________________________]         │
│  [________________________]         │
│                                     │
│  Category:                          │
│  [▼ Personal    ]                   │
│                                     │
│  Priority:                          │
│  [▼ Medium      ]                   │
│                                     │
├─────────────────────────────────────┤
│           [Cancel] [Save Project]   │
└─────────────────────────────────────┘
```

### Settings Modal

```
┌──────────────────────────────────────────────────────────────┐
│  Settings                                                    │
├──────────────────────────────────────────────────────────────┤
│  [General] [Connection] [Mail] [Backup] [Info]               │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  (Content changes based on active tab)                       │
│                                                              │
│  GENERAL TAB:                                                │
│  ────────────                                                │
│  Theme:                                                      │
│    AYDINLIK ◯────● KOYU                                      │
│                                                              │
│  Application Title:                                          │
│    [ToDo - SmartKraft______________]                         │
│                                                              │
│  Category Names:                                             │
│    [Work_________________________]                           │
│    [Personal_____________________]                           │
│    [Projects_____________________]                           │
│                                                              │
│  CONNECTION TAB:                                             │
│  ───────────────                                             │
│    ▼ AP MODE SETTINGS (Active)                               │
│    ▶ PRIMARY WIFI (Not Configured)                           │
│    ▶ BACKUP WIFI (Not Configured)                            │
│                                                              │
│  MAIL TAB:                                                   │
│  ─────────                                                   │
│    Email Notifications: DISABLED ◯───● ENABLED               │
│    ▶ SMTP CONFIGURATION (Not Configured)                     │
│                                                              │
│  BACKUP TAB:                                                 │
│  ───────────                                                 │
│    [EXPORT] [IMPORT]                                         │
│    [FACTORY RESET]                                           │
│                                                              │
│  INFO TAB:                                                   │
│  ─────────                                                   │
│    Version: SmartKraft-ToDo V1.1                             │
│    WiFi: AP Mode                                             │
│    IP: 192.168.4.1                                           │
│    Free Heap: 150 KB                                         │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│                               [Close] [Save Settings]        │
└──────────────────────────────────────────────────────────────┘
```

## Responsive Design

### Desktop (> 768px)
- Full sidebar visible
- Two-column layout
- All features accessible

### Tablet (480px - 768px)
- Narrower sidebar
- Smaller fonts
- Compact buttons

### Mobile (< 480px)
- Single column layout
- Sidebar can be toggled/hidden
- Touch-friendly buttons
- Larger tap targets

## User Interactions

### Creating a Project
1. Click "+ PROJECT" button in sidebar
2. Fill in project details in modal
3. Click "Save Project"
4. Project appears in sidebar under selected category

### Adding a Task
1. Select a project from sidebar
2. Click floating "+" button (bottom right)
3. Fill in task details in modal
4. Click "Save Task"
5. Task appears in main content area

### Completing a Task
1. Click checkbox next to task
2. Task name gets crossed out
3. Opacity reduced to show completion
4. Can uncheck to mark as active again

### Filtering Tasks
1. Select a project
2. Click filter buttons at top of content area
3. View updates to show only filtered tasks

### Searching Projects
1. Type in search box at top of sidebar
2. Projects list filters in real-time
3. Shows matching projects only

### Exporting Data
1. Click "⚙ SETTINGS"
2. Click "Backup" tab
3. Click "EXPORT" button
4. JSON file downloads to computer

### Importing Data
1. Click "⚙ SETTINGS"
2. Click "Backup" tab
3. Click "IMPORT" button
4. Select previously exported JSON file
5. Data restores and page reloads

## Keyboard Shortcuts

Currently not implemented, but planned:
- `N` - New task
- `P` - New project
- `S` - Open settings
- `F` - Focus search
- `Esc` - Close modal
- `Enter` - Save (when in modal)

## Accessibility Features

- High contrast color scheme
- Large clickable areas
- Clear visual hierarchy
- Semantic HTML structure
- Focus indicators
- Keyboard navigation (planned)
- Screen reader support (planned)

## Browser Compatibility

Tested and working on:
- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+
- ✅ Mobile browsers (iOS Safari, Chrome Mobile)

## Performance

- **Page Load**: < 1 second on local network
- **Task Creation**: Instant
- **Search**: Real-time (no lag)
- **Data Save**: < 500ms to SPIFFS
- **Memory Usage**: ~24KB for data buffer

## Future UI Enhancements

Planned improvements:
- Drag-and-drop task reordering
- Bulk operations (multi-select)
- Keyboard shortcuts
- Task quick-add (inline)
- Project color coding
- Custom themes
- Animations and transitions
- Toast notifications
- Loading indicators
- Progress bars for projects
- Calendar view
- Kanban board view
- Task dependencies visualization
