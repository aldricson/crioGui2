// QBaseAnalogReaderTestWidget.cpp
#include "QBaseAnalogReaderTestWidget.h"

QBaseAnalogReaderTestWidget::QBaseAnalogReaderTestWidget(const QString& groupBoxTitle, QWidget *parent)
    : QWidget(parent)
{
    m_client = new QtTcpClient(this);
    m_timer = new QTimer(this);
    m_truthOScope = new QOScope(this);
    m_truthOScope->setFixedWidth(100);
    setupUi(groupBoxTitle);
}

QComboBox *QBaseAnalogReaderTestWidget::getModulesComboBox() const
{
    return m_modulesComboBox;
}

void QBaseAnalogReaderTestWidget::setModulesComboBox(QComboBox *newModulesComboBox)
{
    if (m_modulesComboBox == newModulesComboBox)
        return;
    m_modulesComboBox = newModulesComboBox;
    emit modulesComboBoxChanged();
}



void QBaseAnalogReaderTestWidget::setupUi(const QString& groupBoxTitle)
{
    m_groupBox = new QGroupBox(groupBoxTitle, this);
    m_moduleLabel = new QLabel("Choose module", m_groupBox);
    m_channelLabel = new QLabel("Choose channel", m_groupBox);
    m_modulesComboBox = new QComboBox(m_groupBox);
    m_channelComboBox = new QComboBox(m_groupBox);
    m_readOneShotButton = new QPushButton("Read One Shot", m_groupBox);
    m_pollButton = new QPushButton("Poll", m_groupBox);
    m_resultLabel = new QLabel(m_groupBox);
    m_resultLabel->setText("Nan");

    QGridLayout *layout = new QGridLayout(m_groupBox);
    layout->addWidget(m_moduleLabel, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_channelLabel, 0, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_modulesComboBox, 1, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_channelComboBox, 1, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_resultLabel, 2, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(m_readOneShotButton, 3, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_pollButton, 3, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_truthOScope, 0,2,4,1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_groupBox);

    connect(m_readOneShotButton, &QPushButton::clicked, this, &QBaseAnalogReaderTestWidget::onReadOneShotClicked);
    connect(m_pollButton, &QPushButton::clicked, this, &QBaseAnalogReaderTestWidget::onPollClicked);
}

QComboBox *QBaseAnalogReaderTestWidget::getChannelComboBox() const
{
    return m_channelComboBox;
}

void QBaseAnalogReaderTestWidget::setChannelComboBox(QComboBox *newChannelComboBox)
{
    if (m_channelComboBox == newChannelComboBox)
        return;
    m_channelComboBox = newChannelComboBox;
    emit channelComboBoxChanged();
}

QOScope *QBaseAnalogReaderTestWidget::truthOScope() const
{
    return m_truthOScope;
}

const QString &QBaseAnalogReaderTestWidget::host() const
{
    return m_host;
}

void QBaseAnalogReaderTestWidget::setHost(const QString &newHost)
{
    if (m_host == newHost)
        return;
    m_host = newHost;
    emit hostChanged();
}

quint16 QBaseAnalogReaderTestWidget::port() const
{
    return m_port;
}

void QBaseAnalogReaderTestWidget::setPort(quint16 newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();
}

void QBaseAnalogReaderTestWidget::tcpConnect()
{
    m_client->connectToServer(m_host,m_port);
}

