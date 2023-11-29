#include "QReadVoltageTestWidget.h"


QReadVoltageTestWidget::QReadVoltageTestWidget(QtTcpClient *tcpClient, QWidget *parent)
    : QBaseAnalogReaderTestWidget(tcpClient, "Read Voltage Test", parent)
{
    // Additional setup specific to QReadCurrentTestWidget if needed
    bindTCPClient();
}

void QReadVoltageTestWidget::onReadOneShotClicked()
{
    emit logLastRequest("readVoltage on " + getModulesComboBox()->currentText() + getChannelComboBox()->currentText());
         m_tcpClient->sendReadVoltageRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadVoltageTestWidget::onReadDone(const QString &result)
{

    m_resultLabel->setText(result);
    if (m_truthOScope)
    {
        m_truthOScope->addSample(result.toDouble());
    }
}

void QReadVoltageTestWidget::bindTCPClient()
{
    connect(m_tcpClient, &QtTcpClient::voltageReadedSignal, this, &QReadVoltageTestWidget::onReadDone, Qt::QueuedConnection);

}

void QReadVoltageTestWidget::onPollClicked()
{
    m_inPoll =! m_inPoll;
    m_timer->setInterval(1000);
    m_truthOScope->clearGraph();
    disconnect(m_timer,0);
    connect(m_timer, &QTimer::timeout, this, &QReadVoltageTestWidget::onReadOneShotClicked);
    if (m_inPoll)
    {
        m_timer->start();
    }
    else
    {
        m_timer->stop();
    }
}
