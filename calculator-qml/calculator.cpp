#include <stdexcept>

#include <QGuiApplication>
#include <QClipboard>
#include <QDebug>

#include "calculator.h"

// Constructor ///////////////////////////////////////////////////////////////

Calculator::Calculator(QObject *parent)
    : QObject(parent), mRoot(0), mDisplay()
{
    connect(this, SIGNAL(displayChanged()), this, SLOT(onDisplayChanged()));
}

// Destructor ////////////////////////////////////////////////////////////////

Calculator::~Calculator()
{
}

// setDisplay() //////////////////////////////////////////////////////////////

void Calculator::setDisplay(const QString &display)
{
    if (mDisplay != display) {
        mDisplay = display;
        emit displayChanged();
    }
}

// onDisplayChanged() ////////////////////////////////////////////////////////

void Calculator::onDisplayChanged()
{
    // push the new display value to QML
    if (mRoot) mRoot->setProperty("displayText", mDisplay);
}

// setRootObject() ///////////////////////////////////////////////////////////

void Calculator::setRootObject(QObject *root)
{
    // disconnect from previous root
    if (mRoot != 0) mRoot->disconnect(this);

    mRoot = root;

    if (mRoot) {
        // setup new connections
        connect(mRoot, SIGNAL(keyClicked(int)), this, SLOT(keyClicked(int)));
        connect(mRoot, SIGNAL(cutTriggered()), this, SLOT(editCut()));
        connect(mRoot, SIGNAL(copyTriggered()), this, SLOT(editCopy()));
        connect(mRoot, SIGNAL(pasteTriggered()), this, SLOT(editPaste()));

        // set initial state
        clearAll();
    }
}

// buttonClicked() ///////////////////////////////////////////////////////////

void Calculator::keyClicked(int key)
{
    // Note: This method uses immediate execution mode. There is no operator precedence.

    if (!mRoot) return;

    double operand = mDisplay.toDouble();

    switch (key) {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9: // digit pressed
        if ((mDisplay == "0") && key == Qt::Key_0) return;

        if (mExpectingOperand) {
            mDisplay.clear();
            mExpectingOperand = false;
        }

        mDisplay += QString::number(key-Qt::Key_0);
        break;

    case Qt::Key_Period:
        if (mExpectingOperand) {
            mDisplay = "0";
        }

        if (!mDisplay.contains('.')) {
            mDisplay += '.';
        }

        mExpectingOperand = false;
        break;

    case Qt::Key_Minus:
    case Qt::Key_Plus:
    case Qt::Key_Asterisk:
    case Qt::Key_Slash:
    case Qt::Key_Enter:
        if (mPendingOperator != 0) {
            try {
                mResult = calculate(operand, mPendingOperator);
                mDisplay = QString::number(mResult);
            } catch (std::range_error e) {
                clearAll();
                mDisplay = e.what();
                emit displayChanged();
                return;
            }
        } else {
            mResult = operand;
        }
        if (key == Qt::Key_Enter) mPendingOperator = 0;
        else                      mPendingOperator = key;
        mExpectingOperand = true;
        break;

    case Qt::Key_Delete:
        if ((mDisplay == "0") && mExpectingOperand) {
            // already cleared so clear all
            mPendingOperator = 0;
            mResult = 0.0;
        }

        mDisplay = "0";
        mExpectingOperand = true;
        break;

    default:
        return;
    }

    emit displayChanged();
}

// calculate() ///////////////////////////////////////////////////////////////

double Calculator::calculate(double operand, int operation)
{
    switch (operation) {
    case Qt::Key_Minus:
        return mResult - operand;

    case Qt::Key_Plus:
        return mResult + operand;

    case Qt::Key_Asterisk:
        return mResult * operand;

    case Qt::Key_Slash:
        if (qFuzzyCompare(operand + 1.0, 0.0 + 1.0)) throw std::range_error("nan");
        return mResult / operand;

    default:
        return 0.0;
    }
}

// clearAll() ////////////////////////////////////////////////////////////////

void Calculator::clearAll()
{
    if (mRoot) {
        mDisplay = "0";
        mResult = 0.0;
        mExpectingOperand = true;
        mPendingOperator = 0;
        emit displayChanged();
    }
}

// editCut() /////////////////////////////////////////////////////////////////

void Calculator::editCut()
{
    // copy and delete
    if (mRoot) {
        editCopy();
        clearAll();
    }
}

// editCopy() ////////////////////////////////////////////////////////////////

void Calculator::editCopy()
{
    if (mRoot) {
        QGuiApplication::clipboard()->setText(mDisplay);
    }
}

// editPaste() ///////////////////////////////////////////////////////////////

void Calculator::editPaste()
{
    if (mRoot) {
        setDisplay(QGuiApplication::clipboard()->text());
    }
}
