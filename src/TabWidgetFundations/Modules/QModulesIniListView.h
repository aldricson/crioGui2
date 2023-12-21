#ifndef QMODULESINILISTVIEW_H
#define QMODULESINILISTVIEW_H

#include "BasicWidgets/QIniTreeWidget.h"
#include <QListView>
#include <QObject>
#include <QWidget>


class QStringListModel;


class QModulesIniListView : public QListView
{
    Q_OBJECT
    Q_PROPERTY(QIniTreeWidget *iniTreeWidget READ iniTreeWidget WRITE setIniTreeWidget NOTIFY iniTreeWidgetChanged)
    Q_PROPERTY(QStringListModel *moduleListModel READ moduleListModel WRITE setModuleListModel NOTIFY moduleListModelChanged)
    Q_PROPERTY(QString iniModulesLocalPath READ iniModulesLocalPath WRITE setIniModulesLocalPath NOTIFY iniModulesLocalPathChanged)
public:
    QModulesIniListView(QStringListModel *model, const QString &inifilePath, QWidget *parent = nullptr);
    void updateModelList(QStringList &moduleList);

    QIniTreeWidget *iniTreeWidget() const;
    void setIniTreeWidget(QIniTreeWidget *newIniTreeWidget);

    QStringListModel *moduleListModel() const;
    void setModuleListModel(QStringListModel *newModuleListModel);

    const QString &iniModulesLocalPath() const;
    void setIniModulesLocalPath(const QString &newIniModulesLocalPath);

     void copyTo(QModulesIniListView *target);
private:
    QString           m_iniModulesLocalPath  = ""      ;
    QIniTreeWidget    *m_iniTreeWidget       = nullptr;
    QStringListModel  *m_moduleListModel     = nullptr;


public slots:
    void onModuleItemDoubleClicked(const QModelIndex &index);

signals:
    void iniTreeWidgetChanged();
    void moduleListModelChanged();
    void iniModulesLocalPathChanged();
    void moduleListUpdatedSignal(const QModelIndex &index);

};

#endif // QMODULESINILISTVIEW_H
