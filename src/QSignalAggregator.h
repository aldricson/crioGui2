#ifndef QSIGNALAGGREGATOR_H
#define QSIGNALAGGREGATOR_H

#include <QObject>
#include <QSet>
#include <QMap>

class QSignalAggregator : public QObject
{
    Q_OBJECT

public:
    explicit QSignalAggregator(QObject *parent = nullptr);

    // Registers a signal from a sender object
    void registerSignal(QObject *sender, const char *signal);

signals:
    // Emitted when all registered signals have been received
    void aggregateSignalReadySignal();

private slots:
    // Slot to handle received signals
    void handleSignal();

private:
    QMap<QObject*, QSet<const char*>> registeredSignals;
    QMap<QObject*, QSet<const char*>> receivedSignals;
};

#endif // QSIGNALAGGREGATOR_H
