#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QString>

#include "PriorityQueue.hpp"

class Tab3 : public QWidget
{
    Q_OBJECT

    QTextEdit* inp;
    QTextEdit* log;

public:
    explicit Tab3(QWidget* parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout* lay = new QVBoxLayout(this);
        lay->addWidget(new QLabel("Вагоны (тип номер) - каждый с новой строки:"));

        inp = new QTextEdit;
        inp->setPlainText("2 1\n3 2\n3 3\n1 4\n2 5\n3 6\n2 7\n1 8");
        inp->setMaximumHeight(120);
        lay->addWidget(inp);

        QPushButton* b = new QPushButton("Сортировать");
        lay->addWidget(b);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);

        connect(b, &QPushButton::clicked, this, [this] {
            log->clear();
            QList<QString> lines = inp->toPlainText().split('\n', Qt::SkipEmptyParts);

            PriorityQueue<int, std::greater<int>> pq;

            log->append("Исходный состав:");
            for (QString& line : lines) {
                QList<QString> parts = line.trimmed().split(' ', Qt::SkipEmptyParts);
                if (parts.size() < 2) continue;
                int t = parts[0].toInt(), n = parts[1].toInt();
                log->append(QString("  тип %1, вагон №%2").arg(t).arg(n));
                pq.Push(t * 10000 + n);
            }

            log->append("\nОтсортированный состав:");
            while (!pq.Empty()) {
                int key = pq.Top(); pq.pop();
                log->append(QString("  тип %1, вагон №%2").arg(key / 10000).arg(key % 10000));
            }
        });
    }
};
