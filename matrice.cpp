#include "matrice.h"
#include "qimage.h"

Matrice::Matrice()
{
    double values[] =
    {
        -3,-3,5,
        -3,0,5,
        -3,-3,5
    };

    this->filtre = QGenericMatrix<3, 3, double>(values);
}

void Matrice::LoadImage(const char* Filename)
{
    this->Base = QImage(Filename);
    this->image = QImage(this->Base);
    norme = new double*[image.width()];
    gradDir = new double*[image.width()];

    for (int i = 0; i < image.width(); i++)
    {
        norme[i] = new double[image.height()];
        gradDir[i] = new double[image.height()];

    }
}
void Matrice::Hough() {
    int width = image.width();
    int height = image.height();
    int delt_Thet = 1;
    double hough_h = ((sqrt(2.0)*(double)(height > width ? height : width)) / 2.0);
    int accu_h = hough_h*2.0 + 1;
    int accu_w = 180;
    unsigned int* accu = new unsigned int[180 * accu_h];
    double r;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image.pixelColor(x, y).value() > 230) {
                for (int theta = 0; theta < 180; theta += delt_Thet) {
                    r = ((double)x*cos((double)theta*deg2rad(theta))) + ((double)y*sin((double)theta*deg2rad(theta)));
                    accu[theta + ((int)round(r + hough_h))*accu_w] = (accu[theta + ((int)round(r + hough_h))*accu_w]) + 1;
                }
            }
        }
    }
    QImage testthe = QImage(accu_w, accu_h, QImage::Format_ARGB32);


    
   for (int y = 0; y < accu_h; y++)
        {
			for (int x = 0; x < accu_w; x++)
    {
            testthe.setPixelColor(x, y, accu[x*accu_w + y]);
        }
    }

    testthe.save("theta.png");


    delete[] accu;

}

void Matrice::seuil_max(QImage* enter)
{
    QList<QPair<QPair<int, int>, double>> tri = QList<QPair<QPair<int, int>, double>>();
    int width = enter->width();
    int height = enter->height();
    QImage* ret = new QImage(enter->width(), enter->height(), enter->format());

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            tri.push_back(QPair<QPair<int, int>, double>(QPair<int, int>(i, j), norme[i][j]));
        }
    }

    Matrice::QPairSecondComparer compair = Matrice::QPairSecondComparer();
    qSort(tri.begin(), tri.end(), compair);

    if (this->nb_seuil >= tri.size())
    {
        this->nb_seuil = tri.size();
    }

    for (int i = this->nb_seuil; i < tri.size(); i++)
    {
        norme[tri[i].first.first][tri[i].first.second] = 0;
        ret->setPixelColor(tri[i].first.first, tri[i].first.second, qRgb(0, 0, 0));
    }

}

