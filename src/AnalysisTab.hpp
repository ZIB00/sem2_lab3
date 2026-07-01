#pragma once

#include <memory>

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "../include/sequence/Sequence/ArraySequence.hpp"
#include "Sequence/ArraySequence.hpp"

class AnalysisTab : public QWidget {
    Q_OBJECT

private:
    QTextEdit *log;
    QLineEdit *numInput;
    QPushButton *start;

    QTextEdit *output;

public:
    AnalysisTab(QTextEdit *log, QWidget *parent = nullptr);

    bool Parse(std::shared_ptr<MutableArraySequence<int>> seq);

    void onAnalysis();
};