#include "dvipage.h"


DVIPage::DVIPage()
{

}

void DVIPage::addRectangle(Rectangle &rect)
{
    rectangles.push_back(rect);
}

void DVIPage::setPageIdentifier(int index, int value)
{
    pageIdentifier.at(index) = value;
}

const std::multiset<Character, DVIPage::CustomCompare>& DVIPage::getCharacters() const
{
    return characters;
}

const std::vector<Rectangle>& DVIPage::getRectangles() const
{
    return rectangles;
}

void DVIPage::addCharacter(Character& character)
{
    characters.insert(character);
}
