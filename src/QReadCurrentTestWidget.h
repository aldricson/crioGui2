#ifndef QReadCurrentTestWidget_H
#define QReadCurrentTestWidget_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "QtTcpClient.h"

class QReadCurrentTestWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QComboBox *modulesComboBox READ getModulesComboBox WRITE setModulesComboBox NOTIFY modulesComboBoxChanged)
    Q_PROPERTY(QComboBox *channelComboBox READ getChannelComboBox WRITE setChannelComboBox NOTIFY channelComboBoxChanged)

public:
    QReadCurrentTestWidget(QtTcpClient* tcpClient,QWidget *parent = nullptr);


    QComboBox *getModulesComboBox() const;
    void       setModulesComboBox(QComboBox *newModulesComboBox);

    QComboBox *getChannelComboBox() const;
    void       setChannelComboBox(QComboBox *newChannelComboBox);

signals:
    void modulesComboBoxChanged();
    void channelComboBoxChanged();
    void logLastRequest   (const QString &lastRequest);
    void logLastResponse  (const QString &lastResponse);

private slots:
    void onReadOneShotClicked();
    void onPollClicked       ();
    void onReadCurrentDone   (const QString &result);

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

    void setupUi();

};

#endif // QReadCurrentTestWidget_H
