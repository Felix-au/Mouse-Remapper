// CONFIGMANAGER_H

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
class HookManager;

class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);
    void setHookManager(HookManager *hookManager);
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();

private:
    HookManager *hookManager = nullptr;
};

#endif // CONFIGMANAGER_H