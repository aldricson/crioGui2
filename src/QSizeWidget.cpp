#include "QSizeWidget.h"

QSizeWidget::QSizeWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create widgets
    QLabel* widthLabel = new QLabel("Width", this);
    QLabel* heightLabel = new QLabel("Height", this);
    widthEdit = new QLineEdit(this);
    widthEdit->setMaximumWidth(80);
    heightEdit = new QLineEdit(this);
    heightEdit->setMaximumWidth(80);

    // Set validators
    widthEdit->setValidator(new QIntValidator(this));
    heightEdit->setValidator(new QIntValidator(this));

    // Create layout and add widgets
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(widthLabel, 0, 0);
    layout->addWidget(heightLabel, 0, 1);
    layout->addWidget(widthEdit, 1, 0);
    layout->addWidget(heightEdit, 1, 1);

    setLayout(layout);
}

QSize QSizeWidget::getQSize()
{
    int width = widthEdit->text().toInt();
    int height = heightEdit->text().toInt();
    return QSize(width, height);
}

void QSizeWidget::setQSize(const QSize& size)
{
    widthEdit->setText(QString::number(size.width()));
    heightEdit->setText(QString::number(size.height()));
}
