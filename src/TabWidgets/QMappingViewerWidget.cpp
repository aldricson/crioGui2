#include "QMappingViewerWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QMessageBox>
#include "NetWorking/QSSHCommand.h"
#include "BasicWidgets/QMultiLineTextVisualizer.h"
#include "TabWidgetFundations/ModbusMaping/QMappingRowWidget.h"
#include "TabWidgetFundations/ModbusMaping/QBackUpWidget.h"


QMappingViewerWidget::QMappingViewerWidget(const QString &md5Hash, const QString &executionPath, const QString &modbusMappingPath, QCrioModulesDataExtractor *moduleExtractor, QWidget *parent)
    : QWidget(parent), selectedRow(nullptr)
{
    m_executionPath = executionPath;
    m_extractor = moduleExtractor;
    sshCommand  = new QSSHCommand(md5Hash,m_executionPath,this);
    connect (sshCommand, &QSSHCommand::modbusMappingUploadedSignal, this, &QMappingViewerWidget::onModbusMappingUploaded, Qt::QueuedConnection );
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

    saveBtn = new QPushButton("Save", this);
    connect(saveBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onSave);

    uploadBtn = new QPushButton("Upload", this);
    connect(uploadBtn, &QPushButton::clicked, this, &QMappingViewerWidget::onUpload);

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
    backuWidget   = new QBackUpWidget(this);

    // Layout for the whole widget, including buttons and the scroll area
    QGridLayout *mainLayout = new QGridLayout(this);
    // Ensure the layout does not have extra space that expands
    mainLayout->addWidget(scrollArea    ,0,0,1,2);

    mainLayout->addWidget(btnContainer  ,2,0,1,2); // Add buttons layout at the bottom

    setLayout(mainLayout);
    mainLayout->addWidget(csvVizualizer ,1,0,1,1);
    mainLayout->addWidget(backuWidget   ,1,1,1,1);
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

void QMappingViewerWidget::onModbusMappingUploaded(const QString &output, const QString &lastCommand)
{
    QMessageBox::information(nullptr, "Uploading status:", output, QMessageBox::Ok);
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
    QString destinationFolder   = m_modbusMappingPath + "Backups/";
    backuWidget->analyzeFolder(destinationFolder);
}

bool QMappingViewerWidget::checkAllBeforeLoad(const QStringList& lines)
{
    if (!validateIndices(lines)) return false;
    for (const QString& line : lines)
    {
        if (!checkLineFormat(line)) return false;
        if (!checkLineFields(line)) return false;
        // Add more checks as needed
    }
    return true;
}

bool QMappingViewerWidget::checkLineFormat(const QString& line)
{
    // The expected format is: index;moduleType;moduleAlias;moduleChannel;minSrc;maxSrc;minDest;maxDest;destChannel
    // Total of 9 fields separated by semicolons
    int expectedFieldCount = 9;
    QStringList fields = line.split(";");
    if (fields.count() != expectedFieldCount) {
        // If the number of fields is not as expected, the format is incorrect
        return false;
    }
    return true;
}

bool QMappingViewerWidget::checkLineFields(const QString& line)
{
    // Field specifications:
    // 0: index (integer)
    // 1: moduleType (integer, index in an enum)
    // 2: moduleAlias (string)
    // 3: moduleChannel (string)
    // 4, 5: minSrc, maxSrc (double)
    // 6, 7: minDest, maxDest (u_int16_t)
    // 8: destChannel (integer)

    QStringList fields = line.split(";");

    // Check index field (integer)
    bool indexOk;
    fields[0].toInt(&indexOk);
    if (!indexOk) return false;

    // Check moduleType (integer)
    bool moduleTypeOk;
    fields[1].toInt(&moduleTypeOk);
    if (!moduleTypeOk) return false;

    // moduleAlias and moduleChannel are strings, any value is valid

    // Check minSrc, maxSrc (double)
    bool doubleOk;
    for (int i = 4; i <= 5; ++i) {
        fields[i].toDouble(&doubleOk);
        if (!doubleOk) return false;
    }

    // Check minDest, maxDest (u_int16_t)
    bool uInt16Ok;
    for (int i = 6; i <= 7; ++i) {
        fields[i].toUShort(&uInt16Ok);
        if (!uInt16Ok) return false;
    }

    // Check destChannel (integer)
    bool destChannelOk;
    fields[8].toInt(&destChannelOk);
    if (!destChannelOk) return false;

    return true;
}


bool QMappingViewerWidget::validateIndices(const QStringList &lines)
{
    // This function checks if the index field in each line is:
    // 1. Unique
    // 2. Starts with 0
    // 3. Strictly increments by 1

    QSet<int> indices; // To store unique indices
    int expectedIndex = 0; // Start with 0 and increment

    for (const QString &line : lines) {
        QStringList fields = line.split(";");

        // Parse index
        bool ok;
        int index = fields[0].toInt(&ok);
        if (!ok) return false; // Fail if index is not an integer

        // Check if index matches the expected value
        if (index != expectedIndex) return false;

        // Check for uniqueness
        if (indices.contains(index)) return false;
        indices.insert(index);

        // Increment expected index for next iteration
        expectedIndex++;
    }

    return true;
}



void QMappingViewerWidget::onSave()
{
    m_saveSuccess=true;
    if (checkAllBeforeSave())
    {
        qInfo()<<"all fields are checked";
        csvVizualizer->clear();
        bool ok;
        int clrCode;
        for (int i=0;i<rowWidgets.count();++i)
        {
            auto row = rowWidgets[i];
            QString csvLine = row->getCsvLine(ok);
            qInfo()<<csvLine;
            if (ok)
            {
                i%2==0 ? clrCode=0x87CEEB : clrCode=0x00BFFF;
                csvVizualizer->appendTextWithColor(csvLine,QColor(clrCode),false);
            }
        }
        backUpFile();
        csvVizualizer->saveRawText(m_modbusMappingPath + "mapping.csv");
    }
    else
    {
        m_saveSuccess=false;
        //TODO
    }
}

void QMappingViewerWidget::onUpload()
{
  qInfo()<<"enter on upload";
  onSave();
  if (!m_saveSuccess)
  {
      return;
  }
  sshCommand->uploadMappingSetup(m_modbusMappingPath);
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
    insertBeforeBtn -> blockSignals (blocked);
    insertAfterBtn  -> blockSignals (blocked);
    addBtn          -> blockSignals (blocked);
    reloadBtn       -> blockSignals (blocked);
    saveBtn         -> blockSignals (blocked);
    uploadBtn       -> blockSignals (blocked);
}

void QMappingViewerWidget::enableAllControls(bool enabled)
{
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        row->enableAllControls(enabled);
    }

    insertBeforeBtn -> setEnabled (enabled);
    insertAfterBtn  -> setEnabled (enabled);
    addBtn          -> setEnabled (enabled);
    reloadBtn       -> setEnabled (enabled);
    saveBtn         -> setEnabled (enabled);
    uploadBtn       -> setEnabled (enabled);
}

