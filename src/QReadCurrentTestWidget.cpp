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
    // Call the appropriate function to send the request
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
    connect(m_tcpClient, &QtTcpClient::currentReadedSignal, this, &QBaseAnalogReaderTestWidget::onReadDone, Qt::QueuedConnection);

}
