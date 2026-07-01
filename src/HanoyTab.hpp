#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>
#include "../include/Stack.hpp"

class HanoyTab : public QWidget {
    Q_OBJECT

private:
    QTextEdit *log;
    QSpinBox *numberOfRings;
    QPushButton *startBtn;

    QWidget *rod1;
    QWidget *rod2;
    QWidget *rod3;

    QVBoxLayout *lay1;
    QVBoxLayout *lay2;
    QVBoxLayout *lay3;

    Stack<QLabel*> stack1;
    Stack<QLabel*> stack2;
    Stack<QLabel*> stack3;
    Stack<QLabel*> temp;

    void clearLayout(QLayout *layout);
    void moveRing(Stack<QLabel*>& fromStack, Stack<QLabel*>& toStack, QVBoxLayout *fromLay, QVBoxLayout *toLay);
    void solveHanoy(int n, Stack<QLabel*>& fromStack, Stack<QLabel*>& toStack, Stack<QLabel*>& auxStack,
                                    QVBoxLayout *fromLay, QVBoxLayout *toLay, QVBoxLayout *auxLay);

public:
    HanoyTab(QTextEdit *log, QWidget *parent = nullptr);
    void onHanoy();
};