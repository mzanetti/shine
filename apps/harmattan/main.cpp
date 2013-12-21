#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;

    QStringList imports = view.engine()->importPathList();
    imports.append("/opt/shine/plugins/");
    view.engine()->setImportPathList(imports);

    view.setSource(QUrl("/opt/shine/qml/Shine.qml"));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.showFullscreen();


    return app.exec();
}
