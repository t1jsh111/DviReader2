#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QScreen>
#include <QDebug>
#include "dviinterpreter.h"
#include "resolutionmanager.h"
#include "interpreterresolutionmanager.h"
#include "dvidocument.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto pixelsPerInch = qApp->primaryScreen()->physicalDotsPerInchX();
//    auto pixelsPerInchY = qApp->primaryScreen()->physicalDotsPerInchY();

    ResolutionManager::initialize(pixelsPerInch);
    InterpreterResolutionManager::initialize(pixelsPerInch);
    documentWidget = nullptr;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Dvi File", QDir::currentPath(), "Dvi (*.dvi)");

    auto pixelsPerInchX = qApp->primaryScreen()->physicalDotsPerInchX();
    auto pixelsPerInchY = qApp->primaryScreen()->physicalDotsPerInchY();
    auto pointsPerInchX = qApp->primaryScreen()->logicalDotsPerInchX();
    auto pointsPerInchY = qApp->primaryScreen()->logicalDotsPerInchY();


    DviInterpreter interpreter(filePath.toUtf8().toStdString(), pointsPerInchX, pointsPerInchY, pixelsPerInchX, pixelsPerInchY);
    DviFile file = interpreter.readDvi();
    //file.printDvi();
    DviDocument dviDocument(file);
    documentWidget = new DocumentWidget(file, this);
    documentWidget->setPage(0);

    ui->scrollArea->setWidget(documentWidget);

    qDebug() << "file num" << file.dviFilePre.num;
    qDebug() << "file den" << file.dviFilePre.den;
    qDebug() << "file magnification" << file.dviFilePre.mag;

}

void MainWindow::on_actionZoom_in_triggered()
{
    if(documentWidget == nullptr) {
        return;
    }
    else {
        documentWidget->zoomIn();
    }
}

void MainWindow::on_actionZoom_out_triggered()
{
    if(documentWidget == nullptr) {
        return;
    }
    else {
        documentWidget->zoomOut();
    }
}



void MainWindow::on_nextPage_clicked()
{
    documentWidget->nextPage();
}

void MainWindow::on_previousPage_clicked()
{
    documentWidget->previousPage();
}
