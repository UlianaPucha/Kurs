#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_applyButton_clicked();
    void on_saveButton_clicked();
    void on_resetButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage originalImage;
    QImage currentImage;

    int brightness;
    int contrast;
    int grayscale;
    int saturation;
    int temperature;
    int vignette;
    int sharpness;
    int sepia;


    void updateImageDisplay();
    void toggleUI(bool imageLoaded);
    void applyAllFilters();
    QImage FilterBrightness(const QImage &image, int parameter);
    QImage FilterContrast(const QImage &image, int parameter);
    QImage FilterGrayscale(const QImage &image, int parameter);
    QImage FilterSaturation(const QImage &image, int parameter);
    QImage FilterTemperature(const QImage &image, int parameter);
    QImage FilterVignette(const QImage &image, int parameter);
    QImage FilterSharpness(const QImage &image, int parameter);
    QImage FilterSepia(const QImage &image, int parameter);
};

#endif
