#include "QReadCurrentTestWidget.h"

QReadCurrentTestWidget::QReadCurrentTestWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void QReadCurrentTestWidget::setupUi() {
    groupBox = new QGroupBox("Read Current Test", this);
    moduleLabel = new QLabel("Choose module", groupBox);
    channelLabel = new QLabel("Choose channel", groupBox);
    modulesComboBox = new QComboBox(groupBox);
    channelComboBox = new QComboBox(groupBox);
    readOneShotButton = new QPushButton("Read One Shot", groupBox);
    pollButton = new QPushButton("Poll", groupBox);
    resultLabel = new QLabel(groupBox);
    resultLabel->setText("Nan");

    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(moduleLabel       , 0, 0, 1, 1,Qt::AlignCenter); layout->addWidget(channelLabel    , 0, 1, 1, 1,Qt::AlignCenter);
    layout->addWidget(modulesComboBox   , 1, 0, 1, 1,Qt::AlignCenter); layout->addWidget(channelComboBox , 1, 1, 1, 1,Qt::AlignCenter);
    layout->addWidget(resultLabel       , 2, 0, 1, 2,Qt::AlignCenter);
    layout->addWidget(readOneShotButton , 3, 0, 1, 1,Qt::AlignCenter); layout->addWidget(pollButton      , 3, 1, 1, 1,Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox);

    connect(readOneShotButton, &QPushButton::clicked, this, &QReadCurrentTestWidget::onReadOneShotClicked);
    connect(pollButton,        &QPushButton::clicked, this, &QReadCurrentTestWidget::onPollClicked);
}

void QReadCurrentTestWidget::onReadOneShotClicked() {
    // Implement the logic for reading one shot
    resultLabel->setText("One shot read result here");
}

void QReadCurrentTestWidget::onPollClicked() {
    // Implement the logic for polling
    resultLabel->setText("Polling result here");
}

QComboBox *QReadCurrentTestWidget::getChannelComboBox() const
{
    return channelComboBox;
}

void QReadCurrentTestWidget::setChannelComboBox(QComboBox *newChannelComboBox)
{
    if (channelComboBox == newChannelComboBox)
        return;
    channelComboBox = newChannelComboBox;
    emit channelComboBoxChanged();
}

QComboBox *QReadCurrentTestWidget::getModulesComboBox() const
{
    return modulesComboBox;
}

void QReadCurrentTestWidget::setModulesComboBox(QComboBox *newModulesComboBox)
{
    if (modulesComboBox == newModulesComboBox)
        return;
    modulesComboBox = newModulesComboBox;
    emit modulesComboBoxChanged();
}
