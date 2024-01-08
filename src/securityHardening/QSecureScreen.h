#ifndef QSECURESCREEN_H
#define QSECURESCREEN_H

#include <QWidget>
#include <QScreen>
#include <QList>
#include <QPixmap>
#include <QColor>
#include <QLabel>
#include <QTimer>

class QSecureScreen : public QWidget
{
    Q_OBJECT

public:
    explicit QSecureScreen(QWidget *parent = nullptr);
    ~QSecureScreen();

private:
    QList<QScreen*> m_screenList;
    QList<QWidget*> m_widgetList;
    QTimer *m_refreshTimer;
    void setBackGround(QWidget *aWidget, QPixmap aPixmap);

private slots:
    void onTimeOut();

};

#endif // QSECURESCREEN_H
