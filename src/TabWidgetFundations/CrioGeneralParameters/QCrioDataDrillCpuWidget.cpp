#include "QCrioDataDrillCpuWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioDataDrillCpuWidget::QCrioDataDrillCpuWidget(const QString &md5Hash,const QString &executionPath,QWidget *parent)
    : QCrioBaseCpuWidget(md5Hash, executionPath,"CRIO DataDrill CPU Usage", parent)
{
    m_executionPath=executionPath;
}

void QCrioDataDrillCpuWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::dataDrillCpuSignal, this, &QCrioDataDrillCpuWidget::onDataReceived, Qt::QueuedConnection);
}

void QCrioDataDrillCpuWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getDataDrillCPU();
    m_timer->start();
}

void QCrioDataDrillCpuWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
