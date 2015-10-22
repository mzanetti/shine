#ifndef ICON_MAP
#define ICON_MAP

#include <QObject>
#include <QMap>
#include <QString>
#include <QDebug>

class IconMap : public QObject {
    Q_OBJECT

public:
    IconMap(){
        iconMapFilled["default"] = "entity_filled.svg";
        iconMapFilled["LCT001"] = "a19_filled.svg";
        iconMapFilled["LCT002"] = "br30_filled.svg";
        iconMapFilled["LCT003"] = "gu10_filled.svg";
        iconMapFilled["LST001"] = "lightstrip_filled.svg";
        iconMapFilled["LLC010"] = "lc_iris_filled.svg";
        iconMapFilled["LLC011"] = "lc_bloom_filled.svg";
        iconMapFilled["LLC012"] = "lc_bloom_filled.svg";
        iconMapFilled["LLC006"] = "lc_iris_filled.svg";
        iconMapFilled["LLC007"] = "lc_aura_filled.svg";
        iconMapFilled["LLC013"] = "storylight_filled.svg";
        iconMapFilled["LWB004"] = "a19_filled.svg";
        iconMapFilled["LLM001"] = "entity_filled.svg";
        iconMapFilled["LLM010"] = "entity_filled.svg";
        iconMapFilled["LLM011"] = "entity_filled.svg";
        iconMapFilled["LLM012"] = "entity_filled.svg";
        iconMapFilled["LLC020"] = "huego_filled.svg";

        iconMapOutline["default"] = "entity_outline.svg";
        iconMapOutline["LCT001"] = "a19_outline.svg";
        iconMapOutline["LCT002"] = "br30_outline.svg";
        iconMapOutline["LCT003"] = "gu10_outline.svg";
        iconMapOutline["LST001"] = "lightstrip_outline.svg";
        iconMapOutline["LLC010"] = "lc_iris_outline.svg";
        iconMapOutline["LLC011"] = "lc_bloom_outline.svg";
        iconMapOutline["LLC012"] = "lc_bloom_outline.svg";
        iconMapOutline["LLC006"] = "lc_iris_outline.svg";
        iconMapOutline["LLC007"] = "lc_aura_outline.svg";
        iconMapOutline["LLC013"] = "storylight_outline.svg";
        iconMapOutline["LWB004"] = "a19_outline.svg";
        iconMapOutline["LLM001"] = "entity_outline.svg";
        iconMapOutline["LLM010"] = "entity_outline.svg";
        iconMapOutline["LLM011"] = "entity_outline.svg";
        iconMapOutline["LLM012"] = "entity_outline.svg";
        iconMapOutline["LLC020"] = "huego_outline.svg";
    }

    static IconMap* instance(){
        if (!s_instance) {
            s_instance = new IconMap();
        }
        return s_instance;
    }

    Q_INVOKABLE QString getIcon(QString modelID, bool outline = false){
        QMap<QString, QString> *map;
        if (outline){
            map = &iconMapOutline;
        }else{
            map = &iconMapFilled;
        }

        QString icon = (*map)[modelID];
        if (icon == "") return (*map)["default"];
        return icon;
    }

private:
    QMap<QString, QString> iconMapFilled;
    QMap<QString, QString> iconMapOutline;
    static IconMap* s_instance;
};

#endif // ICON_MAP

