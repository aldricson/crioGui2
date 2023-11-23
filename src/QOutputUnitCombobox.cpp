#include "QOutputUnitCombobox.h"






QOutputUnitCombobox::QOutputUnitCombobox(QWidget *parent,
                                         int aUnitFamillyIndex,
                                         QString unitName)
{
    this->setParent(parent);
    onUnitFamilyChanged(aUnitFamillyIndex);
    this->blockSignals(true);
    this->setCurrentText(unitName);
    setCurrentUnit(unitName);
    setDefaultUnit(unitName);
    setDefaultFamillyIndex(aUnitFamillyIndex);
    oldUnit=unitName;
    connect (this,&QComboBox::currentIndexChanged,this,&QOutputUnitCombobox::onCurrentUnitChanged);
    this->blockSignals(false);
}

QOutputUnitCombobox::~QOutputUnitCombobox()
{

}

int QOutputUnitCombobox::getRealWidth() const
{
    return realWidth;
}

void QOutputUnitCombobox::setRealWidth(int newRealWidth)
{
    if (realWidth == newRealWidth)
        return;
    realWidth = newRealWidth;
    emit realWidthChanged();
}

const QString &QOutputUnitCombobox::getDefaultUnit() const
{
    return defaultUnit;
}

void QOutputUnitCombobox::setDefaultUnit(const QString &newDefaultUnit)
{
    if (defaultUnit == newDefaultUnit)
        return;
    defaultUnit = newDefaultUnit;
    emit defaultUnitChanged();
}

const QString &QOutputUnitCombobox::getCurrentUnit() const
{
    return currentUnit;
}

void QOutputUnitCombobox::setCurrentUnit(const QString &newCurrentUnit)
{
    if (currentUnit == newCurrentUnit)
        return;
    currentUnit = newCurrentUnit;
    emit currentUnitChanged();
}

int QOutputUnitCombobox::getDefaultFamillyIndex() const
{
    return defaultFamillyIndex;
}

void QOutputUnitCombobox::setDefaultFamillyIndex(int newDefaultFamillyIndex)
{
    if (defaultFamillyIndex == newDefaultFamillyIndex)
        return;
    defaultFamillyIndex = newDefaultFamillyIndex;
    emit defaultFamillyIndexChanged();
}

int QOutputUnitCombobox::getHeaderIndex() const
{
    return HeaderIndex;
}

void QOutputUnitCombobox::setHeaderIndex(int newHeaderIndex)
{
    if (HeaderIndex == newHeaderIndex)
        return;
    HeaderIndex = newHeaderIndex;
    emit HeaderIndexChanged();
}

int QOutputUnitCombobox::getHeaderColIndex() const
{
    return headerColIndex;
}

void QOutputUnitCombobox::setHeaderColIndex(int newHeaderColIndex)
{
    if (headerColIndex == newHeaderColIndex)
        return;
    headerColIndex = newHeaderColIndex;
    emit headerColIndexChanged();
}

void QOutputUnitCombobox::onCurrentUnitChanged()
{
    QString newText = this->currentText();
    setCurrentUnit(newText);
    if (oldUnit!=newText)
    {
        emit textChangedSignal(oldUnit, newText );
        oldUnit = newText;
    }
}


void QOutputUnitCombobox::onUnitFamilyChanged(int index)
{
    qInfo()<<"output";
    unitDispatcher=new QUnitDispatcher;
    this->clear();
    QList<QString> lou = unitDispatcher->getOutputList(index);
    qInfo()<<lou<<"index :"<<index;
    for (int i=0;i<lou.count();++i)
    {
        this->addItem(lou[i]);
    }
    delete unitDispatcher;
    setRealWidth (this->minimumSizeHint().width());
}