bool QMappingViewerWidget::checkAllBeforeSave()
{
    qInfo()<<"enter checkAllBeforeSave";
    bool result[] = {true,true,true,true};
    result[0]     = checkRowsForEmptyLineEdits ();
    qInfo()<<"checkRowsForEmptyLineEdits: "<<result[0];
    result[1]     = checkRowsMinMax            ();
    qInfo()<<"checkRowsMinMax: "<<result[1];
    result[2]     = chekRowsForEmptyComboboxes ();
    qInfo()<<"chekRowsForEmptyComboboxes: "<<result[2];
    result[3]     = checkforModulesTypesIsNotAll();
    qInfo()<<"checkforModulesTypesIsNotAll: "<<result[3];

    return result[0] && result[1] && result [2] && result[3];
}



bool QMappingViewerWidget::checkRowsForEmptyLineEdits()
{
    bool result[] = {true,true,true,true,true};
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        if (row)
        {
            result[0] = checkIfALineEditIsEmpty (row -> destChannelLE ());
            result[1] = checkIfALineEditIsEmpty (row -> maxDestLE     ());
            result[2] = checkIfALineEditIsEmpty (row -> minDestLE     ());
            result[3] = checkIfALineEditIsEmpty (row -> maxSrcValueLE ());
            result[4] = checkIfALineEditIsEmpty (row -> minSrcValueLE ());
        }
    }
    return result[0] && result[1] && result[2] && result [3] && result [4];
}


bool QMappingViewerWidget::checkIfALineEditIsEmpty(QLineEdit *aLineEdit)
{
    bool result = !(aLineEdit->text().isEmpty());
    aLineEdit->setProperty("inError",!result);

    aLineEdit->style()->unpolish(aLineEdit);
    aLineEdit->style()->polish  (aLineEdit);
    aLineEdit->update();
    return result;
}



