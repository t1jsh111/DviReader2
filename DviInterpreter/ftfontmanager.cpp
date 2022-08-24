#include "ftfontmanager.h"
#include <QDir>
#include <QDebug>
#include <stdexcept>
#include "interpreterresolutionmanager.h"

//TODO font pointers never destructed....
//TODO find a non QT way to determine application directory.

/*
 * Return instance for singleton pattern
 */
FTFontManager &FTFontManager::getInstance()
{
    static FTFontManager instance;



    return instance;
}


/*
 * If font object already exists, return it. Otherwise a new font object is created and returned.
 * The font objects are never deleted, since this singleton will not be deleted.
 */
FT_Face& FTFontManager::getFont(std::string fontName, double pointSize)
{
    auto key = std::tuple<std::string, double>(fontName, pointSize);
    //Font found
    if(fontMap.find(key) != fontMap.end()) {
        return *fontMap[key];
    }
    else {
        std::string fontLocation = getResourcesPath() + "fonts/" + fontName + ".ttf";
        FT_Face* face = new FT_Face;
        FT_Init_FreeType(&library);

        auto error = FT_New_Face(library, fontLocation.c_str(), 0, face);
        if(error) {
            throw std::runtime_error("Something went wrong with loading font face");
        }
        error = FT_Set_Char_Size(*face, 0, pointSize*64,
                                 InterpreterResolutionManager::pixelsPerInch,
                                 InterpreterResolutionManager::pixelsPerInch);
        if(error) {
            throw std::runtime_error("Something went wrong with setting character size");
        }

        fontMap[key] = face;
        return *fontMap[key];
    }
}

//int FTFontManager::getScaledPointAdvanceWidth(std::string fontName, double pointSize, int characterCode)
//{
//    FT_Face fontFace;
//    try {
//        fontFace = getFont(fontName, pointSize);
//    }
//    catch (std::exception& ex) {
//        qDebug() << (ex.what());
//        return 0;
//    }

//    FT_UInt glyph_index = getGlyphNumber(fontName, pointSize, characterCode);
//    FT_Fixed advancement;

//    // Returns advancement as 1/65536 of a pixel (note that pixel info has been passed at face construction)
//    FT_Get_Advance(fontFace, glyph_index, 0, &advancement);

//    return InterpreterResolutionManager::ptToSp(InterpreterResolutionManager::pxToPt((double) advancement / (double) 65536.0));

//}

//int FTFontManager::getGlyphNumber(std::string fontName, double pointSize, int characterCode)
//{
//    FT_Face fontFace;
//    try {
//        fontFace = getFont(fontName, pointSize);
//    }
//    catch (std::exception& ex) {
//        qDebug() << (ex.what());
//        return 0;
//    }
//    return FT_Get_Char_Index(fontFace, characterCode);
//}




std::string FTFontManager::getResourcesPath()
{
#if defined(Q_OS_WIN)
    return (QDir::currentPath() + "/").toUtf8().toStdString();
#elif defined(Q_OS_OSX)
    return (QDir::currentPath() + "/../Resources/").toUtf8().toStdString();
#elif defined(Q_OS_LINUX)
    return QDir::currentPath() + "/../share/yourapplication/";
#else
    return (QDir::currentPath() + "/").toUtf8().toStdString();
#endif
}
