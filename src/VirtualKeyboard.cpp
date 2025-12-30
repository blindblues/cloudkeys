#include "VirtualKeyboard.h"
#include <windowsx.h>

// Macro per compatibilità
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

// Variabili statiche per l'hook
HHOOK VirtualKeyboard::s_keyboardHook = nullptr;
VirtualKeyboard* VirtualKeyboard::s_instance = nullptr;

const wchar_t* VirtualKeyboard::CLASS_NAME = L"VirtualKeyboardWindowClass";

VirtualKeyboard::VirtualKeyboard(HINSTANCE hInstance) 
    : m_hInstance(hInstance), m_hwnd(nullptr), m_customFont(nullptr) {
    s_instance = this;
    LoadCustomFont();
    InitializeKeys();
    InstallKeyboardHook();
}

VirtualKeyboard::~VirtualKeyboard() {
    UninstallKeyboardHook();
    if (m_customFont) {
        DeleteObject(m_customFont);
    }
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }
}

// Hook globale per tastiera fisica
LRESULT CALLBACK VirtualKeyboard::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && s_instance) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        
        // Debug: stampa il codice del tasto
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // Forza il ridisegno per vedere se l'hook funziona
            InvalidateRect(s_instance->m_hwnd, NULL, FALSE);
        }
        
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            s_instance->OnPhysicalKeyDown(pKeyboard->vkCode);
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            s_instance->OnPhysicalKeyUp(pKeyboard->vkCode);
        }
    }
    
    return CallNextHookEx(s_keyboardHook, nCode, wParam, lParam);
}

void VirtualKeyboard::InstallKeyboardHook() {
    if (!s_keyboardHook) {
        s_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
    }
}

void VirtualKeyboard::UninstallKeyboardHook() {
    if (s_keyboardHook) {
        UnhookWindowsHookEx(s_keyboardHook);
        s_keyboardHook = nullptr;
    }
}

