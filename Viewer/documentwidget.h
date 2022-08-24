#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H


#include <QLabel>
#include <QImage>
#include "dvidocument.h"
#include "dvidocumentpage.h"
#include <QRubberBand>



class DocumentWidget : public QLabel
{
    Q_OBJECT

public:
    DocumentWidget(DviDocument document, QWidget *parent = 0);
    void setDocument(DviDocument document);
    void setPage(int pageNr);
    void nextPage();
    void previousPage();
    void zoomIn();
    void zoomOut();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    void selectedText(QRectF rect);
    void highlightText(QRectF rect);


private:
    DviDocument document;
    std::unique_ptr<DviDocumentPage> page;

    double scale;
    QPoint dragPosition;
    QRubberBand *rubberBand = nullptr;

    QTransform transformMatrix();
};

#endif // DOCUMENTWIDGET_H
