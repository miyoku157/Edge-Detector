#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include "matrice.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    Matrice* mat;

private:
public slots:
    void assignNb();
    void assignMatrix1();
    void assignMatrix2();
    void assignMatrix3();
    void loadPictures();
    void BiDirection();
    void Sauvegarder();
    void assignMax();
    void assignHyst();
    void assignSeuil();
    void ischain();
};

#endif // MAINWINDOW_H
