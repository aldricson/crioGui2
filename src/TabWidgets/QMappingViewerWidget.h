#ifndef QMAPPINGVIEWERWIDGET_H
#define QMAPPINGVIEWERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QTimer>
#include "Extractors/QCrioModulesDataExtractor.h"

class QMultiLineTextVisualizer ;
class QMappingRowWidget        ;
class QScrollBar               ;
class QLineEdit                ;
class QLabel                   ;
class QPushButton              ;
class QBackUpWidget            ;
class QSSHCommand              ;
class QMessageBox              ;

class QMappingViewerWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)
public:
    explicit QMappingViewerWidget(const QString &md5Hash,
                                  const QString &executionPath,
                                  const QString &modbusMappingPath,
                                  QCrioModulesDataExtractor *moduleExtractor,
                                  QWidget *parent = nullptr);
    ~QMappingViewerWidget();
    void enableAllControls(bool enabled);


    const QString &hostName() const;
    void setHostName(const QString &newHostName);

    int port() const;
    void setPort(int newPort);

    const QString &login() const;
    void setLogin(const QString &newLogin);

    const QString &passWord() const;
    void setPassWord(const QString &newPassWord);

private slots:

    void onInsertBefore();
    void onInsertAfter();
    void onAdd();
    void onModbusMappingUploaded ( const QString &output, const QString &lastCommand );
    void onSelectRow   (QMappingRowWidget *row);
    void onDeleteRow   (QMappingRowWidget *row);
    void onMoveRowUp   (QMappingRowWidget *row);
    void onMoveRowDown (QMappingRowWidget *row);

public slots:

    void onLoad  ();
    void onSave  ();
    void onUpload();

signals:
    void hostNameChanged();

    void portChanged();

    void loginChanged();

    void passWordChanged();

protected:
    bool m_saveSuccess;
    bool checkAllBeforeLoad(const QStringList& lines);
    bool checkLineFormat   (const QString& line);
    bool checkLineFields   (const QString& line);
    bool validateIndices   (const QStringList &lines);



    bool checkAllBeforeSave            ();

    bool checkRowsForEmptyLineEdits    ();
    bool checkIfALineEditIsEmpty       (QLineEdit *aLineEdit);

    bool checkRowsMinMax               ();

    bool chekRowsForEmptyComboboxes    ();
    bool checkIfAComboboxIsEmpty       (QComboBox *aComboBox);

    bool checkforModulesTypesIsNotAll ();


    void backUpFile();

private:

    QString             m_executionPath    = ""     ;
    QString             m_hostName         = ""     ;
    int                 m_port             = 22     ;
    QString             m_login            = ""     ;
    QString             m_passWord         = ""     ;

    QVector<QMappingRowWidget*> rowWidgets;
    QScrollArea                *scrollArea      = nullptr;
    QWidget                    *containerWidget = nullptr;
    QVBoxLayout                *containerLayout = nullptr;
    QMappingRowWidget          *selectedRow     = nullptr;
    QWidget                    *fillerWidget    = nullptr;
    QMultiLineTextVisualizer   *csvVizualizer   = nullptr;
    QBackUpWidget              *backuWidget     = nullptr;
    QSSHCommand                *sshCommand      = nullptr;

    QPushButton *insertBeforeBtn = nullptr;
    QPushButton *insertAfterBtn  = nullptr;
    QPushButton *addBtn          = nullptr;

    QPushButton *reloadBtn      = nullptr;
    QPushButton *saveBtn        = nullptr;
    QPushButton *uploadBtn      = nullptr;

    QCrioModulesDataExtractor *m_extractor = nullptr;
    void setupUi();
    QMappingRowWidget* createRowWidget();

    QString m_modbusMappingPath;

    void updateRowIndices();
    void deleteAllRows();
    void blockAllSignals(bool blocked);
    void loadStyleStringFromResource();




};

#endif // QMAPPINGVIEWERWIDGET_H
