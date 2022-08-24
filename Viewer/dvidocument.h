#ifndef DVIDOCUMENT_H
#define DVIDOCUMENT_H

#include "dvifile.h"
#include <QImage>
#include "dvidocumentpage.h"
#include <memory>

class DviDocument
{
public:
//    DviDocument(DviFile file, qreal pixelsPerInchX, qreal pixelsPerInchY, qreal pointsPerInchX, qreal pointsPerInchY);
    DviDocument(DviFile file);

//    QImage getPageByNumber(int pageNumber, double scalar);
    std::unique_ptr<DviDocumentPage> getPageByNumber(int pageNumber);

    int numberOfPages();
    qreal getMagnificationFactor();

//    QVector<QVector<Character>> linesOfCharactersInRange(qreal beginH, qreal endH, qreal beginV, qreal endV, int pageNumber);
//    QString textInRange(qreal beginH, qreal endH, qreal beginV, qreal endV, int pageNumber);
//    QImage& addSelectionToImage(QImage& document, int pageNr, double scalar, QRectF rect);









private:
    DviFile document;
    QTransform imageCoordinateTransformation(double scalar);

//    bool characterInHorizontalRange(const Character& character, qreal beginH, qreal endH) const;
//    bool characterInVerticalRange(const Character& character, qreal beginV, qreal endV) const;




//    void cachePage(int pageNumber, int scalar);

//    int cachedPageNumber = -1;
//    int cachedPageScalar = -1;
//    QImage cachedPage;

//    qreal spToPt(double scaledPoints);
//    qreal ptToPx(double points);
//    qreal pxToPt(double pixels);

//    qreal pixelsPerInchX;
//    qreal pixelsPerInchY;
//    qreal pointsPerInchX;
//    qreal pointsPerInchY;

//    int scaledPointAdvancement(QRawFont font, char c);

};

#endif // DVIDOCUMENT_H
