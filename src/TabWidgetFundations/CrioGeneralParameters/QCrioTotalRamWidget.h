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
    explicit QCrioTotalRamWidget(const QString &md5Hash, const QString &executionPath, QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

protected:
    QString m_executionPath = "";

private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif // QCrioTotalRamWidget_H
