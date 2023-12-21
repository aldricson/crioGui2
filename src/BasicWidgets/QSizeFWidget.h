#ifndef QSIZEFWIDGET_H
#define QSIZEFWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSizeF>
#include <QValidator>

class QSizeFWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QSizeFWidget(QWidget* parent = nullptr);
    QSizeF getQSizeF();
    void setQSizeF(const QSizeF& sizeF);

private:
    QLineEdit* widthEdit;
    QLineEdit* heightEdit;
};

#endif
