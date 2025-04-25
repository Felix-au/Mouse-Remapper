// main.cpp
#include <QGuiApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hookmanager.h"
#include "configmanager.h"
#include "autostart.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    HookManager hookManager;
    ConfigManager configManager;

    configManager.setHookManager(&hookManager);

    engine.rootContext()->setContextProperty("hookManager", &hookManager);
    engine.rootContext()->setContextProperty("configManager", &configManager);

    configManager.loadConfig();

    Autostart::enableAutostart();

    engine.load(QUrl(QStringLiteral("qrc:/resources/qml/Main.qml")));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}