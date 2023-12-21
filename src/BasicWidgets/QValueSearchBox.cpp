#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "QValueSearchBox.h"
#include "BasicWidgets/QBetterSwitchButton.h"

QValueSearchBox::QValueSearchBox(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(300);
    layout = new QGridLayout(this);
    createVisuals();
}
QValueSearchBox::~QValueSearchBox()
{

}

void QValueSearchBox::onSearchBtnClicked()
{
    double minValue = minLineEdit->text().toDouble();
    double maxValue;
    searchType == between ? maxValue=maxLineEdit->text().toDouble() :
                            maxValue=-995.95;
    emit searchAskedSignal(searchType,minValue,maxValue,allOrColButton->getState());
}

void QValueSearchBox::createVisuals()
{

  layoutGb = new QGridLayout();
  searchTypeCb = new QComboBox(this);
  searchTypeCb->addItems(EnumHelper::toStringList<NumericSearchType>());
  searchTypeCb->setCurrentIndex(searchType);
  groupBox = new QGroupBox(this);
  groupBox->setTitle("Search");
  groupBox->setLayout(layoutGb);
  layout->addWidget(groupBox);
  minLabel = new QLabel(this);
  minLabel->setText("min value");
  minLineEdit = new QLineEdit(this);
  maxLabel = new QLabel(this);
  maxLabel->setText("max value");
  maxLineEdit = new QLineEdit(this);
  searchBtn = new QPushButton(this);
  searchBtn->setText("Search");
  searchBtn->setIcon(QIcon(":/images/ressources/images/search.png"));
  allOrColButton = new QBetterSwitchButton("all",
                                           "col",
                                           QColor(0x98FB98),
                                           QColor(0x6495ED),
                                           Qt::red,
                                           true,
                                           this);
  setStdLayout();

  connect(searchBtn,
          &QPushButton::clicked,
          this,
          &QValueSearchBox::onSearchBtnClicked);

  connect (searchTypeCb,
            &QComboBox::currentIndexChanged,
            this,
            [this]{searchType = EnumHelper::fromString<NumericSearchType>
                                (searchTypeCb->currentText());

                    if (searchType==between)
                    {
                        setBetweenLayout();
                    }
                    else
                    {
                        setStdLayout();
                    }
                  });


}

void QValueSearchBox::setStdLayout()
{
    cleanLayout();
    layoutGb->addWidget(searchTypeCb ,0,0,1,2);
    minLabel->setText("Value:");
    layoutGb->addWidget(minLabel     ,1,0,1,2,Qt::AlignHCenter);
    layoutGb->addWidget(minLineEdit  ,2,0,1,2);
    maxLabel->setVisible(false);
    maxLineEdit->setVisible(false);

    layoutGb->addWidget(allOrColButton, 3,0,1,1,Qt::AlignHCenter);
    layoutGb->addWidget(searchBtn     , 3,1,1,1);
}

void QValueSearchBox::setBetweenLayout()
{
    cleanLayout();
    minLabel->setText       ("min:");
    maxLabel->setText       ("max:");
    maxLabel->setVisible    (true);
    maxLineEdit->setVisible (true);
    layoutGb->addWidget     (searchTypeCb ,0,0,1,4);
    layoutGb->addWidget     (minLabel     ,1,0,1,1,Qt::AlignLeft);
    layoutGb->addWidget     (minLineEdit  ,1,1,1,1);
    layoutGb->addWidget     (maxLabel     ,1,2,1,1,Qt::AlignLeft);
    layoutGb->addWidget     (maxLineEdit  ,1,3,1,1);
    layoutGb->addWidget     (searchBtn    ,2,0,1,4);


}

void QValueSearchBox::cleanLayout()
{
    layoutGb->removeWidget(searchTypeCb );
    layoutGb->removeWidget(minLabel     );
    layoutGb->removeWidget(minLineEdit  );
    layoutGb->removeWidget(maxLabel     );
    layoutGb->removeWidget(maxLineEdit  );
    layoutGb->removeWidget(searchBtn    );
}
