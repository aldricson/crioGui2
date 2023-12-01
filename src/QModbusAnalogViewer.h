#ifndef QMODBUSANALOGVIEWER_H
#define QMODBUSANALOGVIEWER_H

#include <QWidget>
#include <QVector>
#include <QTabWidget>
#include <QGridLayout>
#include "QModbusAnalogChannelViewer.h"

class QModbusAnalogViewer : public QWidget
{
    Q_OBJECT

public:
    explicit QModbusAnalogViewer(QWidget *parent = nullptr);

private:
    QVector<QModbusAnalogChannelViewer*> channelViewers;
    QTabWidget *tabWidget;
    QGridLayout *layout0to63;
    QGridLayout *layout64to127;
};

#endif // QMODBUSANALOGVIEWER_H
