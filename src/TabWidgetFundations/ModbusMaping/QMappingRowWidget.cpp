#include "QMappingRowWidget.h"
#include <QHBoxLayout>
#include <QValidator>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

QMappingRowWidget::QMappingRowWidget(QCrioModulesDataExtractor *extractor, QWidget *parent)
    : QWidget(parent)
{
    createUi     ();
    createLayout ();
    connectAll   ();
    m_extractor = extractor;
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(32);
    this->installEventFilter(this);
}

QMappingRowWidget::~QMappingRowWidget()
{

}

int QMappingRowWidget::index() const
{
    return m_index;
}

void QMappingRowWidget::setIndex(int index)
{
    m_index = index;
    m_indexLabel->setText(QString::number(index));
    m_indexLabel->update();
    emit indexChanged(m_index);
}

QLabel *QMappingRowWidget::indexLabel() const
{
    return m_indexLabel;
}

void QMappingRowWidget::setIndexLabel(QLabel *newIndexLabel)
{
    if (m_indexLabel == newIndexLabel)
        return;
    m_indexLabel = newIndexLabel;
    emit indexLabelChanged();
}

QComboBox *QMappingRowWidget::moduleTypeCB() const
{
    return m_moduleTypeCB;
}

void QMappingRowWidget::setModuleTypeCB(QComboBox *newModuleTypeCB)
{
    if (m_moduleTypeCB == newModuleTypeCB)
        return;
    m_moduleTypeCB = newModuleTypeCB;
    emit moduleTypeCBChanged();
}

QComboBox *QMappingRowWidget::moduleAliasCB() const
{
    return m_moduleAliasCB;
}

void QMappingRowWidget::setModuleAliasCB(QComboBox *newModuleAliasCB)
{
    if (m_moduleAliasCB == newModuleAliasCB)
        return;
    m_moduleAliasCB = newModuleAliasCB;
    emit moduleAliasCBChanged();
}

QComboBox *QMappingRowWidget::moduleChannelCB() const
{
    return m_moduleChannelCB;
}

void QMappingRowWidget::setModuleChannelCB(QComboBox *newModuleChannelCB)
{
    if (m_moduleChannelCB == newModuleChannelCB)
        return;
    m_moduleChannelCB = newModuleChannelCB;
    emit moduleChannelCBChanged();
}

QLineEdit *QMappingRowWidget::minSrcValueLE() const
{
    return m_minSrcValueLE;
}

void QMappingRowWidget::setMinSrcValueLE(QLineEdit *newMinSrcValueLE)
{
    if (m_minSrcValueLE == newMinSrcValueLE)
        return;
    m_minSrcValueLE = newMinSrcValueLE;
    emit minSrcValueLEChanged();
}

QLineEdit *QMappingRowWidget::maxSrcValueLE() const
{
    return m_maxSrcValueLE;
}

void QMappingRowWidget::setMaxSrcValueLE(QLineEdit *newMaxSrcValueLE)
{
    if (m_maxSrcValueLE == newMaxSrcValueLE)
        return;
    m_maxSrcValueLE = newMaxSrcValueLE;
    emit maxSrcValueLEChanged();
}

QLineEdit *QMappingRowWidget::minDestLE() const
{
    return m_minDestLE;
}

void QMappingRowWidget::setMinDestLE(QLineEdit *newMinDestLE)
{
    if (m_minDestLE == newMinDestLE)
        return;
    m_minDestLE = newMinDestLE;
    emit minDestLEChanged();
}

QLineEdit *QMappingRowWidget::maxDestLE() const
{
    return m_maxDestLE;
}

void QMappingRowWidget::setMaxDestLE(QLineEdit *newMaxDestLE)
{
    if (m_maxDestLE == newMaxDestLE)
        return;
    m_maxDestLE = newMaxDestLE;
    emit maxDestLEChanged();
}

QLineEdit *QMappingRowWidget::destChannelLE() const
{
    return m_destChannelLE;
}

void QMappingRowWidget::setDestChannelLE(QLineEdit *newDestChannelLE)
{
    if (m_destChannelLE == newDestChannelLE)
        return;
    m_destChannelLE = newDestChannelLE;
    emit destChannelLEChanged();
}

QPushButton *QMappingRowWidget::upBtn() const
{
    return m_upBtn;
}

