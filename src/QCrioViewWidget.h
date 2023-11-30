#ifndef QCrioViewWidget_H
#define QCrioViewWidget_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QAction>
#include <QIcon>
#include <QStringListModel>
#include <QListView>
#include <QProgressBar>
#include <QMessageBox>
#include <QCrioModulesDataExtractor.h>

#include "QIpAddressEditor.h"
#include "QBetterSwitchButton.h"
#include "QMultiLineTextVisualizer.h"
#include "QReadCurrentTestWidget.h"
#include "QReadVoltageTestWidget.h"
#include "QSSHCommand.h"

class QCrioViewWidget : public QWidget
{
    Q_OBJECT
    //to ensure I have access to all the ui from the main
    Q_PROPERTY(QLabel *ipLabel READ ipLabel WRITE setIpLabel NOTIFY ipLabelChangedSignal)
    Q_PROPERTY(QIpAddressEditor *ipEdit READ ipEdit WRITE setIpEdit NOTIFY ipEditChangedSignal)
    Q_PROPERTY(QLabel *passwordLabel READ passwordLabel WRITE setPasswordLabel NOTIFY passwordLabelChangedSignal)
    Q_PROPERTY(QLineEdit *passwordEdit READ passwordEdit WRITE setPasswordEdit NOTIFY passwordEditChangedSignal)
    Q_PROPERTY(QAction *togglePasswordAction READ togglePasswordAction WRITE setTogglePasswordAction NOTIFY togglePasswordActionChangedSignal)
    Q_PROPERTY(QLabel *loginLabel READ loginLabel WRITE setLoginLabel NOTIFY loginLabelChangedSignal)
    Q_PROPERTY(QLineEdit *loginEdit READ loginEdit WRITE setLoginEdit NOTIFY loginEditChangedSignal)
    Q_PROPERTY(QLabel *serverStateLabel READ serverStateLabel WRITE setServerStateLabel NOTIFY serverStateLabelChangedSignal)
    Q_PROPERTY(QBetterSwitchButton *startStopServerSwitchButton READ startStopServerSwitchButton WRITE setStartStopServerSwitchButton NOTIFY startStopServerSwitchButtonChangedSignal)
    Q_PROPERTY(QMultiLineTextVisualizer *terminalOutput READ terminalOutput WRITE setTerminalOutput NOTIFY terminalOutputChangedSignal)
    Q_PROPERTY(QReadCurrentTestWidget *currentTestWidget READ currentTestWidget WRITE setCurrentTestWidget NOTIFY currentTestWidgetChangedSignal)
    Q_PROPERTY(QReadVoltageTestWidget *voltageTestWidget READ voltageTestWidget WRITE setVoltageTestWidget NOTIFY voltageTestWidgetChangedSignal)
    Q_PROPERTY(QProgressBar *modulesLoadingProgressBar READ modulesLoadingProgressBar WRITE setModulesLoadingProgressBar NOTIFY modulesLoadingProgressBarChanged)
    Q_PROPERTY(QListView *modulesListView READ modulesListView WRITE setModulesListView NOTIFY modulesListViewChanged)
    Q_PROPERTY(QLabel *moduleListLabel READ moduleListLabel WRITE setModuleListLabel NOTIFY moduleListLabelChanged)
    Q_PROPERTY(QStringListModel *moduleListModel READ moduleListModel WRITE setModuleListModel NOTIFY moduleListModelChanged)

public:
    explicit QCrioViewWidget(QWidget *parent = nullptr);
    QLabel *ipLabel() const;
    void setIpLabel(QLabel *newIpLabel);

    QIpAddressEditor *ipEdit() const;
    void setIpEdit(QIpAddressEditor *newIpEdit);

    QLabel *passwordLabel() const;
    void setPasswordLabel(QLabel *newPasswordLabel);

    QLineEdit *passwordEdit() const;
    void setPasswordEdit(QLineEdit *newPasswordEdit);

    QAction *togglePasswordAction() const;
    void setTogglePasswordAction(QAction *newTogglePasswordAction);

    QLabel *loginLabel() const;
    void setLoginLabel(QLabel *newLoginLabel);

    QLineEdit *loginEdit() const;
    void setLoginEdit(QLineEdit *newLoginEdit);