void Matrice::Hysteresis(QImage* enter)
{
    int seuil_B = (seuil_bas * 255) / 100;
    int seuil_H = (seuil_haut * 255) / 100;
    unsigned char** s_array = new unsigned char*[enter->width()];

    for (int i = 0; i < enter->width(); i++)
    {
        s_array[i] = new unsigned char[enter->height()];
    }

    QImage* ret = new QImage(enter->width(), enter->height(), enter->format());

    for (int i = 0; i < enter->width(); i++)
    {
        for (int j = 0; j < enter->height(); j++)
        {
            if (enter->pixelColor(i, j).value() > seuil_H)
            {
                s_array[i][j] = 'h';
            }
            else if (enter->pixelColor(i, j).value() < seuil_H&&enter->pixelColor(i, j).value() > seuil_B)
            {
                s_array[i][j] = 'b';
            }
        }
    }

    for (int i = 1; i < enter->width() - 1; i++)
    {
        for (int j = 1; j < enter->height() - 1; j++)
        {
            if (s_array[i][j] == 'h')
            {
                ret->setPixel(i, j, enter->pixel(i, j));
            }
            else if (s_array[i][j] == 'b')
            {
                for (int n = -1; n < 2; n++)
                {
                    for (int m = -1; m < 2; m++)
                    {
                        if (s_array[n + i][m + j] == 'h')
                        {
                            ret->setPixel(i, j, enter->pixel(i + n, j + m));
                            s_array[i][j] = 'h';
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < enter->width(); i++)
    {
        if (s_array[i][0] == 'h')
        {
            ret->setPixel(i, 0, enter->pixel(i, 0));
        }
        else if (s_array[i][0] == 'b')
        {
            for (int n = -1; n < 2; n++)
            {
                for (int m = 0; m < 2; m++)
                {
                    if ((i + n > -1) && (i + n < enter->width()))
                    {
                        if (s_array[n + i][m] == 'h')
                        {
                            ret->setPixel(i, 0, enter->pixel(i + n, m));
                            s_array[i][0] = 'h';
                        }
                    }
                }
            }
        }

        if (s_array[i][enter->height() - 1] == 'h')
        {
            ret->setPixel(i, enter->height() - 1, enter->pixel(i, enter->height() - 1));
        }
        else if (s_array[i][enter->height() - 1] == 'b')
        {
            for (int n = -1; n < 2; n++)
            {
                for (int m = 0; m > -2; m--)
                {
                    if (i + n > -1 && i + n < enter->width())
                    {
                        if (s_array[n + i][enter->height() - 1 + m] == 'h')
                        {
                            ret->setPixel(i, enter->height() - 1, enter->pixel(n + i, enter->height() - 1 + m));
                            s_array[i][enter->height() - 1] = 'h';

                        }
                    }
                }
            }
        }
    }

    for (int j = 0; j < enter->height(); j++)
    {
        if (s_array[0][j] == 'h')
        {
            ret->setPixel(0, j, enter->pixel(0, j));
        }
        else if (s_array[0][j] == 'b')
        {
            for (int m = -1; m < 2; m++)
            {
                for (int n = 0; n < 2; n++)
                {
                    if (j + m > -1 && j + m < enter->height())
                    {
                        if (s_array[n][m + j] == 'h')
                        {
                            ret->setPixel(0, j, enter->pixel(n, m + j));
                            s_array[0][j] = 'h';
                        }
                    }
                }
            }

        }

        if (s_array[enter->width() - 1][j] == 'h')
        {
            ret->setPixel(enter->width() - 1, j, enter->pixel(enter->width() - 1, j));
        }
        else if (s_array[enter->width() - 1][j] == 'b')
        {
            for (int m = -1; m < 2; m++)
            {
                for (int n = 0; n > -2; n--)
                {
                    if (j + m > -1 && j + m < enter->height())
                    {

                        if (s_array[enter->width() - 1 + n][j + m] == 'h')
                        {
                            ret->setPixel(enter->width() - 1, j, enter->pixel(enter->width() - 1 + n, j + m));
                            s_array[enter->width() - 1][j] = 'h';
                        }
                    }
                }
            }

        }
    }

    this->image = *ret;

    for (int i = 0; i < image.width(); i++)
    {
        delete[] s_array[i];
    }

    delete[] s_array;
    delete ret;
}

void Matrice::Multidirectionel(QGenericMatrix<3, 3, double> kernel)
{
    const int tailleMat = 3;
    this->image = this->Base.convertToFormat(QImage::Format_Grayscale8);
    gradDir = new double*[image.width()];

    for (int i = 0; i < image.width(); i++)
    {
        gradDir[i] = new double[image.height()];
    }

    QImage result = QImage(this->image.width(), this->image.height(), QImage::Format_Grayscale8);
    QGenericMatrix<tailleMat, tailleMat, double> kernel_array[8];
    kernel_array[0] = kernel;

    for (int n = 1; n < 8; n++)
    {
        for (int i = 0; i < (tailleMat) / 2; i++) for (int j = 0; j < (tailleMat) / 2; j++)
        {
            kernel_array[n] = QGenericMatrix<tailleMat, tailleMat, double>(kernel_array[n - 1]);
            cyclic_roll(kernel_array[n](i, j), kernel_array[n](tailleMat - 1 - j, i), kernel_array[n](tailleMat - 1 - i, tailleMat - 1 - j), kernel_array[n](j, tailleMat - 1 - i));
        }
    }

    double multidir[8];

    for (int i = 0; i < this->image.width(); i++)              // rows
    {
        for (int j = 0; j < this->image.height(); j++)          // columns
        {
            for (int m = 0; m < 8; m++)
            {
                multidir[m] = convolution(kernel_array[m], i, j);
            }
            for (int o = 0; o < 8; o++)
            {
                for (int u = o; u < 8; u++)
                {
                    double temp = sqrt(pow(multidir[o], 2) + pow(multidir[u], 2));

                    if (temp > norme[i][j])
                    {
                        norme[i][j] = temp;
                        gradDir[i][j] = atan2(multidir[o], multidir[u]);
                    }
                }
            }
            result.setPixel(i, j, norme[i][j]);


        }
    }
    this->image = result;
}

void Matrice::Bidirectionel(QGenericMatrix<3, 3, double> kernel)
{
    const int tailleMat = 3;
    this->image = this->Base.convertToFormat(QImage::Format_Grayscale8);

    QImage* result = new QImage(this->image.width(), this->image.height(), QImage::Format_Grayscale8);
    QGenericMatrix<tailleMat, tailleMat, double> kernel2 = QGenericMatrix<tailleMat, tailleMat, double>(kernel);

    for (int i = 0; i < (tailleMat) / 2; i++) for (int j = 0; j < (tailleMat) / 2; j++)
    {
        cyclic_roll(kernel2(i, j), kernel2(tailleMat - 1 - j, i), kernel2(tailleMat - 1 - i, tailleMat - 1 - j), kernel2(j, tailleMat - 1 - i));
    }

    for (int i = 1; i < this->image.width() - 1; i++)              // rows
    {
        for (int j = 1; j < this->image.height() - 1; j++)          // columns
        {

            double temp = convolution(kernel, i, j);
            double temp1 = convolution(kernel2, i, j);
            norme[i][j] = sqrt(pow(temp, 2) + pow(temp1, 2));
            result->setPixel(i, j, norme[i][j]);

            gradDir[i][j] = atan2(temp, temp1);

        }
    }

    this->image = *result;
    delete result;
}

void Matrice::cyclic_roll(double &a, double &b, double &c, double &d)
{
    int temp = a; a = b; b = c; c = d; d = temp;
}

void Matrice::affinage()
{
    int x = 0;
    int y = 0;

    for (int i = 1; i < image.width() - 1; i++)
    {
        for (int j = 1; j < image.height() - 1; j++)
        {
            if (gradDir[i][j] < -3 * M_PI / 4)
            {
                x = 1;
                y = 0;
            }
            else if (gradDir[i][j] >= -3 * M_PI / 4 && gradDir[i][j] < -2 * M_PI / 4)
            {
                x = 1;
                y = -1;
            }
            else if (gradDir[i][j] >= -2 * M_PI / 4 && gradDir[i][j] < -M_PI / 4)
            {
                x = 0;
                y = -1;
            }
            else if (gradDir[i][j] >= -M_PI / 4 && gradDir[i][j] < 0)
            {
                x = -1;
                y = -1;
            }
            else if (gradDir[i][j] >= 0 && gradDir[i][j] < M_PI / 4)
            {
                x = -1;
                y = 0;
            }
            else if (gradDir[i][j] >= M_PI / 4 && gradDir[i][j] < 2 * M_PI / 4)
            {
                x = -1;
                y = 1;
            }
            else if (gradDir[i][j] >= 2 * M_PI / 4 && gradDir[i][j] < 3 * M_PI / 4)
            {
                x = 0;
                y = 1;
            }
            else
            {
                x = 1;
                y = 1;
            }

            if (i + x >= 0 && i + x < image.width() && j + y >= 0 && j + y < image.height()
                && i - x >= 0 && i - x < image.width() && j - y >= 0 && j - y < image.height())
            {
                if (norme[i + x][j + y] >= norme[i][j] || norme[i - x][j - y] >= norme[i][j])
                {
                    image.setPixelColor(i, j, qRgb(0, 0, 0));
                }
            }
        }
    }

    for (int i = 0; i < image.width(); i++)
    {
        for (int j = 0; j < image.height(); j++)
        {
            if (image.pixelColor(i, j).value() > 0)
            {
                image.setPixelColor(i, j, qRgb(255, 255, 255));
            }
        }
    }
}

double Matrice::convolution(QGenericMatrix<3, 3, double> kernel, int i, int j)
{
    int  kCenterX = 1;
    int  kCenterY = 1;
    double out = 0;

    for (int m = 0; m < 3; m++)     // kernel rows
    {
        int mm = 3 - 1 - m;      // row index of flipped kernel

        for (int n = 0; n < 3; n++) // kernel columns
        {
            int nn = 3 - 1 - n;  // column index of flipped kernel

                                 // index of input signal, used for checking boundary
            int ii = i + (m - kCenterX);
            int  jj = j + (n - kCenterY);
            // ignore input samples which are out of bound
            if (ii >= 0 && ii < this->image.width() && jj >= 0 && jj < this->image.height())
            {
                out += this->image.pixel(ii, jj) * kernel(mm, nn);
            }
        }
    }
    return out;
}

Matrice::~Matrice()
{
    for (int i = 0; i < image.width(); i++)
    {
        delete[] gradDir[i];
        delete[] norme[i];
    }

    delete[] gradDir;
    delete[] norme;
}

void Matrice::Chainage()
{
    int testV8, testV4;

    for (int i = 0; i < image.width(); i++)
    {
        for (int j = 0; j < image.height(); j++)
        {
            if (image.pixelColor(i, j).value() > 0)
            {
                testV8 = CountV8(i, j);

                switch (testV8)
                {
                case 0:
                    image.setPixelColor(i, j, qRgb(0, 0, 0));
                    break;

                case 2:
                    testV4 = CountV4(i, j);

                    if (testV4 == 1)
                    {
                        image.setPixelColor(i, j, qRgb(255, 255, 255));
                    }
                    else
                    {
                        image.setPixelColor(i, j, qRgb(230, 230, 230));
                    }
                    break;

                case 1:
                    // Nothing
                    break;

                default:
                    image.setPixelColor(i, j, qRgb(230, 230, 230));
                    break;
                }
            }
        }
    }
}

Matrice::CountV8(int x, int y)
{
    int ret = CountV4(x, y);

    if (x > 0 && y > 0)
    {
        if (image.pixelColor(x - 1, y - 1).value() > 0)
        {
            ret++;
        }
    }

    if (x + 1 < image.width() && y > 0)
    {
        if (image.pixelColor(x + 1, y - 1).value() > 0)
        {
            ret++;
        }
    }

    if (y + 1 < image.height() && x > 0)
    {
        if (image.pixelColor(x - 1, y + 1).value() > 0)
        {
            ret++;
        }
    }

    if (y + 1 < image.height() && x + 1 < image.width())
    {
        if (image.pixelColor(x + 1, y + 1).value() > 0)
        {
            ret++;
        }
    }

    return ret;
}

Matrice::CountV4(int x, int y)
{
    int ret = 0;

    if (x > 0)
    {
        if (image.pixelColor(x - 1, y).value() > 0)
        {
            ret++;
        }
    }

    if (x + 1 < image.width())
    {
        if (image.pixelColor(x + 1, y).value() > 0)
        {
            ret++;
        }
    }

    if (y > 0)
    {
        if (image.pixelColor(x, y - 1).value() > 0)
        {
            ret++;
        }
    }

    if (y + 1 < image.height())
    {
        if (image.pixelColor(x, y + 1).value() > 0)
        {
            ret++;
        }
    }

    return ret;
}
