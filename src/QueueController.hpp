#pragma once

#include <QString>

class IQueueController {
public:
    virtual ~IQueueController() = default;

    virtual QString Top() = 0;
    virtual QString IsEmpty() = 0;
    virtual QString Size() = 0;
    virtual void Push(const QString& input1, const QString& input2, const QString& secondCombo) = 0;
    virtual void Pop() = 0;
    virtual QString Map(const QString& funcName) = 0;
    virtual QString Where(const QString& predicateName) = 0;
    virtual QString Reduce(const QString& funcName) = 0;
    virtual QString GetSubsequence(const QString& startIndex, const QString& endIndex) = 0;
    virtual QString Split(const QString& predicateName) = 0;

    virtual QString ToString() = 0;
};
