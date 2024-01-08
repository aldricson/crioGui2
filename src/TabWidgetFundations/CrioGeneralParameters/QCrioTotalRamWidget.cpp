#include "QCrioTotalRamWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioTotalRamWidget::QCrioTotalRamWidget(const QString &md5Hash, const QString &executionPath, QWidget *parent)
    : QCrioBaseCpuWidget(md5Hash, executionPath,"Crio Total Ram Usage", parent)
{
    m_executionPath = executionPath;
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
