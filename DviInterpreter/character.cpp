#include "character.h"

Character::Character()
{

}

char Character::getChar() const
{
    return charNumber % 256;
}

int Character::getEncodingValue() const
{
    int encodingValue = charNumber;
    if(0 <= encodingValue and encodingValue <= 9) {
        encodingValue += 161;
    }

    if(10 <= encodingValue and encodingValue <= 32){
        encodingValue += 163;
    }
    if(encodingValue == 127) {
        encodingValue = 196;
    }
    return encodingValue;
}

int Character::getGlyphNumber() const
{
    return this->font->getGlyphNumber(getEncodingValue());
}

int Character::getSpAdvancement() const
{
    return this->font->getSpAdvanceWidth(getEncodingValue());
}






Character::Character(int charNumber, int h, int v, std::shared_ptr<FontDefinition> font) :
    charNumber(charNumber), h(h), v(v), font(font) {

}
