#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>

#include "Tab1.hpp"
#include "Tab2.hpp"
#include "Tab3.hpp"
#include "Tab4.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow win;
    win.setWindowTitle("Priority Queue");
    win.resize(850, 600);

    QTabWidget* tabs = new QTabWidget;
    tabs->addTab(new Tab1, "Очередь с приоритетом");
    tabs->addTab(new Tab2, "Ханойская башня");
    tabs->addTab(new Tab3, "Простые числа");
    tabs->addTab(new Tab4, "Анализ (инверсии)");

    win.setCentralWidget(tabs);
    win.show();

    return app.exec();
}
