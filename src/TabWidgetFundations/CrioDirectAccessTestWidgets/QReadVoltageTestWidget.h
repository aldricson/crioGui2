#ifndef QReadVoltageTestWidget_H
#define QReadVoltageTestWidget_H

#include "QBaseAnalogReaderTestWidget.h"

class QComboBox;
class QLabel;

class QReadVoltageTestWidget : public QBaseAnalogReaderTestWidget {
    Q_OBJECT

public:
    QReadVoltageTestWidget(QWidget *parent = nullptr);

private slots:
    void onReadOneShotClicked() override;
    void onPollClicked() override;

public slots:
    void onReadDone(const QString &result) override;

protected:
    void bindTCPClient();

};

#endif // QReadVoltageTestWidget_H
