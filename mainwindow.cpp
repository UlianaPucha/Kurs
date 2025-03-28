#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , brightness(0)
    , contrast(0)
    , grayscale(0)
    , saturation(100)
    , temperature(0)
    , vignette(0)
{
    ui->setupUi(this);
    toggleUI(false);
    ui->slider->setRange(-100, 100);
    ui->slider->setValue(0);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));
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
    }
    else if (filter == "Контрастность") {
        ui->slider->setRange(-100, 100);
        ui->slider->setValue(contrast);
    }
    else if (filter == "Грейскейл") {
        ui->slider->setRange(0, 100);
        ui->slider->setValue(grayscale);
    }
    else if (filter == "Насыщенность") {
        ui->slider->setRange(0, 200);
        ui->slider->setValue(saturation);
    }
    else if (filter == "Температура") {
        ui->slider->setRange(-100, 100);
        ui->slider->setValue(temperature);
    }
    else if (filter == "Виньетка") {
        ui->slider->setRange(0, 100);
        ui->slider->setValue(vignette);
    }
    else {
        ui->slider->setEnabled(false);
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

    applyAllFilters();
    updateImageDisplay();
}

void MainWindow::on_resetButton_clicked()
{
    brightness = 0;
    contrast = 0;
    grayscale = 0;
    saturation = 100;
    temperature = 0;
    vignette = 0;

    on_comboBox_currentIndexChanged(ui->comboBox->currentIndex());
    currentImage = originalImage;
    updateImageDisplay();
}

void MainWindow::applyAllFilters()
{
    currentImage = originalImage;

    if (brightness != 0) {
        currentImage = applyBrightness(currentImage, brightness);
    }
    if (contrast != 0) {
        currentImage = applyContrast(currentImage, contrast);
    }
    if (saturation != 100) {
        currentImage = applySaturation(currentImage, saturation);
    }
    if (temperature != 0) {
        currentImage = applyTemperature(currentImage, temperature);
    }
    if (grayscale != 0) {
        currentImage = applyGrayscale(currentImage, grayscale);
    }
    if (vignette != 0) {
        currentImage = applyVignette(currentImage, vignette);
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить изображение", "", "Images (*.png *.xpm *.jpg)");
    if (!filePath.isEmpty()) {
        currentImage.save(filePath);
    }
}

QImage MainWindow::applyBrightness(const QImage &image, int value)
{
    QImage newImage = image;
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int r = qBound(0, color.red() + value, 255);
            int g = qBound(0, color.green() + value, 255);
            int b = qBound(0, color.blue() + value, 255);
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}

QImage MainWindow::applyContrast(const QImage &image, int value)
{
    QImage newImage = image;
    double factor = (259.0 * (value + 255)) / (255.0 * (259 - value));
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

QImage MainWindow::applyGrayscale(const QImage &image, int value)
{
    QImage newImage = image;
    double factor = value / 100.0;

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

QImage MainWindow::applySaturation(const QImage &image, int value)
{
    QImage newImage = image;
    float saturation = value / 100.0f;

    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            int r = gray + saturation * (color.red() - gray);
            int g = gray + saturation * (color.green() - gray);
            int b = gray + saturation * (color.blue() - gray);
            newImage.setPixelColor(x, y, QColor(
                qBound(0, r, 255),
                qBound(0, g, 255),
                qBound(0, b, 255)
            ));
        }
    }
    return newImage;
}

QImage MainWindow::applyTemperature(const QImage &image, int value)
{
    QImage newImage = image;
    double temp = value / 100.0;
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

QImage MainWindow::applyVignette(const QImage &image, int value)
{
    QImage newImage = image;
    double strength = value / 100.0;
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

QImage MainWindow::applySepia(const QImage &image)
{
    QImage newImage = image;
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor color = newImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            int r = qMin(255, static_cast<int>(gray * 1.2));
            int g = qMin(255, static_cast<int>(gray * 0.9));
            int b = qMin(255, static_cast<int>(gray * 0.6));
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return newImage;
}

QImage MainWindow::applyInvert(const QImage &image)
{
    QImage newImage = image;
    newImage.invertPixels();
    return newImage;
}
