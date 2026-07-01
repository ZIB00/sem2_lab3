#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "BaseTab.hpp"
#include "HanoyTab.hpp"
#include "PrimeTab.hpp"
#include "AnalysisTab.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QTabWidget *tabsWidget;
    QTextEdit *log;
    
    BaseTab *baseTab;
    HanoyTab *hanoyTab;
    PrimeTab *primeTab;
    AnalysisTab *analysisTab;
};