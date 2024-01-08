#ifndef QGlobalParametersWidget_H
#define QGlobalParametersWidget_H

#include <QWidget>


class QGridLayout              ;
class QCrioTotalCpuWidget      ;
class QCrioDataDrillCpuWidget  ;
class QCrioHDDWidget           ;
class QDataDrillHDDWidget      ;
class QCrioTotalRamWidget      ;
class QDataDrillRamWidget      ;
class QMultiLineTextVisualizer ;
class QSSHCommand              ;
class QTimer                   ;

class QGlobalParametersWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(int sshPort READ sshPort WRITE setSshPort NOTIFY sshPortChanged)
public:
    explicit QGlobalParametersWidget(const QString &md5Hash,const QString &executionPath, QWidget *parent = nullptr);

    const QString &hostName() const;
    void setHostName(const QString &newHostName);

    const QString &userName() const;
    void setUserName(const QString &newUserName);

    const QString &password() const;
    void setPassword(const QString &newPassword);

    int  sshPort() const;
    void setSshPort(int newSshPort);

    void start();
    void stop();

private :

    QGridLayout              *m_layout             = nullptr;
    QCrioTotalCpuWidget      *m_totalCPUWidget     = nullptr;
    QCrioDataDrillCpuWidget  *m_dataDrillCpuWidget = nullptr;
    QCrioHDDWidget           *m_crioHDDWidget      = nullptr;
    QDataDrillHDDWidget      *m_dataDrillHDDWidget = nullptr;
    QCrioTotalRamWidget      *m_crioTotalRamWidget = nullptr;
    QDataDrillRamWidget      *m_dataDrillRamWidget = nullptr;
    QSSHCommand              *m_globalStatCommand  = nullptr;
    QMultiLineTextVisualizer *m_globalStatOutput   = nullptr;
    QTimer                   *m_globalStatTimer    = nullptr;
    void setUpUi();
    void setUpLayout();
    QString m_executionPath = "";
    QString m_sshMd5Hash    = "";
    QString m_hostName      = "";
    QString m_userName      = "";
    QString m_password      = "";
    int     m_sshPort           ;

private slots:
    void onGlobalStats (const QString &output, const QString &lastCommand);
    void onAskStats    ();
public slots:

signals:
    void hostNameChanged();
    void userNameChanged();
    void passwordChanged();
    void sshPortChanged();
};

#endif // QDEVICEPARAMETERSWIDGET_H
