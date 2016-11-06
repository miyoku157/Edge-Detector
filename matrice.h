#ifndef MATRICE_H
#define MATRICE_H
#include "qvector.h"
#include "qimage.h"
#include "math.h"
#include <QGenericMatrix>

class Matrice
{

public:
    struct QPairSecondComparer
    {
        template<typename T1, typename T2, typename T3>
        bool operator()(const QPair<QPair<T1, T2>, T3> & a, const QPair<QPair<T1, T2>, T3> & b) const
        {
            return a.second > b.second;
        }
    };

    double** gradDir;
    double** norme;
    int seuil_type = 0;
    int nb_seuil = 200;
    int seuil_bas = 40;
    int seuil_haut = 60;
    bool done = true;
    bool chain= false;
    QGenericMatrix<3,3,double> filtre;
    QImage Base;
    QImage image;

    Matrice();

    void LoadImage(const char* Filename);
    double convolution(QGenericMatrix<3,3,double>  kernel, int i, int j);
    void Bidirectionel(QGenericMatrix<3,3,double> kernel);
    void Multidirectionel(QGenericMatrix<3,3,double> kernel);
    void cyclic_roll(double &a, double &b, double &c, double &d);
    void Hysteresis(QImage* enter);
    void seuil_max(QImage* enter);
    void affinage();
    void Chainage();
    int CountV8(int, int);
    int CountV4(int, int);
    void Hough();
    ~Matrice();

    inline double deg2rad (double degrees) { static const double pi_on_180 = 4.0 * atan (1.0) / 180.0; return degrees * pi_on_180; }
};

#endif // MATRICE_H
