#include "QCrioHDDWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioHDDWidget::QCrioHDDWidget(QWidget *parent)
    : QCrioBaseCpuWidget("Total CRIO HDD Usage", parent)
{
}

void QCrioHDDWidget::connectSSHSignals()
{
    connect (m_sshCommand, &QSSHCommand::dataDrillTotalHDDSignal, this, &QCrioHDDWidget::onDataReceived, Qt::QueuedConnection);
}

void QCrioHDDWidget::onTimeOut()
{
    m_timer->stop();
    m_sshCommand->getCrioHDDUsage();
    m_timer->start();
}

void QCrioHDDWidget::onDataReceived(const QString &output, const QString &lastCommand)
{
    this->m_donutGauge->setValue(output.toDouble());
}
