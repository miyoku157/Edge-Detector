#include "mainwindow.h"
#include <QApplication>
#include "matrice.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGenericMatrix>
#include <QPushButton>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow Main_window;
     Main_window.show();

    return a.exec();
}
