/**
 * @file QSignalAggregator.h
 * @brief Aggregates multiple signals and emits a single signal when all have been received.
 *
 * @class QSignalAggregator
 * @extends QObject
 *
 * The QSignalAggregator class is a utility class that can be used to monitor multiple signals from various QObject-derived sender objects.
 * It is particularly useful in scenarios where a set of asynchronous operations or events need to be tracked collectively.
 * Once all the registered signals have been received, the QSignalAggregator emits a single `aggregateSignalReadySignal` to indicate
 * the completion of all monitored activities.
 *
 * Key functionalities include:
 * - Registering signals from different sender objects.
 * - Tracking the emission of these registered signals.
 * - Emitting an aggregated signal upon the reception of all registered signals.
 * - Resetting its state after emitting the aggregated signal, making it reusable for multiple rounds of signal aggregation.
 *
 * @note This class does not take ownership of the sender objects, and it is the responsibility of the user to ensure that the lifetime
 * of the sender objects extends at least as long as the lifetime of the QSignalAggregator instance, or until the signal is deregistered.
 */
#include "QSignalAggregator.h"
#include <QMetaMethod>
#include <QDebug>

/**
 * @brief Constructor for QSignalAggregator class.
 *
 * @param parent The parent QObject (default is nullptr).
 */
QSignalAggregator::QSignalAggregator(QObject *parent) : QObject(parent)
{
    // Constructor body is empty as initialization is done in the class initializer list.
}

/**
 * @brief Registers a signal to be monitored by the aggregator.
 *
 * @param sender The object emitting the signal.
 * @param signal The signal to be registered (must be a valid SIGNAL macro).
 */
void QSignalAggregator::registerSignal(QObject *sender, const char *signal)
{
    // Check if sender is valid
    if (!sender) {
        qWarning() << "Invalid sender (nullptr)";
        return;
    }

    // Check if signal is valid
    if (!signal || strlen(signal) == 0) {
        qWarning() << "Invalid signal (empty or nullptr)";
        return;
    }

    // Normalize the signal
    QByteArray normalizedSignal = QMetaObject::normalizedSignature(signal + 1); // +1 to skip the '2' prefix

    // Check if the sender has the signal
    if (sender->metaObject()->indexOfSignal(normalizedSignal) < 0) {
        qWarning() << "Signal not found in sender:" << normalizedSignal;
        return;
    }

    // Register the signal
    registeredSignals[sender].insert(normalizedSignal);

    // Connect the signal to the handleSignal slot
    if (!connect(sender, signal, this, SLOT(handleSignal()))) {
        qWarning() << "Failed to connect signal:" << normalizedSignal;
    } else {
        qDebug() << "Registered and connected signal:" << normalizedSignal;
    }
}


/**
 * @brief Slot to handle received signals.
 *
 * This method tracks received signals and checks if all registered signals have been received.
 * When all signals are received, it emits the aggregateSignalReadySignal.
 */
void QSignalAggregator::handleSignal()
{
    qInfo()<<"Signal recived in aggregator";
    QObject *sender = QObject::sender();
    if (!sender)
    {
        return;
    }

    // Extracting the signal information from the sender
    int signalIndex = sender->metaObject()->indexOfSignal(QMetaMethod::fromSignal(&QSignalAggregator::handleSignal).methodSignature());
    const char* signal = sender->metaObject()->method(signalIndex).methodSignature().constData();

    // Mark the signal as received
    receivedSignals[sender].insert(signal);

    // Check if all signals have been received
    for (auto it = registeredSignals.constBegin(); it != registeredSignals.constEnd(); ++it) {
        if (receivedSignals[it.key()] != it.value()) {
            return; // Not all signals received yet
        }
    }

    // All signals received, emit the aggregate signal
    emit aggregateSignalReadySignal();

    // Reset received signals for the next round
    receivedSignals.clear();
}
