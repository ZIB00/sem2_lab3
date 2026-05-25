#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>

#include "Tab1.h"
#include "Tab2.h"
#include "Tab3.h"
#include "Tab4.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow win;
    win.setWindowTitle("Priority Queue Demo");
    win.resize(850, 600);

    auto* tabs = new QTabWidget;
    tabs->addTab(new Tab1, "Очередь с приоритетом");
    tabs->addTab(new Tab2, "Ханойская башня");
    tabs->addTab(new Tab3, "Сортировочная станция");
    tabs->addTab(new Tab4, "Анализ (инверсии)");

    win.setCentralWidget(tabs);
    win.show();

    return app.exec();
}
