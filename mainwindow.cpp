#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      brightness(0),
      contrast(0),
      grayscale(0),
      saturation(100),
      temperature(0),
      vignette(0),
      sharpness(0),
      sepia(0)
{
    ui->setupUi(this);
    toggleUI(false);
    ui->slider->setRange(-100, 100);
    ui->slider->setValue(0);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleUI(bool imageLoaded)
{
    ui->loadButton->setVisible(!imageLoaded);
    ui->imageLabel->setVisible(imageLoaded);
    ui->comboBox->setVisible(imageLoaded);
    ui->applyButton->setVisible(imageLoaded);
    ui->saveButton->setVisible(imageLoaded);
    ui->resetButton->setVisible(imageLoaded);
    ui->slider->setVisible(imageLoaded);
    ui->valueLabel->setVisible(imageLoaded);
}

void MainWindow::on_loadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Images (*.png *.xpm *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        if (originalImage.load(filePath)) {
            currentImage = originalImage;
            brightness = 0;
            contrast = 0;
            grayscale = 0;
            saturation = 100;
            temperature = 0;
            vignette = 0;
            updateImageDisplay();
            toggleUI(true);
            ui->valueLabel->setText("0");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение.");
        }
    }
}

void MainWindow::updateImageDisplay()
{
    ui->imageLabel->setPixmap(QPixmap::fromImage(currentImage));
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QString filter = ui->comboBox->itemText(index);

    if (filter == "Яркость") {
        ui->slider->setRange(-100, 100);
        ui->slider->setValue(brightness);
        ui->valueLabel->setText(QString::number(brightness));
    }
    else if (filter == "Контрастность") {
        ui->slider->setRange(-100, 100);
        ui->slider->setValue(contrast);
        ui->valueLabel->setText(QString::number(contrast));
    }
    else if (filter == "Грейскейл") {
        ui->slider->setRange(0, 100);
        ui->slider->setValue(grayscale);
        ui->valueLabel->setText(QString::number(grayscale));
    }
    else if (filter == "Насыщенность") {
        ui->slider->setRange(0, 200);
        ui->slider->setValue(saturation);
        ui->valueLabel->setText(QString::number(saturation));
    }
    else if (filter == "Температура") {
        ui->slider->setRange(-100, 100);
        ui->slider->setValue(temperature);
        ui->valueLabel->setText(QString::number(temperature));
    }
    else if (filter == "Виньетка") {
        ui->slider->setRange(0, 100);
        ui->slider->setValue(vignette);
        ui->valueLabel->setText(QString::number(vignette));
    }
    else if (filter == "Резкость") {
           ui->slider->setRange(0, 100);
           ui->slider->setValue(sharpness);
           ui->valueLabel->setText(QString::number(sharpness));
       }
       else if (filter == "Сепия") {
           ui->slider->setRange(0, 100);
           ui->slider->setValue(sepia);
           ui->valueLabel->setText(QString::number(sepia));
       }
       else {
           ui->slider->setEnabled(false);
           ui->valueLabel->setText("0");
       }
}

void MainWindow::on_applyButton_clicked()
{
    QString filter = ui->comboBox->currentText();
    int value = ui->slider->isEnabled() ? ui->slider->value() : 0;

    if (filter == "Яркость") {
        brightness = value;
    }
    else if (filter == "Контрастность") {
        contrast = value;
    }
    else if (filter == "Грейскейл") {
        grayscale = value;
    }
    else if (filter == "Насыщенность") {
        saturation = value;
    }
    else if (filter == "Температура") {
        temperature = value;
    }
    else if (filter == "Виньетка") {
        vignette = value;
    }
    else if (filter == "Резкость") {
          sharpness = value;
      }
      else if (filter == "Сепия") {
          sepia = value;
      }

      applyAllFilters();
      updateImageDisplay();
}

void MainWindow::on_slider_valueChanged(int value)
{
    ui->valueLabel->setText(QString::number(value));
}

void MainWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить изображение", "", "Images (*.png *.xpm *.jpg)");
    if (!filePath.isEmpty()) {
        currentImage.save(filePath);
    }
}

void MainWindow::on_resetButton_clicked()
{
    brightness = 0;
    contrast = 0;
    grayscale = 0;
    saturation = 100;
    temperature = 0;
    vignette = 0;
    sharpness = 0;
    sepia = 0;

    on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
    currentImage = originalImage;
    updateImageDisplay();
}

void MainWindow::applyAllFilters()
{
    currentImage = originalImage;

    if (brightness != 0) {
        currentImage = FilterBrightness(currentImage, brightness);
    }
    if (contrast != 0) {
        currentImage = FilterContrast(currentImage, contrast);
    }
    if (saturation != 100) {
        currentImage = FilterSaturation(currentImage, saturation);
    }
    if (temperature != 0) {
        currentImage = FilterTemperature(currentImage, temperature);
    }
    if (grayscale != 0) {
        currentImage = FilterGrayscale(currentImage, grayscale);
    }
    if (vignette != 0) {
        currentImage = FilterVignette(currentImage, vignette);
    }
    if (sharpness != 0) {
          currentImage = FilterSharpness(currentImage, sharpness);
      }
      if (sepia != 0) {
          currentImage = FilterSepia(currentImage, sepia);
      }
}

