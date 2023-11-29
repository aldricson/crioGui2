#include "QReadCurrentTestWidget.h"

QReadCurrentTestWidget::QReadCurrentTestWidget( QWidget *parent)
    : QBaseAnalogReaderTestWidget("Read Current Test", parent)
{
    // Additional setup specific to QReadCurrentTestWidget if needed
    bindTCPClient();
    m_truthOScope->setVerticalScale(0.0, 0.005);
    m_truthOScope->setHorizontalMaxSamples(60);
}

void QReadCurrentTestWidget::onReadOneShotClicked()
{

    emit logLastRequest("readCurrent on " + getModulesComboBox()->currentText() + getChannelComboBox()->currentText());
    m_client->sendReadCurrentRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadCurrentTestWidget::onPollClicked()
{
    m_inPoll =! m_inPoll;
    m_timer->setInterval(1000);
    m_truthOScope->clearGraph();
    disconnect(m_timer,0);
    connect(m_timer, &QTimer::timeout, this, &QReadCurrentTestWidget::onReadOneShotClicked);
    if (m_inPoll)
    {
        m_timer->start();
    }
    else
    {
        m_timer->stop();
    }
}

void QReadCurrentTestWidget::onReadDone(const QString &result)
{
    if (result.contains("NAK"))
    {
        emit logLastError(result);
        return;
    }
    m_resultLabel->setText(result);
    if (m_truthOScope) {
        m_truthOScope->addSample(result.toDouble());
    }
    emit logLastResponse(result);
}

void QReadCurrentTestWidget::bindTCPClient()
{
    connect(m_client, &QtTcpClient::currentReadedSignal, this, &QReadCurrentTestWidget::onReadDone, Qt::QueuedConnection);
}
