#pragma once
#include <windows.h>

// Struttura per configurazione colore
struct ColorConfig {
    BYTE r, g, b;
    BYTE alpha;  // 0-255
    
    ColorConfig(BYTE red = 0, BYTE green = 0, BYTE blue = 0, BYTE a = 255) 
        : r(red), g(green), b(blue), alpha(a) {}
    
    COLORREF ToColorRef() const { return RGB(r, g, b); }
};

// Struttura per configurazione della title bar
struct TitleBarConfig {
    ColorConfig backgroundColor;
    ColorConfig textColor;
    bool enabled;
    bool customDrawing;
    
    TitleBarConfig() : backgroundColor(20, 40, 80, 255), 
                      textColor(255, 255, 255, 255),
                      enabled(true), customDrawing(false) {}
};

// Struttura per configurazione dello sfondo
struct BackgroundConfig {
    ColorConfig color;
    BYTE transparency;  // 0-100%
    bool blurEnabled;
    
    BackgroundConfig() : color(20, 40, 80, 204), 
                        transparency(80), blurEnabled(false) {}
};

// Struttura per configurazione dei tasti
struct KeyConfig {
    ColorConfig normalColor;
    ColorConfig pressedColor;
    ColorConfig hoverColor;
    ColorConfig toggleColor;
    ColorConfig shiftColor;
    ColorConfig borderColor;
    ColorConfig textColor;
    int borderWidth;
    
    KeyConfig() : normalColor(200, 200, 220, 255),
                  pressedColor(100, 150, 255, 255),
                  hoverColor(150, 180, 255, 255),
                  toggleColor(150, 100, 200, 255),
                  shiftColor(255, 255, 200, 255),
                  borderColor(0, 0, 0, 255),
                  textColor(0, 0, 0, 255),
                  borderWidth(1) {}
};

// Classe principale per gestione temi
class ThemeManager {
private:
    TitleBarConfig m_titleBar;
    BackgroundConfig m_background;
    KeyConfig m_keys;
    
    static ThemeManager* s_instance;
    
public:
    static ThemeManager& GetInstance();
    
    // Getter per le configurazioni
    TitleBarConfig& GetTitleBarConfig() { return m_titleBar; }
    BackgroundConfig& GetBackgroundConfig() { return m_background; }
    KeyConfig& GetKeyConfig() { return m_keys; }
    
    // Const getter
    const TitleBarConfig& GetTitleBarConfig() const { return m_titleBar; }
    const BackgroundConfig& GetBackgroundConfig() const { return m_background; }
    const KeyConfig& GetKeyConfig() const { return m_keys; }
    
    // Applica le configurazioni alla finestra
    void ApplyToWindow(HWND hwnd);
    
    // Carica/salva configurazioni
    void LoadConfig();
    void SaveConfig();
    
    // Temi predefiniti
    void SetDefaultTheme();
    void SetDarkTheme();
    void SetLightTheme();
    void SetBlueTheme();
    
private:
    ThemeManager();
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
};
