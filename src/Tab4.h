#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QRegularExpressionValidator>

#include "../include/sequence/sequences/ArraySequence.hpp" 
#include "../include/sequence/other/Implementation.hpp" 

struct Inversion {
    int GreaterValue;
    int SmallerValue;
};

class Tab4 : public QWidget
{
    Q_OBJECT

    QLineEdit* seqIn;
    QTextEdit* log;

public:
    explicit Tab4(QWidget* parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout* lay = new QVBoxLayout(this);
        lay->addWidget(new QLabel("Последовательность чисел через пробел (Total Ordering):"));

        seqIn = new QLineEdit("3 1 4 1 5 9 2 6");

        QRegularExpression regExp("^[0-9 ]*$");
        QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
        seqIn->setValidator(validator);

        lay->addWidget(seqIn);

        QPushButton* b = new QPushButton("Подсчитать и перечислить инверсии");
        lay->addWidget(b);

        log = new QTextEdit; 
        log->setReadOnly(true);
        lay->addWidget(log);

        connect(b, &QPushButton::clicked, this, [this] {
            log->clear();
            
            QStringList parts = seqIn->text().split(' ', Qt::SkipEmptyParts);
            
            MutableArraySequence<int> nums;
            for (QString& p : parts) { 
                bool ok; 
                int v = p.toInt(&ok); 
                if (ok) {
                    nums.Append(v);
                } 
            }
            
            if (nums.GetLength() == 0) { 
                log->append("Нет элементов для анализа"); 
                return; 
            }

            DefaultRing<int> ring;
            int totalInversions = ring.Zero(); 

            MutableArraySequence<int> sortedRightElements;
            MutableArraySequence<Inversion> inversionList;

            int n = static_cast<int>(nums.GetLength());
            
            for (int i = n - 1; i >= 0; --i) {
                int current = nums[i];

                int low = 0;
                int high = static_cast<int>(sortedRightElements.GetLength());
                int insertIndex = high; 

                while (low < high) {
                    int mid = low + (high - low) / 2;
                    if (sortedRightElements[mid] >= current) {
                        insertIndex = mid;
                        high = mid; 
                    } else {
                        low = mid + 1; 
                    }
                }
                if (low == high) {
                    insertIndex = low;
                }

                for (int j = 0; j < insertIndex; ++j) {
                    Inversion inv;
                    inv.GreaterValue = current;
                    inv.SmallerValue = sortedRightElements[j];
                    inversionList.Append(inv);
                }

                int currentInversions = insertIndex;
                totalInversions = ring.Add(totalInversions, currentInversions);
                
                sortedRightElements.InsertAt(current, static_cast<size_t>(insertIndex));
            }

            if (inversionList.GetLength() == 0) {
                log->append("Инверсий не обнаружено (последовательность отсортирована).");
            } else {
                log->append("\n Инверсии: ");

                int totalPairs = static_cast<int>(inversionList.GetLength());
                for (int i = totalPairs - 1; i >= 0; --i) {
                    Inversion inv = inversionList[i];
                    log->append(QString("(%1 > %2)").arg(inv.GreaterValue).arg(inv.SmallerValue));
                }

                log->append("Всего инверсий: " + QString::number(totalInversions));
            }
        });
    }
};