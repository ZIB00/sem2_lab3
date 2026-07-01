#include <QRegularExpressionValidator>
#include <cstddef>
#include <memory>
#include <qobject.h>

#include "AnalysisTab.hpp"
#include "Sequence/ArraySequence.hpp"

AnalysisTab::AnalysisTab(QTextEdit *log, QWidget *parent) : QWidget(parent), log(log) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout *topLayout = new QHBoxLayout();
    numInput = new QLineEdit(this);
    numInput->setText("1 22 3 23 45 456 756 453 32 1");
    start = new QPushButton("Начать", this);

    QRegularExpression val("^[0-9\\s]*$");
    QValidator *validator = new QRegularExpressionValidator(val, this);
    numInput->setValidator(validator);
    
    topLayout->addWidget(numInput, 1);
    topLayout->addWidget(start, 0);
    
    output = new QTextEdit(this);
    output->setReadOnly(true);
    
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(output);

    connect(start, &QPushButton::clicked, this, &AnalysisTab::onAnalysis);
}

bool AnalysisTab::Parse(std::shared_ptr<MutableArraySequence<int>> seq) 
{
    QString input = numInput->text();
    QStringList parts = input.split(' ', Qt::SkipEmptyParts);

    if (parts.isEmpty()) {
        log->append("Поле ввода пустое");
        return false;
    }

    for (const QString &part : parts) {
        bool ok;
        int val = part.toInt(&ok);
        if (!ok) {
            log->append("Ошибка: " + part + " слишком большое число");
            return false;
        }
        seq->Append(val);
    }

    log->append("Получена последовательность: " + parts.join(", "));
    return true;
}

void AnalysisTab::onAnalysis() 
{
    output->clear();

    auto seq = std::make_shared<MutableArraySequence<int>>();
    if(Parse(seq)) {
        size_t count = 0;
        size_t size = seq->GetLength();

        if (size > 1) {
            for (size_t i = 0; i < size - 1; ++i) {
                for (size_t j = i + 1; j < size; ++j) {
                    if (seq->Get(i) > seq->Get(j)) {
                        count++;
                        output->append(QString("%1 > %2").arg(seq->Get(i)).arg(seq->Get(j)));
                    }
                }
            }
        }
        
        output->append(QString("Количество инверсий: %1").arg(count));
        log->append(QString("Посчитано количество инверсий: %1").arg(count));
        return;
    }
}