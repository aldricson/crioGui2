#ifndef QSIZEWIDGET_H
#define QSIZEWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSize>
#include <QValidator>

class QSizeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QSizeWidget(QWidget* parent = nullptr);
    QSize getQSize();
    void setQSize(const QSize& size);

private:
    QLineEdit* widthEdit;
    QLineEdit* heightEdit;
};

#endif
