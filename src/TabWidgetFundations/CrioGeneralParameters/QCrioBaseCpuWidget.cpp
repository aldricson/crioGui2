#include "QCrioBaseCpuWidget.h"
#include <QGridLayout >
#include <QGroupBox   >
#include <QTimer      >
#include "BasicWidgets/QDonutGauge.h"
#include "NetWorking/QSSHCommand.h"

QCrioBaseCpuWidget::QCrioBaseCpuWidget(const QString &md5Hash, const QString &execution_path , const QString &title, QWidget *parent)
{
    m_executionPath = execution_path;
    m_timer = new QTimer(this);
    m_timer->setInterval(6000);
    connect (m_timer, &QTimer::timeout, this, &QCrioBaseCpuWidget::onTimeOut, Qt::QueuedConnection);
    m_title = title;
    setUpUi();
    m_sshCommand = new QSSHCommand(md5Hash,m_executionPath,this);
}

const QString &QCrioBaseCpuWidget::hostName() const
{
    return m_hostName;
}

void QCrioBaseCpuWidget::setHostName(const QString &newHostName)
{
    m_hostName = newHostName;
    m_sshCommand->setHostName(m_hostName);
    Q_EMIT hostNameChanged();
}

int QCrioBaseCpuWidget::port() const
{
    return m_port;
}

void QCrioBaseCpuWidget::setPort(int newPort)
{
    m_sshCommand->setPortNum(newPort);
    m_port = newPort;
    Q_EMIT portChanged();
}

const QString &QCrioBaseCpuWidget::login() const
{
    return m_login;
}

void QCrioBaseCpuWidget::setLogin(const QString &newLogin)
{

    m_sshCommand->setUserName(newLogin);
    m_login = newLogin;
    Q_EMIT loginChanged();
}

const QString &QCrioBaseCpuWidget::passWord() const
{
    return m_passWord;
}

void QCrioBaseCpuWidget::setPassWord(const QString &newPassWord)
{
    m_sshCommand->setPassword(newPassWord);
    m_passWord = newPassWord;
    Q_EMIT passWordChanged();
}

void QCrioBaseCpuWidget::setUpUi()
{
    m_container      = new QGroupBox(m_title,this);
    m_donutGauge  = new QDonutGauge(this);
    m_donutGauge->setRange(0,100);
    m_donutGauge->setStartingPoint(QDonutGauge::StartingPoint::Left);
    m_donutGauge->setSuffix(" %");
    m_donutGauge->setValue(0);
    m_donutGauge->setClockwiseRotation(false);
    QHBoxLayout *gbLayout = new QHBoxLayout(m_container);
    gbLayout->addWidget(m_donutGauge);
    m_layout    = new QGridLayout(this);
    m_layout->addWidget(m_container);
    this->setFixedSize(200,200);

}

void QCrioBaseCpuWidget::start()
{
    m_timer->start();
}

void QCrioBaseCpuWidget::stop()
{
   m_timer->stop();
}



