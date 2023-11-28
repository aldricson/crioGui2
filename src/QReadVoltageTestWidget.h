#ifndef QReadVoltageTestWidget_H
#define QReadVoltageTestWidget_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include "QtTcpClient.h"
#include "QOScope.h"

class QReadVoltageTestWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QComboBox *modulesComboBox READ getModulesComboBox WRITE setModulesComboBox NOTIFY modulesComboBoxChanged)
    Q_PROPERTY(QComboBox *channelComboBox READ getChannelComboBox WRITE setChannelComboBox NOTIFY channelComboBoxChanged)
    Q_PROPERTY(QOScope *truthOScope READ truthOScope WRITE setTruthOScope NOTIFY truthOScopeChanged)

public:
    QReadVoltageTestWidget(QtTcpClient* tcpClient,QWidget *parent = nullptr);


    QComboBox *getModulesComboBox() const;
    void       setModulesComboBox(QComboBox *newModulesComboBox);

    QComboBox *getChannelComboBox() const;
    void       setChannelComboBox(QComboBox *newChannelComboBox);

    QOScope *truthOScope() const;
    void setTruthOScope(QOScope *newTruthOScope);

signals:
    void modulesComboBoxChanged();
    void channelComboBoxChanged();
    void logLastRequest   (const QString &lastRequest);
    void logLastResponse  (const QString &lastResponse);

    void truthOScopeChanged();

private slots:
    void onReadOneShotClicked();
    void onPollClicked       ();
    void onReadVoltageDone   (const QString &result);

private:
    QGroupBox    *m_groupBox          = nullptr;
    QLabel       *m_moduleLabel       = nullptr;
    QLabel       *m_channelLabel      = nullptr;
    QComboBox    *m_modulesComboBox   = nullptr;
    QComboBox    *m_channelComboBox   = nullptr;
    QPushButton  *m_readOneShotButton = nullptr;
    QPushButton  *m_pollButton        = nullptr;
    QLabel       *m_resultLabel       = nullptr;
    QtTcpClient  *m_tcpClient         = nullptr;
    QOScope      *m_truthOScope       = nullptr;
    QTimer       *m_timer             = nullptr;

    void setupUi();

};

#endif // QReadVoltageTestWidget_H
