#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override = default;

    private:
        void SetupInterface();
};