    QPushButton *connectButton() const;
    void setConnectButton(QPushButton *newConnectButton);

    QLabel *serverStateLabel() const;
    void setServerStateLabel(QLabel *newServerStateLabel);

    QBetterSwitchButton *startStopServerSwitchButton() const;
    void setStartStopServerSwitchButton(QBetterSwitchButton *newStartStopServerSwitchButton);

    QMultiLineTextVisualizer *terminalOutput() const;
    void setTerminalOutput(QMultiLineTextVisualizer *newTerminalOutput);

    QReadCurrentTestWidget *currentTestWidget() const;
    void setCurrentTestWidget(QReadCurrentTestWidget *newCurrentTestWidget);

    QReadVoltageTestWidget *voltageTestWidget() const;
    void setVoltageTestWidget(QReadVoltageTestWidget *newVoltageTestWidget);

    QProgressBar *modulesLoadingProgressBar() const;
    void setModulesLoadingProgressBar(QProgressBar *newModulesLoadingProgressBar);

    QListView *modulesListView() const;
    void setModulesListView(QListView *newModulesListView);

    QLabel *moduleListLabel() const;
    void setModuleListLabel(QLabel *newModuleListLabel);

    QStringListModel *moduleListModel() const;
    void setModuleListModel(QStringListModel *newModuleListModel);

public slots:
    //mainly used to update the interface
    void onServerStartSuccesfull       (const int &screenSession , const QString &lastCommand);
    void onResetModuleLoadProgressBar  (const int& min, const int &max, const int &value);
    void onUpdateModelList             (QStringList &moduleList);
    void onUpdateControlsAfterModulesDownloaded (QCrioModulesDataExtractor *dataExtractor,
                                                 QStringList &currentModulePathList,
                                                 QStringList &voltageModulePathList,
                                                 const quint16 &commandPort);
    void onUpdateControlsInReactionToServerState (const QString &lastCommand, const bool &serverState);
    void onUpdateControlsInReactionOfaStopFailure();

private:
    QLabel                   *m_ipLabel                     = nullptr;
    QIpAddressEditor         *m_ipEdit                      = nullptr;
    QLabel                   *m_passwordLabel               = nullptr;
    QLineEdit                *m_passwordEdit                = nullptr;
    QAction                  *m_togglePasswordAction        = nullptr;
    QLabel                   *m_loginLabel                  = nullptr;
    QLineEdit                *m_loginEdit                   = nullptr;
    QPushButton              *m_connectButton               = nullptr;
    QLabel                   *m_serverStateLabel            = nullptr;
    QBetterSwitchButton      *m_startStopServerSwitchButton = nullptr;
    QMultiLineTextVisualizer *m_terminalOutput              = nullptr;
    QReadCurrentTestWidget   *m_currentTestWidget           = nullptr;
    QReadVoltageTestWidget   *m_voltageTestWidget           = nullptr;
    QStringListModel         *m_moduleListModel             = nullptr;
    QLabel                   *m_moduleListLabel             = nullptr;
    QListView                *m_modulesListView             = nullptr;
    QProgressBar             *m_modulesLoadingProgressBar   = nullptr;

    void setupUi();
    void setupConnections();




signals:
    void serverChangeStateSignal                  (bool isOn);
    void sshConnectionAskedSignal                 ();
    void ipLabelChangedSignal                     ();
    void ipEditChangedSignal                      ();
    void passwordLabelChangedSignal               ();
    void passwordEditChangedSignal                ();
    void togglePasswordActionChangedSignal        ();
    void loginLabelChangedSignal                  ();
    void loginEditChangedSignal                   ();
    void connectButtonChangedSignal               ();
    void serverStateLabelChangedSignal            ();
    void startStopServerSwitchButtonChangedSignal ();
    void terminalOutputChangedSignal              ();
    void currentTestWidgetChangedSignal           ();
    void voltageTestWidgetChangedSignal           ();
    void modulesLoadingProgressBarChanged         ();
    void modulesListViewChanged                   ();
    void moduleListLabelChanged                   ();
    void moduleListModelChanged                   ();

private slots:
    void handleConnection();
    void onServerChangeState();

public slots:


};

#endif // QCrioViewWidget_H
