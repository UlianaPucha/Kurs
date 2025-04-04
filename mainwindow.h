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

private:
    Ui::MainWindow *ui;
    QImage originalImage;
    QImage currentImage;

    // Параметры фильтров
    int brightness;
    int contrast;
    int grayscale;
    int saturation;
    int temperature;
    int vignette;

    void updateImageDisplay();
    void toggleUI(bool imageLoaded);
    void applyAllFilters();
    QImage applyBrightness(const QImage &image, int value);
    QImage applyContrast(const QImage &image, int value);
    QImage applyGrayscale(const QImage &image, int value);
    QImage applySaturation(const QImage &image, int value);
    QImage applyTemperature(const QImage &image, int value);
    QImage applyVignette(const QImage &image, int value);
    QImage applySepia(const QImage &image);
    QImage applyInvert(const QImage &image);
};

#endif 
