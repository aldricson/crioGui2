#ifndef QCrioBaseCpuWidget_H
#define QCrioBaseCpuWidget_H

#include <QWidget>


class QGridLayout  ;
class QGroupBox    ;
class QSSHCommand  ;
class QTimer       ;
class QDonutGauge  ;


class QCrioBaseCpuWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(int     port     READ port     WRITE setPort     NOTIFY portChanged)
    Q_PROPERTY(QString login    READ login    WRITE setLogin    NOTIFY loginChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)

public:
    explicit QCrioBaseCpuWidget(const QString &md5Hash, const QString &execution_path, const QString &title, QWidget *parent = nullptr);

    const QString &hostName() const;
    void setHostName(const QString &newHostName);

    int  port() const;
    void setPort(int newPort);

    const QString &login() const;
    void setLogin(const QString &newLogin);

    const QString &passWord() const;
    void setPassWord(const QString &newPassWord);

    void start          ()                          ;
    void stop           ()                          ;

    virtual void connectSSHSignals() = 0;

protected :

    QGridLayout         *m_layout          = nullptr;
    QSSHCommand         *m_sshCommand      = nullptr;
    QTimer              *m_timer           = nullptr;
    QGroupBox           *m_container       = nullptr;
    QDonutGauge         *m_donutGauge      = nullptr;
    QString             m_executionPath    = ""     ;
    QString             m_hostName         = ""     ;
    int                 m_port             = 22     ;
    QString             m_login            = ""     ;
    QString             m_passWord         = ""     ;
    QString             m_title            = ""     ;
	void setUpUi        ()                          ;
    void setUpLayout    ()                          ;


private slots:

    virtual void onTimeOut() = 0;
    virtual void onDataReceived ( const QString &output        , const QString &lastCommand ) = 0;

public slots:

signals:

    void hostNameChanged();
    void portChanged();
    void loginChanged();
    void passWordChanged();
};

#endif 
