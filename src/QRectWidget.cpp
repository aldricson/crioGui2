#include "QRectWidget.h"

QRectWidget::QRectWidget(QWidget* parent)
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
    leftEdit->setValidator(new QIntValidator(this));
    topEdit->setValidator(new QIntValidator(this));
    widthEdit->setValidator(new QIntValidator(this));
    heightEdit->setValidator(new QIntValidator(this));

    // Create layout and add widgets
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(topLabel,   0, 0);  layout->addWidget(leftLabel,   0, 1);
    layout->addWidget(topEdit,    1, 0);  layout->addWidget(leftEdit,    1, 1);
    layout->addWidget(widthLabel, 2, 0);  layout->addWidget(heightLabel, 2, 1);
    layout->addWidget(widthEdit,  3, 0);  layout->addWidget(heightEdit,  3, 1);



    setLayout(layout);
}


QRect QRectWidget::rect() const
{
    int left = leftEdit->text().toInt();
    int top = topEdit->text().toInt();
    int width = widthEdit->text().toInt();
    int height = heightEdit->text().toInt();
    return QRect(left, top, width, height);
}

void QRectWidget::setRect(const QRect &rect)
{
    leftEdit->setText(QString::number(rect.left()));
    topEdit->setText(QString::number(rect.top()));
    widthEdit->setText(QString::number(rect.width()));
    heightEdit->setText(QString::number(rect.height()));
    emit rectChanged(rect);
}
