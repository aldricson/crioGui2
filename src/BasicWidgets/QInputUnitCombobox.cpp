#include "QInputUnitCombobox.h"



QInputUnitCombobox::QInputUnitCombobox(QWidget *parent, int aUnitFamillyIndex, QString unitName)
{
    this->setParent(parent);
    onUnitFamilyChanged(aUnitFamillyIndex);
    this->blockSignals(true);
    this->setCurrentText(unitName);
    setCurrentUnit(unitName);
    setDefaultUnit(unitName);
    setDefaultFamillyIndex(aUnitFamillyIndex);
    oldUnit=unitName;
    connect (this,&QComboBox::currentIndexChanged,this,&QInputUnitCombobox::onCurrentUnitChanged);
    this->blockSignals(false);
}



QInputUnitCombobox::~QInputUnitCombobox()
{

}

void QInputUnitCombobox::onUnitFamilyChanged(int index)
{
    unitDispatcher=new QUnitDispatcher;
    this->clear();
    QList<QString> liu = unitDispatcher->getInputList(index);
    for (int i=0;i<liu.count();++i)
    {
        this->addItem(liu[i]);
    }
    delete unitDispatcher;
    setRealWidth (this->minimumSizeHint().width());
}

void QInputUnitCombobox::onCurrentUnitChanged()
{
    QString newText = this->currentText();
    setCurrentUnit(newText);
    if (oldUnit!=newText)
    {
        emit textChangedSignal(oldUnit, newText );
        oldUnit = newText;
    }
}

const QString &QInputUnitCombobox::getCurrentUnit() const
{
    return currentUnit;
}

void QInputUnitCombobox::setCurrentUnit(const QString &newCurrentUnit)
{
    if (currentUnit == newCurrentUnit)
        return;
    currentUnit = newCurrentUnit;
    emit currentUnitChanged();
}

const QString &QInputUnitCombobox::getDefaultUnit() const
{
    return defaultUnit;
}

void QInputUnitCombobox::setDefaultUnit(const QString &newDefaultUnit)
{
    if (defaultUnit == newDefaultUnit)
        return;
    defaultUnit = newDefaultUnit;
    emit defaultUnitChanged();
}

const QString &QInputUnitCombobox::getOldUnit() const
{
    return oldUnit;
}

void QInputUnitCombobox::setOldUnit(const QString &newOldUnit)
{
    if (oldUnit == newOldUnit)
        return;
    oldUnit = newOldUnit;
    emit oldUnitChanged();
}

int QInputUnitCombobox::getDefaultFamillyIndex() const
{
    return defaultFamillyIndex;
}

void QInputUnitCombobox::setDefaultFamillyIndex(int newDefaultFamillyIndex)
{
    if (defaultFamillyIndex == newDefaultFamillyIndex)
        return;
    defaultFamillyIndex = newDefaultFamillyIndex;
    emit defaultFamillyIndexChanged();
}

int QInputUnitCombobox::getRealWidth() const
{
    return realWidth;
}

void QInputUnitCombobox::setRealWidth(int newRealWidth)
{
    if (realWidth == newRealWidth)
        return;
    realWidth = newRealWidth;
    emit realWidthChanged();
}
