#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    setWindowTitle("Очередь с приоритетами");
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    log = new QTextEdit(this);
    log->setReadOnly(true);
    log->setMaximumHeight(150);

    tabsWidget = new QTabWidget(this);
    
    baseTab = new BaseTab(log, this);
    hanoyTab = new HanoyTab(log, this);
    primeTab = new PrimeTab(log, this);
    analysisTab = new AnalysisTab(log, this);

    tabsWidget->addTab(baseTab, "Очередь");
    tabsWidget->addTab(hanoyTab, "Ханойская башня");
    tabsWidget->addTab(primeTab, "Простые числа");
    tabsWidget->addTab(analysisTab, "Анализ инверсий");


    layout->addWidget(tabsWidget);
    layout->addWidget(log);

    setCentralWidget(centralWidget);
    resize(800, 600);
}