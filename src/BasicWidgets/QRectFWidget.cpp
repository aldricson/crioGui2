#include "QRectFWidget.h"

QRectFWidget::QRectFWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create widgets
    QLabel* leftLabel = new QLabel("Left:", this);
    QLabel* topLabel = new QLabel("Top:", this);
    QLabel* widthLabel = new QLabel("Width:", this);
    QLabel* heightLabel = new QLabel("Height:", this);
    leftEdit = new QLineEdit(this);
    topEdit = new QLineEdit(this);
    widthEdit = new QLineEdit(this);
    heightEdit = new QLineEdit(this);

    // Set validators
    leftEdit->setValidator(new QDoubleValidator(this));
    topEdit->setValidator(new QDoubleValidator(this));
    widthEdit->setValidator(new QDoubleValidator(this));
    heightEdit->setValidator(new QDoubleValidator(this));

    // Create layout and add widgets
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(leftLabel, 0, 0);
    layout->addWidget(leftEdit, 0, 1);
    layout->addWidget(topLabel, 1, 0);
    layout->addWidget(topEdit, 1, 1);
    layout->addWidget(widthLabel, 2, 0);
    layout->addWidget(widthEdit, 2, 1);
    layout->addWidget(heightLabel, 3, 0);
    layout->addWidget(heightEdit, 3, 1);

    setLayout(layout);
}


QRectF QRectFWidget::rectF() const
{
    double left = leftEdit->text().toDouble();
    double top = topEdit->text().toDouble();
    double width = widthEdit->text().toDouble();
    double height = heightEdit->text().toDouble();
    return QRectF(left, top, width, height);
}

void QRectFWidget::setRectF(const QRectF &rectF)
{
    leftEdit->setText(QString::number(rectF.left()));
    topEdit->setText(QString::number(rectF.top()));
    widthEdit->setText(QString::number(rectF.width()));
    heightEdit->setText(QString::number(rectF.height()));
    emit rectFChanged(rectF);
}
