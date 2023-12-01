#ifndef QMODBUSANALOGCHANNELVIEWER_H
#define QMODBUSANALOGCHANNELVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QString>

class QModbusAnalogChannelViewer : public QWidget
{
    Q_OBJECT

public:
    explicit QModbusAnalogChannelViewer(int channelIndex, QWidget *parent = nullptr);
    void setValue(uint16_t value);
protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *channelLabel;
    QLabel *valueLabel;
};

#endif // QMODBUSANALOGCHANNELVIEWER_H
