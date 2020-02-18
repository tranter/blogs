#include <QRegularExpression>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create validators.
    intValidator = new QIntValidator(this);
    doubleValidator = new QDoubleValidator(this);
    regularExpressionValidator = new QRegularExpressionValidator(this);

    // Initialize.
    setValidator();

    // Connect the buttons.
    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->clearButton, &QPushButton::clicked, [=]() { ui->inputLineEdit->clear(); });
    connect(ui->clearButton, &QPushButton::clicked, [=]() { ui->maskLineEdit->clear(); });

    // Update input mask when it changes.
    connect(ui->maskLineEdit, &QLineEdit::textChanged, [=](const QString &newMask) { ui->inputLineEdit->setInputMask(newMask); });

    // Handle validator type combo box changing.
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    connect(ui->validatorTypeComboBox, qOverload<int, const QString &>(&QComboBox::currentIndexChanged), this, &MainWindow::setValidator);
#else
    connect(ui->validatorTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::setValidator);
#endif

    // If any settings change, update the validator.
    connect(ui->intTopSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::setValidator);
    connect(ui->intBottomSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::setValidator);
    connect(ui->doubleTopSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &MainWindow::setValidator);
    connect(ui->doubleBottomSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &MainWindow::setValidator);
    connect(ui->decimalsSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::setValidator);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    connect(ui->notationComboBox, qOverload<int, const QString &>(&QComboBox::currentIndexChanged), this, &MainWindow::setValidator);
#else
    connect(ui->notationComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::setValidator);
#endif
    connect(ui->regularExpressionLineEdit, &QLineEdit::textChanged, this, &MainWindow::setValidator);

    // Update status bar when line edit changes.
    connect(ui->inputLineEdit, &QLineEdit::textChanged, this, &MainWindow::updateStatusBar);

    // Check for validity when regular expression changes.
    connect(ui->regularExpressionLineEdit, &QLineEdit::textChanged, this, &MainWindow::checkRegularExpression);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setValidator()
{
    // Set stacked widget to appropriate index from combo box.
    ui->stackedWidget->setCurrentIndex(ui->validatorTypeComboBox->currentIndex());

    // Set the validator to the correct type.
    switch (ui->validatorTypeComboBox->currentIndex()) {

    case 0: // Integer
        intValidator->setRange(ui->intBottomSpinBox->value(), ui->intTopSpinBox->value());
        ui->inputLineEdit->setValidator(intValidator);
        break;

    case 1: // Double
        doubleValidator->setRange(ui->doubleBottomSpinBox->value(), ui->doubleTopSpinBox->value());
        doubleValidator->setDecimals(ui->decimalsSpinBox->value());
        doubleValidator->setNotation(static_cast<QDoubleValidator::Notation>(ui->notationComboBox->currentIndex()));
        ui->inputLineEdit->setValidator(doubleValidator);
        break;

    case 2: // Regular expression
        regularExpressionValidator->setRegularExpression(QRegularExpression(ui->regularExpressionLineEdit->text()));
        ui->inputLineEdit->setValidator(regularExpressionValidator);
        break;
    }
}

void MainWindow::updateStatusBar()
{
    if (ui->inputLineEdit->hasAcceptableInput()) {
        this->statusBar()->showMessage(tr("Input is acceptable"));
    } else {
        this->statusBar()->showMessage(tr("Input is not acceptable"));
    }
}

void MainWindow::checkRegularExpression()
{
    QRegularExpression regexp(ui->regularExpressionLineEdit->text());

    if (!regexp.isValid()) {
        this->statusBar()->showMessage(tr("Regular expression is not valid"));
    } else {
        this->statusBar()->clearMessage();
    }
}
