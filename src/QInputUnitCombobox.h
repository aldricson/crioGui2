#ifndef QINPUTUNITCOMBOBOX_H
#define QINPUTUNITCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QWidget>
#include "QUnitDispatcher.h"

class QInputUnitCombobox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(int realWidth READ getRealWidth WRITE setRealWidth NOTIFY realWidthChanged)
    Q_PROPERTY(int defaultFamillyIndex READ getDefaultFamillyIndex WRITE setDefaultFamillyIndex NOTIFY defaultFamillyIndexChanged)
    Q_PROPERTY(QString oldUnit READ getOldUnit WRITE setOldUnit NOTIFY oldUnitChanged)
    Q_PROPERTY(QString defaultUnit READ getDefaultUnit WRITE setDefaultUnit NOTIFY defaultUnitChanged)
    Q_PROPERTY(QString currentUnit READ getCurrentUnit WRITE setCurrentUnit NOTIFY currentUnitChanged)
public:


    explicit QInputUnitCombobox (QWidget *parent = nullptr,
                                int aUnitFamillyIndex = 0,
                                QString unitName = "");

    ~QInputUnitCombobox();
    int getRealWidth() const;
    void setRealWidth(int newRealWidth);

    int getDefaultFamillyIndex() const;
    void setDefaultFamillyIndex(int newDefaultFamillyIndex);

    const QString &getOldUnit() const;
    void setOldUnit(const QString &newOldUnit);

    const QString &getDefaultUnit() const;
    void setDefaultUnit(const QString &newDefaultUnit);

    const QString &getCurrentUnit() const;
    void setCurrentUnit(const QString &newCurrentUnit);

public slots:
    void onUnitFamilyChanged(int index);

private slots:
  void onCurrentUnitChanged();

signals:
    void realWidthChanged();
    void defaultFamillyIndexChanged();
    void oldUnitChanged();
    void defaultUnitChanged();
    void currentUnitChanged();
    void textChangedSignal(const QString &oldText, const QString &newText);

private:
    QUnitDispatcher *unitDispatcher;
    int realWidth = 0;
    int defaultFamillyIndex = 0;
    QString oldUnit="";
    QString defaultUnit = "";
    QString currentUnit = "";


};

#endif // QUNITFAMILLYCOMBOBOX_H
