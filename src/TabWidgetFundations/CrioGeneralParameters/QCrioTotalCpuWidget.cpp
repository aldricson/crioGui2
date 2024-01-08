#include "QCrioTotalCpuWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioTotalCpuWidget::QCrioTotalCpuWidget(const QString &md5Hash, const QString &executionPath, QWidget *parent)
    : QCrioBaseCpuWidget(md5Hash, executionPath,"Total CRIO CPU Usage", parent)
{
    m_executionPath = executionPath;
}

void QCrioTotalCpuWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::totalCPUSignal, this, &QCrioTotalCpuWidget::onDataReceived, Qt::QueuedConnection);
}

void QCrioTotalCpuWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getTotalCPU();
    m_timer->start();
}

void QCrioTotalCpuWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
