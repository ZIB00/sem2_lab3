#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QEvent>
#include <QRegularExpressionValidator>

#include "../include/sequence/sequences/ArraySequence.hpp" 
#include "../include/sequence/other/Implementation.hpp" 

struct Inversion {
    int GreaterValue;
    int SmallerValue;
};

class Tab4 : public QWidget {
    QLineEdit* seqIn;
    QTextEdit* log;
    QPushButton* b;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (obj == b && event->type() == QEvent::MouseButtonRelease) {
            log->clear();
            QStringList parts = seqIn->text().split(' ', Qt::SkipEmptyParts);
            
            MutableArraySequence<int> nums;
            for (QString& p : parts) { 
                bool ok; int v = p.toInt(&ok); 
                if (ok) nums.Append(v);
            }
            if (nums.GetLength() == 0) return true;

            DefaultRing<int> ring;
            int totalInversions = ring.Zero(); 

            MutableArraySequence<int> sortedRightElements;
            MutableArraySequence<Inversion> inversionList;

            int n = static_cast<int>(nums.GetLength());
            for (int i = n - 1; i >= 0; --i) {
                int current = nums[i];
                int low = 0, high = static_cast<int>(sortedRightElements.GetLength());
                int insertIndex = high; 

                while (low < high) {
                    int mid = low + (high - low) / 2;
                    if (sortedRightElements[mid] >= current) {
                        insertIndex = mid; high = mid; 
                    } else { low = mid + 1; }
                }
                if (low == high) insertIndex = low;

                for (int j = 0; j < insertIndex; ++j) {
                    inversionList.Append({current, sortedRightElements[j]});
                }

                totalInversions = ring.Add(totalInversions, insertIndex);
                sortedRightElements.InsertAt(current, static_cast<size_t>(insertIndex));
            }

            if (inversionList.GetLength() == 0) {
                log->append("Отсортировано.");
            } else {
                for (int i = static_cast<int>(inversionList.GetLength()) - 1; i >= 0; --i) {
                    Inversion inv = inversionList[i];
                    log->append(QString("(%1 > %2)").arg(inv.GreaterValue).arg(inv.SmallerValue));
                }
                log->append("Всего: " + QString::number(totalInversions));
            }
            return true;
        }
        return QWidget::eventFilter(obj, event);
    }

public:
    Tab4(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* lay = new QVBoxLayout(this);
        lay->addWidget(new QLabel("Числа через пробел:"));

        seqIn = new QLineEdit("3 1 4 1 5 9 2 6");
        seqIn->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9 ]*$"), this));
        lay->addWidget(seqIn);

        b = new QPushButton("Подсчитать инверсии"); b->installEventFilter(this);
        lay->addWidget(b);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);
    }
};