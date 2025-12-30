#include <windows.h>
#include <vector>
#include <string>
#include "VirtualKeyboard.h"
#include "KeyButton.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    VirtualKeyboard keyboard(hInstance);
    
    if (!keyboard.Create()) {
        return 1;
    }
    
    ShowWindow(keyboard.GetWindowHandle(), nCmdShow);
    UpdateWindow(keyboard.GetWindowHandle());
    
    // Message loop con PeekMessage per l'hook
    MSG msg = {};
    while (true) {
        // Processa i messaggi della finestra
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // Piccolo delay per non usare troppo CPU
        Sleep(1);
    }
    
    return (int)msg.wParam;
}
