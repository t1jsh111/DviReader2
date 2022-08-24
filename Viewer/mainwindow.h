#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "documentwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_nextPage_clicked();

    void on_previousPage_clicked();

private:
    Ui::MainWindow *ui;
    DocumentWidget* documentWidget;
};
#endif // MAINWINDOW_H
