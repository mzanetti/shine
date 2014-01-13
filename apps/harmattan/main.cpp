#include "keystore.h"

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;

    QStringList imports = view.engine()->importPathList();
    imports.append("/opt/shine/plugins/");
    view.engine()->setImportPathList(imports);
    view.engine()->rootContext()->setContextProperty("keystore", new KeyStore());

    view.setSource(QUrl("/opt/shine/qml/main.qml"));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.showFullScreen();


    return app.exec();
}
