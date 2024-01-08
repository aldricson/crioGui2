#include "QGlobalParametersWidget.h"
#include <QGridLayout>
#include <QTimer>
#include "TabWidgetFundations/CrioGeneralParameters/QCrioTotalCpuWidget.h"
#include "TabWidgetFundations/CrioGeneralParameters/QCrioDataDrillCpuWidget.h"
#include "TabWidgetFundations/CrioGeneralParameters/QCrioHDDWidget.h"
#include "TabWidgetFundations/CrioGeneralParameters/QDataDrillHDDWidget.h"
#include "TabWidgetFundations/CrioGeneralParameters/QCrioTotalRamWidget.h"
#include "TabWidgetFundations/CrioGeneralParameters/QDataDrillRamWidget.h"
#include "BasicWidgets/QMultiLineTextVisualizer.h"
#include "NetWorking/QSSHCommand.h"

QGlobalParametersWidget::QGlobalParametersWidget(const QString &md5Hash,const QString &executionPath, QWidget *parent)
    : QWidget{parent}
{
    m_executionPath     = executionPath;
    m_sshMd5Hash        = md5Hash;
    m_globalStatCommand = new QSSHCommand(m_sshMd5Hash,m_executionPath,this);
    m_globalStatTimer   = new QTimer     (this);
    m_globalStatTimer   -> setInterval(20000);
    connect (m_globalStatCommand, &QSSHCommand::globalStatSignal, this , &QGlobalParametersWidget::onGlobalStats, Qt::QueuedConnection);
    connect (m_globalStatTimer, &QTimer::timeout, this , &QGlobalParametersWidget::onAskStats, Qt::QueuedConnection);
    setUpUi();
    setUpLayout();

}

void QGlobalParametersWidget::setUpUi()
{
    m_layout             = new QGridLayout              (this);
    m_totalCPUWidget     = new QCrioTotalCpuWidget      (m_sshMd5Hash, m_executionPath,this);
    m_dataDrillCpuWidget = new QCrioDataDrillCpuWidget  (m_sshMd5Hash, m_executionPath,this);
    m_crioHDDWidget      = new QCrioHDDWidget           (m_sshMd5Hash, m_executionPath,this);
    m_dataDrillHDDWidget = new QDataDrillHDDWidget      (m_sshMd5Hash, m_executionPath,this);
    m_crioTotalRamWidget = new QCrioTotalRamWidget      (m_sshMd5Hash, m_executionPath,this);
    m_dataDrillRamWidget = new QDataDrillRamWidget      (m_sshMd5Hash, m_executionPath,this);
    m_globalStatOutput   = new QMultiLineTextVisualizer (this);

    m_totalCPUWidget     -> connectSSHSignals();
    m_dataDrillCpuWidget -> connectSSHSignals();
    m_crioHDDWidget      -> connectSSHSignals();
    m_dataDrillHDDWidget -> connectSSHSignals();
    m_crioTotalRamWidget -> connectSSHSignals();
    m_dataDrillRamWidget -> connectSSHSignals();
}


void QGlobalParametersWidget::setUpLayout()
{
    m_layout->addWidget(m_totalCPUWidget     , 0,0,1,1);
    m_layout->addWidget(m_dataDrillCpuWidget , 1,0,1,1);
    m_layout->addWidget(m_crioHDDWidget      , 0,1,1,1);
    m_layout->addWidget(m_dataDrillHDDWidget , 1,1,1,1);
    m_layout->addWidget(m_crioTotalRamWidget , 0,2,1,1);
    m_layout->addWidget(m_dataDrillRamWidget , 1,2,1,1);
    m_layout->addWidget(m_globalStatOutput   , 0,3,2,1);
}

void QGlobalParametersWidget::onGlobalStats(const QString &output, const QString &lastCommand)
{
  m_globalStatOutput->clear();
  m_globalStatOutput->addLastOutput(output);
}

void QGlobalParametersWidget::onAskStats()
{
    m_globalStatTimer->stop();
    m_globalStatCommand->getCrioGlobalStats();
    m_globalStatTimer->start();
}

int QGlobalParametersWidget::sshPort() const
{
    return m_sshPort;
}

void QGlobalParametersWidget::setSshPort(int newSshPort)
{
    m_sshPort = newSshPort;
    m_totalCPUWidget     -> setPort    (m_sshPort);
    m_dataDrillCpuWidget -> setPort    (m_sshPort);
    m_crioHDDWidget      -> setPort    (m_sshPort);
    m_dataDrillHDDWidget -> setPort    (m_sshPort);
    m_crioTotalRamWidget -> setPort    (m_sshPort);
    m_dataDrillRamWidget -> setPort    (m_sshPort);
    m_globalStatCommand  -> setPortNum (m_sshPort);
    Q_EMIT  sshPortChanged();
}

void QGlobalParametersWidget::start()
{
    m_globalStatCommand->getCrioGlobalStats();
    m_totalCPUWidget     -> start();
    m_dataDrillCpuWidget -> start();
    m_dataDrillHDDWidget -> start();
    m_crioHDDWidget      -> start();
    m_crioTotalRamWidget -> start();
    m_dataDrillRamWidget -> start();
    m_globalStatTimer    -> start();
}

void QGlobalParametersWidget::stop()
{
    m_totalCPUWidget     -> stop();
    m_dataDrillCpuWidget -> stop();
    m_dataDrillHDDWidget -> stop();
    m_crioHDDWidget      -> stop();
    m_crioTotalRamWidget -> stop();
    m_dataDrillRamWidget -> stop();
    m_globalStatTimer    -> stop();
}

const QString &QGlobalParametersWidget::password() const
{
    return m_password;
}

void QGlobalParametersWidget::setPassword(const QString &newPassword)
{
    m_password = newPassword;
    m_totalCPUWidget     -> setPassWord(m_password);
    m_dataDrillCpuWidget -> setPassWord(m_password);
    m_crioHDDWidget      -> setPassWord(m_password);
    m_dataDrillHDDWidget -> setPassWord(m_password);
    m_crioTotalRamWidget -> setPassWord(m_password);
    m_dataDrillRamWidget -> setPassWord(m_password);
    m_globalStatCommand  -> setPassword(m_password);
    Q_EMIT  passwordChanged();
}

const QString &QGlobalParametersWidget::userName() const
{
    return m_userName;
}

void QGlobalParametersWidget::setUserName(const QString &newUserName)
{
    m_userName = newUserName;
    m_totalCPUWidget     -> setLogin    (m_userName);
    m_dataDrillCpuWidget -> setLogin    (m_userName);
    m_crioHDDWidget      -> setLogin    (m_userName);
    m_dataDrillHDDWidget -> setLogin    (m_userName);
    m_crioTotalRamWidget -> setLogin    (m_userName);
    m_dataDrillRamWidget -> setLogin    (m_userName);
    m_globalStatCommand  -> setUserName (m_userName);
    Q_EMIT  userNameChanged();
}

const QString &QGlobalParametersWidget::hostName() const
{
    return m_hostName;
}

void QGlobalParametersWidget::setHostName(const QString &newHostName)
{
    m_hostName = newHostName;
    m_totalCPUWidget     -> setHostName(m_hostName);
    m_dataDrillCpuWidget -> setHostName(m_hostName);
    m_crioHDDWidget      -> setHostName(m_hostName);
    m_dataDrillHDDWidget -> setHostName(m_hostName);
    m_crioTotalRamWidget -> setHostName(m_hostName);
    m_dataDrillRamWidget -> setHostName(m_hostName);
    m_globalStatCommand  -> setHostName(m_hostName);
    Q_EMIT  hostNameChanged();
}




