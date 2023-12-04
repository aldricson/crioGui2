#include "QModbusAnalogViewer.h"
#include <QScrollArea>

QModbusAnalogViewer::QModbusAnalogViewer(QWidget *parent)
    : QWidget(parent), tabWidget(new QTabWidget(this))
{
    // Set up the layouts for the two ranges
    layout0to63 = new QGridLayout();
    layout64to127 = new QGridLayout();

    // Create and add channel viewers to the vector and layouts
    for (int i = 0; i < 128; ++i) {
        QModbusAnalogChannelViewer *viewer = new QModbusAnalogChannelViewer(i, this);
        channelViewers.append(viewer);

        if (i < 64) {
            layout0to63->addWidget(viewer, i / 8, i % 8); // 8 channels per row
        } else {
            layout64to127->addWidget(viewer, (i - 64) / 8, (i - 64) % 8); // 8 channels per row
        }
    }

    // Create scroll areas for the layouts
    QScrollArea *scrollArea0to63 = new QScrollArea(this);
    QScrollArea *scrollArea64to127 = new QScrollArea(this);

    QWidget *widget0to63 = new QWidget();
    QWidget *widget64to127 = new QWidget();
    widget0to63->setLayout(layout0to63);
    widget64to127->setLayout(layout64to127);

    scrollArea0to63->setWidget(widget0to63);
    scrollArea0to63->setWidgetResizable(true);
    scrollArea64to127->setWidget(widget64to127);
    scrollArea64to127->setWidgetResizable(true);

    // Add scroll areas to the tabs
    tabWidget->addTab(scrollArea0to63, "Channels 0 to 63");
    tabWidget->addTab(scrollArea64to127, "Channels 64 to 127");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void QModbusAnalogViewer::resetOffset(int offSet)
{
    for (int i = 0; i < 128; ++i) {
        channelViewers[i]->getChannelLabel()->setText(QString("Channel: %1").arg(i + offSet));
    }

    // Update the titles of the tabs
    tabWidget->setTabText(0, QString("Channels %1 to %2").arg(offSet).arg(offSet + 63));
    tabWidget->setTabText(1, QString("Channels %1 to %2").arg(offSet + 64).arg(offSet + 127));
}
