#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "QueueControllerImpl.hpp"

class BaseTab : public QWidget {
    Q_OBJECT

private:
    IQueueController *currentController;

    QTextEdit *log;
    QComboBox *queueType;
    QComboBox *secondComboBox;
    QLabel *label1;
    QLabel *label2;
    QLineEdit *input1;
    QLineEdit *input2;

    QTextEdit *output;
    
    QPushButton *btnTop;
    QPushButton *btnEmpty;
    QPushButton *btnSize;
    QPushButton *btnPush;
    QPushButton *btnPop;
    QPushButton *btnMap;
    QPushButton *btnWhere;
    QPushButton *btnReduce;
    QPushButton *btnConcat;
    QPushButton *btnGetSubsequence;
    QPushButton *btnIsSubsequence;
    QPushButton *btnAppend;
    QPushButton *btnSplit;

    QLineEdit *inputPush;
    QComboBox *comboMap;
    QComboBox *comboWhere;
    QComboBox *comboReduce;
    QLineEdit *inputConcat;
    QLineEdit *inputSubseqStart;
    QLineEdit *inputSubseqEnd;
    QLineEdit *inputIsSubseq;
    QLineEdit *inputAppend;
    QComboBox *comboSplit;

public:
    BaseTab(QTextEdit *log, QWidget *parent = nullptr);
    ~BaseTab();

    void updateGUI();

    void onTop();
    void onEmpty();
    void onSize();
    void onPush();
    void onPop();
    void onMap();
    void onWhere();
    void onReduce();
    void onConcat();
    void onGetSubsequence();
    void onIsSubsequence();
    void onAppend();
    void onSplit();

    void updateOutput();
};