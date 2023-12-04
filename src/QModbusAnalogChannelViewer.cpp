#include "QModbusAnalogChannelViewer.h"
#include <QVBoxLayout>
#include <QFont>

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
}

void QModbusAnalogChannelViewer::setValue(uint16_t value) {
    QString valueText = QString("%1").arg(value, 5, 10, QChar('0'));
    valueLabel->setText(valueText);
}

void QModbusAnalogChannelViewer::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::blue, 1)); // Blue pen with 1px width
    painter.drawRect(0, 0, width() - 1, height() - 1); // Draw a rectangle border
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
