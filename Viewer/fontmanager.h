#ifndef FONTMANAGER_H
#define FONTMANAGER_H


#include <QRawFont>
#include <QMap>
#include <tuple>
#include <QString>
#include "dvifile.h"

class FontManager
{
public:
    static FontManager& getInstance();


private:
    FontManager() {};

    FontManager(FontManager const&) = delete;
    void operator=(FontManager const&) = delete;

    QMap<std::tuple<QString, qreal>, QRawFont> fontMap;

    QString getResourcesPath();


public:
    QRawFont& getRawFont(QString fontName, qreal pointSize);


};

#endif // FONTMANAGER_H
