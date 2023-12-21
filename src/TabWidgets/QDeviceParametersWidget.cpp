#include "QDeviceParametersWidget.h"
#include <QGridLayout>
#include <QStringListModel>
#include "BasicWidgets/QIniTreeWidget.h"
#include "TabWidgetFundations/Modules/QModulesIniListView.h"


QDeviceParametersWidget::QDeviceParametersWidget(const QString &iniModulesLocalPath, QWidget *parent)
    : QWidget{parent}
{
    m_iniPath = iniModulesLocalPath;
    setUpUi();
    setUpLayout();
}



void QDeviceParametersWidget::onUpdateModelList(QIniTreeWidget *tree, QStringList &moduleList)
{
    m_modulesListView->setIniTreeWidget(tree);
    m_moduleListModel->setStringList(moduleList);
    m_modulesListView->setModel(m_moduleListModel);
}


QIniTreeWidget *QDeviceParametersWidget::iniTreeWidget() const
{
    return m_iniTreeWidget;
}

void QDeviceParametersWidget::setIniTreeWidget(QIniTreeWidget *newIniTreeWidget)
{
    if (m_iniTreeWidget == newIniTreeWidget)
        return;
    m_iniTreeWidget = newIniTreeWidget;
    Q_EMIT iniTreeWidgetChanged();
}

QModulesIniListView *QDeviceParametersWidget::modulesListView() const
{
    return m_modulesListView;
}

void QDeviceParametersWidget::setModulesListView(QModulesIniListView *newMoulesListView)
{
    if (m_modulesListView == newMoulesListView)
        return;
    m_modulesListView = newMoulesListView;
    emit moulesListViewChanged();
}

void QDeviceParametersWidget::setUpUi()
{
    m_layout          = new QGridLayout         (this);
    m_iniTreeWidget   = new QIniTreeWidget      (this);
    m_moduleListModel = new QStringListModel    (this);
    m_modulesListView = new QModulesIniListView (m_moduleListModel,m_iniPath,this);

}

void QDeviceParametersWidget::setUpLayout()
{
    m_layout->addWidget(m_modulesListView,0,0,1,1);
    m_layout->addWidget(m_iniTreeWidget  ,0,1,1,1);
}




