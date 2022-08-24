#include "dvidocumentpage.h"
#include "resolutionmanager.h"
#include <QPainter>
#include "dvipage.h"
#include "fontmanager.h"
#include <QGlyphRun>
#include <QPointF>




DviDocumentPage::DviDocumentPage(int pageNumber, DVIPage page) : pageNumber(pageNumber), page(page)
{
    setLinesOfPage();
}

QString DviDocumentPage::textInRange(qreal beginH, qreal endH, qreal beginV, qreal endV) {
    QString charactersInRange = "";
    QVector<QVector<Character>> linesInRange = linesInRangeVertically(beginV, endV);
    
    if(not linesInRange.isEmpty()) {
        auto& firstLine = linesInRange.first();
        removeFromLineBeforeH(beginH, firstLine);

        auto& lastLine = linesInRange.last();
        removeFromLineAfterH(endH, lastLine);
        
    }
    for(auto line : linesInRange) {
        for(auto character = line.begin(), oldCharacter = line.end(); character != line.end(); oldCharacter=character, ++character) {
                if(oldCharacter != line.end() && spaceBetweenCharacters(*oldCharacter, *character)) {
                    charactersInRange += " ";
                }
            charactersInRange += character->getChar();

        }
        charactersInRange += "\n";

    }
        

    return charactersInRange;
}

QVector<QVector<Character>> DviDocumentPage::linesInRangeVertically(qreal beginV, qreal endV) {
    Character lowerChar(0, 0, beginV);
    QVector<Character> lowerLine({lowerChar});
    auto firstLineIt = std::lower_bound(pageLines.begin(), pageLines.end(), lowerLine, [](const QVector<Character>& lhs, const QVector<Character>& rhs){
        return lhs.first().v < rhs.first().v;
    });

    Character upperChar(0, 0, endV);
    QVector<Character> upperLine({upperChar});
    auto lastLineIt = std::upper_bound(pageLines.begin(), pageLines.end(), upperLine, [](const QVector<Character>& lhs, const QVector<Character>& rhs){
        return lhs.first().v < rhs.first().v;
    });
    
    QVector<QVector<Character>> lines = {};
    //std::copy(firstLineIt, lastLineIt, lines.begin());
    for(firstLineIt; firstLineIt != lastLineIt; firstLineIt++) {
        lines.append(*firstLineIt);
    }
    return lines;


}

void DviDocumentPage::removeFromLineBeforeH(int h, QVector<Character> &line)
{
    auto removalIterator = std::remove_if(line.begin(), line.end(), [h](const Character& character){ return character.h < h ;});
    line.erase(removalIterator, line.end());
}

void DviDocumentPage::removeFromLineAfterH(int h, QVector<Character> &line)
{
    auto removalIterator = std::remove_if(line.begin(), line.end(), [h](const Character& character){ return character.h > h ;});
    line.erase(removalIterator, line.end());
}

void DviDocumentPage::setLinesOfPage(){
    const Character* oldCharacter = nullptr;
    const auto& characters = page.getCharacters();

    QVector<Character> characterLine{};
    for(auto lower_it = characters.begin(); lower_it != characters.end(); lower_it++) {
        const auto& character = *lower_it;

        if(oldCharacter != nullptr) {
            bool charactersOnSameLine = (character.v == oldCharacter->v);

            if(!charactersOnSameLine) {
                pageLines.append(characterLine);
                characterLine = {};
            }
        }
        characterLine.append(character);
        oldCharacter = &character;
    }
    if(not characterLine.isEmpty()) {
        pageLines.append(characterLine);
    }

}

