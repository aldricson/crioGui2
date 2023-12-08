#include "QMappingViewerWidget.h"
#include "QMappingRowWidget.h"

QMappingViewerWidget::QMappingViewerWidget(QWidget *parent)
    : QWidget(parent), selectedRow(nullptr)
{
    setupUi();
}

QMappingViewerWidget::~QMappingViewerWidget() {}

void QMappingViewerWidget::setupUi()
{
    // Initialize the scroll area and its container
    scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    containerWidget = new QWidget();
    containerLayout = new QVBoxLayout(containerWidget);

    containerLayout->setSpacing(0); // Spacing between widgets
    containerLayout->setContentsMargins(0, 0, 0, 0);

    fillerWidget = new QWidget();
    QPalette pal = fillerWidget->palette();
    pal.setColor(fillerWidget->backgroundRole(), Qt::black);
    fillerWidget->setPalette(pal);
    fillerWidget->setAutoFillBackground(true);
    containerLayout->addWidget(fillerWidget);

    // Set up the buttons as per the mockup
    insertBeforeBtn = new QPushButton("Insert Before", this);
    connect(insertBeforeBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onInsertBefore);

    insertAfterBtn = new QPushButton("Insert After", this);
    connect(insertAfterBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onInsertAfter);

    addBtn = new QPushButton("Add", this);
    connect(addBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onAdd);

    QPushButton *reloadBtn = new QPushButton("(Re)Load", this); // Placeholder button
    QPushButton *saveBtn = new QPushButton("Save", this); // Placeholder button
    QPushButton *uploadBtn = new QPushButton("Upload", this); // Placeholder button

    // Create a horizontal layout for the buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(insertBeforeBtn);
    buttonsLayout->addWidget(insertAfterBtn);
    buttonsLayout->addWidget(addBtn);
    buttonsLayout->addStretch(); // This will push all the buttons to the left
    buttonsLayout->addWidget(reloadBtn);
    buttonsLayout->addWidget(saveBtn);
    buttonsLayout->addWidget(uploadBtn);

    // Set the container widget to be the scroll area's widget
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);

    // Layout for the whole widget, including buttons and the scroll area
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // Ensure the layout does not have extra space that expands
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonsLayout); // Add buttons layout at the bottom

    setLayout(mainLayout);
}


QMappingRowWidget* QMappingViewerWidget::createRowWidget()
{
    QMappingRowWidget *row = new QMappingRowWidget(containerWidget);
    connect(row, &QMappingRowWidget::rowSelectedSignal, this, &QMappingViewerWidget::onSelectRow);
    containerLayout->addWidget(row);
    rowWidgets.append(row);

    // Make sure the filler widget is still at the end and takes up all remaining space
    containerLayout->removeWidget(fillerWidget);
    containerLayout->addWidget(fillerWidget);

    // Force an update of the container widget's geometry
    containerWidget->updateGeometry();

    // Use a single-shot timer to delay the call to ensureWidgetVisible
    QTimer::singleShot(0, [this, row]() {
        scrollArea->ensureWidgetVisible(row);
    });

    return row;
}


/*QMappingRowWidget* QMappingViewerWidget::createRowWidget()
{
    QMappingRowWidget *row = new QMappingRowWidget(containerWidget);
    // Make sure the QMappingRowWidget's layout also has the correct spacing and margins

    connect(row, &QMappingRowWidget::rowSelectedSignal, this, &QMappingViewerWidget::onSelectRow);
    containerLayout->addWidget(row);
    rowWidgets.append(row);

    // Make sure the filler widget is still at the end and takes up all remaining space
    containerLayout->removeWidget(fillerWidget);
    containerLayout->addWidget(fillerWidget);
    scrollArea->ensureWidgetVisible(row);
    return row;
}*/

void QMappingViewerWidget::onInsertBefore()
{
    if (!selectedRow) return;
    int index = rowWidgets.indexOf(selectedRow);
    if (index == -1) return;

    QMappingRowWidget *row = createRowWidget();
    containerLayout->insertWidget(index, row);
    rowWidgets.insert(index, row);
    updateRowIndices();
}

void QMappingViewerWidget::onInsertAfter()
{
    if (!selectedRow) return;
    int index = rowWidgets.indexOf(selectedRow);
    if (index == -1) return;

    QMappingRowWidget *row = createRowWidget();
    containerLayout->insertWidget(index + 1, row);
    rowWidgets.insert(index + 1, row);
    updateRowIndices();
}

void QMappingViewerWidget::updateRowIndices()
{
    for (int i = 0; i < rowWidgets.size(); ++i)
    {
        rowWidgets[i]->setIndex(i); // This sets the internal index, if you have such a function.
    }
}



void QMappingViewerWidget::onAdd()
{
    createRowWidget();
    updateRowIndices();
}

void QMappingViewerWidget::onSelectRow(QMappingRowWidget *row)
{
    if (selectedRow) {
        selectedRow->setStyleSheet(""); // Remove style from previously selected row
    }
    selectedRow = row;
    selectedRow->setStyleSheet("border: 2px solid blue;"); // Highlight new selected row
}