void QMappingRowWidget::setUpBtn(QPushButton *newUpBtn)
{
    if (m_upBtn == newUpBtn)
        return;
    m_upBtn = newUpBtn;
    emit upBtnChanged();
}

QPushButton *QMappingRowWidget::downBtn() const
{
    return m_downBtn;
}

void QMappingRowWidget::setDownBtn(QPushButton *newDownBtn)
{
    if (m_downBtn == newDownBtn)
        return;
    m_downBtn = newDownBtn;
    emit downBtnChanged();
}

QPushButton *QMappingRowWidget::delBtn() const
{
    return m_delBtn;
}

void QMappingRowWidget::setDelBtn(QPushButton *newDelBtn)
{
    if (m_delBtn == newDelBtn)
        return;
    m_delBtn = newDelBtn;
    emit delBtnChanged();
}

QString QMappingRowWidget::getCsvLine(bool &ok)
{
    ok = false;
    QString result ="";
    //ensure type combobox is not on all
    QVariant userData = m_moduleAliasCB->itemData(m_moduleAliasCB->currentIndex());
    NIDeviceModule *module = userData.value<NIDeviceModule*>();
    //add index field
    result.append(m_indexLabel->text()+";");
    if (module)
    {
        ModuleType type = module->getModuleType();
        if (type==-1)
        {
            return result;
        }
        m_moduleTypeCB->blockSignals(true);
        m_moduleTypeCB->setCurrentIndex(type);
        m_moduleTypeCB->blockSignals(false);
        //add type field
        result.append(QString::number(m_moduleTypeCB->currentIndex())+";");
        //add module alias field
        result.append(m_moduleAliasCB->currentText()+";");
        //add channel field
        result.append(m_moduleChannelCB->currentText()+";");
        //add chanel min
        result.append(m_minSrcValueLE->text()+";");
        //add channel max
        result.append(m_maxSrcValueLE->text()+";");
        //add destination min
        result.append(m_minDestLE->text()+";");
        //add destination max
        result.append(m_maxDestLE->text()+";");
        //add modbus channel index
        result.append(m_destChannelLE->text());
        //add terminator
        result.append("\n");
        ok=true;
        return result;
    }
    return result;

}

void QMappingRowWidget::setCsvLine(const QString &csvLine)
{
    QStringList fields = csvLine.split(";");

    // Check if the CSV line has the correct number of fields
    if (fields.size() != 9) // Assuming 9 fields based on the getCsvLine format
    {
        // Handle error: Incorrect number of fields
        return;
    }

    // Set the fields of the widget
    // Note: Error handling should be added for each field for robustness
    bool ok;

    // Index
    int index = fields[0].toInt(&ok);
    if (ok) setIndex(index);

    // Module Type
    int moduleTypeIndex = fields[1].toInt(&ok);
    if (ok && moduleTypeIndex >= 0 && moduleTypeIndex < m_moduleTypeCB->count())
    {
        m_moduleTypeCB->setCurrentIndex(moduleTypeIndex);
        onModuleTypeChanged(moduleTypeIndex); // This will update related fields
    }

    // Module Alias
    m_moduleAliasCB->setCurrentText(fields[2]); // Assuming the text is in the combo box list

    // Module Channel
    m_moduleChannelCB->setCurrentText(fields[3]);

    // Source Min Value
    m_minSrcValueLE->setText(fields[4]);

    // Source Max Value
    m_maxSrcValueLE->setText(fields[5]);

    // Destination Min Value
    m_minDestLE->setText(fields[6]);

    // Destination Max Value
    m_maxDestLE->setText(fields[7]);

    // Modbus Channel Index
    m_destChannelLE->setText(fields[8]);
}

void QMappingRowWidget::blockAllSignals(bool blocked)
{
    m_indexLabel      ->blockSignals(blocked);
    m_moduleTypeCB    ->blockSignals(blocked);
    m_moduleAliasCB   ->blockSignals(blocked);
    m_moduleChannelCB ->blockSignals(blocked);
    m_minSrcValueLE   ->blockSignals(blocked);
    m_maxSrcValueLE   ->blockSignals(blocked);
    m_minDestLE       ->blockSignals(blocked);
    m_maxDestLE       ->blockSignals(blocked);
    m_destChannelLE   ->blockSignals(blocked);
    m_upBtn           ->blockSignals(blocked);
    m_downBtn         ->blockSignals(blocked);
    m_delBtn          ->blockSignals(blocked);
    m_extractor       ->blockSignals(blocked);
}

