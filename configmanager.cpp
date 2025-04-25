//configmanager.cpp

#include "configmanager.h"
#include "hookmanager.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {}

void ConfigManager::setHookManager(HookManager *hook) {
    this->hookManager = hook;
}

void ConfigManager::saveConfig() {
    if (!hookManager) {
        qWarning() << "saveConfig failed: hookManager is null";
        return;
    }

    QMap<int, RemapConfig> config = hookManager->getInternalConfig();
    QFile file(QDir::currentPath() + "/config.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        for (auto it = config.begin(); it != config.end(); ++it) {
            QJsonObject cfg;
            QJsonArray keys;
            for (int key : it.value().virtualKeys) keys.append(key);
            cfg["keys"] = keys;
            cfg["remap"] = it.value().isRemapped;
            cfg["repeat"] = it.value().repeatEnabled;
            cfg["untilClick"] = it.value().repeatUntilClick;
            obj[QString::number(it.key())] = cfg;
        }
        file.write(QJsonDocument(obj).toJson());
        file.close();
    } else {
        qWarning() << "Could not open config file for writing";
    }
}

void ConfigManager::loadConfig() {
    if (!hookManager) {
        qWarning() << "loadConfig failed: hookManager is null";
        return;
    }

    QFile file(QDir::currentPath() + "/config.json");
    if (file.open(QIODevice::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(file.readAll());
        auto obj = doc.object();
        for (auto key : obj.keys()) {
            auto cfg = obj[key].toObject();
            QJsonArray keys = cfg["keys"].toArray();
            QList<int> keyList;
            for (const auto &val : keys) keyList.append(val.toInt());
            hookManager->setRemap(
                key.toInt(),
                keyList,
                cfg["remap"].toBool(),
                cfg["repeat"].toBool(),
                cfg["untilClick"].toBool()
                );
        }
        file.close();
    } else {
        qWarning() << "Could not open config file for reading";
    }
}