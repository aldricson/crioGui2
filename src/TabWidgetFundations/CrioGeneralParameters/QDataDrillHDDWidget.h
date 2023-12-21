#ifndef QDATADRILLHDDWIDGET_H
#define QDATADRILLHDDWIDGET_H

#include "QCrioBaseCpuWidget.h"

class QTImer;
class QSSHCommand;
class QDonutGauge;

class QDataDrillHDDWidget : public QCrioBaseCpuWidget
{
    Q_OBJECT

public:
    explicit QDataDrillHDDWidget(QWidget *parent = nullptr);
    virtual void connectSSHSignals() override;

private slots:
    virtual void onTimeOut() override;
    virtual void onDataReceived(const QString &output, const QString &lastCommand) override;
};

#endif // QDATADRILLHDDWIDGET_H
