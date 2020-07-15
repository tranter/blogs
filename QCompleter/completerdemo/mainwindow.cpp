#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file(QStringLiteral(":/words"));
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "Unable to open dictionary file";
        return;
    }

    QTextStream in(&file);
    QString s = in.readAll();
    QStringList list = s.split('\n');
    statusBar()->showMessage(tr("Loaded %1 words from dictionary file.").arg(list.count()), 3000);

    completer = new QCompleter(list, this);
    connect(completer, qOverload<const QString &>(&QCompleter::activated), this, &MainWindow::Activated);
    connect(completer, qOverload<const QString &>(&QCompleter::highlighted), this, &MainWindow::Highlighted);

    ui->lineEdit->setCompleter(completer);

    ui->comboBox->setEditable(true);
    ui->comboBox->addItems(list);
    ui->comboBox->setCompleter(completer);

    connect(ui->completionModeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::UpdateSettings);
    connect(ui->modelSortingComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::UpdateSettings);
    connect(ui->filterModeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::UpdateSettings);
    connect(ui->maxVisibleItemsSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::UpdateSettings);
    connect(ui->caseSensitiveCheckBox, &QCheckBox::stateChanged, this, &MainWindow::UpdateSettings);

    UpdateSettings();
}

MainWindow::~MainWindow()
{
    delete completer;
    delete ui;
}

void MainWindow::UpdateSettings()
{
    if (ui->completionModeComboBox->currentText() == QLatin1String("Popup")) {
        completer->setCompletionMode(QCompleter::PopupCompletion);
    } else if (ui->completionModeComboBox->currentText() == QLatin1String("Inline")) {
        completer->setCompletionMode(QCompleter::InlineCompletion);
    } else if (ui->completionModeComboBox->currentText() == QLatin1String("Unfiltered Popup")) {
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    } else {
        Q_ASSERT(false);
    }

    if (ui->modelSortingComboBox->currentText() == QLatin1String("Unsorted")) {
        completer->setModelSorting(QCompleter::UnsortedModel);
    } else if (ui->modelSortingComboBox->currentText() == QLatin1String("Case Sensitively Sorted")) {
        completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    } else if (ui->modelSortingComboBox->currentText() == QLatin1String("Case Insensitively Sorted")) {
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    } else {
        Q_ASSERT(false);
    }

    if (ui->filterModeComboBox->currentText() == QLatin1String("Starts With")) {
        completer->setFilterMode(Qt::MatchStartsWith);
    } else if (ui->filterModeComboBox->currentText() == QLatin1String("Contains")) {
        completer->setFilterMode(Qt::MatchContains);
    } else if (ui->filterModeComboBox->currentText() == QLatin1String("Ends With")) {
        completer->setFilterMode(Qt::MatchEndsWith);
    } else {
         Q_ASSERT(false);
    }

    completer->setMaxVisibleItems(ui->maxVisibleItemsSpinBox->value());

    if (ui->caseSensitiveCheckBox->isChecked()) {
        completer->setCaseSensitivity(Qt::CaseSensitive);
    } else {
        completer->setCaseSensitivity(Qt::CaseInsensitive);
    }
}

void MainWindow::Highlighted(const QString &text)
{
    statusBar()->showMessage(tr("Highlighted '%1'.").arg(text), 3000);
}

void MainWindow::Activated(const QString &text)
{
    statusBar()->showMessage(tr("Activated '%1'.").arg(text), 3000);
}
