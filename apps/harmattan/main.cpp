#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;

    // For easy running
    QStringList imports = view.engine()->importPathList();
    imports.append(QDir::currentPath() + "/../../plugin/");
    view.engine()->setImportPathList(imports);

    view.setSource(QUrl("qml/Shine.qml"));
    view.setResizeMode(QDeclarativeView::SizeViewToRootObject);
    view.show();


    return app.exec();
}
