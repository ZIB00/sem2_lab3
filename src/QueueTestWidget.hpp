#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QEvent>
#include <sstream>

#include "PriorityQueue.hpp"
#include "MyComplex.hpp"
#include "FuncStudTeach.hpp"
#include "other/Exceptions.hpp"

template<class Type, class Comp>
static std::string pqToStr(PriorityQueue<Type, Comp> pq) {
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

template<typename T>
struct MapLen {
    int operator()(const T& x) const {
        std::ostringstream ss; ss << x;
        return static_cast<int>(ss.str().length());
    }
};

template<typename T>
struct WhereThresh {
    T threshold;
    bool operator()(const T& x) const { return threshold < x; }
};

template<typename T>
struct ReduceSum {
    T operator()(const T& a, const T& b) const { return a + b; }
};

template <typename T, typename Compare = std::less<T>>
class QueueTestWidget : public QWidget {
    PriorityQueue<T, Compare> pq;

    QTextEdit* log;
    QLineEdit* valIn;
    QLineEdit* mapN;
    QLineEdit* filterN;
    QLabel* topLbl;

    QPushButton* bPush;
    QPushButton* bPop;
    QPushButton* bMap;
    QPushButton* bWhere;
    QPushButton* bRed;
    QPushButton* bCat;
    QPushButton* bSub;
    QPushButton* bIsSub;

    typedef T (*Parser)(const QString&);
    Parser parseFunc;

    void addLog(const QString& s) { log->append(s); }
    void addCustomErrorLog(const std::string& type, const char* msg) { 
        log->append("[" + QString::fromStdString(type) + "]: " + QString::fromUtf8(msg)); 
    }

    void refresh() {
        QString top = pq.Empty() ? "—" : QString::fromStdString((std::ostringstream() << pq.Top()).str());
        topLbl->setText("Top: " + top + "   Очередь: " + QString::fromStdString(pqToStr(pq)));
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::MouseButtonRelease) {
            if (obj == bPush) { doPush(); return true; }
            if (obj == bPop) { doPop(); return true; }
            if (obj == bMap) { doMap(); return true; }
            if (obj == bWhere) { doWhere(); return true; }
            if (obj == bRed) { doRed(); return true; }
            if (obj == bCat) { doCat(); return true; }
            if (obj == bSub) { doSub(); return true; }
            if (obj == bIsSub) { doIsSub(); return true; }
        }
        return QWidget::eventFilter(obj, event);
    }

    void doPush() {
        if (valIn->text().trimmed().isEmpty()) {
            addCustomErrorLog("InvalidArgument", "Поле ввода пусто!");
            return;
        }
        try {
            pq.Push(parseFunc(valIn->text()));
            addLog("Элемент успешно добавлен.");
            refresh();
        } 
        catch (const OtherError& e) { addCustomErrorLog("Error", e.what()); }
        catch (...) { addCustomErrorLog("UnknownError", "Неизвестный сбой."); }
    }

    void doPop() {
        if (pq.Empty()) { 
            addCustomErrorLog("LogicError", "Попытка извлечь из пустой очереди!"); 
            return; 
        }
        pq.pop();
        addLog("Элемент извлечен.");
        refresh();
    }

    void doMap() {
        if (pq.Empty()) return;
        auto r = pq.Map(MapLen<T>());
        addLog("Map выполнен: " + QString::fromStdString(pqToStr<int, std::less<int>>(r)));
    }

    void doWhere() {
        if (filterN->text().trimmed().isEmpty()) return;
        try {
            WhereThresh<T> cond{ parseFunc(filterN->text()) };
            auto r = pq.Where(cond);
            addLog("Where: " + QString::fromStdString(pqToStr(r)));
        } catch (...) { addCustomErrorLog("Error", "Ошибка фильтрации."); }
    }

    void doRed() {
        if (pq.Empty()) return;
        try {
            T s = pq.template Reduce<T>(ReduceSum<T>());
            std::ostringstream ss; ss << s;
            addLog("Reduce = " + QString::fromStdString(ss.str()));
        } catch (...) { addCustomErrorLog("Error", "Ошибка агрегации."); }
    }

    void doCat() {
        auto r = pq.Concat(pq);
        addLog("Concat: " + QString::fromStdString(pqToStr(r)));
    }

    void doSub() {
        if (pq.Size() < 3) return;
        auto r = pq.GetSubsequence(0, 2);
        addLog("Subsequence [0,2]: " + QString::fromStdString(pqToStr(r)));
    }

    void doIsSub() {
        if (valIn->text().trimmed().isEmpty()) {
            addCustomErrorLog("InvalidArgument", "Введите значение в поле ввода для проверки");
            return;
        }
        try {
            PriorityQueue<T, Compare> sub;
            sub.Push(parseFunc(valIn->text()));
            addLog(QString("IsSubsequence([valIn]): ") + (pq.IsSubsequence(sub) ? "true" : "false"));
        } catch (...) { 
            addCustomErrorLog("Error", "Ошибка парсинга элемента."); 
        }
    }

public:
    QueueTestWidget(Parser parser, QWidget* parent = nullptr) 
        : QWidget(parent), parseFunc(parser) 
    {
        QVBoxLayout* lay = new QVBoxLayout(this);

        QHBoxLayout* row1 = new QHBoxLayout;
        valIn = new QLineEdit; 
        bPush = new QPushButton("Push"); bPush->installEventFilter(this);
        bPop = new QPushButton("Pop"); bPop->installEventFilter(this);

        row1->addWidget(valIn); 
        row1->addWidget(bPush); 
        row1->addWidget(bPop);

        lay->addLayout(row1);

        topLbl = new QLabel("Top: —   Очередь: []");
        lay->addWidget(topLbl);

        QHBoxLayout* rowMap = new QHBoxLayout;
        bMap = new QPushButton("Map (len(item))"); bMap->installEventFilter(this);
        rowMap->addWidget(bMap);
        lay->addLayout(rowMap);

        QHBoxLayout* rowFilter = new QHBoxLayout;
        filterN = new QLineEdit; 
        bWhere = new QPushButton("Where (> n)"); bWhere->installEventFilter(this);
        
        rowFilter->addWidget(filterN); 
        rowFilter->addWidget(bWhere);
        lay->addLayout(rowFilter);

        QHBoxLayout* rowOps = new QHBoxLayout;
        bRed = new QPushButton("Reduce"); bRed->installEventFilter(this);
        bCat = new QPushButton("Concat self"); bCat->installEventFilter(this);
        bSub = new QPushButton("Subseq [0,2]"); bSub->installEventFilter(this);
        bIsSub = new QPushButton("IsSubseq([valIn])"); bIsSub->installEventFilter(this);

        rowOps->addWidget(bRed); rowOps->addWidget(bCat); rowOps->addWidget(bSub); rowOps->addWidget(bIsSub);
        lay->addLayout(rowOps);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);
    }
};