// autostart.cpp
#include "autostart.h"
#include <QCoreApplication>
#include <windows.h>

void Autostart::enableAutostart() {
    HKEY hKey;
    QString appName = "MouseRemapper";
    QString appPath = QCoreApplication::applicationFilePath().replace("/", "\\");
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
    RegSetValueEx(hKey, (LPCWSTR)appName.utf16(), 0, REG_SZ, (BYTE*)appPath.utf16(), appPath.length() * sizeof(wchar_t));
    RegCloseKey(hKey);
}