#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "KeyButton.h"

class VirtualKeyboard {
private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    std::vector<KeyButton> m_keys;
    static const wchar_t* CLASS_NAME;
    static HHOOK s_keyboardHook;
    static VirtualKeyboard* s_instance;
    
    // Font personalizzato
    HFONT m_customFont;
    
    // Glassmorphism effects
    void DrawGlassBackground(HDC hdc, const RECT& rect);
    void DrawGlassKey(HDC hdc, const KeyButton& key);
    void CreateGlassEffect(HDC hdc, const RECT& rect, COLORREF baseColor, float transparency);
    void DrawGlassBorder(HDC hdc, const RECT& rect);
    void DrawGlassReflection(HDC hdc, const RECT& rect);
    
    // Hover animation
    int m_hoverKeyIndex;
    float m_hoverAnimation;
    void UpdateHoverAnimation();
    bool IsPointOverKey(int xPos, int yPos, int& keyIndex);
    
    // Layout tastiera italiana
    void InitializeKeys();
    void CreateKey(int x, int y, int width, int height, const std::wstring& label, const std::wstring& shiftLabel, WORD keyCode);
    
    // Window procedure
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    // Hook per tastiera fisica
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void InstallKeyboardHook();
    void UninstallKeyboardHook();
    
    // Eventi
    void OnPaint();
    void OnLButtonDown(int xPos, int yPos);
    void OnLButtonUp(int xPos, int yPos);
    void OnMouseMove(int xPos, int yPos);
    void OnPhysicalKeyDown(WPARAM keyCode);
    void OnPhysicalKeyUp(WPARAM keyCode);
    void CheckPhysicalKeyboardState();
    bool IsMouseOverKey(const KeyButton& key);
    void OnSize(int width, int height);
    bool LoadCustomFont();
    
public:
    VirtualKeyboard(HINSTANCE hInstance);
    ~VirtualKeyboard();
    
    bool Create();
    HWND GetWindowHandle() const { return m_hwnd; }
};
