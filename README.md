# Tastiera Virtuale Italiana

Una tastiera virtuale completa in C++ con layout italiano standard QWERTY.

## Caratteristiche

- **Layout italiano completo**: Include tutti i caratteri speciali italiani (è, é, ò, à, ù, ì)
- **Tasti funzionali**: Shift, Ctrl, Alt, Win, Tab, Enter, Backspace, Delete
- **Interfaccia grafica**: Finestra con tasti cliccabili
- **Input diretto**: I tasti vengono inviati direttamente all'applicazione attiva
- **Design responsive**: Tasti che cambiano colore quando premuti

## Struttura del Progetto

```
CloudKeys/main/
├── src/
│   ├── main.cpp              # Entry point del programma
│   └── VirtualKeyboard.cpp   # Implementazione della tastiera
├── include/
│   ├── VirtualKeyboard.h     # Header della classe principale
│   └── KeyButton.h           # Struttura per i singoli tasti
├── compile_keyboard.bat      # Script di compilazione
└── README.md                 # Questo file
```

## Compilazione

### Prerequisiti
- MinGW-w64 (g++) installato
- Windows 10/11

### Compilazione ed Esecuzione
1. Esegui lo script di compilazione:
   ```cmd
   .\compile_keyboard.bat
   ```

Oppure compila manualmente:
```cmd
g++ -o keyboard.exe src/main.cpp src/VirtualKeyboard.cpp -Iinclude -lgdi32 -luser32
```

## Utilizzo

1. Avvia il programma `keyboard.exe`
2. Clicca sui tasti virtuali per digitare
3. I caratteri verranno inviati all'applicazione attualmente in uso
4. Usa i tasti Shift per accedere ai caratteri maiuscoli e simboli

## Layout Italiano

### Riga 1: Numeri e Simboli
`1 2 3 4 5 6 7 8 9 0 ' ì Backspace`
`! " £ $ % & / ( ) = ? ^`

### Riga 2: QWERTYUIOP
`Tab q w e r t y u i o p è + Enter`

### Riga 3: ASDFGHJKL
`Caps a s d f g h j k l ò à ù Del`

### Riga 4: ZXCVBNM
`Shift z x c v b n m , . - Shift`

### Riga 5: Controllo
`Ctrl Win Alt Space AltGr Menu Ctrl`

## Caratteri Speciali Italiani

- **è**: tasto è (shift: é)
- **ò**: tasto ò (shift: ç)
- **à**: tasto à (shift: °)
- **ù**: tasto ù (shift: §)
- **ì**: tasto ì (shift: ^)

## Note Tecniche

- Utilizza WinAPI per l'interfaccia grafica
- Invio input tramite `SendInput()` API
- Gestione eventi mouse per interazione
- Layout basato su standard italiano QWERTY

## Requisiti di Sistema

- Windows 10/11
- MinGW-w64 per la compilazione
- Librerie: gdi32, user32
