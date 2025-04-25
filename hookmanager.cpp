// hookmanager.cpp
#include "hookmanager.h"
#include <thread>

QMap<int, RemapConfig> HookManager::config;
std::atomic<bool> HookManager::repeatActive[5] = {false};
HHOOK HookManager::mouseHook = nullptr;

HookManager::HookManager(QObject *parent) : QObject(parent) {}

void HookManager::startHook() {
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, nullptr, 0);
}

void HookManager::stopHook() {
    UnhookWindowsHookEx(mouseHook);
    mouseHook = nullptr;
    for (int i = 0; i < 5; ++i) repeatActive[i] = false;
}

void HookManager::setRemap(int button, QList<int> keys, bool remap, bool repeat, bool untilClick) {
    config[button] = {keys, remap, repeat, untilClick};
}

QMap<int, RemapConfig> HookManager::getInternalConfig() {
    return config;
}

QVariantMap HookManager::getConfig() {
    QVariantMap result;
    for (auto it = config.begin(); it != config.end(); ++it) {
        QVariantMap entry;
        QVariantList keyList;
        for (int key : it.value().virtualKeys)
            keyList.append(key);
        entry["keys"] = keyList;
        entry["remap"] = it.value().isRemapped;
        entry["repeat"] = it.value().repeatEnabled;
        entry["untilClick"] = it.value().repeatUntilClick;
        result[QString::number(it.key())] = entry;  // 🔁 use string keys
    }
    return result;
}


void HookManager::simulateKey(int button) {
    for (int key : config[button].virtualKeys) {
        keybd_event(key, 0, 0, 0);
        keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
    }
}

LRESULT CALLBACK HookManager::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT *mi = (MSLLHOOKSTRUCT *)lParam;
        int btn = 0;
        if (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP) btn = 1;
        if (wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP) btn = 2;
        if (wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP) btn = 3;
        if (wParam == WM_XBUTTONDOWN || wParam == WM_XBUTTONUP) {
            btn = (GET_XBUTTON_WPARAM(mi->mouseData) == XBUTTON1) ? 4 : 5;
        }

        if (btn > 0 && config[btn].isRemapped) {
            auto cfg = config[btn];

            if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN || wParam == WM_XBUTTONDOWN) {
                if (cfg.repeatEnabled) {
                    if (cfg.repeatUntilClick) {
                        if (repeatActive[btn - 1]) {
                            repeatActive[btn - 1] = false;
                        } else {
                            repeatActive[btn - 1] = true;
                            std::thread([btn]() {
                                while (repeatActive[btn - 1]) {
                                    simulateKey(btn);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                }
                            }).detach();
                        }
                    } else {
                        repeatActive[btn - 1] = true;
                        std::thread([btn]() {
                            while (repeatActive[btn - 1]) {
                                simulateKey(btn);
                                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            }
                        }).detach();
                    }
                } else {
                    simulateKey(btn);
                }
            }

            if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP || wParam == WM_MBUTTONUP || wParam == WM_XBUTTONUP) {
                if (cfg.repeatEnabled && !cfg.repeatUntilClick) {
                    repeatActive[btn - 1] = false;
                }
                return 1;
            }

            return 1;
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}