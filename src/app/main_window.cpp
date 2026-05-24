#include "main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    SetupInterface();
}

void MainWindow::SetupInterface() {
    QWidget* main_window = new QWidget(this);
    setCentralWidget(main_window);
    setWindowTitle("Sequences");
    resize(1300, 800);
}