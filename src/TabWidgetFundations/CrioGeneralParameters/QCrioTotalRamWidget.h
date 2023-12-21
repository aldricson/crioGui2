#ifndef QCrioTotalRamWidget_H
#define QCrioTotalRamWidget_H

#include "QCrioBaseCpuWidget.h"

class QTImer;
class QSSHCommand;
class QDonutGauge;

class QCrioTotalRamWidget : public QCrioBaseCpuWidget
{
    Q_OBJECT

public:
    explicit QCrioTotalRamWidget(QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif // QCrioTotalRamWidget_H
