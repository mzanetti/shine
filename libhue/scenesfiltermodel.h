#ifndef SCENESFILTERMODEL_H
#define SCENESFILTERMODEL_H

#include <QSortFilterProxyModel>

class Scene;
class Scenes;

class ScenesFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Scenes* scenes READ scenes WRITE setScenes NOTIFY scenesChanged)
    Q_PROPERTY(bool hideOtherApps READ hideOtherApps WRITE setHideOtherApps NOTIFY hideOtherAppsChanged)

public:
    ScenesFilterModel(QObject *parent = 0);

    Scenes* scenes() const;
    void setScenes(Scenes *scenes);

    bool hideOtherApps() const;
    void setHideOtherApps(bool hideOtherApps);

    Q_INVOKABLE Scene* get(int row) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void scenesChanged();
    void hideOtherAppsChanged();

private:
    Scenes *m_scenes;
    bool m_hideOtherApps;
};

#endif
