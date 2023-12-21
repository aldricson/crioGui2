#ifndef QUNITDISPATCHER_H
#define QUNITDISPATCHER_H

#include <QLibrary>
#include <QtDebug>
#include <string.h>
#include <QEvent>
#include "Globals/globalEnumSpace.h"
#include "qwindowdefs.h"

extern "C" { typedef  bool    (__stdcall * initDLLPrototype)                       ();               }
extern "C" { typedef  bool    (__stdcall * setLibWidgetPrototype)                  (WId,int*,int*);  }
extern "C" { typedef  void    (__stdcall * rePaintPrototype)                       (int,int);        }
extern "C" { typedef  char *  (__stdcall * giveUnitFamillyNameAndIndexesPrototype) ();               }
extern "C" { typedef  char *  (__stdcall * giveInputUnitsByFamillyIndexPrototype)  (int);            }
extern "C" { typedef  char *  (__stdcall * giveOutputUnitsByFamillyIndexPrototype) (int);            }
extern "C" { typedef  double  (__stdcall * ConvertUserToSiPrototype)                (double,char*);  }
extern "C" { typedef  double  (__stdcall * ConvertSiToUserPrototype)                (double,char*);  }
extern "C" { typedef  double  (__stdcall * ConvertValuePrototype)                   (double,            // inputvalue
                                                                                    double,            // inputTVD if not used put 0
                                                                                    char*,             // inUnit
                                                                                    char*,             //  outUnit
                                                                                    int);            } //famillyIndex
extern "C" { typedef  bool    (__stdcall * showTestFormPrototype)                   ();              }

class QUnitDispatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QPair<QString, int> > unitFamillyAndIndexList READ getUnitFamillyAndIndexList WRITE setUnitFamillyAndIndexList NOTIFY unitFamillyAndIndexListChanged)
    Q_PROPERTY(QList<QString> inputUnitsList READ getInputUnitsList WRITE setInputUnitsList NOTIFY inputUnitsListChanged)
public:
    explicit QUnitDispatcher(QObject *parent = nullptr);

    QList<QPair<QString, int>> getUnitFamillyAndIndexList();
    void setUnitFamillyAndIndexList(const QList<QPair<QString, int> > &newUnitFamillyAndIndexList);

    QList<QString> getInputUnitsList();
    void setInputUnitsList(const QList<QString> &newInputUnitsList);

    QList<QString> getInputList  (int     anIndex);
    QList<QString> getOutputList (int     anIndex);
    double convToSiFromUnit      (double  aValue,     QString aUnit);
    double convFromSiToUnit      (double  aValue,     QString aUnit);
    double convFromUnitToUnit    (double  aValue,     double aTvdValue,
                                  QString inputUnit, QString outputUnit,
                                  int     aFamillyIndex);

private:
    QList<QPair<QString,int>>                 unitFamillyAndIndexList;
    QList<QString>                            inputUnitsList;
    QList<QString>                            outputUnitsList;
    QLibrary                                  * myConverterLib;
    initDLLPrototype                          initDLL;
    setLibWidgetPrototype                     setLibWidget;
    rePaintPrototype                          rePaint;
    giveUnitFamillyNameAndIndexesPrototype    fillUnitFamillyList;
    giveInputUnitsByFamillyIndexPrototype     fillInputUnitList;
    giveOutputUnitsByFamillyIndexPrototype    fillOutputUnitList;
    ConvertUserToSiPrototype                  convertToSi;
    ConvertSiToUserPrototype                  convertFromSi;
    ConvertValuePrototype                     convertFromUnitToUnit;
    showTestFormPrototype                     showTestForm;


    void setFamillyListFromCsvString(QString aCsvString, QString aSeparator);





signals:

    void unitFamillyAndIndexListChanged();
    void inputUnitsListChanged();

public slots:
};

#endif // QUNITDISPATCHER_H
