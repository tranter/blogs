#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QComboBox;
class QLabel;
class QToolButton;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void slotAcceptParameters();
    void slotComboChanged(int ind);

private:
    QComboBox* createCombobox(QStringList& list, QWidget* parent = nullptr);
    void updateUI();
    QToolButton* createButton();

    // Data members.
    QLabel*         m_label;
    QComboBox*      m_combo;
    QToolButton*    m_cancelBtn;
};

#endif // DIALOG_H
