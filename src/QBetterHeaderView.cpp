#include "QBetterHeaderView.h"

QBetterHeaderView::QBetterHeaderView(Qt::Orientation orientation, QWidget * parent)
    : QHeaderView(orientation, parent)
{
    // Initialization code here, if necessary
}

bool QBetterHeaderView::getIsUnitConverter() const
{
    return isUnitConverter;
}

void QBetterHeaderView::setIsUnitConverter(bool newIsUnitConverter)
{
    if (isUnitConverter == newIsUnitConverter)
        return;
    isUnitConverter = newIsUnitConverter;
    emit isUnitConverterChanged();
}

bool QBetterHeaderView::getIsStdLineEdit() const
{
    return isStdLineEdit;
}

void QBetterHeaderView::setIsStdLineEdit(bool newIsStdLineEdit)
{
    if (isStdLineEdit == newIsStdLineEdit)
        return;
    isStdLineEdit = newIsStdLineEdit;
    emit isStdLineEditChanged();
}
