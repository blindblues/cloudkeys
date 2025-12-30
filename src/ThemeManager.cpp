#include "ThemeManager.h"
#include <fstream>
#include <iostream>

// Istanza statica
ThemeManager* ThemeManager::s_instance = nullptr;

ThemeManager& ThemeManager::GetInstance() {
    if (!s_instance) {
        s_instance = new ThemeManager();
    }
    return *s_instance;
}

ThemeManager::ThemeManager() {
    SetDefaultTheme();
}

void ThemeManager::ApplyToWindow(HWND hwnd) {
    if (!hwnd) return;
    
    // Applica trasparenza standard all'intera finestra
    const BackgroundConfig& bg = GetBackgroundConfig();
    
    // Imposta la finestra come layered se non lo è già
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (!(exStyle & WS_EX_LAYERED)) {
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
    }
    
    // Applica la trasparenza
    SetLayeredWindowAttributes(hwnd, 0, bg.color.alpha, LWA_ALPHA);
    
    // Forza il ridisegno della finestra
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
}

void ThemeManager::SetDefaultTheme() {
    // Title bar predefinita
    m_titleBar = TitleBarConfig();
    
    // Sfondo predefinito
    m_background = BackgroundConfig();
    
    // Tasti predefiniti
    m_keys = KeyConfig();
}

void ThemeManager::SetDarkTheme() {
    // Title bar scura
    m_titleBar.backgroundColor = ColorConfig(30, 30, 30, 255);
    m_titleBar.textColor = ColorConfig(255, 255, 255, 255);
    
    // Sfondo scuro
    m_background.color = ColorConfig(30, 30, 30, 204);
    m_background.transparency = 80;
    
    // Tasti scuri
    m_keys.normalColor = ColorConfig(60, 60, 60, 255);
    m_keys.pressedColor = ColorConfig(80, 80, 80, 255);
    m_keys.hoverColor = ColorConfig(70, 70, 70, 255);
    m_keys.toggleColor = ColorConfig(120, 80, 160, 255);
    m_keys.shiftColor = ColorConfig(200, 200, 100, 255);
    m_keys.textColor = ColorConfig(255, 255, 255, 255);
}

void ThemeManager::SetLightTheme() {
    // Title bar chiara
    m_titleBar.backgroundColor = ColorConfig(240, 240, 240, 255);
    m_titleBar.textColor = ColorConfig(0, 0, 0, 255);
    
    // Sfondo chiaro
    m_background.color = ColorConfig(240, 240, 240, 204);
    m_background.transparency = 80;
    
    // Tasti chiari
    m_keys.normalColor = ColorConfig(255, 255, 255, 255);
    m_keys.pressedColor = ColorConfig(230, 230, 230, 255);
    m_keys.hoverColor = ColorConfig(240, 240, 240, 255);
    m_keys.toggleColor = ColorConfig(200, 200, 255, 255);
    m_keys.shiftColor = ColorConfig(255, 255, 200, 255);
    m_keys.textColor = ColorConfig(0, 0, 0, 255);
}

void ThemeManager::SetBlueTheme() {
    // Title bar blu
    m_titleBar.backgroundColor = ColorConfig(0, 120, 215, 255);
    m_titleBar.textColor = ColorConfig(255, 255, 255, 255);
    
    // Sfondo blu
    m_background.color = ColorConfig(20, 40, 80, 204);
    m_background.transparency = 80;
    
    // Tasti blu
    m_keys.normalColor = ColorConfig(200, 200, 220, 255);
    m_keys.pressedColor = ColorConfig(100, 150, 255, 255);
    m_keys.hoverColor = ColorConfig(120, 170, 255, 255);
    m_keys.toggleColor = ColorConfig(150, 100, 200, 255);
    m_keys.shiftColor = ColorConfig(255, 255, 200, 255);
    m_keys.textColor = ColorConfig(0, 0, 0, 255);
}

void ThemeManager::LoadConfig() {
    // Implementazione base per caricare da file
    std::ifstream file("theme_config.txt");
    if (!file.is_open()) {
        SetDefaultTheme();
        return;
    }
    
    // TODO: Implementare parsing del file di configurazione
    // Per ora usa il tema di default
    SetDefaultTheme();
    
    file.close();
}

void ThemeManager::SaveConfig() {
    std::ofstream file("theme_config.txt");
    if (!file.is_open()) return;
    
    // TODO: Implementare salvataggio della configurazione
    file << "# CloudKeys Theme Configuration\n";
    file << "background_color=" << (int)m_background.color.r << "," 
         << (int)m_background.color.g << "," << (int)m_background.color.b << "\n";
    file << "background_alpha=" << (int)m_background.color.alpha << "\n";
    file << "transparency=" << (int)m_background.transparency << "\n";
    
    file.close();
}