void QMappingRowWidget::enableAllControls(bool enabled)
{
    m_indexLabel      ->setEnabled(enabled);
    m_moduleTypeCB    ->setEnabled(enabled);
    m_moduleAliasCB   ->setEnabled(enabled);
    m_moduleChannelCB ->setEnabled(enabled);
    m_minSrcValueLE   ->setEnabled(enabled);
    m_maxSrcValueLE   ->setEnabled(enabled);
    m_minDestLE       ->setEnabled(enabled);
    m_maxDestLE       ->setEnabled(enabled);
    m_destChannelLE   ->setEnabled(enabled);
    m_upBtn           ->setEnabled(enabled);
    m_downBtn         ->setEnabled(enabled);
    m_delBtn          ->setEnabled(enabled);
    m_extractor       ->blockSignals(enabled);
}



bool QMappingRowWidget::eventFilter(QObject *watched, QEvent *event)
{
    // Check if the event is a mouse press event
    if (event->type() == QEvent::MouseButtonPress)
    {
        // If the watched object is one of the buttons, let the button handle it
        if (watched == m_upBtn         ||
            watched == m_downBtn       ||
            watched == m_delBtn        ||
            watched == m_moduleAliasCB ||
            watched == m_moduleChannelCB)
        {
            return false;
        }

        // If the watched object is a child widget other than the buttons
        if (watched->isWidgetType())
        {
            // Emit the selected signal
            emit rowSelectedSignal(this);
            // Do not consume the event, let the child widget handle its usual behavior
            return false;
        }
    }
    // Call base class eventFilter if this event is not handled
    return QWidget::eventFilter(watched, event);
}


void QMappingRowWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent( event );
    this->ensurePolished();
}

void QMappingRowWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void QMappingRowWidget::createUi()
{
    QFont standardFont("Arial", 10); // Example: Arial font, size 10

    m_indexLabel = new QLabel(this);
    m_indexLabel->setFont(standardFont);
    // Set a reasonable minimum width for the index label based on the expected maximum index
    int maxIndexWidth = QFontMetrics(m_indexLabel->font()).horizontalAdvance("999"); // Example for 3 digits
    m_indexLabel->setMinimumWidth(maxIndexWidth);
    // Set the size policy to allow the label to grow
    m_indexLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);


    m_moduleTypeCB    = new QComboBox (this);
    QStringList items = EnumHelper::toStringList<globalEnumSpace::ModuleType>();
    items<<"all";
    m_moduleTypeCB->blockSignals(true);
    m_moduleTypeCB->addItems(items);
    m_moduleTypeCB->setCurrentIndex(m_moduleTypeCB->count()-1);
    m_moduleTypeCB->setFixedWidth(160);
    m_moduleTypeCB->blockSignals(false);


    m_moduleAliasCB   = new QComboBox (this);
    m_moduleAliasCB->setFixedWidth(70);

    m_moduleChannelCB = new QComboBox (this);
    m_moduleChannelCB->setFixedWidth(70);

    m_minSrcValueLE   = new QLineEdit (this);
    m_minSrcValueLE -> setValidator(new QDoubleValidator(this));

    m_maxSrcValueLE = new QLineEdit(this);
    m_maxSrcValueLE->setValidator(new QDoubleValidator(this));

    m_minDestLE = new QLineEdit(this);
    m_minDestLE->setValidator(new QIntValidator(0, 65535, this));

    m_maxDestLE = new QLineEdit(this);
    m_maxDestLE->setValidator(new QIntValidator(0, 65535, this));

    m_destChannelLE = new QLineEdit(this);
    m_destChannelLE->setValidator(new QIntValidator(0, 65535, this));

    m_upBtn   = new QPushButton("Up"    , this);
    m_downBtn = new QPushButton("Down"  , this);
    m_delBtn  = new QPushButton("Delete", this);
}

