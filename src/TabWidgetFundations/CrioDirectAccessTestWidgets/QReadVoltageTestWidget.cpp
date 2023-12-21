#include "QReadVoltageTestWidget.h"
#include <QComboBox>
#include <QLabel>

QReadVoltageTestWidget::QReadVoltageTestWidget( QWidget *parent)
    : QBaseAnalogReaderTestWidget( "Read Voltage Test", parent)
{
    this->setObjectName("Voltage reader");
    // Additional setup specific to QReadCurrentTestWidget if needed
    bindTCPClient();
    m_truthOScope->setVerticalScale(-10.0, 10.0);
    m_truthOScope->setHorizontalMaxSamples(60);
}


void QReadVoltageTestWidget::onReadOneShotClicked()
{
    emit logLastRequest("readVoltage on " + getModulesComboBox()->currentText() + getChannelComboBox()->currentText());
    m_client->sendReadVoltageRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadVoltageTestWidget::onReadDone(const QString &result)
{
    if (result.contains("NAK"))
    {
        emit logLastError(result);
        return;
    }
    m_resultLabel->setText(result);
    if (m_truthOScope)
    {
        m_truthOScope->addSample(result.toDouble());
    }
    emit logLastResponse(result);
}

void QReadVoltageTestWidget::bindTCPClient()
{
    connect(m_client, &QtTcpClient::voltageReadedSignal, this, &QReadVoltageTestWidget::onReadDone, Qt::QueuedConnection);

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
