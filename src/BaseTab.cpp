#include "BaseTab.hpp"
#include <QString>
#include <qlabel.h>

BaseTab::BaseTab(QTextEdit *log, QWidget *parent) : QWidget(parent), log(log) {
    currentController = nullptr;
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    
    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *queueTypeLabel = new QLabel("Тип элемента: ");
    queueType = new QComboBox(this);
    queueType->addItem("Целое число");
    queueType->addItem("Вещественное число");
    queueType->addItem("Комплексные числа");
    queueType->addItem("Строки/символы");
    queueType->addItem("Функции");
    queueType->addItem("Студенты");
    queueType->addItem("Преподаватели");
    connect(queueType, &QComboBox::currentTextChanged, this, &BaseTab::updateGUI);
    leftLayout->addWidget(queueTypeLabel);
    leftLayout->addWidget(queueType);

    QGridLayout *gridLayout = new QGridLayout();

    label1 = new QLabel("Имя:", this);
    input1 = new QLineEdit(this);

    label2 = new QLabel("Число:", this);
    input2 = new QLineEdit(this);

    gridLayout->addWidget(label1, 0, 0);
    gridLayout->addWidget(input1, 0, 1);
    gridLayout->addWidget(label2, 1, 0);
    gridLayout->addWidget(input2, 1, 1);

    leftLayout->addLayout(gridLayout);

    secondComboBox = new QComboBox(this);
    secondComboBox->addItem("x + 69");
    secondComboBox->addItem("x = 96");
    leftLayout->addWidget(secondComboBox);

    leftLayout->addStretch();


    QVBoxLayout *middleLayout = new QVBoxLayout();
    output = new QTextEdit();
    output->setReadOnly(true);
    middleLayout->addWidget(output);


    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    btnTop = new QPushButton("Top", this);
    rightLayout->addWidget(btnTop);

    btnEmpty = new QPushButton("Empty", this);
    rightLayout->addWidget(btnEmpty);

    btnSize = new QPushButton("Size", this);
    rightLayout->addWidget(btnSize);

    QHBoxLayout *pushLayout = new QHBoxLayout();
    btnPush = new QPushButton("Push", this);
    inputPush = new QLineEdit(this);
    inputPush->setPlaceholderText("value");
    pushLayout->addWidget(btnPush);
    pushLayout->addWidget(inputPush);
    rightLayout->addLayout(pushLayout);

    btnPop = new QPushButton("Pop", this);
    rightLayout->addWidget(btnPop);

    QHBoxLayout *mapLayout = new QHBoxLayout();
    btnMap = new QPushButton("Map", this);
    comboMap = new QComboBox(this);
    comboMap->addItems({"x + 228"});
    mapLayout->addWidget(btnMap);
    mapLayout->addWidget(comboMap);
    rightLayout->addLayout(mapLayout);

    QHBoxLayout *whereLayout = new QHBoxLayout();
    btnWhere = new QPushButton("Where", this);
    comboWhere = new QComboBox(this);
    comboWhere->addItems({"x > 5"});
    whereLayout->addWidget(btnWhere);
    whereLayout->addWidget(comboWhere);
    rightLayout->addLayout(whereLayout);

    QHBoxLayout *reduceLayout = new QHBoxLayout();
    btnReduce = new QPushButton("Reduce", this);
    comboReduce = new QComboBox(this);
    comboReduce->addItems({"Сумма"});
    reduceLayout->addWidget(btnReduce);
    reduceLayout->addWidget(comboReduce);
    rightLayout->addLayout(reduceLayout);

    QHBoxLayout *concatLayout = new QHBoxLayout();
    btnConcat = new QPushButton("Concat", this);
    inputConcat = new QLineEdit(this);
    inputConcat->setPlaceholderText("other queue");
    concatLayout->addWidget(btnConcat);
    concatLayout->addWidget(inputConcat);
    rightLayout->addLayout(concatLayout);

    QHBoxLayout *subseqLayout = new QHBoxLayout();
    btnGetSubsequence = new QPushButton("GetSubsequence", this);
    inputSubseqStart = new QLineEdit(this);
    inputSubseqStart->setPlaceholderText("start index");
    inputSubseqEnd = new QLineEdit(this);
    inputSubseqEnd->setPlaceholderText("end index");
    subseqLayout->addWidget(btnGetSubsequence);
    subseqLayout->addWidget(inputSubseqStart);
    subseqLayout->addWidget(inputSubseqEnd);
    rightLayout->addLayout(subseqLayout);

    QHBoxLayout *isSubseqLayout = new QHBoxLayout();
    btnIsSubsequence = new QPushButton("IsSubsequence", this);
    inputIsSubseq = new QLineEdit(this);
    inputIsSubseq->setPlaceholderText("subQueue");
    isSubseqLayout->addWidget(btnIsSubsequence);
    isSubseqLayout->addWidget(inputIsSubseq);
    rightLayout->addLayout(isSubseqLayout);

    QHBoxLayout *appendLayout = new QHBoxLayout();
    btnAppend = new QPushButton("Append", this);
    inputAppend = new QLineEdit(this);
    inputAppend->setPlaceholderText("other queue");
    appendLayout->addWidget(btnAppend);
    appendLayout->addWidget(inputAppend);
    rightLayout->addLayout(appendLayout);

    QHBoxLayout *splitLayout = new QHBoxLayout();
    btnSplit = new QPushButton("Split", this);
    comboSplit = new QComboBox(this);
    comboSplit->addItems({"x > 5"});
    splitLayout->addWidget(btnSplit);
    splitLayout->addWidget(comboSplit);
    rightLayout->addLayout(splitLayout);

    rightLayout->addStretch();

    connect(btnTop, &QPushButton::clicked, this, &BaseTab::onTop);
    connect(btnEmpty, &QPushButton::clicked, this, &BaseTab::onEmpty);
    connect(btnSize, &QPushButton::clicked, this, &BaseTab::onSize);
    connect(btnPush, &QPushButton::clicked, this, &BaseTab::onPush);
    connect(btnPop, &QPushButton::clicked, this, &BaseTab::onPop);
    connect(btnMap, &QPushButton::clicked, this, &BaseTab::onMap);
    connect(btnWhere, &QPushButton::clicked, this, &BaseTab::onWhere);
    connect(btnReduce, &QPushButton::clicked, this, &BaseTab::onReduce);
    connect(btnConcat, &QPushButton::clicked, this, &BaseTab::onConcat);
    connect(btnGetSubsequence, &QPushButton::clicked, this, &BaseTab::onGetSubsequence);
    connect(btnIsSubsequence, &QPushButton::clicked, this, &BaseTab::onIsSubsequence);
    connect(btnAppend, &QPushButton::clicked, this, &BaseTab::onAppend);
    connect(btnSplit, &QPushButton::clicked, this, &BaseTab::onSplit);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(rightLayout);

    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 2);
    mainLayout->setStretch(2, 2);

    updateGUI();
}

