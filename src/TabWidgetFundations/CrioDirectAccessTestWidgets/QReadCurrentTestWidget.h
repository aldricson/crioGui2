#ifndef QReadCurrentTestWidget_H
#define QReadCurrentTestWidget_H

#include "QBaseAnalogReaderTestWidget.h"

class QComboBox;
class QLabel;

class QReadCurrentTestWidget : public QBaseAnalogReaderTestWidget {
    Q_OBJECT

public:
    QReadCurrentTestWidget(QWidget *parent);

private slots:
    void onReadOneShotClicked() override;
    void onPollClicked()        override;

public slots:
    void onReadDone(const QString &result) override;

protected:
    void bindTCPClient();

};

#endif // QReadCurrentTestWidget_H
