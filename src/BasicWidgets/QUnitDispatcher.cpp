#include "QUnitDispatcher.h"
#include "Globals/globalStringHelper.h"

QUnitDispatcher::QUnitDispatcher(QObject *parent) : QObject(parent)
{
    bool ok;
    myConverterLib = new QLibrary;
    myConverterLib->setFileName("C:\\RamsesLibraries\\QUnitConvLib");
    ok=myConverterLib->load();
    if (!ok)
    {
        qCritical() << "Impossible to load C:/RamsesLibraries/QUnitConvLib \n error message:"+myConverterLib->errorString();
        return;
    }
    initDLL = (initDLLPrototype) myConverterLib->resolve("initDll");
    if (!initDLL)
    {
        qCritical() << "Impossible to resolve initDll \n error message:"+myConverterLib->errorString();
        return;
    }

    setLibWidget= (setLibWidgetPrototype) myConverterLib->resolve("setLibWidget");
    if (!setLibWidget)
    {
        qCritical() << "Impossible to resolve setLibWidget \n error message:"+myConverterLib->errorString();
        return;
    }

    rePaint= (rePaintPrototype) myConverterLib->resolve("rePaint");
    if (!rePaint)
    {
        qCritical() << "Impossible to resolve rePaint \n error message:"+myConverterLib->errorString();
        return;
    };

    qInfo()<<"Initialize the library";
    ok = initDLL();
    if (!ok)
    {
        qWarning("Something goes really wrong");
        return;
    }
    fillUnitFamillyList= (giveUnitFamillyNameAndIndexesPrototype) myConverterLib->resolve("giveUnitFamillyNameAndIndexes");
    if (!fillUnitFamillyList)
    {
        qCritical() << "Impossible to resolve fillUnitTypeList \n error message:"+myConverterLib->errorString();
        return;
    };




    fillInputUnitList = (giveInputUnitsByFamillyIndexPrototype)  myConverterLib->resolve("giveInputUnitsByFamillyIndex");
    if (!fillInputUnitList)
    {
        qCritical() << "Impossible to resolve fillInputUnitList \n error message:"+myConverterLib->errorString();
        return;
    };

    fillOutputUnitList = (giveOutputUnitsByFamillyIndexPrototype)  myConverterLib->resolve("giveOutputUnitsByFamillyIndex");
    if (!fillOutputUnitList)
    {
        qCritical() << "Impossible to resolve fillOutputUnitList \n error message:"+myConverterLib->errorString();
        return;
    };

    convertToSi           =  (ConvertUserToSiPrototype) myConverterLib->resolve("ConvertUserToSi");
    if (!convertToSi)
    {
        qCritical() << "Impossible to resolve ConvertUserToSi \n error message:"+myConverterLib->errorString();
        return;
    };
    convertFromSi         =  (ConvertSiToUserPrototype) myConverterLib->resolve("ConvertSiToUser");
    if (!convertFromSi)
    {
        qCritical() << "Impossible to resolve ConvertSiToUser \n error message:"+myConverterLib->errorString();
        return;
    };
    convertFromUnitToUnit =  (ConvertValuePrototype)    myConverterLib->resolve("ConvertValue");
    if (!convertFromUnitToUnit)
    {
        qCritical() << "Impossible to resolve ConvertValue \n error message:"+myConverterLib->errorString();
        return;
    };
    showTestForm          = (showTestFormPrototype) myConverterLib->resolve("ShowTestForm");
    if (!showTestForm)
    {
        qCritical() << "Impossible to resolve ShowTestForm \n error message:"+myConverterLib->errorString();
        return;
    };
}

QList<QPair<QString, int>> QUnitDispatcher::getUnitFamillyAndIndexList()
{
    QString unitsTypeAsCsv = shortStringToQString(fillUnitFamillyList());
    QString str;
    this->unitFamillyAndIndexList.clear();
    QTextStream sIn(&unitsTypeAsCsv);
    while (!sIn.atEnd())
      {
        str=sIn.readLine();
        setFamillyListFromCsvString(str,";");
      }
    return unitFamillyAndIndexList;
}

void QUnitDispatcher::setUnitFamillyAndIndexList(const QList<QPair<QString, int> > &newUnitFamillyAndIndexList)
{
    if (unitFamillyAndIndexList == newUnitFamillyAndIndexList)
        return;
    unitFamillyAndIndexList = newUnitFamillyAndIndexList;
    Q_EMIT unitFamillyAndIndexListChanged();
}

QList<QString> QUnitDispatcher::getInputUnitsList()
{
   return inputUnitsList;
}


void QUnitDispatcher::setInputUnitsList(const QList<QString> &newInputUnitsList)
{
    if (inputUnitsList == newInputUnitsList)
        return;
    inputUnitsList = newInputUnitsList;
    Q_EMIT inputUnitsListChanged();
}

QList<QString> QUnitDispatcher::getInputList(int anIndex)
{
    inputUnitsList.clear();
    QString units = shortStringToQString(fillInputUnitList(anIndex));
    QTextStream sIn(&units);
    while (!sIn.atEnd())
      {
        inputUnitsList.append(sIn.readLine());
      }
    return inputUnitsList;
}

QList<QString> QUnitDispatcher::getOutputList(int anIndex)
{
    outputUnitsList.clear();
    QString units = shortStringToQString(fillOutputUnitList(anIndex));
    QTextStream sIn(&units);
    while (!sIn.atEnd())
      {
        outputUnitsList.append(sIn.readLine());
      }
    return outputUnitsList;
}

double QUnitDispatcher::convToSiFromUnit(double aValue, QString aUnit)
{
    char * charBuffer = qStringToShortString(aUnit);
    return convertToSi(aValue,charBuffer);
}

double QUnitDispatcher::convFromSiToUnit(double aValue, QString aUnit)
{
    char * charBuffer = qStringToShortString(aUnit);
    return convertFromSi(aValue,charBuffer);
}

double QUnitDispatcher::convFromUnitToUnit(double aValue, double aTvdValue, QString inputUnit, QString outputUnit, int aFamillyIndex)
{
  char * inUnitCharBuffer  = qStringToShortString(inputUnit);
  char * outUnitCharBuffer = qStringToShortString(outputUnit);
  return convertFromUnitToUnit(aValue,aTvdValue,inUnitCharBuffer,outUnitCharBuffer,aFamillyIndex);
}

void QUnitDispatcher::setFamillyListFromCsvString(QString aCsvString, QString aSeparator)
{
    QStringList strList;
    QPair<QString,int> result;
    strList  = aCsvString.split(aSeparator);
    result.first  = strList[0];
    result.second = strList[1].toInt();
    unitFamillyAndIndexList.append(result);
}


