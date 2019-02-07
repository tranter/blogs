#include <QtGui>

#include "gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(okButton, &QPushButton::clicked, this, &GoToCellDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &GoToCellDialog::reject);
    connect(lineEdit, &QLineEdit::textChanged, this, &GoToCellDialog::textChanged);
}

void GoToCellDialog::textChanged()
{
    okButton->setEnabled(lineEdit->hasAcceptableInput());
}