void VirtualKeyboard::InitializeKeys() {
    // Riga 0: Tasti Funzione F1-F12
    CreateKey(10, 10, 45, 30, L"F1", L"F1", VK_F1);
    CreateKey(60, 10, 45, 30, L"F2", L"F2", VK_F2);
    CreateKey(110, 10, 45, 30, L"F3", L"F3", VK_F3);
    CreateKey(160, 10, 45, 30, L"F4", L"F4", VK_F4);
    CreateKey(210, 10, 45, 30, L"F5", L"F5", VK_F5);
    CreateKey(260, 10, 45, 30, L"F6", L"F6", VK_F6);
    CreateKey(310, 10, 45, 30, L"F7", L"F7", VK_F7);
    CreateKey(360, 10, 45, 30, L"F8", L"F8", VK_F8);
    CreateKey(410, 10, 45, 30, L"F9", L"F9", VK_F9);
    CreateKey(460, 10, 45, 30, L"F10", L"F10", VK_F10);
    CreateKey(510, 10, 45, 30, L"F11", L"F11", VK_F11);
    CreateKey(560, 10, 45, 30, L"F12", L"F12", VK_F12);
    
    // Layout tastiera italiana standard QWERTY
    // Riga 1: tasti numerici e simboli
    CreateKey(10, 50, 50, 40, L"Esc", L"Esc", VK_ESCAPE);
    CreateKey(65, 50, 40, 40, L"1", L"!", '1');
    CreateKey(110, 50, 40, 40, L"2", L"\"", '2');
    CreateKey(155, 50, 40, 40, L"3", L"£", '3');
    CreateKey(200, 50, 40, 40, L"4", L"$", '4');
    CreateKey(245, 50, 40, 40, L"5", L"%", '5');
    CreateKey(290, 50, 40, 40, L"6", L"&", '6');
    CreateKey(335, 50, 40, 40, L"7", L"/", '7');
    CreateKey(380, 50, 40, 40, L"8", L"(", '8');
    CreateKey(425, 50, 40, 40, L"9", L")", '9');
    CreateKey(470, 50, 40, 40, L"0", L"=", '0');
    CreateKey(515, 50, 40, 40, L"'", L"?", VK_OEM_4);
    CreateKey(560, 50, 40, 40, L"ì", L"^", VK_OEM_1);
    CreateKey(605, 50, 70, 40, L"Back", L"Back", VK_BACK);
    
    // Riga 2: QWERTYUIOP
    CreateKey(10, 100, 60, 40, L"Tab", L"Tab", VK_TAB);
    CreateKey(75, 100, 40, 40, L"q", L"Q", 'Q');
    CreateKey(120, 100, 40, 40, L"w", L"W", 'W');
    CreateKey(165, 100, 40, 40, L"e", L"E", 'E');
    CreateKey(210, 100, 40, 40, L"r", L"R", 'R');
    CreateKey(255, 100, 40, 40, L"t", L"T", 'T');
    CreateKey(300, 100, 40, 40, L"y", L"Y", 'Y');
    CreateKey(345, 100, 40, 40, L"u", L"U", 'U');
    CreateKey(390, 100, 40, 40, L"i", L"I", 'I');
    CreateKey(435, 100, 40, 40, L"o", L"O", 'O');
    CreateKey(480, 100, 40, 40, L"p", L"P", 'P');
    CreateKey(525, 100, 40, 40, L"è", L"é", VK_OEM_6);
    CreateKey(570, 100, 40, 40, L"+", L"*", VK_OEM_PLUS);
    CreateKey(615, 100, 60, 40, L"Enter", L"Enter", VK_RETURN);
    
    // Riga 3: ASDFGHJKLòàù
    CreateKey(10, 150, 70, 40, L"Caps", L"Caps", VK_CAPITAL);
    CreateKey(85, 150, 40, 40, L"a", L"A", 'A');
    CreateKey(130, 150, 40, 40, L"s", L"S", 'S');
    CreateKey(175, 150, 40, 40, L"d", L"D", 'D');
    CreateKey(220, 150, 40, 40, L"f", L"F", 'F');
    CreateKey(265, 150, 40, 40, L"g", L"G", 'G');
    CreateKey(310, 150, 40, 40, L"h", L"H", 'H');
    CreateKey(355, 150, 40, 40, L"j", L"J", 'J');
    CreateKey(400, 150, 40, 40, L"k", L"K", 'K');
    CreateKey(445, 150, 40, 40, L"l", L"L", 'L');
    CreateKey(490, 150, 40, 40, L"ò", L"ç", VK_OEM_3);
    CreateKey(535, 150, 40, 40, L"à", L"°", VK_OEM_5);
    CreateKey(580, 150, 40, 40, L"ù", L"§", VK_OEM_7);
    CreateKey(625, 150, 50, 40, L"Del", L"Del", VK_DELETE);
    
    // Riga 4: ZXCVBNM,.-_<> 
    CreateKey(10, 200, 90, 40, L"Shift", L"Shift", VK_SHIFT);
    CreateKey(105, 200, 40, 40, L"<", L">", VK_OEM_102);
    CreateKey(150, 200, 40, 40, L"z", L"Z", 'Z');
    CreateKey(195, 200, 40, 40, L"x", L"X", 'X');
    CreateKey(240, 200, 40, 40, L"c", L"C", 'C');
    CreateKey(285, 200, 40, 40, L"v", L"V", 'V');
    CreateKey(330, 200, 40, 40, L"b", L"B", 'B');
    CreateKey(375, 200, 40, 40, L"n", L"N", 'N');
    CreateKey(420, 200, 40, 40, L"m", L"M", 'M');
    CreateKey(465, 200, 40, 40, L",", L";", VK_OEM_COMMA);
    CreateKey(510, 200, 40, 40, L".", L":", VK_OEM_PERIOD);
    CreateKey(555, 200, 40, 40, L"-", L"_", VK_OEM_MINUS);
    CreateKey(600, 200, 75, 40, L"Shift", L"Shift", VK_SHIFT);
    
    // Riga 5: Space bar e altri
    CreateKey(10, 250, 60, 40, L"Ctrl", L"Ctrl", VK_CONTROL);
    CreateKey(75, 250, 60, 40, L"Win", L"Win", VK_LWIN);
    CreateKey(140, 250, 60, 40, L"Alt", L"Alt", VK_MENU);
    CreateKey(205, 250, 280, 40, L"Space", L"Space", VK_SPACE);
    CreateKey(490, 250, 60, 40, L"AltGr", L"AltGr", VK_MENU);
    CreateKey(555, 250, 60, 40, L"Menu", L"Menu", VK_APPS);
    CreateKey(620, 250, 60, 40, L"Ctrl", L"Ctrl", VK_CONTROL);
}

void VirtualKeyboard::CreateKey(int x, int y, int width, int height, const std::wstring& label, const std::wstring& shiftLabel, WORD keyCode) {
    m_keys.emplace_back(x, y, width, height, label, shiftLabel, keyCode);
}

