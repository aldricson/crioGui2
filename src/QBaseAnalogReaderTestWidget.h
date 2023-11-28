// QBaseAnalogReaderTestWidget.h
#ifndef QBaseAnalogReaderTestWidget_H
#define QBaseAnalogReaderTestWidget_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include "QtTcpClient.h"
#include "QOScope.h"

class QBaseAnalogReaderTestWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QComboBox *modulesComboBox READ getModulesComboBox WRITE setModulesComboBox NOTIFY modulesComboBoxChanged)
    Q_PROPERTY(QComboBox *channelComboBox READ getChannelComboBox WRITE setChannelComboBox NOTIFY channelComboBoxChanged)
    Q_PROPERTY(QOScope *truthOScope READ truthOScope WRITE setTruthOScope NOTIFY truthOScopeChanged)

public:
    QBaseAnalogReaderTestWidget(QtTcpClient* tcpClient, const QString& groupBoxTitle, QWidget *parent = nullptr);

    QComboBox *getModulesComboBox() const;
    void setModulesComboBox(QComboBox *newModulesComboBox);

    QComboBox *getChannelComboBox() const;
    void setChannelComboBox(QComboBox *newChannelComboBox);

    QOScope *truthOScope() const;
    void setTruthOScope(QOScope *newTruthOScope);

signals:
    void modulesComboBoxChanged();
    void channelComboBoxChanged();
    void logLastRequest(const QString &lastRequest);
    void logLastResponse(const QString &lastResponse);

    void truthOScopeChanged();

private slots:
    virtual void onReadOneShotClicked() = 0; // Pure virtual function
    void onPollClicked();

public slots:
    virtual void onReadDone(const QString &result) = 0; //pure virtual

protected:
    QGroupBox *m_groupBox = nullptr;
    QLabel *m_moduleLabel = nullptr;
    QLabel *m_channelLabel = nullptr;
    QComboBox *m_modulesComboBox = nullptr;
    QComboBox *m_channelComboBox = nullptr;
    QPushButton *m_readOneShotButton = nullptr;
    QPushButton *m_pollButton = nullptr;
    QLabel *m_resultLabel = nullptr;
    QtTcpClient *m_tcpClient = nullptr;
    QOScope *m_truthOScope = nullptr;
    QTimer *m_timer = nullptr;

    void setupUi(const QString& groupBoxTitle);



};

#endif // QBaseAnalogReaderTestWidget_H
