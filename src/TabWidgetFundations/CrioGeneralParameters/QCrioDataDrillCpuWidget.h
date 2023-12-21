#ifndef QCrioDataDrillCpuWidget_H
#define QCrioDataDrillCpuWidget_H

#include "QCrioBaseCpuWidget.h"

class QTImer;
class QSSHCommand;
class QDonutGauge;

class QCrioDataDrillCpuWidget : public QCrioBaseCpuWidget
{
    Q_OBJECT

public:
    explicit QCrioDataDrillCpuWidget(QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif
