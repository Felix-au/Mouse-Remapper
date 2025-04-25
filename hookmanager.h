// HOOKMANAGER_H
#ifndef HOOKMANAGER_H
#define HOOKMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QThread>
#include <windows.h>

struct RemapConfig {
    QList<int> virtualKeys;
    bool isRemapped;
    bool repeatEnabled;
    bool repeatUntilClick;
};

class HookManager : public QObject {
    Q_OBJECT

public:
    explicit HookManager(QObject *parent = nullptr);

    Q_INVOKABLE void startHook();
    Q_INVOKABLE void stopHook();
    Q_INVOKABLE void setRemap(int button, QList<int> keys, bool remap, bool repeat, bool untilClick);
    QMap<int, RemapConfig> getInternalConfig(); // C++ use
    Q_INVOKABLE QVariantMap getConfig();

private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static void simulateKey(int button);

    static QMap<int, RemapConfig> config;
    static std::atomic<bool> repeatActive[5];
    static HHOOK mouseHook;
};

#endif // HOOKMANAGER_H