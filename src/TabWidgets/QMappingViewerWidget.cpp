#include "QMappingViewerWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include "BasicWidgets/QMultiLineTextVisualizer.h"
#include "TabWidgetFundations/ModbusMaping/QMappingRowWidget.h"


QMappingViewerWidget::QMappingViewerWidget(QString modbusMappingPath, QCrioModulesDataExtractor *moduleExtractor, QWidget *parent)
    : QWidget(parent), selectedRow(nullptr)
{
    m_extractor = moduleExtractor;
    m_modbusMappingPath = modbusMappingPath;
    setupUi();
    loadStyleStringFromResource();
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

    reloadBtn = new QPushButton("(Re)Load", this);
    connect (reloadBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onLoad);

    saveBtn = new QPushButton("Save", this); // Placeholder button
    connect(saveBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onSave);

    uploadBtn = new QPushButton("Upload", this); // Placeholder button

    // Create a horizontal layout for the buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(insertBeforeBtn);
    buttonsLayout->addWidget(insertAfterBtn);
    buttonsLayout->addWidget(addBtn);
    buttonsLayout->addStretch(); // This will push all the buttons to the left
    buttonsLayout->addWidget(reloadBtn);
    buttonsLayout->addWidget(saveBtn);
    buttonsLayout->addWidget(uploadBtn);
    QWidget *btnContainer = new QWidget(this);
    btnContainer->setLayout(buttonsLayout);

    // Set the container widget to be the scroll area's widget
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);

    csvVizualizer = new QMultiLineTextVisualizer(this);

    // Layout for the whole widget, including buttons and the scroll area
    QGridLayout *mainLayout = new QGridLayout(this);
    // Ensure the layout does not have extra space that expands
    mainLayout->addWidget(scrollArea    ,0,0,1,1);

    mainLayout->addWidget(btnContainer  ,2,0,1,1); // Add buttons layout at the bottom

    setLayout(mainLayout);
    mainLayout->addWidget(csvVizualizer ,1,0,1,1);
}


QMappingRowWidget* QMappingViewerWidget::createRowWidget()
{
    QMappingRowWidget *row = new QMappingRowWidget(m_extractor,containerWidget);
    m_extractor->extractAllModules(row->moduleAliasCB());


    connect(row , &QMappingRowWidget::rowSelectedSignal , this , &QMappingViewerWidget::onSelectRow   );
    connect(row , &QMappingRowWidget::deleteRowSignal   , this , &QMappingViewerWidget::onDeleteRow   );
    connect(row , &QMappingRowWidget::moveUpRowSignal   , this , &QMappingViewerWidget::onMoveRowUp   );
    connect(row , &QMappingRowWidget::moveDownRowSignal , this , &QMappingViewerWidget::onMoveRowDown );

    containerLayout->addWidget(row);
    rowWidgets.append(row);

    // Make sure the filler widget is still at the end and takes up all remaining space
    containerLayout->removeWidget(fillerWidget);
    containerLayout->addWidget(fillerWidget);

    // Force an update of the container widget's geometry
    containerWidget->updateGeometry();

    // Use a single-shot timer to delay the call to ensureWidgetVisible
    QTimer::singleShot(50, this, [this, row]() {
        scrollArea->ensureWidgetVisible(row);

    });

    return row;
}


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

void QMappingViewerWidget::onLoad()
{
    QString fileName = m_modbusMappingPath + "mapping.csv";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handle error (e.g., file not found or unable to open)
        // TODO show a message to the user or log the error
        return;
    }
    deleteAllRows();
    csvVizualizer->clearText();
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();
    // Now fileContent contains the entire contents of the CSV file
    static const QRegularExpression re("\r\n|\n|\r");
    QStringList lines = fileContent.split(re, Qt::SkipEmptyParts);
    for (int i=0;i<lines.count();++i)
    {
        int clrCode;
        i%2==0 ? clrCode=0x87CEEB : clrCode=0x00BFFF;
        csvVizualizer->appendTextWithColor(lines[i],QColor(clrCode),false);
        auto row = createRowWidget();
        QStringList fields = lines[i].split(";");
        QTimer::singleShot(50, this, [row]()
        {
            row->blockAllSignals(true);

        });

        row->setIndex(fields[0].toInt());
        row->moduleTypeCB()->setCurrentIndex(fields[1].toInt());
        row->onModuleTypeChanged(row->moduleTypeCB()->currentIndex());
        int aliasIndex = row->moduleAliasCB()->findText(fields[2]);
        row->moduleAliasCB()->setCurrentIndex(aliasIndex);
        row->onModuleAliasChanged(aliasIndex);
        int channelIndex = row->moduleChannelCB()->findText(fields[3]);
        row->moduleChannelCB () ->setCurrentIndex(channelIndex);
        row->minSrcValueLE   () ->setText(fields[4]);
        row->maxSrcValueLE   () ->setText(fields[5]);
        row->minDestLE       () ->setText(fields[6]);
        row->maxDestLE       () ->setText(fields[7]);
        row->destChannelLE   () ->setText(fields[8]);
        QTimer::singleShot(50, this, [row]()
        {
            row->blockAllSignals(false);

        });
    }
}

void QMappingViewerWidget::onSave()
{
    checkFields();
}


void QMappingViewerWidget::onSelectRow(QMappingRowWidget *row)
{
    if (selectedRow) {
        selectedRow->setStyleSheet(""); // Remove style from previously selected row
    }
    selectedRow = row;
    selectedRow->setStyleSheet("border: 2px solid blue;"); // Highlight new selected row
}

