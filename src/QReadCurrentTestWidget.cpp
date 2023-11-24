#include "QReadCurrentTestWidget.h"

QReadCurrentTestWidget::QReadCurrentTestWidget(QtTcpClient *tcpClient, QWidget *parent) : QWidget(parent)
{
    m_tcpClient = tcpClient;
    connect(m_tcpClient,&QtTcpClient::currentReadedSignal,this,&QReadCurrentTestWidget::onReadCurrentDone,Qt::QueuedConnection);
    setupUi();
}

void QReadCurrentTestWidget::setupUi() {
    m_groupBox = new QGroupBox("Read Current Test", this);
    m_moduleLabel = new QLabel("Choose module", m_groupBox);
    m_channelLabel = new QLabel("Choose channel", m_groupBox);
    m_modulesComboBox = new QComboBox(m_groupBox);
    m_channelComboBox = new QComboBox(m_groupBox);
    m_readOneShotButton = new QPushButton("Read One Shot", m_groupBox);
    m_pollButton = new QPushButton("Poll", m_groupBox);
    m_resultLabel = new QLabel(m_groupBox);
    m_resultLabel->setText("Nan");

    QGridLayout *layout = new QGridLayout(m_groupBox);
    layout->addWidget(m_moduleLabel       , 0, 0, 1, 1,Qt::AlignCenter); layout->addWidget(m_channelLabel    , 0, 1, 1, 1,Qt::AlignCenter);
    layout->addWidget(m_modulesComboBox   , 1, 0, 1, 1,Qt::AlignCenter); layout->addWidget(m_channelComboBox , 1, 1, 1, 1,Qt::AlignCenter);
    layout->addWidget(m_resultLabel       , 2, 0, 1, 2,Qt::AlignCenter);
    layout->addWidget(m_readOneShotButton , 3, 0, 1, 1,Qt::AlignCenter); layout->addWidget(m_pollButton      , 3, 1, 1, 1,Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_groupBox);

    connect(m_readOneShotButton, &QPushButton::clicked, this, &QReadCurrentTestWidget::onReadOneShotClicked);
    connect(m_pollButton,        &QPushButton::clicked, this, &QReadCurrentTestWidget::onPollClicked);
}

void QReadCurrentTestWidget::onReadOneShotClicked()
{
    emit logLastRequest("readCurrent on "+m_modulesComboBox->currentText()+m_channelComboBox->currentText());
    m_tcpClient->sendReadCurrentRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadCurrentTestWidget::onPollClicked() {
    // Implement the logic for polling
    m_resultLabel->setText("Polling result here");
}

void QReadCurrentTestWidget::onReadCurrentDone(const QString &result)
{
    m_resultLabel->setText(result);
    emit logLastResponse(result);
}

QComboBox *QReadCurrentTestWidget::getChannelComboBox() const
{
    return m_channelComboBox;
}

void QReadCurrentTestWidget::setChannelComboBox(QComboBox *newChannelComboBox)
{
    if (m_channelComboBox == newChannelComboBox)
        return;
    m_channelComboBox = newChannelComboBox;
    emit channelComboBoxChanged();
}

QComboBox *QReadCurrentTestWidget::getModulesComboBox() const
{
    return m_modulesComboBox;
}

void QReadCurrentTestWidget::setModulesComboBox(QComboBox *newModulesComboBox)
{
    if (m_modulesComboBox == newModulesComboBox)
        return;
    m_modulesComboBox = newModulesComboBox;
    emit modulesComboBoxChanged();
}
