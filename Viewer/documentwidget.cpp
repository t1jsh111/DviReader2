#include "documentwidget.h"
#include <QDebug>
#include <QtGui>
#include "resolutionmanager.h"
#include <algorithm>



DocumentWidget::DocumentWidget(DviDocument document, QWidget *parent)
    : QLabel(parent), document(document), page(document.getPageByNumber(0)), scale(1)
{
    setAlignment(Qt::AlignCenter);
}

void DocumentWidget::setDocument(DviDocument document)
{
    this->document = document;
}

void DocumentWidget::setPage(int pageNr)
{
    int lastPageNr = document.numberOfPages() - 1;
    if(pageNr < 0) {
        pageNr = 0;
    }
    else if(pageNr > lastPageNr){
        pageNr = lastPageNr;
    }

    page = document.getPageByNumber(pageNr);
    qreal magnificationFactor = document.getMagnificationFactor();

    setPixmap(QPixmap::fromImage(page->getImage(scale, magnificationFactor)));

}

void DocumentWidget::nextPage()
{
    setPage(page->getPageNumber() + 1);
}

void DocumentWidget::previousPage()
{
    setPage(page->getPageNumber() - 1);
}

void DocumentWidget::zoomIn()
{
    scale += 0.2;
    setPage(page->getPageNumber());
}

void DocumentWidget::zoomOut()
{
    scale -= 0.2;
    setPage(page->getPageNumber());
}


void DocumentWidget::mousePressEvent(QMouseEvent *event)
{

    dragPosition = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(dragPosition, QSize()));
    qDebug() << "rubber band";
    rubberBand->show();
}

void DocumentWidget::mouseMoveEvent(QMouseEvent *event)
{

//    rubberBand->setGeometry(QRect(dragPosition, event->pos()).normalized());

//    if (!rubberBand->size().isEmpty()) {
//        // Correct for the margin around the image in the label.
//        QRectF rect = QRectF(rubberBand->pos(), rubberBand->size());
//        qDebug() << rect;

//        rect.moveLeft(rect.left() - (width() - this->pixmap(Qt::ReturnByValue).width()) / 2.0);
//        rect.moveTop(rect.top() - (height() - this->pixmap(Qt::ReturnByValue).height()) / 2.0);

//        //TeX margins are one inch
//        qDebug() << "rect after transformation" << rect;
//        qreal marginSize = ResolutionManager::ptToPx(72);
//        rect.moveLeft(rect.left() - marginSize * scale);
//        rect.moveTop(rect.top() - marginSize * scale);
//        qDebug() << "rect after margin transformation" << rect;

//        qDebug() << "rect after matrix transformation" << transformMatrix().inverted().mapRect(rect);
//        auto rectInRegularSystem = transformMatrix().inverted().mapRect(rect);
//        highlightText(rectInRegularSystem);

//    }

}

void DocumentWidget::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(dragPosition, event->pos()).normalized());

    if (!rubberBand->size().isEmpty()) {
        // Correct for the margin around the image in the label.
        QRectF rect = QRectF(rubberBand->pos(), rubberBand->size());
        qDebug() << rect;

        rect.moveLeft(rect.left() - (width() - this->pixmap(Qt::ReturnByValue).width()) / 2.0);
        rect.moveTop(rect.top() - (height() - this->pixmap(Qt::ReturnByValue).height()) / 2.0);

        //TeX margins are one inch
        qDebug() << "rect after transformation" << rect;
        qreal marginSize = ResolutionManager::tpToPx(72.27);
        rect.moveLeft(rect.left() - marginSize * scale);
        rect.moveTop(rect.top() - marginSize * scale);
        qDebug() << "rect after margin transformation" << rect;

        qDebug() << "rect after matrix transformation" << transformMatrix().inverted().mapRect(rect);
        auto rectInRegularSystem = transformMatrix().inverted().mapRect(rect);
        selectedText(rectInRegularSystem);

    }

    rubberBand->hide();
}