BaseTab::~BaseTab() {
    delete currentController;
}

void BaseTab::updateGUI() {
    label1->setVisible(true);
    input1->setVisible(true);
    label2->setVisible(true);
    input2->setVisible(true);
    secondComboBox->setVisible(false);

    QRegularExpression val1("^[0-9\\-]*$");
    QValidator *validator1 = new QRegularExpressionValidator(val1, this);

    QRegularExpression val2("^[0-9.,\\-]*$");
    QValidator *validator2 = new QRegularExpressionValidator(val2, this);

    QRegularExpression val3("^[A-Za-zА-Яа-яЁё]*$");
    QValidator *validator3 = new QRegularExpressionValidator(val3, this);

    comboMap->clear();
    comboWhere->clear();
    comboReduce->clear();
    comboSplit->clear();
    input1->clear();
    input2->clear();
    btnSplit->setEnabled(true);

    switch (queueType->currentIndex()) {
        case 0:
            label1->setVisible(false);
            input1->setVisible(false);
            
            label2->setText("Число:");
            input2->setValidator(validator1);
            
            comboMap->addItems({"x + 228", "x * 2"});
            comboWhere->addItems({"x > 5", "x % 2 == 0"});
            comboReduce->addItems({"Сумма", "Наибольшее"});
            comboSplit->addItems({"x > 5"});
            break;

        case 1:
            label1->setVisible(false);
            input1->setVisible(false);
            
            label2->setText("Число:");
            input2->setValidator(validator2);
            
            comboMap->addItems({"x + 0.5", "x * 1.5"});
            comboWhere->addItems({"x > 5", "x < 0"});
            comboReduce->addItems({"Сумма", "Наименьшее"});
            comboSplit->addItems({"x > 5"});
            break;
        
        case 2:
            label1->setText("Реальная часть");
            input2->setValidator(validator1);
            
            label2->setText("Мнимая часть");
            input2->setValidator(validator1);
            
            comboMap->addItems({"z + 228", "y * 2"});
            comboWhere->addItems({"z > 5", "z % 2 == 0"});
            comboReduce->addItems({"Сумма", "Наибольшее(по модулю)"});
            comboSplit->addItems({"z > 5"});
            break;
        
        case 3:
            label1->setText("Строка/символ:");
            input1->setValidator(validator3);

            label2->setVisible(false);
            input2->setVisible(false);
            
            comboMap->addItems({"x + zachet", "nezachet"});
            comboWhere->addItems({"len > 5", "len < 5"});
            comboReduce->addItems({"Объединение", "Наименьшее"});
            comboSplit->addItems({"len > 5"});
            break;

        case 4:
            label1->setVisible(false);
            input1->setVisible(false);

            label2->setText("Приоритет:");
            input2->setValidator(validator1);
            secondComboBox->setVisible(true);
            
            
            comboMap->addItems({"zachet", "nezachet"});
            comboWhere->addItems({"zachet", "nezachet"});
            comboReduce->addItems({"zachet", "nezachet"});
            comboSplit->addItems({"zachet", "nezachet"});
            comboSplit->setEnabled(false);
            btnSplit->setEnabled(false);
            break;

        case 5:
            label1->setText("Имя:");
            input1->setValidator(validator3);

            label2->setText("Средний балл.:");
            input2->setValidator(validator2);
            
            comboMap->addItems({"Поставить зачёт", "Сделать отличником"});
            comboWhere->addItems({"Больше 3", "больше 4"});
            comboReduce->addItems({"Лучший балл", "Худший балл"});
            comboSplit->addItems({"Больше 3"});
            break;

        case 6:
            label1->setText("Имя:");
            input1->setValidator(validator3);

            label2->setText("Зарплата:");
            input2->setValidator(validator2);
            
            comboMap->addItems({"Премировать", "Обогатить"});
            comboWhere->addItems({"Больше 100 тыс.", "Больше 200 тыс."});
            comboReduce->addItems({"Обобщить", "Наибольшая"});
            comboSplit->addItems({"Больше 100 тыс."});
            break;

        default:
            label1->setText("Поле 1:");
            label2->setText("Поле 2:");
            break;
    }

    delete currentController;

    switch (queueType->currentIndex()) {
        case 0: 
            currentController = new QueueController<int>(); 
            break;

        case 1: 
            currentController = new QueueController<double>(); 
            break;

        case 2: 
            currentController = new QueueController<MyComplex<int>>(); 
            break;

        case 3: 
            currentController = new QueueController<QString>(); 
            break;

        case 4: 
            currentController = new QueueController<FuncType>(); 
            break;

        case 5: 
            currentController = new QueueController<Student>(); 
            break;

        case 6: 
            currentController = new QueueController<Teacher>(); 
            break;

        default: 
            currentController = nullptr; 
            break;
    }
}

