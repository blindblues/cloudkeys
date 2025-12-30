@echo off
echo Compilazione di CloudKeys - Tastiera Virtuale...
echo.

echo Impostazione PATH per MinGW...
set PATH=%PATH%;C:\mingw\mingw64\bin

echo Compilazione con g++...
g++ -o CloudKeys.exe src/main.cpp src/VirtualKeyboard.cpp src/ThemeManager.cpp -Iinclude -lgdi32 -luser32 -municode -static -mwindows

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo COMPILAZIONE COMPLETATA CON SUCCESSO!
    echo ========================================
    echo.
    echo Avvio di CloudKeys...
    echo.
    start CloudKeys.exe
) else (
    echo.
    echo ========================================
    echo ERRORE DURANTE LA COMPILAZIONE!
    echo ========================================
    echo.
    echo Controlla che tutti i file siano presenti:
    echo - src\main.cpp
    echo - src\VirtualKeyboard.cpp
    echo - include\VirtualKeyboard.h
    echo - include\KeyButton.h
)

pause
