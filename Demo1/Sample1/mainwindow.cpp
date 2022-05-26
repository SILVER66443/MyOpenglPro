#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actDrawRect_triggered() {
    ui->openGLWidget->drawShape(glwidget::Rect);
}


void MainWindow::on_actClear_triggered() {
    ui->openGLWidget->drawShape(glwidget::None);
}


void MainWindow::on_actWireFrame_triggered() {
    ui->openGLWidget->setPolygon(!ui->actWireFrame->isChecked());
}