void DviDocumentPage::addCharactersToImage(QImage& documentImage, qreal scalar, qreal magnificationFactor)
{
    QPainter painter(&documentImage);

    FontManager& fontManager = FontManager::getInstance();

    painter.setPen(Qt::black);
    const auto& characters = page.getCharacters();

    for (const auto& character : characters) {
        qreal x = character.h;
        qreal y = character.v;

        qreal pointSize = ResolutionManager::spToTp(character.font->scaleFactor) * scalar;
        QString fontName = QString::fromStdString(character.font->fontName);
        QRawFont& font = fontManager.getRawFont(fontName, magnificationFactor*pointSize);
        QGlyphRun letterRun;
        letterRun.setRawFont(font);

        QPointF point = imageCoordinateTransformation(scalar, magnificationFactor).map(QPointF(x, y));

        letterRun.setGlyphIndexes({ (quint32)character.getGlyphNumber() });
        letterRun.setPositions({ point });

        //We add an ofset of one inch: i.e. 72 point in order for the margins to be appropriate....
        painter.drawGlyphRun(QPoint(0, 0), letterRun);
    }
}

void DviDocumentPage::addRectanglesToImage(QImage& documentImage, qreal scalar, qreal magnificationFactor)
{
    QPainter painter(&documentImage);
    painter.setBrush(Qt::black);
    const auto& rectangles = page.getRectangles();

    for (const auto& rectangle : rectangles) {
        qreal x = rectangle.h;
        qreal y = rectangle.v;

        qreal width = rectangle.width;
        qreal height = rectangle.height;

        QPointF transformedPoint = imageCoordinateTransformation(scalar, magnificationFactor).map(QPointF(x, y - height));

        qDebug() << "rectangle" << transformedPoint << " width " << width << " height " << height;

        width = ResolutionManager::spToPx(rectangle.width) * scalar;
        height = ResolutionManager::spToPx(rectangle.height) * scalar;

        painter.drawRect(transformedPoint.x(), transformedPoint.y(), width, height);
    }
}


const QImage& DviDocumentPage::getImage(qreal scalar, qreal magnificationFactor)
{
    //If image has already been constructed into an image, there is no need to regenerate it.
    if (scalar > 0 and scalar == scalarOfCachedImage) {
        return cachedImage;
    }

    double documentWidth = pageWidthInch * ResolutionManager::pixelsPerInch;
    double documentHeight = pageHeightInch * ResolutionManager::pixelsPerInch;

    QImage documentImage(documentWidth * scalar, documentHeight * scalar, QImage::Format_RGB32);
    documentImage.fill(Qt::white);

    addCharactersToImage(documentImage, scalar, magnificationFactor);
    addRectanglesToImage(documentImage, scalar, magnificationFactor);

    cachedImage = documentImage;
    scalarOfCachedImage = scalar;

    return cachedImage;
}

int DviDocumentPage::getPageNumber() const
{
    return pageNumber;
}


QTransform DviDocumentPage::imageCoordinateTransformation(qreal scalar, qreal magnificationFactor)
{
    double margin = ResolutionManager::tpToPx(72.27) * scalar;

    double pixelsPerPoint = (double) ResolutionManager::pixelsPerInch  / (double) ResolutionManager::tpPerInch;
    double pixelsPerScaledPoint = (double) pixelsPerPoint / 65536.0;
    pixelsPerScaledPoint = magnificationFactor * pixelsPerScaledPoint;

    QTransform scalingTransformation(scalar * pixelsPerScaledPoint, 0, 0, scalar * pixelsPerScaledPoint, margin, margin);
    return scalingTransformation;
}

bool DviDocumentPage::spaceBetweenCharacters(const Character& oldCharacter, const Character& newCharacter) {
    //TeX  sometimes moves glyphs a bit more to the right (e.g. because of kerning), hence for space checking we should add some additional width.
    //Idea is that for a white space there should be at least an 'i' character as white space in order for it to be considered as a white space...
        Character iChar('i', 0, 0, newCharacter.font);
        int advancementOfICharacter = iChar.getSpAdvancement();
        bool containsVerticalWhiteSpace = newCharacter.h > ((oldCharacter.h + oldCharacter.getSpAdvancement()) + advancementOfICharacter);
        bool charactersOnSameLine = (newCharacter.v == oldCharacter.v);
        if(charactersOnSameLine and containsVerticalWhiteSpace) {
            return true;
        }
        //Consecutive characters on different lines should have a space between them if the previous character was not the hyphen (break line)
        else if (!charactersOnSameLine) {
            if (oldCharacter.getChar() != '-') {
                return true;
            }
        }
    return false;
}
