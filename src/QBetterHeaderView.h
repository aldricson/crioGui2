#ifndef QBETTERHEADERVIEW_H
#define QBETTERHEADERVIEW_H

#include <QHeaderView>
#include <QObject>
#include <QWidget>

class QBetterHeaderView : public QHeaderView
{
    Q_OBJECT
public:
   explicit QBetterHeaderView(Qt::Orientation orientation, QWidget * parent = nullptr);

    bool getIsUnitConverter() const;
    void setIsUnitConverter(bool newIsUnitConverter);

    bool getIsStdLineEdit() const;
    void setIsStdLineEdit(bool newIsStdLineEdit);

signals:
    void isUnitConverterChanged();

    void isStdLineEditChanged();

private:
    bool isUnitConverter = false;
    bool isStdLineEdit   = false;
    Q_PROPERTY(bool isUnitConverter READ getIsUnitConverter WRITE setIsUnitConverter NOTIFY isUnitConverterChanged)
    Q_PROPERTY(bool isStdLineEdit READ getIsStdLineEdit WRITE setIsStdLineEdit NOTIFY isStdLineEditChanged)
};

#endif // QBETTERHEADERVIEW_H
