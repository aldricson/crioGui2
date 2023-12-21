#include "QCrioTotalRamWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioTotalRamWidget::QCrioTotalRamWidget(QWidget *parent)
    : QCrioBaseCpuWidget("Crio Total Ram Usage", parent)
{
}

void QCrioTotalRamWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::dataDrillTotalRamSignal, this, &QCrioTotalRamWidget::onDataReceived, Qt::QueuedConnection);
}

void QCrioTotalRamWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getCrioTotalRamUsage();
    m_timer->start();
}

void QCrioTotalRamWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
