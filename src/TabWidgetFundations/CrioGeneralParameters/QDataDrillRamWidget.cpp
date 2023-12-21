#include "QDataDrillRamWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QDataDrillRamWidget::QDataDrillRamWidget(QWidget *parent)
    : QCrioBaseCpuWidget("dataDrill Ram Usage", parent)
{
}

void QDataDrillRamWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::dataDrillRamUsageSignal, this, &QDataDrillRamWidget::onDataReceived, Qt::QueuedConnection);
}

void QDataDrillRamWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getDataDrillRamUsage();
    m_timer->start();
}

void QDataDrillRamWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
