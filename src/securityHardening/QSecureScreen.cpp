#include "QSecureScreen.h"
#include <QGuiApplication>


QSecureScreen::QSecureScreen(QWidget *parent) : QWidget(parent)
{    
    m_screenList = QGuiApplication::screens();
    for (auto aScreen : m_screenList)
    {
       QWidget *aWidget = new QWidget();
       aWidget->setGeometry(aScreen->geometry());
       setBackGround(aWidget,aScreen->grabWindow(0));
       aWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
       aWidget->showFullScreen();
       m_widgetList.append(aWidget);
    }

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(10);
    connect (m_refreshTimer, &QTimer::timeout, this, &QSecureScreen::onTimeOut, Qt::QueuedConnection);
    m_refreshTimer->start();
}

QSecureScreen::~QSecureScreen()
{
    m_refreshTimer->stop();
    disconnect (m_refreshTimer, 0);
    delete (m_refreshTimer);
    m_refreshTimer = nullptr;
    for (int i = m_widgetList.count() - 1; i >= 0; --i)
    {
        delete m_widgetList[i];
        m_widgetList[i] = nullptr; // Nullify the pointer after deletion
    }
    m_widgetList.clear();
}



void QSecureScreen::setBackGround(QWidget *aWidget, QPixmap aPixmap)
{
    QLabel *backgroundLabel = new QLabel(aWidget);
    backgroundLabel->setGeometry(aWidget->geometry());
    backgroundLabel->setPixmap(aPixmap);
    backgroundLabel->setScaledContents(false);
}

void QSecureScreen::onTimeOut()
{
    m_refreshTimer->stop();
    for (auto aWidget : m_widgetList)
    {
        aWidget->raise();
        aWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
        aWidget->update();
    }
    m_refreshTimer->start();
}
