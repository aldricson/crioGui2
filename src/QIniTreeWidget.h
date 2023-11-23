#ifndef QIniTreeWidget_H
#define QIniTreeWidget_H

#include <QTreeWidget>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSettings>

class QIniTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    QIniTreeWidget(QWidget *parent = nullptr);
    void loadFromFile(const QString &filePath);
    void saveToFile(const QString &filePath) const;

private:
    void clearTree();
    void addItemsRecursively(QTreeWidgetItem *parent, const QMap<QString, QVariant> &properties);
};

#endif // QINITREEWIDGET_H
