#include "QModbusAnalogViewer.h"
#include <QScrollArea>
#include <QFile>
#include <QScrollBar>

QModbusAnalogViewer::QModbusAnalogViewer(QWidget *parent)
    : QWidget(parent), tabWidget(new QTabWidget(this))
{
    // Set up the layouts for the two ranges
    layout0to63   = new QGridLayout();
    layout64to127 = new QGridLayout();
    layout0to63 ->setSpacing(0);
    layout64to127->setSpacing(0);

    // Create and add channel viewers to the vector and layouts
    for (int i = 0; i < 128; ++i)
    {
        QModbusAnalogChannelViewer *viewer = new QModbusAnalogChannelViewer(i, this);
        m_channelViewers.append(viewer);

        if (i < 64)
        {
            layout0to63->addWidget(viewer, i / 8, i % 8); // 8 channels per row
        }
        else
        {
            layout64to127->addWidget(viewer, (i - 64) / 8, (i - 64) % 8); // 8 channels per row
        }
    }

    // Create scroll areas for the layouts
    scrollArea64to127 = new QScrollArea(this);

    QFile file(":/styles/scrollArea.scss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    widget0to63   = new QWidget(this);
    widget0to63->setProperty("mustDarken",true);
    widget0to63->setStyleSheet(content);
    widget0to63->ensurePolished();
    widget0to63->update();

    widget64to127 = new QWidget(this);
    widget64to127->setProperty("mustDarken",true);
    widget64to127->setStyleSheet(content);

    scrollArea64to127->setStyleSheet(content);
    scrollArea0to63   = new QScrollArea(this);
    scrollArea0to63->setStyleSheet(content);



    widget0to63   -> setLayout (layout0to63)  ;
    widget64to127 -> setLayout (layout64to127);

    scrollArea0to63   -> setWidget          (widget0to63)  ;
    scrollArea0to63   -> setWidgetResizable (true)         ;
    scrollArea64to127 -> setWidget          (widget64to127);
    scrollArea64to127 -> setWidgetResizable (true)         ;


    // Add scroll areas to the tabs
    tabWidget->addTab(scrollArea0to63  , "Channels 0 to 63"  );
    tabWidget->addTab(scrollArea64to127, "Channels 64 to 127");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void QModbusAnalogViewer::resetOffset(int offSet)
{
    for (int i = 0; i < 128; ++i)
    {
        m_channelViewers[i]->getChannelLabel()->setText(QString("Channel: %1").arg(i + offSet));
    }
    // Update the titles of the tabs
    tabWidget->setTabText(0, QString("Channels %1 to %2").arg(offSet).arg(offSet + 63));
    tabWidget->setTabText(1, QString("Channels %1 to %2").arg(offSet + 64).arg(offSet + 127));
    m_offset = offSet;
}

void QModbusAnalogViewer::onMapValuesToInterface(const QVector<quint16> &data)
{
    for (int i=0;i<data.count();++i)
    {
        m_channelViewers[i]->getValueLabel()->setText(QString::number(data[i]));
    }
}

void QModbusAnalogViewer::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

const QVector<QModbusAnalogChannelViewer *> &QModbusAnalogViewer::channelViewers() const
{
    return m_channelViewers;
}

void QModbusAnalogViewer::setChannelViewers(const QVector<QModbusAnalogChannelViewer *> &newChannelViewers)
{
    if (m_channelViewers == newChannelViewers)
        return;
    m_channelViewers = newChannelViewers;
    emit channelViewersChanged();
}
