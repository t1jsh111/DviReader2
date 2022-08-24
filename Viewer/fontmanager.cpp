#include "fontmanager.h"
#include "mainwindow.h"
#include <QScreen>
#include <QDebug>
#include <QDir>
#include "exception"
#include "resolutionmanager.h"

/*
 * Return instance for singleton pattern
 */
FontManager &FontManager::getInstance()
{

    static FontManager instance;

    return instance;
}


/*
 * If font object already exists, return it. Otherwise a new font object is created and returned.
 */
QRawFont& FontManager::getRawFont(QString fontName, qreal pointSize)
{
    auto key = std::tuple<QString, qreal>(fontName, pointSize);
    if(fontMap.contains(key)) {
        return fontMap[key];
    }
    else {
        QString fontLocation = getResourcesPath() + "fonts/" + fontName + ".ttf";
        auto pixelSize = ResolutionManager::tpToPx(pointSize);
        QRawFont font(fontLocation, pixelSize);
        //qDebug() << "Fontlocation" << fontLocation << " Font family " << font.familyName();
        fontMap[key] = font;
        return fontMap[key];
    }
}

QString FontManager::getResourcesPath()
{
#if defined(Q_OS_WIN)
    return (QDir::currentPath() + "/");
#elif defined(Q_OS_OSX)
    return (QDir::currentPath() + "/../Resources/");
#elif defined(Q_OS_LINUX)
    return QDir::currentPath() + "/../share/yourapplication/";
#else
    return (QDir::currentPath() + "/")
#endif
}
