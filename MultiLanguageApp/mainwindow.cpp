#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "translation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    connect(m_ui->actionSelect_Language,SIGNAL(triggered()), this, SLOT(slotSelectLanguage()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::slotSelectLanguage()
{
    Dialog dlg(this);
    dlg.exec();
}

/* If you dynamically change the language in your application,
 * a changeEvent of type QEvent::LanguageChange is emitted.
 * You have to catch that and reset your text everywhere.
 */
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // Set all text.
        setWindowTitle(qtTrId(ID_APP_TITLE));
        m_ui->menuFile->setTitle(qtTrId(ID_MENU_FILE));
        m_ui->menuSettings->setTitle(qtTrId(ID_MENU_SETTINGS));
        m_ui->actionOpen->setText(qtTrId(ID_MENU_FILE_OPEN));
        m_ui->actionSave->setText(qtTrId(ID_MENU_FILE_SAVE));
        m_ui->actionClose->setText(qtTrId(ID_MENU_FILE_CLOSE));
        m_ui->action_Exit->setText(qtTrId(ID_MENU_FILE_EXIT));
        m_ui->actionSelect_Language->setText(qtTrId(ID_MENU_SETTINGS_SELECT));
    }
    else
    {
        QMainWindow::changeEvent(event);
    }
}
