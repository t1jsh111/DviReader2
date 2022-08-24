#include "dvidocument.h"

#include <QPainter>
#include <QImage>
#include <QGlyphRun>
#include <QRawFont>
#include "fontmanager.h"

#include <QDebug>
#include "dviinterpreter.h"
#include "resolutionmanager.h"
#include "ftfontmanager.h"
#include <algorithm>


DviDocument::DviDocument(DviFile file) : document(file) {
    
}

std::unique_ptr<DviDocumentPage>DviDocument::getPageByNumber(int pageNumber)
{
    if(pageNumber > numberOfPages()) {
        throw "Page out of bound";
    }
    return std::unique_ptr<DviDocumentPage>(new DviDocumentPage(pageNumber, document.dviPages.at(pageNumber)));
}



//QImage DviDocument::getPageByNumber(int pageNumber, double scalar)
//{

//    //For now we assume A4 Size
//    //TODO support multiple file formats
//    //Now default TeX width and height, but via special it can actually be different....
//    double documentWidth = 8.5 * ResolutionManager::pixelsPerInchX;
//    double documentHeight = 10.9 * ResolutionManager::pixelsPerInchY;


//    QImage documentImage(documentWidth * scalar, documentHeight * scalar, QImage::Format_RGB32);
//    documentImage.fill(Qt::white);
//    QPainter painter(&documentImage);
//    //painter.setTransform(imageCoordinateTransformation(scalar));

//    painter.setPen(Qt::black);
//    //painter.setRenderHint(QPainter::TextAntialiasing, true);
//    //painter.setRenderHint(QPainter::Antialiasing, true);

//    FontManager& fontManager = FontManager::getInstance();

//    DVIPage page = document.dviPages.at(pageNumber);

//    for(auto character : page.getCharacters()) {
////        qreal x = ResolutionManager::spToPx(character.h);
////        qreal y = ResolutionManager::spToPx(character.v);

//        qreal x = character.h;
//        qreal y = character.v;



//        qreal pointSize = ResolutionManager::spToPt(character.font->scaleFactor) * scalar;
//        QString fontName = QString::fromStdString(character.font->fontName);
//        QRawFont& font = fontManager.getRawFont(fontName, pointSize);
//        QGlyphRun letterRun;
//        letterRun.setRawFont(font);

//        QPointF point = imageCoordinateTransformation(scalar).map(QPointF(x,y));

//        letterRun.setGlyphIndexes({(quint32) character.getGlyphNumber()});
//        letterRun.setPositions({point});

//        //We add an ofset of one inch: i.e. 72 point in order for the margins to be appropriate....
//        painter.drawGlyphRun(QPoint(0,0), letterRun);

//    }


//        painter.setBrush(Qt::black);
//        for(auto rectangle : page.getRectangles()) {
//            qreal x = rectangle.h;
//            qreal y = rectangle.v;

//            qreal width = rectangle.width;
//            qreal height = rectangle.height;

//            QPointF transformedPoint = imageCoordinateTransformation(scalar).map(QPointF(x,y-height));

//            qDebug() << "rectangle" << transformedPoint << " width " << width << " height " << height;

//            width = ResolutionManager::spToPx(rectangle.width) * scalar;
//            height = ResolutionManager::spToPx(rectangle.height) * scalar;

//            painter.drawRect(transformedPoint.x(), transformedPoint.y(), width, height);
//        }
//    return documentImage;

//}




int DviDocument::numberOfPages()
{
    return document.dviPages.size();
}

qreal DviDocument::getMagnificationFactor()
{
    return (double) document.dviFilePre.mag / 1000.0;
}

//QVector<QVector<Character>> DviDocument::linesOfCharactersInRange(qreal beginH, qreal endH, qreal beginV, qreal endV, int pageNumber)
//{
//    QVector<QVector<Character>> lines{};
//    if(pageNumber < 0 or pageNumber > (int) document.dviPages.size() - 1) {
//        return lines;
//    }
//    DVIPage page = document.dviPages.at(pageNumber);
//    const Character* oldCharacter = nullptr;

//    const auto& characters = page.getCharacters();

//    Character lowerChar(0, beginH, beginV);

//    Character upperChar(0, endH, endV);

//    auto upper_it = page.getCharacters().upper_bound(upperChar);



//    QVector<Character> line{};
//    for(auto lower_it = page.getCharacters().lower_bound(lowerChar); lower_it != upper_it; lower_it++ ) {
//        const auto& character = *lower_it;
//        qDebug() << "char is" << character.getChar() << " h: " << character.h << " v: " << character.v;

//        if(oldCharacter != nullptr) {

//            bool charactersOnSameLine = character.v == oldCharacter->v;
//            //Consecutive characters on different lines should have a space between them if the previous character was not the hyphen (break line)
//            if(!charactersOnSameLine) {
//                lines.append(line);
//                line = {};
//            }
//        }
//        line.append(character);
//            oldCharacter = &character;


