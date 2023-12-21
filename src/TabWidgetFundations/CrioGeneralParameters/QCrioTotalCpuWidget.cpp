#include "QCrioTotalCpuWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioTotalCpuWidget::QCrioTotalCpuWidget(QWidget *parent)
    : QCrioBaseCpuWidget("Total CRIO CPU Usage", parent)
{
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
