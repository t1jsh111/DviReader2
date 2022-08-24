#ifndef DVIPAGE_H
#define DVIPAGE_H

#include "DviInterpreter_global.h"
#include "character.h"
#include "rectangle.h"
#include <vector>
#include <set>
#include <array>

class DVIINTERPRETER_EXPORT DVIPage{
private:
    struct CustomCompare {
        bool operator()(const Character& lhs, const Character& rhs) const{
            return (lhs.v < rhs.v) or (lhs.v == rhs.v and lhs.h < rhs.h);
        }
    };

    // The CustomCapre ensures that characters are placed in order
    std::multiset<Character, CustomCompare> characters;
    std::vector<Rectangle> rectangles;

    // Tex pages are identified by 10 integers (the \count0,...,\count10 registers)
    std::array<int, 10> pageIdentifier;
public:

    DVIPage();

    void addCharacter(Character& character);
    void addRectangle(Rectangle& rect);
    const std::array<int, 10>& getPageIdentifier();
    void setPageIdentifier(int index, int value);

    const std::multiset<Character, CustomCompare>& getCharacters() const;
    const std::vector<Rectangle>& getRectangles() const;


};

#endif // DVIPAGE_H
