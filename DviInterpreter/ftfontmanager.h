#ifndef FTFONTMANAGER_H
#define FTFONTMANAGER_H

#include <string>
#include <map>

#include "DviInterpreter_global.h"
#include <ft2build.h>
#include "dvifile.h"

#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H
#include FT_GLYPH_H
#include FT_ADVANCES_H



class DVIINTERPRETER_EXPORT FTFontManager
{
public:
    static FTFontManager& getInstance();
    FT_Face& getFont(std::string fontName, double pointSize);

    std::map<std::tuple<std::string, double>, FT_Face*> fontMap;

//    int getScaledPointAdvanceWidth(std::string fontName, double pointSize, int characterCode);
//    int getGlyphNumber(std::string fontName, double pointSize, int characterCode);


private:
    std::string getResourcesPath();
    FT_Library library;



};

#endif // FTFONTMANAGER_H
