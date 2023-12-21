// QBaseAnalogReaderTestWidget.h
#ifndef QBaseAnalogReaderTestWidget_H
#define QBaseAnalogReaderTestWidget_H

#include <QWidget>
#include <QTimer>
#include "BasicWidgets/QOScope.h"
#include "NetWorking/QtTcpClient.h"
#include "stringUtils.h"

class QStyleOption;
class QGroupBox   ;
class QLabel      ;
class QComboBox   ;
class QPushButton ;
class QGridLayout ;
class QFile       ;
class QPainter    ;

class QBaseAnalogReaderTestWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QComboBox *modulesComboBox READ getModulesComboBox WRITE setModulesComboBox NOTIFY modulesComboBoxChanged)
    Q_PROPERTY(QComboBox *channelComboBox READ getChannelComboBox WRITE setChannelComboBox NOTIFY channelComboBoxChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

public:
    QBaseAnalogReaderTestWidget(const QString& groupBoxTitle, QWidget *parent = nullptr);

    QComboBox *getModulesComboBox() const;
    void setModulesComboBox(QComboBox *newModulesComboBox);

    QComboBox *getChannelComboBox() const;
    void setChannelComboBox(QComboBox *newChannelComboBox);

    QOScope *truthOScope() const;

    const QString &host() const;
    void setHost(const QString &newHost);

    quint16 port() const;
    void setPort(quint16 newPort);

    void tcpConnect();
signals:
    void modulesComboBoxChanged();
    void channelComboBoxChanged();
    void logLastRequest(const QString &lastRequest);
    void logLastResponse(const QString &lastResponse);
    void logLastError   (const QString &lastError);


    void hostChanged();

    void portChanged();

private slots:
    virtual void onReadOneShotClicked() = 0; // Pure virtual function
    virtual void onPollClicked()        = 0;


public slots:
    virtual void onReadDone(const QString &result) = 0; //pure virtual
    void onMustDisconnect();

protected:
    QGroupBox *m_groupBox = nullptr;
    QLabel *m_moduleLabel = nullptr;
    QLabel *m_channelLabel = nullptr;
    QComboBox *m_modulesComboBox = nullptr;
    QComboBox *m_channelComboBox = nullptr;
    QPushButton *m_readOneShotButton = nullptr;
    QPushButton *m_pollButton = nullptr;
    QLabel *m_resultLabel = nullptr;
    QOScope *m_truthOScope = nullptr;
    QTimer *m_timer = nullptr;
    bool m_inPoll = false;
    QString m_host;
    quint16 m_port;
    QtTcpClient *m_client = nullptr;

    void setupUi(const QString& groupBoxTitle);
    void paintEvent(QPaintEvent *event);


private:
    void loadStyleStringFromResource();


};

#endif // QBaseAnalogReaderTestWidget_H
