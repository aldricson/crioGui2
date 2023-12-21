#include "QModulesIniListView.h"
#include <QStringListModel>



QModulesIniListView::QModulesIniListView(QStringListModel *model,const QString &inifilePath, QWidget *parent):QListView(parent)
{
    m_iniModulesLocalPath = inifilePath;
    m_moduleListModel = model;
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this, &QListView::doubleClicked , this , &QModulesIniListView::onModuleItemDoubleClicked, Qt::QueuedConnection);

}

void QModulesIniListView::updateModelList(QStringList &moduleList)
{
    m_moduleListModel->setStringList(moduleList);
}

QIniTreeWidget *QModulesIniListView::iniTreeWidget() const
{
    return m_iniTreeWidget;
}

void QModulesIniListView::setIniTreeWidget(QIniTreeWidget *newIniTreeWidget)
{
    if (m_iniTreeWidget == newIniTreeWidget)
        return;
    m_iniTreeWidget = newIniTreeWidget;
    emit iniTreeWidgetChanged();
}

QStringListModel *QModulesIniListView::moduleListModel() const
{
    return m_moduleListModel;
}

void QModulesIniListView::setModuleListModel(QStringListModel *newModuleListModel)
{
    if (m_moduleListModel == newModuleListModel)
        return;
    m_moduleListModel = newModuleListModel;
    emit moduleListModelChanged();
}

const QString &QModulesIniListView::iniModulesLocalPath() const
{
    return m_iniModulesLocalPath;
}

void QModulesIniListView::setIniModulesLocalPath(const QString &newIniModulesLocalPath)
{
    if (m_iniModulesLocalPath == newIniModulesLocalPath)
        return;
    m_iniModulesLocalPath = newIniModulesLocalPath;
    emit iniModulesLocalPathChanged();
}


void QModulesIniListView::onModuleItemDoubleClicked(const QModelIndex &index)
{
  if (index.isValid())
  {
          QString fileName = m_iniModulesLocalPath + m_moduleListModel->data(index, Qt::DisplayRole).toString();
          m_iniTreeWidget->loadFromFile(fileName);
  }
  Q_EMIT moduleListUpdatedSignal(index);
}


void QModulesIniListView::copyTo(QModulesIniListView *target)
{
    if (!target)
        return;

    // Copying iniModulesLocalPath
    target->setIniModulesLocalPath(this->iniModulesLocalPath());

    // Copying the module list model data
    if (this->moduleListModel())
    {
        QStringListModel *newModel = new QStringListModel(target);
        newModel->setStringList(this->moduleListModel()->stringList());
        target->setModuleListModel(newModel);
    }

    // Copying the iniTreeWidget reference
    target->setIniTreeWidget(this->iniTreeWidget());
}
