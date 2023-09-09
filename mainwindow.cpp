#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(geometry().width(), geometry().height());

    connect(ui->btnSendMessage1, SIGNAL(clicked()), this, SLOT(slotSendMessage1()));
    connect(ui->btnSendMessage2, SIGNAL(clicked()), this, SLOT(slotSendMessage2()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSendMessage1()
{
    HWND h = (HWND) MainWindow::getInstance()->winId();

    QByteArray data;
    QString s = ui->leMessage->text();
    data += s;

    COPYDATASTRUCT cds{};
    cds.dwData = 1;
    cds.lpData = (PVOID) &data;
    cds.cbData = strlen((char*)cds.lpData);

    SendMessage(h, WM_COPYDATA, (WPARAM)0x00, (LPARAM)&cds);
}

void MainWindow::slotSendMessage2()
{
    HWND h = (HWND) MainWindow::getInstance()->winId();
    SendMessage(h, WM_USER, 0x0B, 0xB0);
}

bool MainWindow::nativeEvent(const QByteArray &type, void *vMsg, long *result)
{
    Q_UNUSED(type);
    MSG *msg = (MSG*)vMsg;

    if (msg->message == WM_USER)
    {
        if ((0x0B == msg->wParam) && (0xB0 == msg->lParam)) {
            ui->teRecvMessage->append(QString("WM_USER: wParam(0x%1), lParam(0x%2)").arg(msg->wParam, 2, 16, QLatin1Char('0')).arg(msg->lParam, 2, 16, QLatin1Char('0')));
        }
    }
    else if (msg->message == WM_COPYDATA) {
        PCOPYDATASTRUCT pcds = reinterpret_cast<PCOPYDATASTRUCT>(msg->lParam);
        QByteArray* data = (QByteArray *)pcds->lpData;
        ui->teRecvMessage->append(QString("WM_COPYDATA: data(%1)").arg(data->constData()));
    }

    // end of if msg->message
    *result = 0; //get rid of obnoxious compiler warning

    return false; // let qt handle the rest
}
