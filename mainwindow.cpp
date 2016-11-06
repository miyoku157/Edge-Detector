#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrice.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QObject::connect(ui->action_1, SIGNAL(triggered()), this, SLOT(assignMatrix1()));
    mat = new Matrice();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::assignMatrix1()
{
    double values[] =
    {
        -1,0,1,
        -1,0,1,
        -1,0,1
    };

    mat->filtre = QGenericMatrix<3, 3, double>(values);
}
void MainWindow::assignMatrix2()
{
    double values[] =
    {
        -1,0,1,
        -2,0,2,
        -1,0,1
    };

    mat->filtre = QGenericMatrix<3, 3, double>(values);
}
void MainWindow::assignNb()
{
    mat->nb_seuil=ui->lin_seuil->text().toInt();
}

void MainWindow::assignMatrix3()
{
    double values[] =
    {
        -3,-3,5,
        -3,0,5,
        -3,-3,5
    };

    mat->filtre = QGenericMatrix<3, 3, double>(values);
}
void MainWindow::ischain(){
    if(!mat->chain){
        mat->chain=true;
    }else{
        mat->chain=false;
    }
}

void MainWindow::loadPictures()
{
    QString filename = QFileDialog::getOpenFileName(this);

    if (filename != "")
    {
        mat->LoadImage(filename.toStdString().c_str());
        QPixmap p = QPixmap::fromImage(mat->Base);
        ui->Image->setPixmap(p.scaled(ui->Image->width(), ui->Image->height(), Qt::KeepAspectRatio));
    }
}

void MainWindow::BiDirection()
{
    if (!mat->image.isNull() && !mat->filtre.isIdentity())
    {
        mat->Bidirectionel(mat->filtre);
       // mat->Multidirectionel(mat->filtre);

        if(mat->seuil_type == 0)
        {
            mat->Hysteresis(&mat->image);
        }
        else if(mat->seuil_type == 1)
        {
            mat->seuil_max(&mat->image);
        }

        mat->affinage();

        if(mat->chain){
            mat->Chainage();
        }
        //mat->Hough();
        mat->done = true;
        QPixmap p = QPixmap::fromImage(mat->image);
        ui->Retour->setPixmap(p.scaled(ui->Retour->width(), ui->Retour->height(), Qt::KeepAspectRatio));
    }
}

void MainWindow::Sauvegarder()
{
    QString filename = "";
    if (!mat->image.isNull() && mat->done)
    {
        filename = QFileDialog::getSaveFileName(this);
        mat->image.save(filename, "PNG");
    }

}
void MainWindow::assignHyst()
{
    mat->seuil_type = 0;
}
void MainWindow::assignMax()
{
    mat->seuil_type = 1;
}

void MainWindow::assignSeuil()
{

    mat->seuil_haut = ui->Text_sh->text().toInt();
    mat->seuil_bas = ui->Text_sb->text().toInt();
}