void QMappingViewerWidget::onDeleteRow(QMappingRowWidget *row)
{
    int index = row->indexLabel()->text().toInt();
    //remove it form layout
    containerLayout->removeWidget(row);
    //disconnect it
    disconnect (row,0);
    //remove it from the list
    rowWidgets.remove(index);
    //delete the object
    delete (row);
    //null it
    row = nullptr;
    // Make sure the filler widget is still at the end and takes up all remaining space
    containerLayout->removeWidget(fillerWidget);
    containerLayout->addWidget(fillerWidget);
    //reindex everybody
    updateRowIndices();
    // Force an update of the container widget's geometry
    containerWidget->updateGeometry();
    // Use a single-shot timer to delay the call to ensureWidgetVisible
    QTimer::singleShot(0,this,[this]()
    {
        scrollArea->ensureWidgetVisible(rowWidgets.last());
    });
}

void QMappingViewerWidget::onMoveRowUp(QMappingRowWidget *row)
{
    if (!row || rowWidgets.size() < 2) return; // Check if row is valid and there are at least two rows

    int index = rowWidgets.indexOf(row);
    if (index <= 0) return; // Check if it's not the first row

    // Remove the rows from the layout
    containerLayout->removeWidget(rowWidgets[index]);
    containerLayout->removeWidget(rowWidgets[index - 1]);

    // Swap the rows in the list
    std::swap(rowWidgets[index], rowWidgets[index - 1]);

    // Re-add the swapped rows to the layout in their new positions
    containerLayout->insertWidget(index - 1, rowWidgets[index - 1]);
    containerLayout->insertWidget(index, rowWidgets[index]);

    // Update the indices of the rows
    updateRowIndices();
}

void QMappingViewerWidget::onMoveRowDown(QMappingRowWidget *row)
{
    if (!row || rowWidgets.size() < 2) return; // Check if row is valid and there are at least two rows

    int index = rowWidgets.indexOf(row);
    if (index == -1 || index >= rowWidgets.size() - 1) return; // Check if it's not the last row

    // Remove the rows from the layout
    containerLayout->removeWidget(rowWidgets[index]);
    containerLayout->removeWidget(rowWidgets[index + 1]);

    // Swap the rows in the list
    std::swap(rowWidgets[index], rowWidgets[index + 1]);

    // Re-add the swapped rows to the layout in their new positions
    containerLayout->insertWidget(index, rowWidgets[index]);
    containerLayout->insertWidget(index + 1, rowWidgets[index + 1]);

    // Update the indices of the rows
    updateRowIndices();
}


void QMappingViewerWidget::deleteAllRows()
{
    // Iterate backwards through the list to safely delete each widget
    for (int i = rowWidgets.size() - 1; i >= 0; --i) {
        QMappingRowWidget* row = rowWidgets[i];

        // Remove the widget from the layout
        containerLayout->removeWidget(row);

        // Disconnect all signals connected to the row widget
        row->disconnect();

        // Delete the row widget
        delete row;
    }

    // Clear the rowWidgets list
    rowWidgets.clear();

    // Make sure the filler widget is still at the end and takes up all remaining space
    containerLayout->removeWidget(fillerWidget);
    containerLayout->addWidget(fillerWidget);

    // Reset the selectedRow pointer as all rows are now deleted
    selectedRow = nullptr;

    // Force an update of the container widget's geometry
    containerWidget->updateGeometry();
}

void QMappingViewerWidget::blockAllSignals(bool blocked)
{
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        row->blockAllSignals(blocked);
    }
    insertBeforeBtn ->blockSignals(blocked);
    insertAfterBtn  ->blockSignals(blocked);
    addBtn          ->blockSignals(blocked);
    reloadBtn       ->blockSignals(blocked);
    saveBtn         ->blockSignals(blocked);
    uploadBtn       ->blockSignals(blocked);
}

void QMappingViewerWidget::enableAllControls(bool enabled)
{
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        row->enableAllControls(enabled);
    }

    insertBeforeBtn ->setEnabled(enabled);
    insertAfterBtn  ->setEnabled(enabled);
    addBtn          ->setEnabled(enabled);
    reloadBtn       ->setEnabled(enabled);
    saveBtn         ->setEnabled(enabled);
    uploadBtn       ->setEnabled(enabled);
}

bool QMappingViewerWidget::checkFields()
{
    bool result = true;
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        if (row)
        {
            QTimer::singleShot(50,this,[row]{row->blockAllSignals(true);});

            if (row->destChannelLE()->text().isEmpty())
            {
                row->destChannelLE()->setProperty("inError", true);
                result = false;
            }
            else
            {
                row->destChannelLE()->setProperty("inError", false);
            }

            row->destChannelLE()->style()->unpolish(row->destChannelLE());
            row->destChannelLE()->style()->polish(row->destChannelLE());
            row->destChannelLE()->update();

            if (row->maxDestLE()->text().isEmpty())
            {
                row->maxDestLE()->setProperty("inError", true);
                result = false;
            }
            else
            {
                row->maxDestLE()->setProperty("inError", false);
            }

            row->maxDestLE()->style()->unpolish(row->maxDestLE());
            row->maxDestLE()->style()->polish(row->maxDestLE());
            row->maxDestLE()->update();

            if (row->minDestLE()->text().isEmpty())
            {
                row->minDestLE()->setProperty("inError", true);
                result = false;
            }
            else
            {
                row->minDestLE()->setProperty("inError", false);
            }

            row->minDestLE()->style()->unpolish(row->minDestLE());
            row->minDestLE()->style()->polish(row->minDestLE());
            row->minDestLE()->update();

            QTimer::singleShot(50,this,[row]{row->blockAllSignals(false);});
        }
    }
    return result;
}


void QMappingViewerWidget::loadStyleStringFromResource()
{
        QFile file(":/styles/darkScrollBar.scss");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        this->scrollArea->verticalScrollBar()->setStyleSheet(content);
}


