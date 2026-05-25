#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <sstream>
#include <functional>

#include "PriorityQueue.hpp"
#include "MyComplex.hpp"
#include "FuncStudTeach.hpp"
#include "other/Exceptions.hpp"

template<class Type, class Comp>
static std::string pqToStr(PriorityQueue<Type, Comp> pq)
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

template <typename T, typename Compare = std::less<T>>
class QueueTestWidget : public QWidget {
    PriorityQueue<T, Compare> pq;

    QTextEdit* log;
    QLineEdit* valIn;
    QLineEdit* mapN;
    QLineEdit* filterN;
    QLabel* topLbl;

    std::function<T(const QString&)> parseFunc;

    void addLog(const QString& s) { log->append(s); }
    
    void addCustomErrorLog(const std::string& type, const char* msg) { 
        log->append("<font color='red'><b>[" + QString::fromStdString(type) + "]:</b> " + QString::fromUtf8(msg) + "</font>"); 
    }

    void refresh()
    {
        QString top = pq.Empty() ? "—" : QString::fromStdString((std::ostringstream() << pq.Top()).str());
        topLbl->setText("Top: " + top + "   Очередь: " + QString::fromStdString(pqToStr(pq)));
    }

public:
    QueueTestWidget(std::function<T(const QString&)> parser, QWidget* parent = nullptr) 
        : QWidget(parent), parseFunc(parser) 
    {
        QVBoxLayout* lay = new QVBoxLayout(this);

        QHBoxLayout* row1 = new QHBoxLayout;
        valIn = new QLineEdit; 
        valIn->setPlaceholderText("Значение элемента");
        QPushButton* bPush = new QPushButton("Push");
        QPushButton* bPop = new QPushButton("Pop");
        row1->addWidget(valIn); row1->addWidget(bPush); row1->addWidget(bPop);
        lay->addLayout(row1);

        topLbl = new QLabel("Top: —   Очередь: []");
        lay->addWidget(topLbl);

        // Map
        QHBoxLayout* rowMap = new QHBoxLayout;
        mapN = new QLineEdit; mapN->setPlaceholderText("Функция Map (автоматический подсчет длин)");
        mapN->setReadOnly(true);
        QPushButton* bMap = new QPushButton("Map (В длины строк)");
        rowMap->addWidget(mapN); rowMap->addWidget(bMap);
        lay->addLayout(rowMap);

        // Where
        QHBoxLayout* rowFilter = new QHBoxLayout;
        filterN = new QLineEdit; filterN->setPlaceholderText("Порог для фильтрации");
        QPushButton* bWhere = new QPushButton("Where (Элементы > порога)");
        rowFilter->addWidget(filterN); rowFilter->addWidget(bWhere);
        lay->addLayout(rowFilter);

        // Остальные операции
        QHBoxLayout* rowOps = new QHBoxLayout;
        QPushButton* bRed = new QPushButton("Reduce (Сумма)");
        QPushButton* bCat = new QPushButton("Concat self");
        QPushButton* bSub = new QPushButton("Subseq [0,2]");
        QPushButton* bIsSub = new QPushButton("IsSubseq([top])");
        rowOps->addWidget(bRed); rowOps->addWidget(bCat); rowOps->addWidget(bSub); rowOps->addWidget(bIsSub);
        lay->addLayout(rowOps);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);

        connect(bPush, &QPushButton::clicked, this, [this] {
            if (valIn->text().trimmed().isEmpty()) {
                addCustomErrorLog("InvalidArgument", "Поле ввода пусто!");
                return;
            }
            try {
                T val = parseFunc(valIn->text());
                pq.Push(val);
                addLog("Элемент успешно добавлен.");
                refresh();
            } 
            catch (const InvalidArgument& e) { addCustomErrorLog("InvalidArgument", e.what()); }
            catch (const OutOfRange& e)      { addCustomErrorLog("OutOfRange", e.what()); }
            catch (const LogicError& e)      { addCustomErrorLog("LogicError", e.what()); }
            catch (const std::exception& e)  { addCustomErrorLog("StdException", e.what()); }
            catch (...)                      { addCustomErrorLog("UnknownError", "Неизвестный сбой приложения."); }
        });

        connect(bPop, &QPushButton::clicked, this, [this] {
            if (pq.Empty()) { 
                addCustomErrorLog("LogicError", "Попытка извлечь корень из пустой очереди!"); 
                return; 
            }
            pq.pop();
            addLog("Элемент извлечен из вершины.");
            refresh();
        });

        connect(bMap, &QPushButton::clicked, this, [this] {
            if (pq.Empty()) { 
                addCustomErrorLog("LogicError", "Очередь пуста. Операция Map невозможна."); 
                return; 
            }
            auto r = pq.Map([](const T& x) -> int {
                std::ostringstream ss; ss << x;
                return static_cast<int>(ss.str().length());
            });
            std::string resStr = pqToStr<int, std::less<int>>(r);
            addLog("Map выполнен → " + QString::fromStdString(resStr));
        });

        connect(bWhere, &QPushButton::clicked, this, [this] {
            if (filterN->text().trimmed().isEmpty()) { 
                addCustomErrorLog("InvalidArgument", "Поле порога пусто!"); 
                return; 
            }
            try {
                T threshold = parseFunc(filterN->text());
                auto r = pq.Where([threshold](const T& x) { return threshold < x; });
                addLog("Where (элементы > порога) → " + QString::fromStdString(pqToStr(r)));
            } 
            catch (const InvalidArgument& e) { addCustomErrorLog("InvalidArgument", e.what()); }
            catch (const OutOfRange& e)      { addCustomErrorLog("OutOfRange", e.what()); }
            catch (...)                      { addCustomErrorLog("Error", "Ошибка фильтрации."); }
        });

        connect(bRed, &QPushButton::clicked, this, [this] {
            if (pq.Empty()) { 
                addCustomErrorLog("LogicError", "Нечего редуцировать, очередь пуста."); 
                return; 
            }
            try {
                T s = pq.template Reduce<T>([&](const T& a, const T& b) { return a + b; });
                std::ostringstream ss; ss << s;
                addLog("Reduce (Сумма элементов) = " + QString::fromStdString(ss.str()));
            } catch (...) {
                addCustomErrorLog("OtherError", "Ошибка выполнения агрегации.");
            }
        });

        connect(bCat, &QPushButton::clicked, this, [this] {
            auto r = pq.Concat(pq);
            addLog("Concat (Очередь + Очередь) → " + QString::fromStdString(pqToStr(r)));
        });

        connect(bSub, &QPushButton::clicked, this, [this] {
            if (pq.Size() < 3) { 
                addCustomErrorLog("OutOfRange", "Недостаточно элементов для извлечения индексов [0,2]!"); 
                return; 
            }
            auto r = pq.GetSubsequence(0, 2);
            addLog("Subsequence [0,2] → " + QString::fromStdString(pqToStr(r)));
        });

        connect(bIsSub, &QPushButton::clicked, this, [this] {
            PriorityQueue<T, Compare> sub;
            if (!pq.Empty()) sub.Push(pq.Top());
            bool res = pq.IsSubsequence(sub);
            addLog(QString("IsSubsequence → ") + (res ? "true" : "false"));
        });
    }
};