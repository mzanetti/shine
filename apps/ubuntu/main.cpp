#include <QtGui/QGuiApplication>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setSource(QStringLiteral("qml/Shine.qml"));
    view.show();

    return app.exec();
}