void QMappingRowWidget::createLayout()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout -> addWidget (m_indexLabel)     ;
    layout -> addWidget (m_moduleTypeCB)   ;
    layout -> addWidget (m_moduleAliasCB)  ;
    layout -> addWidget (m_moduleChannelCB);
    layout -> addWidget (m_minSrcValueLE)  ;
    layout -> addWidget (m_maxSrcValueLE)  ;
    layout -> addWidget (m_minDestLE)      ;
    layout -> addWidget (m_maxDestLE)      ;
    layout -> addWidget (m_destChannelLE)  ;
    layout -> addWidget (m_upBtn)          ;
    layout -> addWidget (m_downBtn)        ;
    layout -> addWidget (m_delBtn)         ;
    layout -> setSpacing(2);
    layout -> setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    loadStyleStringFromResource();
}

void QMappingRowWidget::connectAll()
{
    connect (m_moduleTypeCB  , &QComboBox::currentIndexChanged , this, &QMappingRowWidget::onModuleTypeChanged);
    connect (m_moduleAliasCB , &QComboBox::activated           , this, &QMappingRowWidget::onModuleAliasChanged);
    connect (m_delBtn        , &QPushButton::clicked , this, [this](){Q_EMIT deleteRowSignal   (this);});
    connect (m_upBtn         , &QPushButton::clicked , this, [this](){Q_EMIT moveUpRowSignal   (this);});
    connect (m_downBtn       , &QPushButton::clicked , this, [this](){Q_EMIT moveDownRowSignal (this);});
}

void QMappingRowWidget::onModuleTypeChanged(int index)
{
    if (index==m_moduleTypeCB->count()-1)
    {
       m_extractor->extractAllModules(m_moduleAliasCB);
    }
    else
    {
        switch (index)
        {
            case ModuleType::isAnalogicInputCurrent:
            {
                m_extractor->extractCurrentModules(m_moduleAliasCB);
                break;
            }
            case ModuleType::isAnalogicInputVoltage:
            {
                m_extractor->extractVoltageModules(m_moduleAliasCB);
                break;
            }

        }


    }
}

void QMappingRowWidget::onModuleAliasChanged(int index)
{
    QVariant userData = m_moduleAliasCB->itemData(index);
    NIDeviceModule *module = userData.value<NIDeviceModule*>();
    if (module)
    {
        ModuleType type = module->getModuleType();
        if (type==-1)
        {
            return;
        }
        m_moduleTypeCB->blockSignals(true);
        m_moduleTypeCB->setCurrentIndex(type);
        m_moduleTypeCB->blockSignals(false);
        std::vector<std::string> strLst = module->getChanNames();
        m_moduleChannelCB->blockSignals(true);
        m_moduleChannelCB->clear();
        for (size_t i=0;i<strLst.size();++i)
        {
            m_moduleChannelCB->addItem(QString::fromStdString(strLst[i]));
        }
        m_moduleChannelCB->blockSignals(false);
        m_minSrcValueLE->setText(QString::number(module->getChanMin()));
        m_maxSrcValueLE->setText(QString::number(module->getChanMax()));
        m_minDestLE->setText("0");
        m_maxDestLE->setText("65535");
    }
}


void QMappingRowWidget::loadStyleStringFromResource()
{
    if (!loadAndApplyStyleSheet(":/styles/darkMappingRowWidget.scss", this)) {
        return;
    }

    if (!loadAndApplyStyleSheet(":/styles/darkModulesComboBoxes.scss", m_moduleAliasCB)) {
        return;
    }

    if (!loadAndApplyStyleSheet(":/styles/darkChannelsComboBoxes.scss", m_moduleChannelCB)) {
        return;
    }


    // Apply the same stylesheet to multiple line edits
    QList<QLineEdit*> lineEdits = { destChannelLE(), maxDestLE(), minDestLE(), maxSrcValueLE(), minSrcValueLE() };
    for (auto lineEdit : lineEdits)
    {
        if (!loadAndApplyStyleSheet(":/styles/darkModbusChannelLE.scss", lineEdit))
        {
            return;
        }
        lineEdit->setProperty("isMaxValue", lineEdit == maxDestLE() || lineEdit == maxSrcValueLE());
        lineEdit->setProperty("isMinValue", lineEdit == minDestLE() || lineEdit == minSrcValueLE());
    }
}


bool QMappingRowWidget::loadAndApplyStyleSheet(const QString &resourcePath, QWidget *widget)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    widget->setStyleSheet(content);
    return true;
}