bool QMappingViewerWidget::checkRowsMinMax()
{
    bool result[] = {true,true};
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        if (row)
        {
            result[0] = row->minDestLE()->text().toDouble() <  row->maxDestLE()->text().toDouble();
            if (!result[0])
            {
                row->minDestLE()->setProperty("inError", true);
                row->maxDestLE()->setProperty("inError", true);
            }
            result[1] = row->minSrcValueLE()->text().toDouble() < row->maxSrcValueLE()->text().toDouble();
            if (!result[1])
            {
                row->minSrcValueLE()->setProperty("inError", true);
                row->maxSrcValueLE()->setProperty("inError", true);
            }
        }
    }

    return result[0] && result[1];
}


//bool QMappingViewerWidget::checkForMinMaxConsistency(QLineEdit *minLE, QLineEdit *maxLE)
//{
//    bool check[] ={true,true};
//    bool result;
//    check[0] = checkIfALineEditIsEmpty(maxLE);
//    check[1] = checkIfALineEditIsEmpty(minLE);
//    check[0] && check[1] ? minLE>=maxLE ?  result = false : result = true : result = false;
//    if (!result)
//    {
//          maxLE->setProperty("inError", true);
//          minLE->setProperty("inError", true);
//    }
//    else
//    {
//        maxLE->setProperty("inError", false);
//        minLE->setProperty("inError", false);
//    }
//
//    return result;
//}

bool QMappingViewerWidget::chekRowsForEmptyComboboxes()
{
    bool result[] = {true,true,true};
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        if (row)
        {
            result[0] = checkIfAComboboxIsEmpty(row->moduleTypeCB());
            result[1] = checkIfAComboboxIsEmpty(row->moduleAliasCB());
            result[2] = checkIfAComboboxIsEmpty(row->moduleChannelCB());
        }
    }
    return result[0] && result[1] && result[2];

}

bool QMappingViewerWidget::checkIfAComboboxIsEmpty(QComboBox *aComboBox)
{
    bool result;
    result = !(aComboBox->currentText().isEmpty() || aComboBox->count() == 0);
    aComboBox->setProperty("inError",!result);
    aComboBox->style()->unpolish(aComboBox);
    aComboBox->style()->polish  (aComboBox);
    aComboBox->update();
    return result;
}

bool QMappingViewerWidget::checkforModulesTypesIsNotAll()
{
    bool result      = true;
    bool finalResult = true;
    for (int i=0;i<rowWidgets.count();++i)
    {
        auto row = rowWidgets[i];
        if (row)
        {
            //QTimer::singleShot(50,this,[row]{row->blockAllSignals(true);});
            result =  row->moduleTypeCB()->currentText() != "all";
            row->moduleTypeCB()->setProperty("inError" , !result);
            row->moduleTypeCB()->style()->unpolish(row->moduleTypeCB());
            row->moduleTypeCB()->style()->polish  (row->moduleTypeCB());
            row->moduleTypeCB()->update();
            finalResult &= result;
            //QTimer::singleShot(50,this,[row]{row->blockAllSignals(false);});
        }
    }
    return finalResult;
}

void QMappingViewerWidget::backUpFile()
{
    // Setup source and destination
    QDateTime now = QDateTime::currentDateTime();
    QString formatedDate        = now.toString("dd_MM_yyyy_hh_mm_ss_");
    QString SourcefileName      = m_modbusMappingPath + "mapping.csv";
    QString destinationFolder   = m_modbusMappingPath + "Backups/";
    QString DestinationFileName = destinationFolder + formatedDate + "mapping.csv";
    // Verify if the backup folder exists and create it if not
    QDir dir(destinationFolder);
    if (!dir.exists())
    {
        dir.mkpath(destinationFolder); // Create the directory if it doesn't exist
    }
    // Copy the file from the source to the destination
    QFile::copy(SourcefileName, DestinationFileName);
}

const QString &QMappingViewerWidget::passWord() const
{
    return m_passWord;
}

void QMappingViewerWidget::setPassWord(const QString &newPassWord)
{
    m_passWord = newPassWord;
    sshCommand->setPassword(m_passWord);
    emit passWordChanged();
}

const QString &QMappingViewerWidget::login() const
{
    return m_login;
}

void QMappingViewerWidget::setLogin(const QString &newLogin)
{
    m_login = newLogin;
    sshCommand->setUserName(m_login);
    emit loginChanged();
}

int QMappingViewerWidget::port() const
{
    return m_port;
}

void QMappingViewerWidget::setPort(int newPort)
{
    m_port = newPort;
    sshCommand->setPortNum(m_port);
    emit portChanged();
}

const QString &QMappingViewerWidget::hostName() const
{
    return m_hostName;
}

void QMappingViewerWidget::setHostName(const QString &newHostName)
{
    m_hostName = newHostName;
    sshCommand->setHostName(m_hostName);
    emit hostNameChanged();
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



