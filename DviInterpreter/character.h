#ifndef CHARACTER_H
#define CHARACTER_H

#include "fontdefinition.h"
#include "DviInterpreter_global.h"
#include "memory"

class DVIINTERPRETER_EXPORT Character{

public:
    int charNumber;
    int h;
    int v;
    std::shared_ptr<FontDefinition> font = nullptr;
    Character(int charNumber, int h = 0, int v = 0, std::shared_ptr<FontDefinition> font = nullptr);
    Character();

    char getChar() const;

    int getEncodingValue() const;

    int getGlyphNumber() const;

    int getSpAdvancement() const;

};
#endif // CHARACTER_H
