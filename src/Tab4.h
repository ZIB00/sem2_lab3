#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <vector>

#include "PriorityQueue.hpp"
#include "Tab1.h"   // pqToStr

class Tab4 : public QWidget
{
    Q_OBJECT

    QLineEdit* seqIn;
    QTextEdit* log;

public:
    explicit Tab4(QWidget* parent = nullptr) : QWidget(parent)
    {
        auto* lay = new QVBoxLayout(this);
        lay->addWidget(new QLabel("Последовательность чисел через пробел:"));

        seqIn = new QLineEdit("3 1 4 1 5 9 2 6");
        lay->addWidget(seqIn);

        auto* b = new QPushButton("Анализ");
        lay->addWidget(b);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);

        connect(b, &QPushButton::clicked, this, [this] {
            log->clear();
            auto parts = seqIn->text().split(' ', Qt::SkipEmptyParts);
            std::vector<int> nums;
            for (auto& p : parts) { bool ok; int v = p.toInt(&ok); if (ok) nums.push_back(v); }
            if (nums.empty()) { log->append("Нет чисел"); return; }

            PriorityQueue<int> pq;
            for (int v : nums) pq.Push(v);

            int sum = pq.Reduce<int>([](int a, int b) { return a + b; });
            double mean = (double)sum / (int)pq.Size();

            log->append("Последовательность: " + seqIn->text());
            log->append("Размер: "   + QString::number(pq.Size()));
            log->append("Максимум: " + QString::number(pq.Top()));
            log->append("Сумма: "    + QString::number(sum));
            log->append("Среднее: "  + QString::number(mean, 'f', 2));

            int inv = 0;
            for (size_t i = 0; i < nums.size(); i++)
                for (size_t j = i+1; j < nums.size(); j++)
                    if (nums[i] > nums[j]) inv++;
            log->append("\nЧисло инверсий: " + QString::number(inv));

            auto above = pq.Where([mean](int x) { return x > mean; });
            log->append(QString("Where > %1: ").arg(mean, 0, 'f', 2)
                      + QString::fromStdString(pqToStr(above)));

            auto doubled = pq.Map([](int x) { return x * 2; });
            log->append("Map *2: " + QString::fromStdString(pqToStr(doubled)));

            int med = nums[nums.size() / 2];
            auto [t, f] = pq.Split([med](int x) { return x > med; });
            log->append(QString("Split >%1: true=%2  false=%3")
                .arg(med)
                .arg(QString::fromStdString(pqToStr(t)))
                .arg(QString::fromStdString(pqToStr(f))));
        });
    }
};
