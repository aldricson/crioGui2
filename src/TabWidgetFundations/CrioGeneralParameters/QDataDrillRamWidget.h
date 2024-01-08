#ifndef QDataDrillRamWidget_H
#define QDataDrillRamWidget_H

#include "QCrioBaseCpuWidget.h"

class QTImer;
class QSSHCommand;
class QDonutGauge;

class QDataDrillRamWidget : public QCrioBaseCpuWidget
{
    Q_OBJECT

public:
    explicit QDataDrillRamWidget(const QString &md5Hash, const QString &executionPath, QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

protected:
    QString m_executionPath = "";
private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif // QDataDrillRamWidget_H
