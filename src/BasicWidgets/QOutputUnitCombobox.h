#ifndef QOUTPUTUNITCOMBOBOX_H
#define QOUTPUTUNITCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QWidget>
#include <QAbstractItemView>
#include "QUnitDispatcher.h"

class QOutputUnitCombobox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(int realWidth READ getRealWidth WRITE setRealWidth NOTIFY realWidthChanged)
    Q_PROPERTY(QString defaultUnit READ getDefaultUnit WRITE setDefaultUnit NOTIFY defaultUnitChanged)
    Q_PROPERTY(QString currentUnit READ getCurrentUnit WRITE setCurrentUnit NOTIFY currentUnitChanged)
    Q_PROPERTY(int defaultFamillyIndex READ getDefaultFamillyIndex WRITE setDefaultFamillyIndex NOTIFY defaultFamillyIndexChanged)


public:


    explicit QOutputUnitCombobox(QWidget *parent = nullptr,
                                 int aUnitFamillyIndex = 0,
                                 QString unitName = "");
    ~QOutputUnitCombobox();

    int getRealWidth() const;
    void setRealWidth(int newRealWidth);


    const QString &getDefaultUnit() const;
    void setDefaultUnit(const QString &newDefaultUnit);

    const QString &getCurrentUnit() const;
    void setCurrentUnit(const QString &newCurrentUnit);

    int getDefaultFamillyIndex() const;
    void setDefaultFamillyIndex(int newDefaultFamillyIndex);

    int getHeaderIndex() const;
    void setHeaderIndex(int newHeaderIndex);

    int getHeaderColIndex() const;
    void setHeaderColIndex(int newHeaderColIndex);

private:
  QUnitDispatcher *unitDispatcher;
  int realWidth = 0;
  int defaultFamillyIndex = 0;
  int HeaderIndex = -2147483648;
  int headerColIndex = -2147483648;
  QString oldUnit="";
  QString defaultUnit = "";
  QString currentUnit = "";


  Q_PROPERTY(int HeaderIndex READ getHeaderIndex WRITE setHeaderIndex NOTIFY HeaderIndexChanged)

  Q_PROPERTY(int headerColIndex READ getHeaderColIndex WRITE setHeaderColIndex NOTIFY headerColIndexChanged)

private slots:
  void onCurrentUnitChanged();


public slots:
  void onUnitFamilyChanged(int index);

signals:
  void realWidthChanged();
  void defaultUnitChanged();
  void currentUnitChanged();
  void defaultFamillyIndexChanged();
  void textChangedSignal(const QString &oldText, const QString &newText);
  void HeaderIndexChanged();
  void headerColIndexChanged();
};

#endif // QUNITFAMILLYCOMBOBOX_H
