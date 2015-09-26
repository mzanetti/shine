#include <QObject>

class Action: public QObject
{
    Q_OBJECT
public:
    Action(QObject *parent = 0);
};