void DocumentWidget::selectedText(QRectF rect) {
    qDebug() << "selected text";

    // we should undo the magnification factor. We have been selected on the basis of the magnified elements.
    // the scaled points are howevers in their unmagnified form, and we should divide to do the query properly
    qreal magnificationFactor = document.getMagnificationFactor();


    int beginH = ResolutionManager::pxToSp(rect.bottomLeft().x()) /magnificationFactor;
    int endH = ResolutionManager::pxToSp(rect.bottomRight().x()) /magnificationFactor;

    int beginV = ResolutionManager::pxToSp(rect.topLeft().y())/magnificationFactor;
    int endV = ResolutionManager::pxToSp(rect.bottomLeft().y())/magnificationFactor;

    QString charactersInRange = page->textInRange(beginH, endH, beginV, endV);


    qDebug() << "Characters in range is " << charactersInRange;

}

void DocumentWidget::highlightText(QRectF rect)
{
//    QImage selectedImage = unselectedImage;
//    document.addSelectionToImage(selectedImage, currentPage, scale, rect);
//    setPixmap(QPixmap::fromImage(page-));
//    int beginH = ResolutionManager::pxToSp(rect.bottomLeft().x());
//    int endH = ResolutionManager::pxToSp(rect.bottomRight().x());

//    int beginV = ResolutionManager::pxToSp(rect.topLeft().y());
//    int endV = ResolutionManager::pxToSp(rect.bottomLeft().y());

//    QPainter painter(&currentImage);

//    painter.setPen(Qt::blue);
//    painter.setBrush(Qt::blue);



//    auto lines = document.linesOfCharactersInRange(beginH, endH, beginV, endV, currentPage);

//    for(auto line : lines) {
//        qDebug() << "line:";
//        auto max_height = std::max_element(line.begin(), line.end(), [](const Character& lhs, const Character& rhs) {
//            return (lhs.font->getFontHeightInPx() < rhs.font->getFontHeightInPx());});

//        int beginX = ResolutionManager::spToPx(line.at(0).h);
//        int beginY = ResolutionManager::spToPx(line.at(0).v);
//        //int height = max_height->font->getFontHeightInPx();
//        int height = 20; //px
//        QPointF beginPoint(beginX, beginY-height);

//        int endX = ResolutionManager::spToPx(line.last().h);
//        int endY = ResolutionManager::spToPx(line.last().v);



//        QPointF endPoint(endX, endY);

//        QRectF selection(beginPoint, endPoint);
//        painter.drawRect(selection);
//    }

//    setPixmap(QPixmap::fromImage(currentImage));
}

//void DocumentWidget::highlightText(QRectF rect)
//{
//    int beginH = ResolutionManager::pxToSp(rect.bottomLeft().x());
//    int endH = ResolutionManager::pxToSp(rect.bottomRight().x());

//    int beginV = ResolutionManager::pxToSp(rect.topLeft().y());
//    int endV = ResolutionManager::pxToSp(rect.bottomLeft().y());

//    auto lines = document.linesOfCharacters(beginH, endH, beginV, endV, currentPage);
//    QVector<int> highlightHeighPerLine{};
//    for(auto line : lines) {
//        std::max_element(line.begin(), line.end(), [](const DviFile::DVIPage::Character& lhs, const DviFile::DVIPage::Character& rhs){ lhs.;});
//    }

//}

QTransform DocumentWidget::transformMatrix()
{
    //QTransform scalingTransformation((physicalDpiX() / 72) * scale, 0, 0, (physicalDpiY() / 72) * scale, 0, 0);
//    QTransform scalingTransformation(scale * physicalDpiX() / 72.0, 0,
//                       0, scale * physicalDpiY() / 72.0,
//                       0, 0);
    qreal marginSize = ResolutionManager::tpToPx(72.27);
    QTransform scalingTransformation(scale, 0, 0, scale, 0, 0);
    return scalingTransformation;
}
