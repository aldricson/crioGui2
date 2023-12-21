#include "QCrioDataDrillCpuWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioDataDrillCpuWidget::QCrioDataDrillCpuWidget(QWidget *parent)
    : QCrioBaseCpuWidget("CRIO DataDrill CPU Usage", parent)
{
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
