#include "pch.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "backend.h"
#include "mandelbruhview.h"

int main(int argc, char* argv[]) 
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<BackEnd>("mf.components", 1, 0, "BackEnd", &BackEnd::qmlInstance);
    qmlRegisterType<MandelbruhView>("mf.components", 1, 0 , "MandelbruhView");

    engine.addImportPath(":/imports");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

