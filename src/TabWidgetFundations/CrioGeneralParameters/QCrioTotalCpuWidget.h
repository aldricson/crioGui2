#ifndef QCrioTotalCpuWidget_H
#define QCrioTotalCpuWidget_H

#include "QCrioBaseCpuWidget.h"

class QTImer;
class QSSHCommand;
class QDonutGauge;

class QCrioTotalCpuWidget : public QCrioBaseCpuWidget
{
    Q_OBJECT

public:
    explicit QCrioTotalCpuWidget(QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif
