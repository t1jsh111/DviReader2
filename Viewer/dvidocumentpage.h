#ifndef DVIDOCUMENTPAGE_H
#define DVIDOCUMENTPAGE_H

#include <QImage>
#include <exception>
#include "character.h"
#include "dvipage.h"

class DviDocumentPage
{
    


public:
    DviDocumentPage(int pageNumber, DVIPage page);
    const QImage& getImage(qreal scalar, qreal magnificationFactor);

    int getPageNumber() const;

    QString textInRange(qreal beginH, qreal endH, qreal beginV, qreal endV);

private:
    QTransform imageCoordinateTransformation(qreal scalar, qreal magnificationFactor);
    void setLinesOfPage();

    void addCharactersToImage(QImage& documentImage, qreal scalar, qreal magnificationFactor);
    void addRectanglesToImage(QImage& documentImage, qreal scalar, qreal magnificationFactor);

    bool spaceBetweenCharacters(const Character& oldCharacter, const Character& newCharacter);
    QVector<QVector<Character>> linesInRangeVertically(qreal beginV, qreal endV);

    void removeFromLineBeforeH(int h, QVector<Character>& line);
    void removeFromLineAfterH(int h, QVector<Character>& line);

private:
    const int pageNumber;

    //For now we assume A4 Size
    //TODO support multiple file formats
    //Now default TeX width and height, but via special it can actually be different....
    const qreal pageWidthInch = 8.5;
    const qreal pageHeightInch = 10.9;

    QVector<QVector<Character>> pageLines;

    double scalarOfCachedImage = 0;
    QImage cachedImage;
    const DVIPage page;




};

#endif // DVIDOCUMENTPAGE_H

//class TextLine{
//  public:
//    int getV() const {return characters.first().v;};
//    int getBeginH() const {return characters.first().h;};
//    int getEndH() const {return characters.last().h;};

//    bool isEmpty() const{return characters.isEmpty();};
//    //Vector line should be non empty
//    TextLine(QVector<Character> characters) : characters(characters) {
//        if(characters.isEmpty()) {
//            throw std::invalid_argument("Vector is empty");
//        }
//    };
//    //make more efficient
//    void removeBeforeH(int beginH){
//        if(beginH > getEndH()) {
//            throw "Vector can not be entirely empty";
//        }
//        auto removalIterator = std::remove_if(characters.begin(), characters.end(), [beginH](const Character& character){ return character.h < beginH ;});
//        characters.erase(removalIterator, characters.end());
//    }
//    void removeAfterH(int endH) {
//        if(endH < getBeginH()) {
//            throw "Vector can not be entirely empty";
//        }
//        auto removalIterator = std::remove_if(characters.begin(), characters.end(), [endH](const Character& character){ return character.h > endH ;});
//        characters.erase(removalIterator, characters.end());
//    }

//    const Character& at(int index) const
//    {
//        return characters.at(index);
//    }

//    Character& operator[](int index){
//        return characters[index];
//    }

//    const Character& operator[](int index) const{
//        return characters[index];
//    }

//    const QVector<Character>& getCharacters() const {return characters;};

//private:
//    QVector<Character> characters;

//};

//class TextLines{
//public:
//    TextLines(){ lines = {}; };
//    void addLine(TextLine line){
//        //empty lines are ignored
//        if(line.isEmpty()) {
//            return;
//        }
//        lines.push_back(line);
//    }
//    void removeOutHorizontalRange(int beginH, int endH){
//        if(beginH > lines.first().getBeginH()) {
//            lines.erase(lines.begin());
//        } else {
//            lines.first().removeBeforeH(beginH);
//        }

//        if(endH < lines.last().getEndH()) {
//            lines.pop_back();
//        } else {
//            lines.last().removeAfterH(endH);
//        }


//    }



//    const QVector<TextLine>& getLines() const{
//        return lines;
//    }
//private:
//    QVector<TextLine> lines;
//};
