#pragma once
#include <windows.h>
#include <string>

struct KeyButton {
    RECT rect;
    std::wstring label;
    std::wstring shiftLabel;
    WORD keyCode;
    bool isPressed;
    bool isShiftPressed;
    bool isToggled;  // Per tasti come Caps Lock che rimangono attivi
    
    KeyButton() : keyCode(0), isPressed(false), isShiftPressed(false), isToggled(false) {}
    KeyButton(int x, int y, int width, int height, const std::wstring& lbl, const std::wstring& shiftLbl, WORD code)
        : label(lbl), shiftLabel(shiftLbl), keyCode(code), isPressed(false), isShiftPressed(false), isToggled(false) {
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;
    }
    
    bool ContainsPoint(int x, int y) const {
        return x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom;
    }
    
    std::wstring GetCurrentLabel() const {
        if (isToggled) {
            return L"●" + (isShiftPressed ? shiftLabel : label);
        }
        return isShiftPressed ? shiftLabel : label;
    }
};
