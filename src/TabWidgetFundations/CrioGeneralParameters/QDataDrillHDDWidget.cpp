#include "QDataDrillHDDWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QDataDrillHDDWidget::QDataDrillHDDWidget(const QString &md5Hash,const QString &executionPath, QWidget *parent)
    : QCrioBaseCpuWidget(md5Hash,executionPath,"dataDrill HDD Usage", parent)
{
    m_executionPath = executionPath;
}

void QDataDrillHDDWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::dataDrillHDDSignal, this, &QDataDrillHDDWidget::onDataReceived, Qt::QueuedConnection);
}

void QDataDrillHDDWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getDataDrillHDDUsage();
    m_timer->start();
}

void QDataDrillHDDWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
