#ifndef QMODBUSANALOGCHANNELVIEWER_H
#define QMODBUSANALOGCHANNELVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPen>
#include <QPainter>

class QStyleOption;
class QFile;

class QModbusAnalogChannelViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QLabel *channelLabel READ getChannelLabel WRITE setChannelLabel NOTIFY channelLabelChanged)
    Q_PROPERTY(QLabel *valueLabel READ getValueLabel WRITE setValueLabel NOTIFY valueLabelChanged)
public:
    explicit QModbusAnalogChannelViewer(int channelIndex, QWidget *parent = nullptr);
    void setValue(uint16_t value);
    QLabel *getChannelLabel() const;
    void setChannelLabel(QLabel *newChannelLabel);

    QLabel *getValueLabel() const;
    void setValueLabel(QLabel *newValueLabel);

signals:
    void channelLabelChanged();

    void valueLabelChanged();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *channelLabel;
    QLabel *valueLabel;

    void loadStyleStringFromResource();

};

#endif // QMODBUSANALOGCHANNELVIEWER_H
