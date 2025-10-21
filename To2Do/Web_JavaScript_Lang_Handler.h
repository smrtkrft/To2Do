#ifndef WEB_JAVASCRIPT_LANG_HANDLER_H
#define WEB_JAVASCRIPT_LANG_HANDLER_H

/*
 * Language Handler JavaScript
 * Handles language switching and translation application
 */

const char* getLanguageHandlerJS() {
  return R"rawliteral(
// Language Handler - Initialized after LANG object is loaded

// Load saved language from server
async function loadLanguage() {
  try {
    const response = await fetch('/api/language');
    const data = await response.json();
    if (data.current) {
      currentLang = data.current;
      updateLanguageButtons();
      applyAllTranslations();
    }
  } catch (error) {
    console.error('[Language] Failed to load:', error);
    currentLang = "EN"; // Fallback to English
  }
}

// Update language button states
function updateLanguageButtons() {
  document.querySelectorAll('.lang-btn').forEach(btn => {
    btn.classList.remove('active');
    if (btn.dataset.lang === currentLang) {
      btn.classList.add('active');
    }
  });
}

// Save language to server
async function saveLanguage(lang) {
  try {
    const response = await fetch('/api/language', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ language: lang })
    });
    const data = await response.json();
    if (data.success) {
      console.log('[Language] Saved:', lang);
      return true;
    }
  } catch (error) {
    console.error('[Language] Failed to save:', error);
  }
  return false;
}

// Apply all translations to the page
function applyAllTranslations() {
  console.log('[Language] Applying translations:', currentLang);
  
  // Stats
  const statLabels = document.querySelectorAll('.stat-label');
  if (statLabels[0]) statLabels[0].textContent = t('stat_total');
  if (statLabels[1]) statLabels[1].textContent = t('stat_active');
  if (statLabels[2]) statLabels[2].textContent = t('stat_projects');
  
  // Sidebar
  const projectSearch = document.getElementById('project-search');
  if (projectSearch) projectSearch.placeholder = t('search_projects');
  
  const newProjectBtn = document.getElementById('new-project-btn');
  if (newProjectBtn) newProjectBtn.textContent = t('new_project_btn');
  
  const settingsBtn = document.getElementById('settings-btn');
  if (settingsBtn) settingsBtn.textContent = t('settings_btn');
  
  // Panel filters
  const filters = document.querySelectorAll('.panel-filter-btn');
  if (filters[0]) filters[0].textContent = t('filter_all');
  if (filters[1]) filters[1].textContent = t('filter_active');
  if (filters[2]) filters[2].textContent = t('filter_completed');
  if (filters[3]) filters[3].textContent = t('filter_task');
  if (filters[4]) filters[4].textContent = t('filter_plan');
  if (filters[5]) filters[5].textContent = t('filter_note');
  if (filters[6]) filters[6].textContent = t('filter_reminder');
  
  // Settings tabs
  const settingsTabs = document.querySelectorAll('.settings-tab');
  if (settingsTabs[0]) settingsTabs[0].textContent = t('settings_tab_gui');
  if (settingsTabs[1]) settingsTabs[1].textContent = t('settings_tab_connection');
  if (settingsTabs[2]) settingsTabs[2].textContent = t('settings_tab_info');
  
  // Notification tabs
  const notifTabs = document.querySelectorAll('[data-tab^="notif-"]');
  if (notifTabs[0]) notifTabs[0].textContent = t('notif_today');
  if (notifTabs[1]) notifTabs[1].textContent = t('notif_tomorrow');
  if (notifTabs[2]) notifTabs[2].textContent = t('notif_week');
  if (notifTabs[3]) notifTabs[3].textContent = t('notif_overdue');
  
  // Labels
  const labels = document.querySelectorAll('.settings-label');
  labels.forEach(label => {
    const text = label.textContent.trim();
    if (text === 'TEMA & DIL' || text === 'THEME & LANGUAGE' || text === 'THEME & SPRACHE') {
      label.textContent = t('gui_theme_language');
    } else if (text === 'UYGULAMA BASLIGI' || text === 'APP TITLE' || text === 'APP-TITEL') {
      label.textContent = t('gui_app_title_label');
    } else if (text === 'KATEGORI ISIMLERI' || text === 'CATEGORY NAMES' || text === 'KATEGORIENAMEN') {
      label.textContent = t('gui_categories_label');
    } else if (text === 'TARIH / SAAT' || text === 'DATE / TIME' || text === 'DATUM / ZEIT') {
      label.textContent = t('gui_datetime_label');
    }
  });
  
  // Theme labels
  const themeLabels = document.querySelectorAll('.theme-label');
  if (themeLabels[0]) themeLabels[0].textContent = t('gui_theme_light');
  if (themeLabels[1]) themeLabels[1].textContent = t('gui_theme_dark');
  
  // Modal titles - will be updated when modals open
  // Buttons - will be updated when modals open
  
  console.log('[Language] Translations applied successfully');
}

// Setup language button handlers
function setupLanguageButtons() {
  document.querySelectorAll('.lang-btn').forEach(btn => {
    btn.addEventListener('click', async () => {
      const newLang = btn.dataset.lang;
      if (newLang !== currentLang) {
        currentLang = newLang;
        updateLanguageButtons();
        applyAllTranslations();
        await saveLanguage(newLang);
        
        // Refresh current content
        if (window.app && window.app.selectedProjectId) {
          window.app.loadTasksForProject(window.app.selectedProjectId);
        }
      }
    });
  });
}

// Initialize language system on page load
document.addEventListener('DOMContentLoaded', async () => {
  await loadLanguage();
  setupLanguageButtons();
});
)rawliteral";
}

#endif
