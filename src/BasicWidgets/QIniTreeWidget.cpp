#include "QIniTreeWidget.h"
#include <QTreeWidgetItem>
#include <QFile>

QIniTreeWidget::QIniTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    loadStyleStringFromResource();
    setHeaderLabels({"Key", "Value"});
}

void QIniTreeWidget::loadFromFile(const QString &filePath) {
    clearTree(); // Clear the tree before loading new data

    QSettings settings(filePath, QSettings::IniFormat);
    const QStringList groups = settings.childGroups();

    for (const QString &group : groups) {
        settings.beginGroup(group);
        QMap<QString, QVariant> properties;

        const QStringList keys = settings.childKeys();
        for (const QString &key : keys) {
            properties[key] = settings.value(key);
        }

        if (properties.value("numberofchannels", 1).toInt() != 0 &&
            properties.value("numberofcounters", 1).toInt() != 0)
        {
            // Creating the group item with 'this' as the parent
            QTreeWidgetItem *groupItem = new QTreeWidgetItem(this);
            groupItem->setText(0, group);
            addItemsRecursively(groupItem, properties);
        }
        settings.endGroup();
    }
    this->expandAll();
}


void QIniTreeWidget::saveToFile(const QString &filePath) const {
    QSettings settings(filePath, QSettings::IniFormat);

    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *groupItem = topLevelItem(i);
        settings.beginGroup(groupItem->text(0));

        for (int j = 0; j < groupItem->childCount(); ++j)
        {
            QTreeWidgetItem *childItem = groupItem->child(j);
            settings.setValue(childItem->text(0), childItem->text(1));
        }
        settings.endGroup();
    }
}

void QIniTreeWidget::clearTree() {
    clear(); // Clear all items in the tree
}

void QIniTreeWidget::addItemsRecursively(QTreeWidgetItem *parent, const QMap<QString, QVariant> &properties)
{
    for (auto it = properties.cbegin(); it != properties.cend(); ++it)
    {
        new QTreeWidgetItem(parent, QStringList() << it.key() << it.value().toString());
    }
}

void QIniTreeWidget::loadStyleStringFromResource()
{
    QFile file(":/styles/darkIniTreeWidget.scss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    this->setStyleSheet(content);
}
