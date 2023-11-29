#include "QReadCurrentTestWidget.h"

QReadCurrentTestWidget::QReadCurrentTestWidget(QtTcpClient *tcpClient, QWidget *parent)
    : QBaseAnalogReaderTestWidget(tcpClient, "Read Current Test", parent)
{
    // Additional setup specific to QReadCurrentTestWidget if needed
    bindTCPClient();
}

void QReadCurrentTestWidget::onReadOneShotClicked()
{

    emit logLastRequest("readCurrent on " + getModulesComboBox()->currentText() + getChannelComboBox()->currentText());
    m_tcpClient->sendReadCurrentRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadCurrentTestWidget::onPollClicked()
{
    m_inPoll =! m_inPoll;
    m_timer->setInterval(1000);
    m_truthOScope->clearGraph();
    disconnect(m_timer,0);
    m_truthOScope->setVerticalScale(0.0, 0.005);
    m_truthOScope->setHorizontalMaxSamples(60);
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
    m_resultLabel->setText(result);
    if (m_truthOScope) {
        m_truthOScope->addSample(result.toDouble());
    }
}

void QReadCurrentTestWidget::bindTCPClient()
{
    connect(m_tcpClient, &QtTcpClient::currentReadedSignal, this, &QReadCurrentTestWidget::onReadDone, Qt::QueuedConnection);
}
