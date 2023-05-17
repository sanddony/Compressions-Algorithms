// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>

#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"

#include "Controllers/EnsembleController.h"
#include "Controllers/MessageController.h"
#include "Controllers/FileController.h"
#include "Controllers/FilesInputController.h"
#include "Controllers/FilesListController.h"


int main(int argc, char *argv[])
{
    set_qt_environment();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:Main/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    EnsembleController::registerMe("EnsembleController");
    FilesInputController::registerMe("FilesInputController");
    FileController::registerMe("FileController");
    FilesListController::registerMe("FilesListController");
    MessageController::registerMe("MessageController");

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");
    engine.load(url);


    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
                                 QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal") };

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