bool VirtualKeyboard::Create() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassW(&wc)) {
        return false;
    }
    
    m_hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_LAYERED,
        CLASS_NAME,
        L"CloudKeys",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 690, 300,  // Dimensioni esatte per i tasti
        NULL,
        NULL,
        m_hInstance,
        this
    );
    
    // Abilita ClearType per migliore rendering del testo
    if (m_hwnd) {
        // Imposta la finestra trasparente all'80%
        SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 204, LWA_ALPHA);
        
        HDC hdc = GetDC(m_hwnd);
        SetGraphicsMode(hdc, GM_ADVANCED);
        SetBkMode(hdc, TRANSPARENT);
        SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
        ReleaseDC(m_hwnd, hdc);
        
        // Avvia il timer per controllare la tastiera fisica
        SetTimer(m_hwnd, 1, 100, NULL); // Ridotto a 100ms per meno interferenza
    }
    
    return m_hwnd != nullptr;
}

LRESULT CALLBACK VirtualKeyboard::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    VirtualKeyboard* pThis = nullptr;
    
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = (CREATESTRUCTW*)lParam;
        pThis = (VirtualKeyboard*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->m_hwnd = hwnd;
    } else {
        pThis = (VirtualKeyboard*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    
    if (pThis) {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT VirtualKeyboard::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
            OnPaint();
            return 0;
        case WM_LBUTTONDOWN:
            OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_LBUTTONUP:
            OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_MOUSEMOVE:
            OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_TIMER:
            if (wParam == 1) {
                // Timer per controllare lo stato dei tasti fisici
                CheckPhysicalKeyboardState();
            } else if (wParam == 2) {
                // Timer per il ridimensionamento - forza il ridisegno finale
                InvalidateRect(m_hwnd, NULL, TRUE); // TRUE per cancellare lo sfondo
                UpdateWindow(m_hwnd); // Forza il ridisegno immediato
                KillTimer(m_hwnd, 2);
            }
            return 0;
        case WM_SIZE:
            OnSize(LOWORD(lParam), HIWORD(lParam));
            return 0;
        case WM_EXITSIZEMOVE:
            // Forza il ridisegno completo quando termina il ridimensionamento
            InvalidateRect(m_hwnd, NULL, TRUE);
            UpdateWindow(m_hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void VirtualKeyboard::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);
    
    // Double buffering per eliminare flickering
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;
    
    // Calcola la dimensione del font responsive basata sulla larghezza della finestra
    int fontSize = std::max(8, (int)(14.0f * ((float)width / 690.0f)));  // Ridotto da 18 a 14
    fontSize = std::min(fontSize, 20); // Ridotto il massimo da 24 a 20
    
    // Crea un DC in memoria per il double buffering
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    
    // Pulisci il background con blu scuro trasparente
    HBRUSH bgBrush = CreateSolidBrush(RGB(20, 40, 80));
    FillRect(memDC, &clientRect, bgBrush);
    DeleteObject(bgBrush);
    
    // Font responsive per i tasti - usa il font personalizzato se disponibile
    HFONT hFont;
    HFONT hOldFont;
    if (m_customFont) {
        // Ricrea il font personalizzato con la dimensione responsive
        hFont = CreateFontW(
            fontSize,             // Altezza responsive
            0,                    // Larghezza (0 = proporzionale all'altezza)
            0,                    // Angolo di escape
            0,                    // Angolo di orientamento
            FW_NORMAL,            // Peso Regular
            FALSE,                // Italic
            FALSE,                // Underline
            FALSE,                // Strikeout
            ANSI_CHARSET,         // Character set specifico
            OUT_TT_PRECIS,        // Output precision per TrueType
            CLIP_DEFAULT_PRECIS,  // Clipping precision
            CLEARTYPE_QUALITY,    // Qualità ClearType per rendering anti-aliasing
            DEFAULT_PITCH | FF_DONTCARE, // Pitch and family
            L"Space Mono"         // Nome del font
        );
    } else {
        // Fallback ad Arial se il font personalizzato non è caricato
        hFont = CreateFontW(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    }
    hOldFont = (HFONT)SelectObject(memDC, hFont);
    
    // Disegna tutti i tasti sul DC in memoria
    for (auto& key : m_keys) {
        // Colore del tasto con stati multipli
        HBRUSH hBrush;
        if (key.isToggled) {
            // Colore viola per tasti toggle attivi (Caps Lock)
            hBrush = CreateSolidBrush(RGB(150, 100, 200));
        } else if (key.isPressed) {
            // Colore vivace quando premuto
            hBrush = CreateSolidBrush(RGB(100, 150, 255));
        } else if (key.isShiftPressed) {
            // Colore per indicare che Shift è attivo
            hBrush = CreateSolidBrush(RGB(255, 255, 200));
        } else {
            // Colore normale per contrastare con lo sfondo blu scuro
            hBrush = CreateSolidBrush(RGB(200, 200, 220));
        }
        
        FillRect(memDC, &key.rect, hBrush);
        DeleteObject(hBrush);
        
        // Bordo del tasto - solo un bordo alla volta
        HPEN hPen;
        if (key.isToggled) {
            hPen = CreatePen(PS_SOLID, 3, RGB(100, 50, 150));
        } else {
            hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        }
        HPEN hOldPen = (HPEN)SelectObject(memDC, hPen);
        Rectangle(memDC, key.rect.left, key.rect.top, key.rect.right, key.rect.bottom);
        SelectObject(memDC, hOldPen);
        DeleteObject(hPen);
        
        // Testo del tasto con colore contrastante e miglior rendering
        if (key.isToggled || key.isPressed) {
            SetTextColor(memDC, RGB(255, 255, 255));
        } else {
            SetTextColor(memDC, RGB(0, 0, 0));
        }
        SetBkMode(memDC, TRANSPARENT);
        
        std::wstring text = key.GetCurrentLabel();
        RECT textRect = key.rect;
        
        // Disegna il testo perfettamente centrato orizzontalmente e verticalmente
        DrawTextW(memDC, text.c_str(), -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX);
    }
    
    // Copia il buffer in memoria sulla finestra
    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
    
    // Cleanup
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    SelectObject(hdc, hOldFont);
    // Elimina il font dinamico (non il font personalizzato originale)
    if (hFont != m_customFont) {
        DeleteObject(hFont);
    }
    
    EndPaint(m_hwnd, &ps);
}

void VirtualKeyboard::OnLButtonDown(int xPos, int yPos) {
    bool keyClicked = false;
    
    // Controlla se il clic è su un tasto
    for (auto& key : m_keys) {
        if (key.ContainsPoint(xPos, yPos)) {
            key.isPressed = true;
            keyClicked = true;
            
            // Gestione del toggle Shift
            if (key.keyCode == VK_SHIFT) {
                // Attiva Shift per tutti i tasti
                for (auto& k : m_keys) {
                    k.isShiftPressed = true;
                }
                InvalidateRect(m_hwnd, NULL, FALSE);
            } else if (key.keyCode == VK_CAPITAL) {
                // Gestione Caps Lock
                key.isToggled = !key.isToggled;
                // Aggiorna lo stato Shift per tutte le lettere
                for (auto& k : m_keys) {
                    if (k.keyCode >= 'A' && k.keyCode <= 'Z') {
                        k.isShiftPressed = key.isToggled;
                    }
                }
                InvalidateRect(m_hwnd, NULL, FALSE);
            } else {
                // Invia il tasto all'applicazione attiva usando il mapping corretto
                INPUT input = {};
                input.type = INPUT_KEYBOARD;
                
                // Mapping corretto per tastiera italiana
                if (key.keyCode >= 'A' && key.keyCode <= 'Z') {
                    // Lettere: gestisci maiuscolo/minuscolo correttamente
                    input.ki.wVk = 0; // Non usare VK
                    wchar_t charToSend = key.isShiftPressed ? key.keyCode : (key.keyCode + 32);
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode >= '0' && key.keyCode <= '9') {
                    // Numeri: usa il codice carattere diretto
                    input.ki.wVk = 0;
                    input.ki.wScan = key.keyCode;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_SPACE) {
                    // Spazio
                    input.ki.wVk = VK_SPACE;
                    input.ki.dwFlags = 0;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_1) { // ì
                    wchar_t charToSend = key.isShiftPressed ? L'^' : L'ì';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_3) { // ò
                    wchar_t charToSend = key.isShiftPressed ? L'ç' : L'ò';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_5) { // à
                    wchar_t charToSend = key.isShiftPressed ? L'°' : L'à';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_6) { // è
                    wchar_t charToSend = key.isShiftPressed ? L'é' : L'è';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_7) { // ù
                    wchar_t charToSend = key.isShiftPressed ? L'§' : L'ù';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_102) { // <>
                    wchar_t charToSend = key.isShiftPressed ? L'>' : L'<';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_COMMA) { // ,;
                    wchar_t charToSend = key.isShiftPressed ? L';' : L',';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_PERIOD) { // .:
                    wchar_t charToSend = key.isShiftPressed ? L':' : L'.';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_MINUS) { // -_
                    wchar_t charToSend = key.isShiftPressed ? L'_' : L'-';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else if (key.keyCode == VK_OEM_PLUS) { // +*
                    wchar_t charToSend = key.isShiftPressed ? L'*' : L'+';
                    input.ki.wVk = 0;
                    input.ki.wScan = charToSend;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                } else {
                    // Tasti speciali standard (Backspace, Enter, etc.)
                    input.ki.wVk = key.keyCode;
                    input.ki.dwFlags = 0;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                }
            }
            
            InvalidateRect(m_hwnd, &key.rect, TRUE);
            break;
        }
    }
    
    // Se non è stato cliccato nessun tasto, inizia il trascinamento della finestra
    if (!keyClicked) {
        PostMessage(m_hwnd, WM_SYSCOMMAND, SC_MOVE | 2, 0);
    }
}

void VirtualKeyboard::CheckPhysicalKeyboardState() {
    static bool lastAnyKeyPressed = false;
    bool needsRedraw = false;
    bool anyKeyPressed = false;
    
    // Controlla lo stato attuale di Shift e Caps Lock
    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool isCapsLockActive = (GetKeyState(VK_CAPITAL) & 1) != 0;
    
    // Prima passa: controlla solo se ci sono cambiamenti
    for (auto& key : m_keys) {
        bool isCurrentlyPressed = (GetAsyncKeyState(key.keyCode) & 0x8000) != 0;
        
        if (isCurrentlyPressed) {
            anyKeyPressed = true;
        }
        
        // Controlla solo i cambiamenti effettivi
        if (key.keyCode == VK_CAPITAL) {
            if (isCapsLockActive != key.isToggled) {
                key.isToggled = isCapsLockActive;
                key.isPressed = isCapsLockActive;
                needsRedraw = true;
            }
        } else if (key.keyCode == VK_SHIFT) {
            // Gestione speciale per Shift - aggiorna solo lo stato shift
            // non lo stato pressed per non interferire con il mouse
        } else {
            // Per gli altri tasti non fare nulla - lasciali gestire solo dal mouse
            // Questo previene interferenze con l'animazione
        }
        
        // Aggiorna lo stato Shift per le etichette
        if (key.keyCode >= 'A' && key.keyCode <= 'Z') {
            bool shouldBeShifted = isCapsLockActive ^ isShiftPressed;
            if (shouldBeShifted != key.isShiftPressed) {
                key.isShiftPressed = shouldBeShifted;
                needsRedraw = true;
            }
        } else {
            if (isShiftPressed != key.isShiftPressed) {
                key.isShiftPressed = isShiftPressed;
                needsRedraw = true;
            }
        }
    }
    
    // Ridisegna solo se ci sono stati cambiamenti reali
    if (needsRedraw) {
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    
    lastAnyKeyPressed = anyKeyPressed;
}

void VirtualKeyboard::OnPhysicalKeyDown(WPARAM keyCode) {
    // Funzione non più usata, mantenuta per compatibilità
}

void VirtualKeyboard::OnPhysicalKeyUp(WPARAM keyCode) {
    // Trova e rilascia il tasto corrispondente
    for (auto& key : m_keys) {
        if (key.keyCode == keyCode) {
            // Non rilasciare i tasti toggle (Caps Lock)
            if (key.keyCode != VK_CAPITAL) {
                key.isPressed = false;
            }
            
            // Gestione speciale per Shift
            if (key.keyCode == VK_SHIFT) {
                // Disattiva Shift per tutti i tasti non-lettere
                for (auto& k : m_keys) {
                    if (k.keyCode >= 'A' && k.keyCode <= 'Z') {
                        // Mantieni lo stato Caps Lock se attivo
                        bool capsLockActive = false;
                        for (auto& capsKey : m_keys) {
                            if (capsKey.keyCode == VK_CAPITAL && capsKey.isToggled) {
                                capsLockActive = true;
                                break;
                            }
                        }
                        k.isShiftPressed = capsLockActive;
                    } else {
                        k.isShiftPressed = false;
                    }
                }
                InvalidateRect(m_hwnd, NULL, FALSE);
            }
            
            InvalidateRect(m_hwnd, &key.rect, TRUE);
            break;
        }
    }
}

void VirtualKeyboard::OnLButtonUp(int xPos, int yPos) {
    for (auto& key : m_keys) {
        if (key.ContainsPoint(xPos, yPos)) {
            // Non rilasciare i tasti toggle (Caps Lock)
            if (key.keyCode != VK_CAPITAL) {
                key.isPressed = false;
            }
            InvalidateRect(m_hwnd, NULL, FALSE);
            UpdateWindow(m_hwnd);
            
            // Gestione speciale per Shift
            if (key.keyCode == VK_SHIFT) {
                // Disattiva Shift per tutti i tasti non-lettere
                for (auto& k : m_keys) {
                    if (k.keyCode >= 'A' && k.keyCode <= 'Z') {
                        // Mantieni lo stato Caps Lock se attivo
                        bool capsLockActive = false;
                        for (auto& capsKey : m_keys) {
                            if (capsKey.keyCode == VK_CAPITAL && capsKey.isToggled) {
                                capsLockActive = true;
                                break;
                            }
                        }
                        k.isShiftPressed = capsLockActive;
                    } else {
                        k.isShiftPressed = false;
                    }
                }
                InvalidateRect(m_hwnd, NULL, FALSE);
            }
            
            InvalidateRect(m_hwnd, &key.rect, TRUE);
            break;
        }
    }
}

void VirtualKeyboard::OnMouseMove(int xPos, int yPos) {
    SetCapture(m_hwnd);
    
    bool needsRedraw = false;
    for (auto& key : m_keys) {
        bool wasPressed = key.isPressed;
        key.isPressed = key.ContainsPoint(xPos, yPos) && (GetKeyState(VK_LBUTTON) < 0);
        
        if (wasPressed != key.isPressed) {
            needsRedraw = true;
        }
    }
    
    if (needsRedraw) {
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    
    ReleaseCapture();
}

bool VirtualKeyboard::IsMouseOverKey(const KeyButton& key) {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(m_hwnd, &cursorPos);
    return key.ContainsPoint(cursorPos.x, cursorPos.y);
}

void VirtualKeyboard::OnSize(int width, int height) {
    // Calcola il fattore di scala basato sulle dimensioni originali (690x300)
    float scaleX = (float)width / 690.0f;  // Aggiornato da 710 a 690
    float scaleY = (float)height / 300.0f; // Aggiornato da 350 a 300
    
    // Ridimensiona tutti i tasti proporzionalmente
    for (auto& key : m_keys) {
        // Salva le posizioni originali (primo ridimensionamento)
        static bool firstResize = true;
        static std::vector<RECT> originalRects;
        
        if (firstResize) {
            originalRects.clear();
            for (const auto& k : m_keys) {
                originalRects.push_back(k.rect);
            }
            firstResize = false;
        }
        
        // Applica la scala
        if (!originalRects.empty()) {
            int index = &key - &m_keys[0];
            key.rect.left = (int)(originalRects[index].left * scaleX);
            key.rect.top = (int)(originalRects[index].top * scaleY);
            key.rect.right = (int)(originalRects[index].right * scaleX);
            key.rect.bottom = (int)(originalRects[index].bottom * scaleY);
        }
    }
    
    // Ridisegna immediatamente usando double buffering
    InvalidateRect(m_hwnd, NULL, FALSE);
    
    // Resetta il timer di ridimensionamento per il cleanup finale
    static UINT_PTR resizeTimer = 0;
    if (resizeTimer) {
        KillTimer(m_hwnd, resizeTimer);
    }
    resizeTimer = SetTimer(m_hwnd, 2, 100, NULL);
}

bool VirtualKeyboard::LoadCustomFont() {
    // Space Mono con ottimizzazioni per migliore visualizzazione
    m_customFont = CreateFontW(
        18,                    // Altezza aumentata per migliore leggibilità
        0,                     // Larghezza (0 = proporzionale all'altezza)
        0,                     // Angolo di escape
        0,                     // Angolo di orientamento
        FW_NORMAL,             // Peso Regular
        FALSE,                 // Italic
        FALSE,                 // Underline
        FALSE,                 // Strikeout
        ANSI_CHARSET,          // Character set specifico
        OUT_TT_PRECIS,         // Output precision per TrueType
        CLIP_DEFAULT_PRECIS,      // Clipping precision
        CLEARTYPE_QUALITY,     // Qualità ClearType per rendering anti-aliasing
        DEFAULT_PITCH | FF_DONTCARE, // Pitch and family
        L"Space Mono"          // Nome del font
    );
    
    return m_customFont != nullptr;
}
