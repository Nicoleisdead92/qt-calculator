#include "calculator.h"
#include "./ui_calculator.h"
#include "qobjectdefs.h"

double calcVal = 0.0;
double memory = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool point = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for (int i = 0; i < 10; ++i) {
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Equal, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->Clear, SIGNAL(released()), this, SLOT(Clear()));
    connect(ui->ClearAll, SIGNAL(released()), this, SLOT(ClearAll()));
    connect(ui->Point, SIGNAL(released()), this, SLOT(Point()));
    connect(ui->MemAdd, SIGNAL(released()), this, SLOT(MemAdd()));
    connect(ui->MemClear, SIGNAL(released()), this, SLOT(MemClear()));
    connect(ui->MemRead, SIGNAL(released()), this, SLOT(MemRead()));
    connect(ui->MemSubstract, SIGNAL(released()), this, SLOT(MemSubstract()));
    connect(ui->Backspace, SIGNAL(released()), this, SLOT(Backspace()));


}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->Display->text();
    if(displayVal == "0") {
        ui->Display->setText(butVal);
    } else if (butVal == "0") {
        ui->Display->setText(displayVal + butVal);
    } else {
        QString newVal = displayVal + butVal;
        double dbNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dbNewVal,'g',16));
    }
}

void Calculator::MathButtonPressed() {
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if(QString::compare(butVal,"/",Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    } else if (QString::compare(butVal,"*",Qt::CaseInsensitive) == 0) {
        multTrigger = true;
    } else if (QString::compare(butVal,"+",Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    } else {
        subTrigger = true;
    }
    ui->Display->setText("");
    if (point) point = false;
}

void Calculator::EqualButtonPressed() {
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dbDisplayVal = displayVal.toDouble();
    if (divTrigger || multTrigger || addTrigger || subTrigger) {
        if (addTrigger) {
            solution = calcVal + dbDisplayVal;
        } else if (subTrigger) {
            solution = calcVal - dbDisplayVal;
        } else if (multTrigger) {
            solution = calcVal * dbDisplayVal;
        } else {
            solution = calcVal / dbDisplayVal;
        }
    }
    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign() {
    QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9]*");
    if (reg.exactMatch(displayVal)) {
        double dbDisplayVal = displayVal.toDouble();
        double dbDisplayValSign = -1 * dbDisplayVal;
        ui->Display->setText(QString::number(dbDisplayValSign));
    }
}

void Calculator::Clear() {
    ui->Display->setText("0");
    if (point) point = false;
}

void Calculator::ClearAll() {
    calcVal = 0.0;
    ui->Display->setText("0");
    if (point) point = false;
}

void Calculator::Point() {
    if (!point) {
        point = true;
        ui->Display->setText(ui->Display->text()+".");
    }
}

void Calculator::MemAdd() {
    QString displayVal = ui->Display->text();
    memory += displayVal.toDouble();
}

void Calculator::MemSubstract() {
    QString displayVal = ui->Display->text();
    memory -= displayVal.toDouble();
}

void Calculator::MemClear() {
    memory = 0.0;
}

void Calculator::MemRead() {
    ui->Display->setText(QString::number(memory));
}

void Calculator::Backspace() {
    QString displayVal = ui->Display->text();
    int size = displayVal.size();
    displayVal.resize(size-1);
    ui->Display->setText(displayVal);
}
