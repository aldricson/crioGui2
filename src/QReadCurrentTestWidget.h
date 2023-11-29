#ifndef QReadCurrentTestWidget_H
#define QReadCurrentTestWidget_H

#include "QBaseAnalogReaderTestWidget.h"

class QReadCurrentTestWidget : public QBaseAnalogReaderTestWidget {
    Q_OBJECT

public:
    QReadCurrentTestWidget(QtTcpClient *tcpClient, QWidget *parent = nullptr);

private slots:
    void onReadOneShotClicked() override;
    void onPollClicked()        override;

public slots:
    void onReadDone(const QString &result) override;

protected:
    void bindTCPClient();
};

#endif // QReadCurrentTestWidget_H