QImage MainWindow::FilterBrightness(const QImage &image, int parameter)
{
    QImage newImage = image;
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int r = qBound(0, color.red() + parameter, 255);
            int g = qBound(0, color.green() + parameter, 255);
            int b = qBound(0, color.blue() + parameter, 255);
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}

QImage MainWindow::FilterContrast(const QImage &image, int parameter)
{
    QImage newImage = image;
    double factor = (259.0 * (parameter + 255)) / (255.0 * (259 - parameter));
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int r = qBound(0, static_cast<int>(factor * (color.red() - 128) + 128), 255);
            int g = qBound(0, static_cast<int>(factor * (color.green() - 128) + 128), 255);
            int b = qBound(0, static_cast<int>(factor * (color.blue() - 128) + 128), 255);
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}

QImage MainWindow::FilterGrayscale(const QImage &image, int parameter)
{
    QImage newImage = image;
    double factor = parameter / 100.0;

    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int gray = qGray(color.red(), color.green(), color.blue());

            int r = color.red() * (1.0 - factor) + gray * factor;
            int g = color.green() * (1.0 - factor) + gray * factor;
            int b = color.blue() * (1.0 - factor) + gray * factor;

            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}

QImage MainWindow::FilterSaturation(const QImage &image, int parameter)
{
    QImage newImage = image;
    float saturation = parameter / 100.0f;

    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            int r = gray + saturation * (color.red() - gray);
            int g = gray + saturation * (color.green() - gray);
            int b = gray + saturation * (color.blue() - gray);
            newImage.setPixelColor(x, y, QColor(qBound(0, r, 255), qBound(0, g, 255), qBound(0, b, 255)));
        }
    }
    return newImage;
}

QImage MainWindow::FilterTemperature(const QImage &image, int parameter)
{
    QImage newImage = image;
    double temp = parameter / 100.0;
    double r, b;

    if (temp > 0) {
        r = 1.0 + 0.5 * temp;
        b = 1.0 - 0.5 * temp;
    } else {
        r = 1.0 + 0.5 * temp;
        b = 1.0 - 0.5 * temp;
    }

    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int red = qBound(0, static_cast<int>(color.red() * r), 255);
            int green = color.green();
            int blue = qBound(0, static_cast<int>(color.blue() * b), 255);
            newImage.setPixelColor(x, y, QColor(red, green, blue));
        }
    }
    return newImage;
}

QImage MainWindow::FilterVignette(const QImage &image, int parameter)
{
    QImage newImage = image;
    double strength = parameter / 100.0;
    int centerX = newImage.width() / 2;
    int centerY = newImage.height() / 2;
    double maxDist = sqrt(centerX * centerX + centerY * centerY);

    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            double dist = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
            double factor = 1.0 - strength * (dist / maxDist);
            factor = qBound(0.0, factor, 1.0);

            QColor color = newImage.pixelColor(x, y);
            int r = qBound(0, static_cast<int>(color.red() * factor), 255);
            int g = qBound(0, static_cast<int>(color.green() * factor), 255);
            int b = qBound(0, static_cast<int>(color.blue() * factor), 255);
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}
QImage MainWindow::FilterSharpness(const QImage &image, int parameter)
{
    if (parameter == 0) return image;

    QImage newImage = image;
    double factor = parameter / 100.0;
    int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

    for (int y = 1; y < image.height()-1; y++) {
        for (int x = 1; x < image.width()-1; x++) {
            int r = 0, g = 0, b = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    QColor color = image.pixelColor(x + kx, y + ky);
                    int weight = kernel[ky+1][kx+1];
                    r += color.red() * weight;
                    g += color.green() * weight;
                    b += color.blue() * weight;
                }
            }

            QColor original = image.pixelColor(x, y);
            r = qBound(0, static_cast<int>(original.red() * (1 - factor) + r * factor), 255);
            g = qBound(0, static_cast<int>(original.green() * (1 - factor) + g * factor), 255);
            b = qBound(0, static_cast<int>(original.blue() * (1 - factor) + b * factor), 255);

            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}


QImage MainWindow::FilterSepia(const QImage &image, int parameter)
{
    if (parameter == 0) return image;

    QImage newImage = image;
    double intensity = parameter / 100.0;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor color = image.pixelColor(x, y);
            int gray = qGray(color.rgb());

            int r = qBound(0, static_cast<int>(gray + 40 * intensity), 255);
            int g = qBound(0, static_cast<int>(gray + 20 * intensity), 255);
            int b = qBound(0, static_cast<int>(gray - 20 * intensity), 255);

            r = static_cast<int>(color.red() * (1 - intensity) + r * intensity);
            g = static_cast<int>(color.green() * (1 - intensity) + g * intensity);
            b = static_cast<int>(color.blue() * (1 - intensity) + b * intensity);

            newImage.setPixelColor(x, y, QColor(
                qBound(0, r, 255),
                qBound(0, g, 255),
                qBound(0, b, 255)));
        }
    }
    return newImage;
}
