#include <QCalendar>
#include <QDate>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    calendar = nullptr;

    ui->comboBox->insertItem(0, tr("Gregorian"), int(QCalendar::System::Gregorian));
    ui->comboBox->insertItem(1, tr("Julian"), int(QCalendar::System::Julian));
    ui->comboBox->insertItem(2, tr("Milankovic"), int(QCalendar::System::Milankovic));
    ui->comboBox->insertItem(3, tr("Jalali"), int(QCalendar::System::Jalali));
    ui->comboBox->insertItem(4, tr("Islamic Civil"), int(QCalendar::System::IslamicCivil));

    connect(ui->comboBox, qOverload<int>(&QComboBox::activated), this, &MainWindow::setSystem);
    connect(ui->yearSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::update);
    connect(ui->monthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::update);
    connect(ui->daySpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::update);

    ui->comboBox->setCurrentIndex(0);
    setSystem(0);
    ui->yearSpinBox->setValue(QDate::currentDate().year());
    ui->monthSpinBox->setValue(QDate::currentDate().month());
    ui->daySpinBox->setValue(QDate::currentDate().day());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete calendar;
}

void MainWindow::setSystem(int)
{
    delete calendar;
    calendar = new QCalendar(QCalendar::System(ui->comboBox->currentData().toInt()));
    update();
}

void MainWindow::update()
{
    int year = ui->yearSpinBox->value();
    int month = ui->monthSpinBox->value();
    int day = ui->daySpinBox->value();
    ui->label2->setText(calendar->name());
    ui->label4->setText(calendar->hasYearZero() ? tr("Yes") : tr("No"));
    ui->label6->setText(calendar->isGregorian() ? tr("Yes") : tr("No"));
    ui->label8->setText(calendar->isLunar() ? tr("Yes") : tr("No"));
    ui->label10->setText(calendar->isSolar() ? tr("Yes") : tr("No"));
    ui->label12->setText(calendar->isLuniSolar() ? tr("Yes") : tr("No"));
    ui->label14->setText(calendar->isProleptic() ? tr("Yes") : tr("No"));
    ui->label16->setText(QString::number(calendar->minimumDaysInMonth()));
    ui->label18->setText(QString::number(calendar->maximumDaysInMonth()));
    ui->label20->setText(QString::number(calendar->maximumMonthsInYear()));
    ui->label22->setText(calendar->isDateValid(year, month, day) ? tr("Yes") : tr("No"));
    ui->label24->setText(calendar->isLeapYear(year) ? tr("Yes") : tr("No"));
    ui->label26->setText(QString::number(calendar->monthsInYear(year)));
    QDate date = calendar->dateFromParts(year, month, day);
    ui->label28->setText(calendar->monthName(QLocale::system(), month, year)); // or standaloneMonthName()
    ui->label30->setText(date.toString());
}
