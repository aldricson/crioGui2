#include "QSizeFWidget.h"

QSizeFWidget::QSizeFWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create widgets
    QLabel* widthLabel = new QLabel("Width", this);
    QLabel* heightLabel = new QLabel("Height", this);
    widthEdit = new QLineEdit(this);
    heightEdit = new QLineEdit(this);

    // Set validators
    widthEdit->setValidator(new QDoubleValidator(this));
    heightEdit->setValidator(new QDoubleValidator(this));

    // Create layout and add widgets
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(widthLabel, 0, 0);
    layout->addWidget(heightLabel, 0, 1);
    layout->addWidget(widthEdit, 1, 0);
    layout->addWidget(heightEdit, 1, 1);

    setLayout(layout);
}

QSizeF QSizeFWidget::getQSizeF()
{
    double width = widthEdit->text().toDouble();
    double height = heightEdit->text().toDouble();
    return QSizeF(width, height);
}

void QSizeFWidget::setQSizeF(const QSizeF& sizeF)
{
    widthEdit->setText(QString::number(sizeF.width()));
    heightEdit->setText(QString::number(sizeF.height()));
}
