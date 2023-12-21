#ifndef QDEVICEPARAMETERSWIDGET_H
#define QDEVICEPARAMETERSWIDGET_H

#include <QWidget>


class QGridLayout         ;
class QGroupBox           ;
class QIniTreeWidget      ;
class QModulesIniListView ;
class QStringListModel    ;

class QDeviceParametersWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIniTreeWidget *iniTreeWidget READ iniTreeWidget WRITE setIniTreeWidget NOTIFY iniTreeWidgetChanged)
    Q_PROPERTY(QModulesIniListView *modulesListView READ modulesListView WRITE setModulesListView NOTIFY moulesListViewChanged)

public:
    explicit QDeviceParametersWidget(const QString &iniModulesLocalPath, QWidget *parent = nullptr);



    QIniTreeWidget *iniTreeWidget() const;
    void setIniTreeWidget(QIniTreeWidget *newIniTreeView);

    QModulesIniListView *modulesListView() const;
    void setModulesListView(QModulesIniListView *newMoulesListView);

private :

    QGridLayout           *m_layout           = nullptr;
    QIniTreeWidget        *m_iniTreeWidget    = nullptr;
    QModulesIniListView   *m_modulesListView  = nullptr;
    QStringListModel      *m_moduleListModel  = nullptr;
    void setUpUi();
    void setUpLayout();
    QString m_iniPath = "";



public slots:
    void onUpdateModelList(QIniTreeWidget *tree, QStringList &moduleList);

signals:

    void iniTreeWidgetChanged();
    void moulesListViewChanged();
};

#endif // QDEVICEPARAMETERSWIDGET_H
