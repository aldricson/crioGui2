#include "QDataDrillHDDWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QDataDrillHDDWidget::QDataDrillHDDWidget(QWidget *parent)
    : QCrioBaseCpuWidget("dataDrill HDD Usage", parent)
{
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
