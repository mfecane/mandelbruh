#include "pch.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "backend.h"

#include "mandelbruhview2.h"

#ifdef MANDELBRUH1

#include "mandelbruh1/mandelbruhview.h"

int mandelbruh1(QGuiApplication &app, QQmlApplicationEngine &engine)
{
    qmlRegisterSingletonType<BackEnd>("mf.components", 1, 0, "BackEnd", &BackEnd::qmlInstance);
    qmlRegisterType<MandelbruhView>("mf.components", 1, 0 , "MandelbruhView");

    engine.addImportPath(":/imports");
    engine.addImageProvider("imgprovider", new ImgProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

#endif

#ifdef MANDELBRUH2

int mandelbruh2(QGuiApplication &app, QQmlApplicationEngine &engine)
{
    qmlRegisterType<MandelbruhView2>("mf.components", 1, 0, "MandelbruhView2");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}

#endif

int main(int argc, char* argv[]) 
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

#ifdef MANDELBRUH1
    return mandelbruh1(app, engine);
#endif

#ifdef MANDELBRUH2
    return mandelbruh2(app, engine);
#endif
}

