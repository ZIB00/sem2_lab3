#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <sstream>

#include "PriorityQueue.hpp"

template<class T, class C>
static std::string pqToStr(PriorityQueue<T, C> pq)
{
    std::ostringstream ss;
    ss << "[";
    bool first = true;
    while (!pq.Empty()) {
        if (!first) ss << ", ";
        ss << pq.Top();
        pq.pop();
        first = false;
    }
    ss << "]";
    return ss.str();
}

class Tab1 : public QWidget
{
    Q_OBJECT

    PriorityQueue<int> pq;
    QTextEdit* log;
    QLineEdit* valIn;
    QLineEdit* mapN;
    QLineEdit* filterN;
    QLabel*    topLbl;

    void addLog(const QString& s) { log->append(s); }

    void refresh()
    {
        QString top = pq.Empty() ? "-" : QString::number(pq.Top());
        topLbl->setText("Top: " + top + "   Очередь: " + QString::fromStdString(pqToStr(pq)));
    }

public:
    explicit Tab1(QWidget* parent = nullptr) : QWidget(parent)
    {
        auto* lay = new QVBoxLayout(this);

        auto* row1 = new QHBoxLayout;
        valIn = new QLineEdit; valIn->setPlaceholderText("число");
        auto* bPush = new QPushButton("Push");
        auto* bPop  = new QPushButton("Pop");
        row1->addWidget(valIn); row1->addWidget(bPush); row1->addWidget(bPop);
        lay->addLayout(row1);

        topLbl = new QLabel("Top: -");
        lay->addWidget(topLbl);

        auto* row2 = new QHBoxLayout;
        mapN = new QLineEdit; mapN->setPlaceholderText("Map *n: n=");
        auto* bMap = new QPushButton("Map *n");
        filterN = new QLineEdit; filterN->setPlaceholderText("Where >n: n=");
        auto* bFlt = new QPushButton("Where >n");
        row2->addWidget(mapN); row2->addWidget(bMap);
        row2->addWidget(filterN); row2->addWidget(bFlt);
        lay->addLayout(row2);

        auto* row3 = new QHBoxLayout;
        auto* bRed   = new QPushButton("Reduce (sum)");
        auto* bCat   = new QPushButton("Concat self");
        auto* bSub   = new QPushButton("Subseq [0,2]");
        auto* bIsSub = new QPushButton("IsSubseq [top]");
        auto* bSpl   = new QPushButton("Split >5");
        for (auto* b : {bRed, bCat, bSub, bIsSub, bSpl}) row3->addWidget(b);
        lay->addLayout(row3);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);

        connect(bPush, &QPushButton::clicked, this, [this] {
            bool ok; int v = valIn->text().toInt(&ok);
            if (!ok) { addLog("Введите целое число"); return; }
            pq.Push(v);
            addLog("Push " + QString::number(v));
            refresh();
        });

        connect(bPop, &QPushButton::clicked, this, [this] {
            if (pq.Empty()) { addLog("Очередь пуста"); return; }
            int v = pq.Top(); pq.pop();
            addLog("Pop -> убрали " + QString::number(v));
            refresh();
        });

        connect(bMap, &QPushButton::clicked, this, [this] {
            bool ok; int n = mapN->text().toInt(&ok);
            if (!ok) { addLog("Введите n"); return; }
            auto r = pq.Map([n](int x) { return x * n; });
            addLog("Map *" + QString::number(n) + " -> " + QString::fromStdString(pqToStr(r)));
        });

        connect(bFlt, &QPushButton::clicked, this, [this] {
            bool ok; int n = filterN->text().toInt(&ok);
            if (!ok) { addLog("Введите n"); return; }
            auto r = pq.Where([n](int x) { return x > n; });
            addLog("Where >" + QString::number(n) + " -> " + QString::fromStdString(pqToStr(r)));
        });

        connect(bRed, &QPushButton::clicked, this, [this] {
            int s = pq.Reduce<int>([](int a, int b) { return a + b; });
            addLog("Reduce(sum) = " + QString::number(s));
        });

        connect(bCat, &QPushButton::clicked, this, [this] {
            auto r = pq.Concat(pq);
            addLog("Concat self+self -> " + QString::fromStdString(pqToStr(r)));
        });

        connect(bSub, &QPushButton::clicked, this, [this] {
            DynamicArray<size_t> idx = {0, 2};
            auto r = pq.GetSubsequence(0, idx.GetSize() - 1);
            addLog("Subseq [0,2] -> " + QString::fromStdString(pqToStr(r)));
        });

        connect(bIsSub, &QPushButton::clicked, this, [this] {
            PriorityQueue<int> sub;
            if (!pq.Empty()) sub.Push(pq.Top());
            bool res = pq.IsSubsequence(sub);
            addLog(QString("IsSubseq([top]) -> ") + (res ? "true" : "false"));
        });

        connect(bSpl, &QPushButton::clicked, this, [this] {
            auto [t, f] = pq.Split([](int x) { return x > 5; });
            addLog("Split >5: true=" + QString::fromStdString(pqToStr(t))
                 + "  false=" + QString::fromStdString(pqToStr(f)));
        });
    }
};
