#include "HanoyTab.hpp"
#include <QCoreApplication>
#include <QThread>

HanoyTab::HanoyTab(QTextEdit *log, QWidget *parent) : QWidget(parent), log(log) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *settings = new QHBoxLayout();
    
    numberOfRings = new QSpinBox(this);
    numberOfRings->setRange(1, 12);
    numberOfRings->setValue(5);
    numberOfRings->setPrefix("Колец: ");
    
    startBtn = new QPushButton("Начать", this);

    settings->addWidget(numberOfRings);
    settings->addWidget(startBtn);
    settings->addStretch();
    
    mainLayout->addLayout(settings);

    QHBoxLayout *rods = new QHBoxLayout();
    rod1 = new QWidget(this);
    rod2 = new QWidget(this);
    rod3 = new QWidget(this);

    rod1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    rod2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    rod3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QString style = "border: 1px solid lightgray;";
    rod1->setStyleSheet(style);
    rod2->setStyleSheet(style);
    rod3->setStyleSheet(style);

    lay1 = new QVBoxLayout(rod1);
    lay1->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    lay2 = new QVBoxLayout(rod2);
    lay2->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    lay3 = new QVBoxLayout(rod3);
    lay3->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    rods->addWidget(rod1);
    rods->addWidget(rod2);
    rods->addWidget(rod3);

    mainLayout->addLayout(rods);

    connect(startBtn, &QPushButton::clicked, this, &HanoyTab::onHanoy);
}

void HanoyTab::clearLayout(QLayout *layout) {
    if (!layout) return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}

void HanoyTab::moveRing(Stack<QLabel*>& fromStack, Stack<QLabel*>& toStack, QVBoxLayout *fromLay, QVBoxLayout *toLay) {
    if (fromStack.IsEmpty()) return;

    QLabel* ring = fromStack.GetRoot(); 
    fromStack.Pop();
    toStack.Push(ring);

    fromLay->removeWidget(ring);
    toLay->insertWidget(0, ring, 0, Qt::AlignHCenter); 
    
    QCoreApplication::processEvents(); 
    QThread::msleep(228); 
}

void HanoyTab::solveHanoy(int n, Stack<QLabel*>& fromStack, Stack<QLabel*>& toStack, Stack<QLabel*>& tempStack,
                                        QVBoxLayout *fromLay, QVBoxLayout *toLay, QVBoxLayout *tempLay) {
    if (n == 0) return;
    
    solveHanoy(n - 1, fromStack, tempStack, toStack, fromLay, tempLay, toLay);
    moveRing(fromStack, toStack, fromLay, toLay);
    solveHanoy(n - 1, tempStack, toStack, fromStack, tempLay, toLay, fromLay);
}

void HanoyTab::onHanoy() {
    startBtn->setEnabled(false);

    clearLayout(lay1);
    clearLayout(lay2);
    clearLayout(lay3);

    while(!stack1.IsEmpty()) stack1.Pop();
    while(!stack2.IsEmpty()) stack2.Pop();
    while(!stack3.IsEmpty()) stack3.Pop();

    int n = numberOfRings->value();

    for (int i = 1; i <= n; ++i) {
        QString color;
        if(i % 3 == 0) {
            color = "#d80000"; 
        } else if(i % 2 == 0) {
            color = "#1b0096"; 
        } else { 
            color = "#dadada";
        }
        QLabel *widget = new QLabel(this);
        
        int width = 50 + i * 25; 
        widget->setFixedSize(width, 20);
        
        widget->setStyleSheet(QString("background-color: %1; border: 1px solid black; border-radius: 10px;").arg(color));
        
        lay1->addWidget(widget, 0, Qt::AlignHCenter); 
        temp.Push(widget);
    }

    while(!temp.IsEmpty()) {
        stack1.Push(temp.GetRoot());
        temp.Pop();
    }
    
    QCoreApplication::processEvents();

    solveHanoy(n, stack1, stack3, stack2, lay1, lay3, lay2);

    startBtn->setEnabled(true);
}