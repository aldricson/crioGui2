#ifndef QReadVoltageTestWidget_H
#define QReadVoltageTestWidget_H

#include "QBaseAnalogReaderTestWidget.h"

class QReadVoltageTestWidget : public QBaseAnalogReaderTestWidget {
    Q_OBJECT

public:
    QReadVoltageTestWidget(QtTcpClient *tcpClient, QWidget *parent = nullptr);

private slots:
    void onReadOneShotClicked() override;
    void onPollClicked() override;
public slots:
    void onReadDone(const QString &result) override;

protected:
    void bindTCPClient();
};

#endif // QReadVoltageTestWidget_H
