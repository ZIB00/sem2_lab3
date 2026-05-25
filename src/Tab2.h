#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QFrame>
#include <QTimer>
#include <QEventLoop>
#include <QString>
#include <string>

#include "PriorityQueue.hpp"
#include "../include/sequence/sequences/ArraySequence.hpp" 

class Tab2 : public QWidget
{
    Q_OBJECT

    QSpinBox* nSpin;
    QPushButton* solveBtn;
    
    QVBoxLayout* pegA;
    QVBoxLayout* pegB;
    QVBoxLayout* pegC;

    void Hanoi(int k, char src, char dst, char aux, Sequence<std::string>*& moves)
    {
        if (k == 0) return;
        Hanoi(k-1, src, aux, dst, moves);
        
        std::string move_str = std::string(1, src) + "->" + std::string(1, dst);
        moves = moves->Append(move_str); 
        
        Hanoi(k-1, aux, dst, src, moves);
    }

    void ClearPeg(QVBoxLayout* layout) {
        if (!layout) return;

        while (layout->count() > 0) {
            QLayoutItem* item = layout->takeAt(0);
            if (item) {
                if (QWidget* widget = item->widget()) {
                    widget->hide();
                    widget->deleteLater();
                }
                delete item;
            }
        }
        
        layout->addStretch(1);
    }

    QLabel* createDiskWidget(int size, int totalDisks) {
    QLabel* disk = new QLabel();
    
    int width = 40 + (size * (160 / totalDisks));
    disk->setFixedSize(width, 25);
    disk->setAlignment(Qt::AlignCenter);
    disk->setText(QString::number(size));

    disk->setStyleSheet(QString(
        "QLabel {"
        "   border-radius: 5px;"
        "   border: 1px solid black;"
        "}"
    ));
    
    return disk;
}

    void MoveDiskVisual(char src, char dst) {
        QVBoxLayout* fromPeg = (src == 'A') ? pegA : ((src == 'B') ? pegB : pegC);
        QVBoxLayout* toPeg = (dst == 'A') ? pegA : ((dst == 'B') ? pegB : pegC);

        if (fromPeg->count() <= 1) return;

        QLayoutItem* item = fromPeg->takeAt(1);
        if (!item) return;

        QWidget* diskWidget = item->widget();
        delete item;

        if (diskWidget) {
            toPeg->insertWidget(1, diskWidget, 0, Qt::AlignHCenter);
            
            diskWidget->update();
        }

        QEventLoop loop;
        QTimer::singleShot(500, &loop, &QEventLoop::quit);
        loop.exec();
    }

public:
    explicit Tab2(QWidget* parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout* mainLay = new QVBoxLayout(this);

        QHBoxLayout* row = new QHBoxLayout;
        nSpin = new QSpinBox; 
        nSpin->setRange(1, 8); 
        nSpin->setValue(3);
        solveBtn = new QPushButton("Решить");
        row->addWidget(new QLabel("Дисков:")); 
        row->addWidget(nSpin); 
        row->addWidget(solveBtn);
        row->addStretch();
        mainLay->addLayout(row);

        QHBoxLayout* gameField = new QHBoxLayout;
        
        QFrame* frameA = new QFrame;
        frameA->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        QFrame* frameB = new QFrame; 
        frameB->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        QFrame* frameC = new QFrame; 
        frameC->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

        pegA = new QVBoxLayout(frameA); 
        pegA->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        pegB = new QVBoxLayout(frameB); 
        pegB->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        pegC = new QVBoxLayout(frameC); 
        pegC->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

        gameField->addWidget(frameA);
        gameField->addWidget(frameB);
        gameField->addWidget(frameC);
        mainLay->addLayout(gameField);

        pegA->addStretch(1);
        pegB->addStretch(1);
        pegC->addStretch(1);

        connect(solveBtn, &QPushButton::clicked, this, [this] {
            solveBtn->setEnabled(false);
            nSpin->setEnabled(false);

            int n = nSpin->value();

            ClearPeg(pegA);
            ClearPeg(pegB);
            ClearPeg(pegC);

            for (int i = n; i >= 1; i--) {
                QLabel* disk = createDiskWidget(i, n);
                pegA->insertWidget(1, disk, 0, Qt::AlignHCenter);
            }

            QEventLoop loop;
            QTimer::singleShot(500, &loop, &QEventLoop::quit);
            loop.exec();

            Sequence<std::string>* moves = new MutableArraySequence<std::string>();
            Hanoi(n, 'A', 'C', 'B', moves);

            PriorityQueue<int> pq;
            size_t moves_len = moves->GetLength();
            for (size_t i = 0; i < moves_len; i++) {
                pq.Push(static_cast<int>(i));
            }

            Sequence<int>* order = new MutableArraySequence<int>();
            while (!pq.Empty()) { 
                order = order->Append(pq.Top()); 
                pq.pop(); 
            }
            
            int order_len = static_cast<int>(order->GetLength());

            for (int i = order_len - 1; i >= 0; i--) {
                int idx = order->Get(i);
                std::string move_text = moves->Get(static_cast<size_t>(idx)); 

                char src = move_text[0];
                char dst = move_text[3];

                MoveDiskVisual(src, dst);
            }
            
            delete moves;
            delete order;

            solveBtn->setEnabled(true);
            nSpin->setEnabled(true);
        });
    }
};