//    }
//    lines.append(line);

//    auto& firstLine = lines.first();
//    auto firstLineRemovalIterator = std::remove_if(firstLine.begin(), firstLine.end(), [beginH](const Character& character){ return character.h < beginH ;});
//    firstLine.erase(firstLineRemovalIterator, firstLine.end());

//    auto& lastLine = lines.last();
//    auto lastLineRemovalIterator = std::remove_if(lastLine.begin(), lastLine.end(), [endH](const Character& character){ return character.h > endH ;});
//    lastLine.erase(lastLineRemovalIterator, lastLine.end());



//    return lines;
//}

//QString DviDocument::textInRange(qreal beginH, qreal endH, qreal beginV, qreal endV, int pageNumber) {
//    QVector<QVector<Character> > lines = linesOfCharactersInRange(beginH, endH, beginV, endV, pageNumber);

//    QString charactersInRange = "";
//    const Character* oldCharacter = nullptr;
//    for (const auto& line : lines) {
//        for (const auto& character : line) {
//            //TeX  sometimes moves glyphs a bit more to the right (e.g. because of kerning), hence for space checking we should add some additional width.
//            //Idea is that for a white space there should be at least an 'i' character as white space in order for it to be considered as a white space...
//            if(oldCharacter != nullptr) {
//                Character iChar('i', 0, 0, character.font);
//                int advancementOfICharacter = iChar.getSpAdvancement();
//                bool containsVerticalWhiteSpace = character.h > ((oldCharacter->h + oldCharacter->getSpAdvancement()) + advancementOfICharacter);
//                bool charactersOnSameLine = (character.v == oldCharacter->v);

//                if (charactersOnSameLine and containsVerticalWhiteSpace) {
//                    charactersInRange.append(" ");
//                }
//                //Consecutive characters on different lines should have a space between them if the previous character was not the hyphen (break line)
//                else if (!charactersOnSameLine) {
//                    if (oldCharacter->getChar() != '-') {
//                        charactersInRange.append(" ");
//                    }
//                }
//            }
//            charactersInRange.append(character.getChar());
//            oldCharacter = &character;
//        }
//    }
//    return charactersInRange;
//}

//QImage& DviDocument::addSelectionToImage(QImage& document, int pageNr, double scalar, QRectF rect) {
//        int beginH = ResolutionManager::pxToSp(rect.bottomLeft().x());
//        int endH = ResolutionManager::pxToSp(rect.bottomRight().x());

//        int beginV = ResolutionManager::pxToSp(rect.topLeft().y());
//        int endV = ResolutionManager::pxToSp(rect.bottomLeft().y());

//        QPainter painter(&document);

//        painter.setPen(Qt::blue);
//        painter.setBrush(Qt::blue);



//        auto lines = linesOfCharactersInRange(beginH, endH, beginV, endV, pageNr);

//        for(auto line : lines) {
//            qDebug() << "line:";
//            auto max_height = std::max_element(line.begin(), line.end(), [](const Character& lhs, const Character& rhs) {
//                return (lhs.font->getFontHeightInPx() < rhs.font->getFontHeightInPx());});
//            if( not line.isEmpty()) {
//                int beginX = line.at(0).h;
//                int beginY = line.at(0).v;
//                //int height = max_height->font->getFontHeightInPx();
//                int height = 7*65536; //px
//                QPointF beginPoint(beginX, beginY-height);

//                int endX = line.last().h;
//                int endY = line.last().v;
//                 QPointF endPoint(endX, endY);


//                beginPoint = imageCoordinateTransformation(scalar).map(beginPoint);
//                endPoint = imageCoordinateTransformation(scalar).map(endPoint);




//                QRectF selection(beginPoint, endPoint);
//                painter.drawRect(selection);
//            }

//        }

//        return document;
//}


QTransform DviDocument::imageCoordinateTransformation(double scalar)
{
    double margin = ResolutionManager::spToPx(72.27) * scalar;

    double pixelsPerPoint = (double) ResolutionManager::pixelsPerInch  / (double) ResolutionManager::tpPerInch;
    double pixelsPerScaledPoint = (double) pixelsPerPoint / 65536.0;

    QTransform scalingTransformation(scalar * pixelsPerScaledPoint, 0, 0, scalar * pixelsPerScaledPoint, margin, margin);
    return scalingTransformation;
}

//bool DviDocument::characterInHorizontalRange(const Character& character, qreal beginH, qreal endH) const
//{
//    return (character.h >= beginH and character.h <= endH);
//}

//bool DviDocument::characterInVerticalRange(const Character& character, qreal beginV, qreal endV) const
//{
//    return (character.v >= beginV and character.v <= endV);
//}



