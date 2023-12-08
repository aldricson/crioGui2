#include "QMappingRowWidget.h"
#include <QHBoxLayout>
#include <QValidator>

QMappingRowWidget::QMappingRowWidget(QWidget *parent)
    : QWidget(parent)
{
    createUi     ();
    createLayout ();
    connectAll   ();
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(32);
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

bool QMappingRowWidget::eventFilter(QObject *watched, QEvent *event)
{
    // Check if the event is a mouse press event and the object is a child of this widget
       if (event->type() == QEvent::MouseButtonPress && watched->isWidgetType())
       {
           // Emit the selected signal
           emit rowSelectedSignal(this);
           // Do not consume the event, let the child widget handle its usual behavior
           return false;
       }
       // Call base class eventFilter if this event is not handled
       return QWidget::eventFilter(watched, event);
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
    m_moduleAliasCB   = new QComboBox (this);
    m_moduleChannelCB = new QComboBox (this);

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
}

void QMappingRowWidget::connectAll()
{

}

// Implement getters and setters as needed
