#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    // For easy running
    QStringList imports = view.engine()->importPathList();
    imports.append(QDir::currentPath() + "/../../plugin/");
    view.engine()->setImportPathList(imports);

    view.setSource(QStringLiteral("qml/Shine.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();


    return app.exec();
}
