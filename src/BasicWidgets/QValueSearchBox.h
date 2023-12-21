#ifndef QVALUESEARCHBOX_H
#define QVALUESEARCHBOX_H

#include <QWidget>
#include <QGridLayout>
#include "Globals/globalEnumSpace.h"

using namespace globalEnumSpace;
using namespace EnumHelper;

class QBetterSwitchButton;
class QComboBox          ;
class QGroupBox          ;
class QLabel             ;
class QPushButton        ;
class QLineEdit          ;

class QValueSearchBox : public QWidget
{
    Q_OBJECT
public:
    explicit QValueSearchBox(QWidget *parent = nullptr);
    ~QValueSearchBox();
signals:
    void searchAskedSignal(globalEnumSpace::NumericSearchType searchType,
                           double minValue,
                           double maxValue,
                           bool onCol);


private slots:
    void onSearchBtnClicked();

private:
    QGridLayout         *layout        = nullptr;
    QGridLayout         *layoutGb      = nullptr;
    QComboBox           *searchTypeCb  = nullptr;
    QGroupBox           *groupBox      = nullptr;
    QLabel              *minLabel      = nullptr;
    QLineEdit           *minLineEdit   = nullptr;
    QLabel              *maxLabel      = nullptr;
    QLineEdit           *maxLineEdit   = nullptr;
    QPushButton         *searchBtn     = nullptr;
    QBetterSwitchButton *allOrColButton = nullptr;
    NumericSearchType searchType=NumericSearchType::egal;
    void createVisuals();
    void setStdLayout();
    void setBetweenLayout();
    void cleanLayout();
};

#endif
