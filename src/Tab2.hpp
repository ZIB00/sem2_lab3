#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QFrame>
#include <QEvent>
#include <QString>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <string>

#include "PriorityQueue.hpp"
#include "../include/sequence/sequences/ArraySequence.hpp" 

class Tab2 : public QWidget {
    QSpinBox* nSpin;
    QPushButton* solveBtn;
    
    QVBoxLayout* pegA;
    QVBoxLayout* pegB;
    QVBoxLayout* pegC;

    void delay(int ms) {
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < ms) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        }
    }

    void Hanoi(int k, char src, char dst, char aux, Sequence<std::string>*& moves) {
        if (k == 0) return;
        Hanoi(k-1, src, aux, dst, moves);
        moves = moves->Append(std::string(1, src) + "->" + std::string(1, dst)); 
        Hanoi(k-1, aux, dst, src, moves);
    }

    void ClearPeg(QVBoxLayout* layout) {
        while (layout->count() > 0) {
            QLayoutItem* item = layout->takeAt(0);
            if (QWidget* w = item->widget()) {
                w->hide();
                w->deleteLater();
            }
            delete item;
        }
        layout->addStretch(1);
    }

    QLabel* createDiskWidget(int size, int totalDisks) {
        QLabel* disk = new QLabel(QString::number(size));
        disk->setFixedSize(40 + (size * (160 / totalDisks)), 25);
        disk->setAlignment(Qt::AlignCenter);
        disk->setStyleSheet("QLabel { background-color: lightgray; border: 1px solid black; border-radius: 3px; }");
        return disk;
    }

    void MoveDiskVisual(char src, char dst) {
        QVBoxLayout* fromPeg = (src == 'A') ? pegA : ((src == 'B') ? pegB : pegC);
        QVBoxLayout* toPeg = (dst == 'A') ? pegA : ((dst == 'B') ? pegB : pegC);

        if (fromPeg->count() <= 1) return;

        QLayoutItem* item = fromPeg->takeAt(1);
        QWidget* diskWidget = item->widget();
        delete item;

        if (diskWidget) {
            toPeg->insertWidget(1, diskWidget, 0, Qt::AlignHCenter);
        }

        delay(500);
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (obj == solveBtn && event->type() == QEvent::MouseButtonRelease) {
            solveBtn->setEnabled(false);
            nSpin->setEnabled(false);

            int n = nSpin->value();
            ClearPeg(pegA); ClearPeg(pegB); ClearPeg(pegC);

            for (int i = n; i >= 1; i--) {
                pegA->insertWidget(1, createDiskWidget(i, n), 0, Qt::AlignHCenter);
            }

            delay(500);

            Sequence<std::string>* moves = new MutableArraySequence<std::string>();
            Hanoi(n, 'A', 'C', 'B', moves);

            PriorityQueue<int> pq;
            for (size_t i = 0; i < moves->GetLength(); i++) pq.Push(static_cast<int>(i));

            Sequence<int>* order = new MutableArraySequence<int>();
            while (!pq.Empty()) { order = order->Append(pq.Top()); pq.pop(); }

            for (int i = order->GetLength() - 1; i >= 0; i--) {
                std::string move_text = moves->Get(order->Get(i)); 
                MoveDiskVisual(move_text[0], move_text[3]);
            }
            
            delete moves; delete order;
            solveBtn->setEnabled(true); nSpin->setEnabled(true);
            return true;
        }
        return QWidget::eventFilter(obj, event);
    }

public:
    Tab2(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* mainLay = new QVBoxLayout(this);

        QHBoxLayout* row = new QHBoxLayout;
        nSpin = new QSpinBox; nSpin->setRange(1, 8); nSpin->setValue(3);
        solveBtn = new QPushButton("Решить"); solveBtn->installEventFilter(this);
        
        row->addWidget(new QLabel("Дисков:")); 
        row->addWidget(nSpin); 
        row->addWidget(solveBtn);
        row->addStretch();
        mainLay->addLayout(row);

        QHBoxLayout* gameField = new QHBoxLayout;
        QFrame* frameA = new QFrame; frameA->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        pegA = new QVBoxLayout(frameA); pegA->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        
        QFrame* frameB = new QFrame; frameB->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        pegB = new QVBoxLayout(frameB); pegB->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        
        QFrame* frameC = new QFrame; frameC->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        pegC = new QVBoxLayout(frameC); pegC->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

        gameField->addWidget(frameA); gameField->addWidget(frameB); gameField->addWidget(frameC);
        mainLay->addLayout(gameField);

        pegA->addStretch(1); pegB->addStretch(1); pegC->addStretch(1);
    }
};