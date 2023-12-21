#include "QModbusAnalogChannelViewer.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QFont>
#include <QFile>

QModbusAnalogChannelViewer::QModbusAnalogChannelViewer(int channelIndex, QWidget *parent)
    : QWidget(parent), channelLabel(new QLabel(this)), valueLabel(new QLabel(this)) {
    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(channelLabel);
    layout->addWidget(valueLabel);

    // Set the channel label
    channelLabel->setText(QString("Channel: %1").arg(channelIndex));

    // Configure the value label
    QFont font = valueLabel->font();
    font.setBold(true);
    font.setPointSize(font.pointSize() + 2); // Slightly bigger font
    valueLabel->setFont(font);
    valueLabel->setAlignment(Qt::AlignCenter);

    // Initialize with zero value
    setValue(0);
    loadStyleStringFromResource();
}

void QModbusAnalogChannelViewer::setValue(uint16_t value) {
    QString valueText = QString("%1").arg(value, 5, 10, QChar('0'));
    valueLabel->setText(valueText);
}

void QModbusAnalogChannelViewer::paintEvent(QPaintEvent *event)
{

    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);

}

QLabel *QModbusAnalogChannelViewer::getValueLabel() const
{
    return valueLabel;
}

void QModbusAnalogChannelViewer::setValueLabel(QLabel *newValueLabel)
{
    if (valueLabel == newValueLabel)
        return;
    valueLabel = newValueLabel;
    emit valueLabelChanged();
}

QLabel *QModbusAnalogChannelViewer::getChannelLabel() const
{
    return channelLabel;
}

void QModbusAnalogChannelViewer::setChannelLabel(QLabel *newChannelLabel)
{
    if (channelLabel == newChannelLabel)
        return;
    channelLabel = newChannelLabel;
    emit channelLabelChanged();
}


void QModbusAnalogChannelViewer::loadStyleStringFromResource()
{
        QFile file(":/styles/darkAnalogViewer.scss");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        this->setStyleSheet(content);
}