void BaseTab::onTop() {
    if (!currentController) return;
    log->append(currentController->Top());
    updateOutput();
}

void BaseTab::onEmpty() {
    if (!currentController) return;
    log->append("Пусто? " + currentController->IsEmpty());
    updateOutput();
}

void BaseTab::onSize() {
    if (!currentController) return;
    log->append("Размер: " + currentController->Size());
    updateOutput();
}

void BaseTab::onPush() {
    if (!currentController) return;
    currentController->Push(input1->text(), input2->text(), secondComboBox->currentText());
    log->append("Элемент добавлен");
    updateOutput();
}

void BaseTab::onPop() {
    if (!currentController) return;
    currentController->Pop();
    log->append("Вызван метод Pop");
    updateOutput();
}

void BaseTab::onMap() {
    if (!currentController) return;
    log->append(currentController->Map(comboMap->currentText()));
    updateOutput();
}

void BaseTab::onWhere() {
    if (!currentController) return;
    log->append(currentController->Where(comboWhere->currentText()));
    updateOutput();
}

void BaseTab::onReduce() {
    if (!currentController) return;
    log->append(currentController->Reduce(comboReduce->currentText()));
    updateOutput();
}

void BaseTab::onConcat() {
    QString val = inputConcat->text();
    log->append("Вызван метод Concat() с аргументом: " + val);
    updateOutput();
}

void BaseTab::onGetSubsequence() {
    if (!currentController) return;
    log->append(currentController->GetSubsequence(inputSubseqStart->text(), inputSubseqEnd->text()));
    updateOutput();
}

void BaseTab::onIsSubsequence() {
    QString val = inputIsSubseq->text();
    log->append("Вызван метод IsSubsequence() с аргументом: " + val);
    updateOutput();
}

void BaseTab::onAppend() {
    QString val = inputAppend->text();
    log->append("Вызван метод Append() с аргументом: " + val);
    updateOutput();
}

void BaseTab::onSplit() {
    if (!currentController) return;
    log->append(currentController->Split(comboSplit->currentText()));
    updateOutput();
}

void BaseTab::updateOutput() {
    if (!currentController) {
        output->clear();
        return;
    }
    
    output->setText(currentController->ToString());
}