#include "fontdefinition.h"

#include "ftfontmanager.h"
#include "interpreterresolutionmanager.h"



FontDefinition::FontDefinition(int checkSum, int scaleFactor, int designSize, int mag, std::string directory, std::string fontName) :
    checksum(checkSum), scaleFactor(scaleFactor), designSize(designSize), directory(directory), fontName(fontName), mag(mag)
{

}

FT_Face& FontDefinition::getFontFace() const
{
    double pointSize = InterpreterResolutionManager::spToTp(this->scaleFactor);
    return FTFontManager::getInstance().getFont(this->fontName, pointSize);
}

int FontDefinition::getGlyphNumber(int encodingValue) const {
    FT_Face& fontFace = getFontFace();
    int charIndex = FT_Get_Char_Index(fontFace, encodingValue);
    return charIndex;
}

int FontDefinition::getFontHeightInPx() const
{
    return getFontFace()->size->metrics.height;
}

int FontDefinition::getSpAdvanceWidth(int encodingValue) const
{
    FT_Face& fontFace = getFontFace();

    FT_UInt glyph_index = getGlyphNumber(encodingValue);
    FT_Fixed advancement;

    // Returns advancement as 1/65536 of a pixel (note that pixel info has been passed at face construction)
    FT_Get_Advance(fontFace, glyph_index, 0, &advancement);
    double advancementInPixels = (double) advancement / (double) 65536.0;

    return InterpreterResolutionManager::tpToSp(InterpreterResolutionManager::pxToTp(advancementInPixels));
}



double FontDefinition::getPointSize() const
{
    return (scaleFactor / 65536.0);
}
