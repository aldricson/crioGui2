#ifndef QMODBUSANALOGVIEWER_H
#define QMODBUSANALOGVIEWER_H

#include <QWidget>
#include <QVector>
#include <QTabWidget>
#include <QGridLayout>
#include "QModbusAnalogChannelViewer.h"
#include <QStyleOption>

class QScrollArea;
class QScrollBar;
class QFile;

class QModbusAnalogViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QVector<QModbusAnalogChannelViewer *> channelViewers READ channelViewers WRITE setChannelViewers NOTIFY channelViewersChanged)

public:
    explicit QModbusAnalogViewer(QWidget *parent = nullptr);
    void  resetOffset(int offSet)           ;
    const QVector<QModbusAnalogChannelViewer *> &channelViewers () const;
    void setChannelViewers(const QVector<QModbusAnalogChannelViewer *> &newChannelViewers);

public slots:
    void onMapValuesToInterface(const QVector<quint16> &data);
signals:
    void channelViewersChanged();

private:
    QVector<QModbusAnalogChannelViewer*> m_channelViewers;
    QTabWidget                           *tabWidget    = nullptr;
    QGridLayout                          *layout0to63  ;
    QGridLayout                          *layout64to127;
    QScrollArea                          *scrollArea0to63   = nullptr;
    QScrollArea                          *scrollArea64to127 = nullptr;
    QWidget                              *widget0to63       = nullptr;
    QWidget                              *widget64to127     = nullptr;
    int                                  m_offset     ;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // QMODBUSANALOGVIEWER_H
