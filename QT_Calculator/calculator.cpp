#include "calculator.h"
#include "ui_calculator.h"
#include <QRegularExpression>

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
//set all initial function values to false

//Constructor
Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator) //create UI class and assign it to UI member
{
    ui->setupUi(this); //setup UI

    ui->Display->setText(QString::number(calcVal)); //tell display what to display

    QPushButton *numButtons[10]; //create array to store buttons

    //cycle through and assign each number button to numButtons
    for(int i=0;i<10;i++){
        QString butName = "Button" + QString::number(i); //create button name

        numButtons[i] = Calculator::findChild<QPushButton *>(butName); //get buttons by name and add to array

        connect(numButtons[i],SIGNAL(released()),this,
                SLOT(NumPressed())); //when button pressed, call NumPressed()

    }
    connect(ui->Add,SIGNAL(released()),this,
            SLOT(MathButtonPressed())); //when button pressed, call NumPressed()
    connect(ui->Subtract,SIGNAL(released()),this,
            SLOT(MathButtonPressed())); //when button pressed, call NumPressed()
    connect(ui->Multiply,SIGNAL(released()),this,
            SLOT(MathButtonPressed())); //when button pressed, call NumPressed()
    connect(ui->Divide,SIGNAL(released()),this,
            SLOT(MathButtonPressed())); //when button pressed, call NumPressed()

    connect(ui->Equals,SIGNAL(released()),this,
            SLOT(EqualButtonPressed())); //when button pressed, call NumPressed()

    connect(ui->ChangeSign,SIGNAL(released()),this,
            SLOT(ChangeNumberSign())); //when button pressed, call NumPressed()
}

//deconstructor
Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get number on button
    QString butVal = button->text();

    // Get the value in the display
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){

        // calcVal = butVal.toDouble();
        ui->Display->setText(butVal);

    } else {
        // Put the new number to the right of whats there
        QString newVal = displayVal + butVal;

        // Double version of number
        double dblNewVal = newVal.toDouble();

        // calcVal = newVal.toDouble();

        // Set value in display and allow up to 16
        // digits before using exponents
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));

    }
}

void Calculator::MathButtonPressed(){
    //first, reset all previous math button clicks ( we dont want to divide and add at the same time)
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;


    QString displayVal = ui->Display->text(); //Display selected sign
    calcVal = displayVal.toDouble();


    QPushButton *button = (QPushButton *)sender(); //return identity of clicked sign as pointer
    QString butVal = button->text();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    }
    else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    }
    else if(QString::compare(butVal, "-", Qt::CaseInsensitive) == 0){
        subTrigger = true;
    }else{
        const std::string FRIENDLY_ERROR = "Error deciding symbol on button press";
    }
    ui->Display->setText(""); //reset display for next input

}

void Calculator::EqualButtonPressed(){
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();
    if(addTrigger || subTrigger || multTrigger || divTrigger){
        if(addTrigger){
            solution = calcVal + dblDisplayVal;
        }
        else if(subTrigger){
            solution = calcVal - dblDisplayVal;
        }
        else if(multTrigger){
            solution = calcVal * dblDisplayVal;
        }
        else if(divTrigger){
            solution = calcVal / dblDisplayVal;
        }
        else{
            const std::string FRIENDLY_ERROR = "Error on equal button press";
        }
    }
    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign(){
    QString displayVal = ui->Display->text();
    QRegularExpression reg("[-]?[0-9.]*");
    static QRegularExpressionMatch match = reg.match(displayVal);
    if(match.hasMatch()) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}
