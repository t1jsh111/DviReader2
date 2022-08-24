#ifndef FONTDEFINITION_H
#define FONTDEFINITION_H


#include "DviInterpreter_global.h"

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H
#include FT_GLYPH_H
#include FT_ADVANCES_H



class DVIINTERPRETER_EXPORT FontDefinition {
public:
    int checksum;
    int scaleFactor;
    int designSize;
    int mag;
    std::string directory;
    std::string fontName;

public:
    double getPointSize() const;
    FontDefinition(int checkSum, int scaleFactor, int designSize, int mag, std::string directory, std::string fontName);
    FT_Face& getFontFace() const;
    int getSpAdvanceWidth(int encodingValue) const;
    int getGlyphNumber(int encodingValue) const;
    int getFontHeightInPx() const;
};

#endif // FONTDEFINITION_H
