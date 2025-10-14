# Changelog

All notable changes to the To2Do project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2024-10-14

### Added
- Initial public release of To2Do ESP32 application
- Web-based task management interface
- Project and task organization system
- Three customizable categories (Work, Personal, Projects)
- Priority levels for projects and tasks (High, Medium, Low)
- SPIFFS-based local data storage
- Import/Export functionality for data backup
- Factory reset capability
- WiFi client mode support
- Access Point (AP) mode for standalone operation
- mDNS support for friendly URL access
- SMTP email notification framework
- Modern dark-themed responsive UI
- Project search functionality
- Task filtering (All, Active, Completed, High Priority)
- Demo data with 7 sample projects
- Comprehensive documentation
  - README.md with full feature documentation
  - GETTING_STARTED.md for quick setup
  - CONFIGURATION_EXAMPLES.md for common setups
  - HARDWARE_SETUP.md for physical assembly
- Settings management:
  - GUI customization (theme, categories, app title)
  - Network configuration (Primary and Backup WiFi)
  - SMTP email configuration
  - System information display
- Serial monitor debugging output
- Real-time statistics (total tasks, active tasks, total projects)

### Core Components Implemented
- `Data_Manager.h` - SPIFFS data persistence with 24KB JSON buffer
- `WiFi_Manager.h` - Dual-mode WiFi management
- `Mail_Manager.h` - SMTP email integration
- `Backup_Manager.h` - Import/Export functionality
- `Notification_Manager.h` - Notification framework
- `Persistence.h` - Unified data access layer
- `Web_Interface.h` - HTML/CSS web interface
- `Web_JavaScript_Core.h` - Client-side application logic
- `To2Do.ino` - Main Arduino sketch

### Technical Details
- ESP32 platform support
- ArduinoJson v6.x integration
- ESP Mail Client library integration
- RESTful API endpoints for all operations
- Single JSON file storage (`/userdata.json`)
- Accordion-style settings interface
- Modal dialogs for project/task editing
- Floating Action Button (FAB) for quick task creation

### Planned (Not Yet Implemented)
- OLED display support
- Physical button input
- Startup animations
- Advanced notification scheduling
- Multi-language support (EN, DE, TR)
- Browser password protection
- Cloud sync option

## [Unreleased]

### Planned Features
- OLED display integration
  - Startup animation (first boot only)
  - Real-time task statistics
  - WiFi status display
  - Project and task counts
  - mDNS address display
- Hardware button support
  - Menu navigation
  - Task completion toggle
  - Quick project switching
- Enhanced notifications
  - Due date reminders
  - Email scheduling
  - Daily/weekly summaries
- Localization
  - English (EN)
  - German (DE)
  - Turkish (TR)
- Security features
  - Optional password protection
  - Login system for web interface
- Advanced features
  - Task tags and labels
  - Calendar integration
  - Recurring tasks
  - Task dependencies
  - Time tracking
  - Statistics and analytics

### Under Consideration
- Mobile app (companion)
- Desktop application
- Cloud backup (optional, privacy-focused)
- Multi-user support
- API access for third-party integrations
- Voice control integration
- Dark/Light theme auto-switching
- Customizable color schemes
- Widget system for dashboard
- Task templates
- Project templates
- Gantt chart view
- Kanban board view

## Version Numbering

**Format:** MAJOR.MINOR.PATCH

- **MAJOR**: Incompatible API changes or major feature additions
- **MINOR**: Backwards-compatible functionality additions
- **PATCH**: Backwards-compatible bug fixes

## Contributing

See AUTHORS.md for contribution guidelines.

## Links

- [GitHub Repository](https://github.com/smrtkrft/To2Do)
- [Issue Tracker](https://github.com/smrtkrft/To2Do/issues)
- [Documentation](README.md)
