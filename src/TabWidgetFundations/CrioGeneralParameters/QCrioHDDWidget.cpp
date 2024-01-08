#include "QCrioHDDWidget.h"
#include <QTimer>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QDonutGauge.h"

QCrioHDDWidget::QCrioHDDWidget(const QString &md5Hash, const QString &executionPath,QWidget *parent)
    : QCrioBaseCpuWidget(md5Hash,executionPath,"Total CRIO HDD Usage", parent)
{
    m_executionPath = executionPath;
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
