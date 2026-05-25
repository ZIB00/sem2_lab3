#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QTextEdit>
#include <QString>
#include <string>

#include "PriorityQueue.hpp"
#include "../include/sequence/sequences/ArraySequence.hpp" 

class Tab2 : public QWidget
{
    Q_OBJECT

    QSpinBox* nSpin;
    QTextEdit* log;

    void hanoi(int k, char src, char dst, char aux, Sequence<std::string>*& moves)
    {
        if (k == 0) return;
        hanoi(k-1, src, aux, dst, moves);
        
        std::string move_str = "Диск " + std::to_string(k) + ": " + src + " -> " + dst;
        moves = moves->Append(move_str); 
        
        hanoi(k-1, aux, dst, src, moves);
    }

public:
    explicit Tab2(QWidget* parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout* lay = new QVBoxLayout(this);

        QHBoxLayout* row = new QHBoxLayout;
        nSpin = new QSpinBox; 
        nSpin->setRange(1, 12); 
        nSpin->setValue(3);
        QPushButton* b = new QPushButton("Решить");
        row->addWidget(new QLabel("Дисков:")); row->addWidget(nSpin); row->addWidget(b);
        row->addStretch();
        lay->addLayout(row);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);

        connect(b, &QPushButton::clicked, this, [this] {
            log->clear();
            int n = nSpin->value();

            Sequence<std::string>* moves = new MutableArraySequence<std::string>();
            hanoi(n, 'A', 'C', 'B', moves);

            // Кладём индексы в PriorityQueue
            PriorityQueue<int> pq;
            size_t moves_len = moves->GetLength();
            for (size_t i = 0; i < moves_len; i++) {
                pq.Push(static_cast<int>(i));
            }

            log->append(QString("Ханойская башня, %1 дисков, %2 ходов:\n").arg(n).arg(moves_len));

            Sequence<int>* order = new MutableArraySequence<int>();
            while (!pq.Empty()) { 
                order = order->Append(pq.Top()); 
                pq.pop(); 
            }

            auto* arrayOrder = dynamic_cast<ArraySequence<int>*>(order);
            auto* arrayMoves = dynamic_cast<ArraySequence<std::string>*>(moves);

            if (arrayOrder && arrayMoves) {
                int order_len = static_cast<int>(arrayOrder->GetLength());
                for (int i = order_len - 1; i >= 0; i--) {
                    int idx = arrayOrder->Get(i); 
                    std::string move_text = arrayMoves->Get(idx);

                    log->append(QString("Шаг %1: %2")
                        .arg(idx + 1)
                        .arg(QString::fromStdString(move_text)));
                }
            }

            delete moves;
            delete order;
        });
    }
};