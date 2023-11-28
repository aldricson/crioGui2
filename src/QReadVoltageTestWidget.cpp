#include "QReadVoltageTestWidget.h"

QReadVoltageTestWidget::QReadVoltageTestWidget(QtTcpClient *tcpClient, QWidget *parent) : QWidget(parent)
{
    m_tcpClient = tcpClient;
    m_timer = new QTimer(this);
    connect(m_tcpClient,&QtTcpClient::voltageReadedSignal,this,&QReadVoltageTestWidget::onReadVoltageDone,Qt::QueuedConnection);
    setupUi();
}

void QReadVoltageTestWidget::setupUi() {
    m_groupBox = new QGroupBox("Read Voltage Test", this);
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

    connect(m_readOneShotButton, &QPushButton::clicked, this, &QReadVoltageTestWidget::onReadOneShotClicked);
    connect(m_pollButton,        &QPushButton::clicked, this, &QReadVoltageTestWidget::onPollClicked);
}

void QReadVoltageTestWidget::onReadOneShotClicked()
{
    emit logLastRequest("readVoltage on "+m_modulesComboBox->currentText()+m_channelComboBox->currentText());
    m_tcpClient->sendReadVoltageRequest(m_modulesComboBox->currentText(),m_channelComboBox->currentIndex());
}

void QReadVoltageTestWidget::onPollClicked() {
    // Implement the logic for polling
    m_resultLabel->setText("Polling result here");
}

void QReadVoltageTestWidget::onReadVoltageDone(const QString &result)
{
    m_resultLabel->setText(result);
    emit logLastResponse(result);
}

QComboBox *QReadVoltageTestWidget::getChannelComboBox() const
{
    return m_channelComboBox;
}

void QReadVoltageTestWidget::setChannelComboBox(QComboBox *newChannelComboBox)
{
    if (m_channelComboBox == newChannelComboBox)
        return;
    m_channelComboBox = newChannelComboBox;
    emit channelComboBoxChanged();
}

QOScope *QReadVoltageTestWidget::truthOScope() const
{
    return m_truthOScope;
}

void QReadVoltageTestWidget::setTruthOScope(QOScope *newTruthOScope)
{
    if (m_truthOScope == newTruthOScope)
        return;
    m_truthOScope = newTruthOScope;
    m_truthOScope->setVerticalScale(-1.0,10.0);
    m_truthOScope->setHorizontalMaxSamples(60);
    emit truthOScopeChanged();
}

QComboBox *QReadVoltageTestWidget::getModulesComboBox() const
{
    return m_modulesComboBox;
}

void QReadVoltageTestWidget::setModulesComboBox(QComboBox *newModulesComboBox)
{
    if (m_modulesComboBox == newModulesComboBox)
        return;
    m_modulesComboBox = newModulesComboBox;
    emit modulesComboBoxChanged();
}
