#include "QBetterTableWidget.h"

QBetterTableWidget::QBetterTableWidget(QWidget *parent) : QTableWidget(parent)
{

    connect(this,
            &QTableWidget::itemChanged,
            this,
            &QBetterTableWidget::onItemChanged);

    m_outOfFocusRefreshTimer = new QTimer(this);
    m_outOfFocusRefreshTimer->setInterval(250);
    connect(m_outOfFocusRefreshTimer,
            &QTimer::timeout,
            this,
            [this]{this->viewport()->update();this->update();});

    globalTableContainer = new QWidget();
    tableLayout = new QGridLayout(globalTableContainer);
    tableLayout->addWidget(this,tablePosInLayout,0,1,1);
    if (parent==nullptr)
    {
        globalTableContainer->show();
    }
    else
    {
        globalTableContainer->setParent(parent);
        globalTableContainer->show();
    }


    headerList.clear();

/*    QHeaderView *headerView = dynamic_cast<QHeaderView *>(this->horizontalHeader());
    if (headerView) {
        QBetterHeaderView *betterHeader = dynamic_cast<QBetterHeaderView *>(headerView);
        if (betterHeader) {
            headerList.append(betterHeader);
        } else {
            qDebug() << "&&&&&&&&&&&&&&&&&&&The headerView is not a QBetterHeaderView";
        }
    } else {
        qDebug() << "The this->horizontalHeader() is not a QHeaderView";
    }*/

  /*  this->horizontalHeader()->setParent(new QBetterHeaderView(Qt::Horizontal, this));

    QBetterHeaderView *betterHeader = dynamic_cast<QBetterHeaderView *>(this->horizontalHeader());
    if (betterHeader)
    {
        headerList.append(betterHeader);
    }
    else
    {
        // Debugging code to log the actual type of the object being returned
        qInfo() << "The actual type of the object being returned is: " << this->horizontalHeader()->metaObject()->className();
    }*/



    //headerList.append(horizontalHeader());
 /*   QBetterHeaderView *betterHeader = dynamic_cast<QBetterHeaderView *>(horizontalHeader());
    if (betterHeader)
    {
        headerList.append(betterHeader);
    }
    else
    {
        qInfo()<<"WWWWWWWOOOOOOOOWWWWWWW BUUUUGGG";
    }*/
    valueSearchBox = new QValueSearchBox();

    connect (valueSearchBox,
             &QValueSearchBox::searchAskedSignal,
             this,
             &QBetterTableWidget::onSearchNumericAsked);


    connect (this->horizontalScrollBar(),
             &QScrollBar::valueChanged,
             this,
             &QBetterTableWidget::onHorizontalScrollBarChanged
             );

    m_contextMenu       = new QMenu(this);
    //Copy/paste/cut section
    m_copyAction        = new QAction(QIcon(":/ressources/images/copy.png") ,
                                      tr("Copy"), this);

    m_pasteAction       = new QAction(QIcon(":/ressources/images/paste.png"),
                                      tr("Paste"), this);

    m_cutAction         = new QAction(QIcon(":/ressources/images/cut.png")  ,
                                      tr("Cut"), this);
    //search section


    m_searchTextAction  = new QAction(QIcon(":/ressources/images/text.png") ,
                                      tr("Search text"), this);

    m_searchValueAction = new QAction(QIcon(":/ressources/images/numbers.png"),
                                      tr("Search value"), this);

    //statistical section
    m_giveMinValueOnCol = new QAction(QIcon(":/ressources/images/min.png")  ,
                                        tr("Min value on col"), this);


    m_giveMaxValueOnCol = new QAction(QIcon(":/ressources/images/max.png")  ,
                                        tr("Max value on col"), this);



    m_giveAverageCol      = new QAction(QIcon(":/ressources/images/average.png")  ,
                                        tr("give col average"), this);


    m_giveMinValueOnSelected = new QAction(QIcon(":/ressources/images/min.png"),
                                   tr("Min value on selected"),this);

    m_giveMaxValueOnSelected = new QAction(QIcon(":/ressources/images/max.png"),
                                   tr("Max value on selected"),this);

    m_giveAverageOnSelected  = new QAction(QIcon(":/ressources/images/average.png"),
                                   tr("average on selected"),this);




    m_contextMenu->addAction(m_copyAction);
    m_contextMenu->addAction(m_pasteAction);
    m_contextMenu->addAction(m_cutAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_searchValueAction);
    m_contextMenu->addAction(m_searchTextAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_giveMinValueOnCol);
    m_contextMenu->addAction(m_giveMaxValueOnCol);
    m_contextMenu->addAction(m_giveAverageCol);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_giveMinValueOnSelected);
    m_contextMenu->addAction(m_giveMaxValueOnSelected);
    m_contextMenu->addAction(m_giveAverageOnSelected );
    //copy/paste/cut section
    connect(m_copyAction,  &QAction::triggered, this, [this]{copyToClipboard();});
    connect(m_pasteAction, &QAction::triggered, this, &QBetterTableWidget::pasteFromClipboard);
    connect(m_cutAction  , &QAction::triggered, this, [this]{copyToClipboard();delSelection();});
    //search section
    connect(m_searchValueAction, &QAction::triggered, this, [this]{valueSearchBox->show();});
    // connect(m_searchTextAction, &QAction::triggered, this, &QBetterTableWidget::showTextSearchBox);

    //Statitical section
    connect(m_giveMaxValueOnCol, &QAction::triggered, this, [this]{findMaxValueOnCol();});
    connect(m_giveMinValueOnCol, &QAction::triggered, this, [this]{findMinValueOnCol();});
    connect(m_giveAverageCol,    &QAction::triggered, this, [this]{giveAverageOnCol(true);});

    connect(m_giveMaxValueOnSelected, &QAction::triggered, this, [this]{findMaxValueOnSelected();});
    connect(m_giveMinValueOnSelected, &QAction::triggered, this, [this]{findMinValueOnSelected();});
    connect(m_giveAverageOnSelected,  &QAction::triggered, this, [this]{giveAverageOnSelected(true);});

}



QBetterTableWidget::~QBetterTableWidget()
{
    clearAllWidgets();
}

/**
 * @brief This function adds a push button to a specific cell in the QBetterTableWidget.
 *
 * It creates a new QPushButton, connects its clicked signal to the onButtonClicked slot,
 * and then sets it as the widget for the specified cell.
 *
 * @param row the row of the cell where the button will be added
 * @param col the column of the cell where the button will be added
 */
QPushButton * QBetterTableWidget::addPushBtn(int row, int col,QString text)
{
    // Create new push button
    QPushButton *btn = new QPushButton(text, this);
    // Connect the clicked signal of the button to the onButtonClicked slot
    connect(btn, &QPushButton::clicked, this, &QBetterTableWidget::onButtonClicked);
    // Set the button as the widget for the specified cell
    setCellWidget(row, col, btn);
    pushButtonList.append(btn);
    return btn;
}

QCheckBox *QBetterTableWidget::addCheckBox(int row, int col, bool state)
{
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new colorBtn
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setChecked(state);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    setCellWidget(row, col, widget);
    checkBoxList.append(checkBox);
    containerWidgetList.append(widget);
    return checkBox;
}

QBetterTableWidget *QBetterTableWidget::addBetterTableWidget(int row, int col)
{
    qInfo()<<"todo implement QBetterTableWidget::"
             "addBetterTableWidget(int row, int col)";
    return nullptr;
}

QSpinBox * QBetterTableWidget::addIntSpinEdit(int row, int col, int value)
{
    QSpinBox *spnb = new QSpinBox(this);
    spnb->setMaximum(std::numeric_limits<int>::max());
    spnb->setMinimum(std::numeric_limits<int>::min());
    spnb->setValue(value);
    spnb->setMaximumWidth(200);
    spnb->setAlignment(Qt::AlignCenter);
    setCellWidget(row,col,spnb);
    spinBoxList.append(spnb);
    spnb->setFocusPolicy(Qt::ClickFocus);
    spnb->installEventFilter(this);
    return spnb;
}

QSizeWidget * QBetterTableWidget::addSizeWidget(int row, int col, QSize size)
{
    QSizeWidget *swg = new QSizeWidget(this);
    swg->setQSize(size);
    setCellWidget(row,col,swg);
    sizeWidgetList.append(swg);
    return swg;
}

QSizeFWidget * QBetterTableWidget::addSizeFWidget(int row, int col, QSizeF sizeF)
{
    QSizeFWidget *swg = new QSizeFWidget(this);
    swg->setQSizeF(sizeF);
    setCellWidget(row,col,swg);
    sizeFWidgetList.append(swg);
    return swg;
}

QRectWidget *QBetterTableWidget::addRectWidget(int row,
                                               int col,
                                               QRect rect)
{
    QRectWidget *rctW = new QRectWidget(this);
    rctW->setRect(rect);
    setCellWidget(row,col,rctW);

    return rctW;

}

QRectFWidget *QBetterTableWidget::addRectFWidget(int row, int col, QRectF rectF)
{
    QRectFWidget *rctFW = new QRectFWidget(this);
    rctFW->setRectF(rectF);
    setCellWidget(row,col,rctFW);
    return rctFW;
}

QLineEdit * QBetterTableWidget::addIntLineEdit(int row, int col, int value)
{
    // Create new line edit
    QLineEdit *intLineEdit = new QLineEdit(this);
    // Set validator for integers
    QIntValidator *validator = new QIntValidator(this);
    intLineEdit->setValidator(validator);
    // Set the value
    intLineEdit->setText(QString::number(value));
    //set the alignment of lineEdit
    intLineEdit->setAlignment(Qt::AlignCenter);
    // Add line edit to table
    setCellWidget(row, col, intLineEdit);
    //
    intLineEditList.append(intLineEdit);
    return intLineEdit;
}

QLineEdit * QBetterTableWidget::addDoubleLineEdit(int row, int col, double value)
{
    // Create new line edit
    QLineEdit *doubleLineEdit = new QLineEdit(this);
    // Set validator for integers
    QDoubleValidator *validator = new QDoubleValidator(this);
    doubleLineEdit->setValidator(validator);
    // Set the value
    doubleLineEdit->setText(QString::number(value));
    //set the alignment of lineEdit
    doubleLineEdit->setAlignment(Qt::AlignCenter);
    // Add line edit to table
    setCellWidget(row, col, doubleLineEdit);
    //
    doubleLineEditList.append(doubleLineEdit);
    return doubleLineEdit;
}

QLineEdit *QBetterTableWidget::addSimpleLineEdit(int row, int col, QString str)
{
    // Create new line edit
    QLineEdit *intLineEdit = new QLineEdit(this);
    // Set the value
    intLineEdit->setText(str);
    //set the alignment of lineEdit
    intLineEdit->setAlignment(Qt::AlignCenter);
    // Add line edit to table
    setCellWidget(row, col, intLineEdit);
    //
    lineEditList.append(intLineEdit);
    return intLineEdit;
}

QTableWidgetItem *QBetterTableWidget::addText(int row, int col, QString text)
{

    QTableWidgetItem* item = this->itemAt(row, col);
      if (item != nullptr)
      {
          item->setText(text);
      }
      item = new QTableWidgetItem();
      item->setText(text);
      setItem(row, col, new QTableWidgetItem(text));
      tableWidgetItemList.append(item);
      return item;
}

QColorButton *QBetterTableWidget::addColorButton(int row, int col, QColor color)
{
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new colorBtn
    QColorButton *colorBtn = new QColorButton(this);
    colorBtn->setColor(color);
    layout->addWidget(colorBtn);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    setCellWidget(row, col, widget);
    colorButtonList.append(colorBtn);
    containerWidgetList.append(widget);
    return colorBtn;
}



/**
 * @brief This function adds a switch button to a specific cell in the QBetterTableWidget.
 *
 * It creates a new QBetterSwitchButton, connects its stateChanged signal to the
 * onStateChanged slot,
 * and then sets it as the widget for the specified cell.
 *
 * @param row the row of the cell where the button will be added
 * @param col the column of the cell where the button will be added
 */
QBetterSwitchButton * QBetterTableWidget::addSwitchWidget(int row, int col)
{
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new push button
    QBetterSwitchButton *btn = new QBetterSwitchButton("false",
                                                       "true" ,
                                                       Qt::lightGray,
                                                       Qt::green,
                                                       Qt::darkCyan,
                                                       false,
                                                       nullptr);
    layout->addWidget(btn);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);

    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    // Connect the stateChanged signal of the button to the onButtonClicked slot
    connect(btn,
            &QBetterSwitchButton::stateChanged,
            this,
            &QBetterTableWidget::onStateChanged);

    betterSwitchButtonList.append(btn);
    containerWidgetList.append(widget);
    // Set the switch as the widget for the specified cell
    setCellWidget(row, col, widget);
    return btn;
}

QBetterSwitchButton *QBetterTableWidget::addSwitchWidget(int row, int col, bool state)
{
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new push button
    QBetterSwitchButton *btn = new QBetterSwitchButton("false",
                                                       "true" ,
                                                       Qt::lightGray,
                                                       Qt::green,
                                                       Qt::darkCyan,
                                                       state,
                                                       nullptr);
    layout->addWidget(btn);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);

    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    // Connect the stateChanged signal of the button to the onButtonClicked slot
    connect(btn,
            &QBetterSwitchButton::stateChanged,
            this,
            &QBetterTableWidget::onStateChanged);

    betterSwitchButtonList.append(btn);
    containerWidgetList.append(widget);
    // Set the switch as the widget for the specified cell
    setCellWidget(row, col, widget);
    return btn;
}

/**
*  @brief  add a simple combobox to the table
*  @param  row - row of the cell where the combobox will be added
*  @param  col - column of the cell where the combobox will be added
*  @param  lst - list of items to be added to the combobox
*  @param  startIndex - index of the item that will be set as current
*  @return pointer to the created combobox
*/
QComboBox *QBetterTableWidget::addSimpleComboBox(int row,
                                                 int col,
                                                 QStringList lst,
                                                 int startIndex=0)
{
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new comboBox
    QComboBox *cb = new QComboBox(this);
    //add items
    cb->addItems(lst);
    //set current Index
    cb->setCurrentIndex(startIndex);

    layout->addWidget(cb);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);

    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    connect (cb,
             &QComboBox::currentIndexChanged,
             this,
             &QBetterTableWidget::onComboBoxChanged);
    //add to list
    simpleComboBoxList.append(cb);
    cb->setObjectName("CB_"+QString::number(simpleComboBoxList.count()));
    containerWidgetList.append(widget);
    // Set the comboBox as the widget for the specified cell
    setCellWidget(row, col, widget);
    return cb;
}



/**
* @brief Adds a QOutputUnitCombobox widget to the header at the specified index and column.
*
* @param headerIndex Index of the header in the headerList.
* @param colIndex Index of the column.
*
* @return A pointer to the QOutputUnitCombobox added, or nullptr if the indexes are invalid.
*/
QOutputUnitCombobox  *QBetterTableWidget::addUnitComboBoxToHeader(int headerIndex,
                                                                  int colIndex,
                                                                  TunitsFamilyType famillyType,
                                                                  QString defaultUnit)
{
    if (headerIndex < 0                   ||
        headerIndex >= headerList.count() ||
        colIndex < 0                      ||
        colIndex >= columnCount())
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Invalid indexes for header or column";
        return nullptr;
    }
    // Create a layout
    QHBoxLayout* layout = new QHBoxLayout();
    // Create new unit comboBox
    QOutputUnitCombobox *unitComboBox = new QOutputUnitCombobox(this,famillyType,defaultUnit);
    //set the unitcombox header index
    unitComboBox->setHeaderIndex(headerIndex);
    //set the unitCombobox col index
    unitComboBox->setHeaderColIndex(colIndex);
    // Connect the text changed signal to the onTextChangedSignal
    connect(unitComboBox, &QOutputUnitCombobox::textChangedSignal,
                          this,
                          &QBetterTableWidget::onUnitCbTextChanged);
    layout->addWidget(unitComboBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    // Create a widget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    //to ease destruction when needed
    containerWidgetList.append(widget);
    unitComboboxList.append(unitComboBox);
    //tricks the header
     //QHeaderView *currentHeader = headerList[headerIndex];
    QBetterHeaderView *currentHeader = headerList[headerIndex];
    currentHeader->setIsUnitConverter(true);
    widget->setParent   (currentHeader);
    widget->setGeometry (currentHeader->sectionViewportPosition(colIndex),
                         0,
                         currentHeader ->sectionSize(colIndex)-2 ,
                         currentHeader->height()-2);
    return unitComboBox;
}

/*!
 * \brief Adds Unit ComboBoxes to the specified header row.
 * \param headerIndex The index of the header row where the ComboBoxes will be added.
 * \param famillyTypes A vector containing the default family type for each ComboBox.
 * \param defaultUnits A vector containing the default unit for each ComboBox.
 * \return True if the ComboBoxes were successfully added, false otherwise.
 *
 * This function adds a unit ComboBox to each cell in the specified header row. The default family type and default unit for each ComboBox are specified in the `famillyTypes` and `defaultUnits` vectors, respectively.
 */
bool QBetterTableWidget::addUnitComboBoxesToHeader(int headerIndex,
                                                   const QVector<TunitsFamilyType> &famillyTypes,
                                                   const QVector<QString> &defaultUnits)
{
    if (headerIndex < 0 || headerIndex >= headerList.count())
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Invalid header index";
        return false;
    }
    if (columnCount() != famillyTypes.count() || columnCount() != defaultUnits.count())
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : colIndexes, famillyTypes, and defaultUnits must have the same count";
        return false;
    }
    for (int i = 0; i < columnCount(); i++)
    {
        int colIndex = i;
        if (colIndex < 0 || colIndex >= columnCount())
        {
            qInfo() << metaObject()->className()
                    << "::"
                    << __FUNCTION__
                    << " : Invalid col index";
            return false;
        }
        // Create a layout
        QHBoxLayout* layout = new QHBoxLayout();
        // Create new unit comboBox
        QOutputUnitCombobox *unitComboBox = new QOutputUnitCombobox(this,
                                                                    famillyTypes[i],
                                                                    defaultUnits[i]);;

        //set the unitcombox header index
        unitComboBox->setHeaderIndex(headerIndex);
        //set the unitCombobox col index
        unitComboBox->setHeaderColIndex(colIndex);
        // Connect the text changed signal to the onTextChangedSignal
        connect(unitComboBox, &QOutputUnitCombobox::textChangedSignal,
                      this,
                      &QBetterTableWidget::onUnitCbTextChanged);
        layout->addWidget(unitComboBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0,0,0,0);
        // Create a widget to hold the layout
        QWidget* widget = new QWidget();
        widget->setLayout(layout);
        //to ease destruction when needed
        containerWidgetList.append(widget);
        unitComboboxList.append(unitComboBox);
        //tricks the header
        //QHeaderView *currentHeader = headerList[headerIndex];
        QBetterHeaderView *currentHeader = headerList[headerIndex];
        currentHeader->setIsUnitConverter(true);
        widget->setParent   (currentHeader);
        widget->setGeometry (currentHeader->sectionViewportPosition(colIndex),
                             0,
                             currentHeader ->sectionSize(colIndex)-2 ,
                             currentHeader->height()-2);
        widget->show();
    }
    return true;
}


/**
*  @brief  merge cells in a row
*  @param  row - the row to be merged
*  @return true if the merge is done successfully, false otherwise
*/
bool QBetterTableWidget::mergeRowCells(int row)
{
    // Check if the object is a QTableWidget
    if(!qobject_cast<QTableWidget*>(this))
    {
        qInfo()<<"QBetterTableWidget::mergeRowCells : the object is not a QTableWidget";
        return false;
    }
    // Check if the row value is within the valid range
    if (row < 0 || row >= rowCount())
    {
        qInfo()<<"QBetterTableWidget::mergeRowCells : invalid row value";
        return false;
    }
    setSpan(row, 0, 1, columnCount());
    //int colCount = columnCount();
    //for (int i = 0; i < colCount; i++)
    //{
    //    // Merge the first cell of the row with the entire row
    //    if(i == 0) setSpan(row, i, 1, colCount);
        // Merge the rest of the cells in the row with a single cell
    //    else setSpan(row, i, 1, 1);
    //}
    return true;
}

/**
*  @brief  merge cells in a square range
*  @param  rowStart - start row of the square
*  @param  colStart - start column of the square
*  @param  rowSpan - width the square in cells
*  @param  colEnd -  height of the square in cells
*  @return true if the merge is done successfully, false otherwise
*/
bool QBetterTableWidget::mergeSquareCells(int rowStart,
                                          int colStart,
                                          int _rowSpan,
                                          int _colSpan)
{
    // Check if the object is a QTableWidget
    if(!qobject_cast<QTableWidget*>(this)){
        qInfo()<<"QBetterTableWidget::mergeSquareCells : the object is not a QTableWidget";
        return false;
    }

    // Check if the rowStart value is within the valid range
    if (rowStart < 0 || rowStart >= rowCount())
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid rowStart value";
        return false;
    }
    // Check if the colStart value is within the valid range
    if (colStart < 0 || colStart >= columnCount())
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid colStart value";
        return false;
    }
    // Check if the rowEnd value is within the valid range
    int rowEnd=rowStart+_rowSpan;
    if (rowEnd < 0 || rowEnd >= rowCount())
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid rowEnd value";
        return false;
    }
    // Check if the colEnd value is within the valid range
    int colEnd=colStart+_colSpan;
    if (colEnd < 0 || colEnd >= columnCount())
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid colEnd value";
        return false;
    }
    // Check if the range is valid, rowStart should be <= rowEnd
    if (rowStart > rowEnd)
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid range, rowStart > rowEnd";
        return false;
    }
    // Check if the range is valid, colStart should be <= colEnd
    if (colStart > colEnd)
    {
        qInfo()<<"QBetterTableWidget::mergeSquareCells : invalid range, colStart > colEnd";
        return false;
    }
    // Iterate through the specified square range and check if cells are already merged or not before calling setSpan
    for (int row = rowStart; row <= rowEnd; row++)
    {
        for (int col = colStart; col <= colEnd; col++)
        {

            if (rowSpan(row, col) == 1 && columnSpan(row, col) == 1)
            {
                continue;
            }
            else
            {
                qInfo()<<"QBetterTableWidget::mergeSquareCells : one of the cells in the range is already merged";
                return false;
            }
        }
    }
    setSpan(rowStart,colStart,_rowSpan,_colSpan);
    return true;
}


/**
 * @brief Unmerges cells in a square range
 * @param rowStart - start row of the square
 * @param colStart - start column of the square
 * @param rowSpan - width of the square in cells
 * @param colSpan - height of the square in cells
 * @return true if the unmerge is done successfully, false otherwise
 */
bool QBetterTableWidget::unmergeSquareCells(int rowStart, int colStart, int rowSpan, int colSpan)
{
    // Check if the object is a QTableWidget
    if(!qobject_cast<QTableWidget*>(this))
    {
        qInfo()<<"QBetterTableWidget::unmergeSquareCells : the object is not a QTableWidget";
        return false;
    }

    // Check if the rowStart value is within the valid range
    if (rowStart < 0 || rowStart >= rowCount())
    {
        qInfo()<<"QBetterTableWidget::unmergeSquareCells : invalid rowStart value";
        return false;
    }
    // Check if the colStart value is within the valid range
    if (colStart < 0 || colStart >= columnCount())
    {
        qInfo()<<"QBetterTableWidget::unmergeSquareCells : invalid colStart value";
    }
    // Check if the rowSpan value is within the valid range
    if (rowSpan < 1 || rowStart + rowSpan > rowCount())
    {
        qInfo()<<"QBetterTableWidget::unmergeSquareCells : invalid rowSpan value";
        return false;
    }
    // Check if the colSpan value is within the valid range
    if (colSpan < 1 || colStart + colSpan > columnCount())
    {
        qInfo()<<"QBetterTableWidget::unmergeSquareCells : invalid colSpan value";
        return false;
    }
    //iterate trhough the cells and unmerge them
    for (int row = rowStart; row < rowStart + rowSpan; row++)
    {
        for (int col = colStart; col < colStart + colSpan; col++)
        {
            setSpan(row, col, 1, 1);
        }
    }
    return true;
}

/**
* @brief Add a title row to the table with a given text
* @param row The index of the row to add the title to
* @param text The text to display in the title row
* @return true if the row is in range and the operation was successful, false otherwise
*/
bool QBetterTableWidget::addTitleRow(int row, const QString& text)
{
    qInfo()<<"addTitleRow";
    if (row < 0 || row >= this->rowCount())
    {
        // The row is out of range
        qInfo() << "QBetterTableWidget::addTitleRow: row index is out of range";
        return false;
    }

    if (row==0)
    {
        insertRow(0);
    }

    for (int i = 0; i < this->columnCount(); ++i)
    {
        if (this->columnSpan(row, i) > 1)
        {
            // The cells in the given row are already merged
            qInfo() << "QBetterTableWidget::addTitleRow: cells in the given row are already merged";
            return false;
        }
    }

    // Set the background color of the row to 0xadd8e6
    for (int i = 0; i < this->columnCount(); ++i)
    {
        QTableWidgetItem* item = this->item(row, i);
        if(!item)
        {
            item = new QTableWidgetItem();
            this->setItem(row, i, item);
        }
        item->setBackground(QColor(0xadd8e6));
    }

    mergeRowCells(row);

    // Center the text in the row and set the font to bold
    QTableWidgetItem* item = this->item(row, 0);
    QFont font = item->font();
    font.setBold(true);
    item->setFont(font);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(text);

    return true;
}


void QBetterTableWidget::verifyOrInitializeCell(int aRow, int aCol)
{
   QTableWidgetItem* item = this->itemAt(aRow, aCol);
     if (item == nullptr)
     {
         addText(aRow,aCol,"");
     }
}

QString QBetterTableWidget::setSpecialWidgetCsvText(int aRow, int aCol)
{
    //is it a pushButton ?

    QPushButton *pb = qobject_cast<QPushButton*>(this->cellWidget(aRow,aCol));
    if (pb)
    {
        return "pushButton="+pb->text();
    }
    // no obviously if we are here so...
    //is it a QSpinBox ?
    QSpinBox *spnb = qobject_cast<QSpinBox*>(this->cellWidget(aRow,aCol));
    if (spnb)
    {
        return "spinBox=["+
                QString::number(spnb->maximum())+","+
                QString::number(spnb->minimum())+","+"]>>"+
                QString::number(spnb->value());
    }
    // no obviously if we are here so...
    //is it a QLineEdit ?
    QLineEdit *lnE = qobject_cast<QLineEdit*>(this->cellWidget(aRow,aCol));
    if (lnE)
    {
        const QValidator *validator = lnE->validator();
        if (validator)
        {
            if (validator->inherits("QIntValidator"))
            {
                // The validator is a QIntValidator
                return "intLineEdit="+lnE->text();
            }
            else if (validator->inherits("QDoubleValidator"))
            {
                // The validator is a QDoubleValidator
                return "doubleLineEdit="+lnE->text();
            }

        }
        else
        {
            // The line edit does not have a validator
            return "simpleLineEdit="+lnE->text();
        }
    }

    // no obviously if we are here so...
    //is it a Widget ?
    QString str = "";
    QWidget* _widget = qobject_cast<QWidget*>(this->cellWidget(aRow,aCol));
    if (_widget)
    {
        //then it's a container widget
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          //Is it a ComboBox ?
          QComboBox *_cmbB = qobject_cast<QComboBox*>(objLst[i]);
          if (_cmbB)
          {
              str = "comboBox=[";
              for (int j = 0; j < _cmbB->count(); ++j)
              {
                  str += _cmbB->itemText(j);
                  if (i < _cmbB->count() - 1)
                  {
                      str += ",";
                  }
              }
              str += "]>>"+_cmbB->currentText();
              return str;
          }
          // If we are here it is not... What else ?
          //is it a QBetterSwitch
          QBetterSwitchButton *swB = qobject_cast<QBetterSwitchButton*>(objLst[i]);
          if (swB)
          {

              str = "switchButton=["+
                    swB->getFalseText()+","+
                    swB->getTrueText()+","+
                    swB->getFalseColor().name()+","+
                    swB->getTrueColor().name()+"]>>";
             swB->getState() ? str+="true" : str+="false";
             return str;
        }
       // If we are here it is not... What else ?
       //is it a QColorButton
        QColorButton *colorBtn = qobject_cast<QColorButton*>(objLst[i]);
        if (colorBtn)
        {
           str = "colorButton="+colorBtn->color().name();
           return str;
        }

     }
   }
        /* QString str = widget->objectName();
        if (str==("widgetChkBx:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow)))
        {
            QCheckBox* chkbx = widget->findChild<QCheckBox*>("chkBx:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow));
            {
                return chkbx->isChecked() ? "true" : "false";
            }
        }
        else
        if (str==("widgetBtn:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow)))
        {
            QPushButton* button = widget->findChild<QPushButton*>("btn:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow));
            return "button="+button->text();
        }
        else
        if (str==("widgetComboBx:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow)))
        {
            int count;
            QString content;
            QComboBox* comboBox = widget->findChild<QComboBox*>("comboBx:Col:"+QString::number(aCol)+":Row:"+QString::number(aRow));
            count=comboBox->count();
            QStringList itemsInComboBox;
            for (int i = 0; i < count; i++)
              {
                itemsInComboBox.append(comboBox->itemText(i));
              }
            itemsInComboBox.append(QString::number(comboBox->currentIndex()));
            content = itemsInComboBox.join(comboBoxSeparator);
            return "comboBox="+content;
         }

    }*/
    return "NS";
}

void QBetterTableWidget::saveToCsv(QString aFileName, QString aSeparator)
{
    QStringList strList;
   // QString tableContent="";
    QString str;

    if (headerList.count()>0)
    {
        for (int headerIndex=0;headerIndex<headerList.count();++headerIndex)
        {
            str = "";
            QBetterHeaderView *currentHeader = headerList[headerIndex];
            if (currentHeader->getIsUnitConverter())
            {
                for (int i=0;i<this->columnCount();++i)
                {
                   QOutputUnitCombobox *currentCb = unitComboboxList[i];
                   str=str+"HUCB"
                           +QString::number(headerIndex)
                           +"_"+QString::number(currentCb->getDefaultFamillyIndex())
                           +"_"
                           +currentCb->currentText()+aSeparator;

                }
                str.chop(1);
                //hapend to list
                strList<<str;
                //strList.insert(headerIndex,str);
            }
            else if (currentHeader->getIsStdLineEdit())
            {
               //the case of a standard line edit header
               QVector<QLineEdit   *> vect = headerLineEditList[headerIndex];
               if (vect.count()>0)
               {

                   for (int i=0;i<this->columnCount();++i)
                   {
                      QLineEdit *currentLe = vect[i];
                      if (currentLe)
                      {
                        str=str+"H"+QString::number(headerIndex)+"_"+currentLe->text()+aSeparator;
                      }
                }
               str.chop(1);
               //hapend to list
               //strList.insert(headerIndex,str);
               strList<<str;
             }

        }
    }
  }


 int nbRow = this->rowCount();
 int nbCol = this->columnCount();

  for(int _row=0;_row < nbRow; ++_row)
  {
      str = "";
      for (int _col=0;_col < nbCol; ++_col)
      {
         // verifyOrInitializeCell(_row,_col);
          QString specialStr = setSpecialWidgetCsvText(_row,_col);
          if (specialStr=="NS")
          {
              QTableWidgetItem* _item = this->itemAt(_row, _col);
              if (_item != nullptr)
              {
                  str += _item->text()+aSeparator;
              }
              else
              {
                  str +=""+aSeparator;
              }

          }
          else
          {
              str += specialStr+aSeparator;
          }

      }

      str.chop(1);
    //hapend to list
      strList<<str;

    }

    QMessageBox bx;
    bx.setText(strList.join("\n"));
    bx.exec();

//        QString str = "";
//        QVector<QLineEdit   *> vect = headerLineEditList[headerIndex];
        //the case where there's a line Edit
//        if (vect.count()>0)
//        {
//            for (int i=0;i<this->columnCount();++i)
//            {
//               QLineEdit *currentLe = vect[i];
//               if (currentLe)
///               {
//                  str=str+"H"+QString::number(headerIndex)+"_"+currentLe->text()+aSeparator;
//
//               }


//            }
            //remove the last char
//            str.chop(1);
            //hapend to list
//            strList.append(str);
//        }
        //The case if it's a unit combobox
      //  else
//        {
//            for (int j=0;j<unitComboboxList.count();++j)
//             {
//                QOutputUnitCombobox *currentCb = unitComboboxList[j];
//                if (currentCb->getHeaderIndex()==headerIndex)
//                {
//                    str = "";
//                    QString _str = QString::number(currentCb->getHeaderIndex())+" "+QString::number(headerIndex);
//                    QMessageBox msg;
//                    msg.setText(_str);
//                    msg.exec();
//                    for (int k=0;k<this->columnCount();++k)
//                    {
//                       str=str+"HCB"
//                              +QString::number(headerIndex)
//                              +"_"
//                             +currentCb->currentText()+aSeparator;

//                    }
//                   str.chop(1);
                   //hapend to list
//                   strList.insert(headerIndex,str);
//                }
//             }
//        }
//        QString _str = strList.join("\n");
//        QMessageBox msg;
//        msg.setText(_str);
//        msg.exec();
//    }
  /*  for(int col=0;col < nbCol; ++col)
    {
        if (!myTableWidget->horizontalHeaderItem(col))
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            myTableWidget->setHorizontalHeaderItem(col,item);
        }
        topHeader = topHeader+myTableWidget->horizontalHeaderItem(col)->text();
        if (col<nbCol-1)
        {
          topHeader = topHeader+aSeparator;
        }
    }
    strList << topHeader;
    for(int row=0;row < nbRow; ++row)
    {
        for (int col=0;col < nbCol; ++col)
        {
            verifyOrInitializeCell(row,col);
            QString str = setSpecialWidgetCsvText(row,col);
            if (str=="NS")
            {
                tableContent = tableContent + myTableWidget->item(row,col)->text();
            }
            else
            {
                tableContent = tableContent + str;
            }
            if (col<nbCol-1)
            {
                tableContent = tableContent+aSeparator;
            }
            else
            {
               strList << tableContent;
               tableContent ="";
            }
        }

    }
   QFile fileOut(aFileName);
   if (fileOut.open(QFile::WriteOnly | QFile::Text))
    {
      QTextStream s(&fileOut);
     for (int i = 0; i < strList.size(); ++i)
            s << strList.at(i) << '\n';
    }
    else
    {
      qCritical() << "error opening output file: "<<aFileName;
      return ;
     }
   fileOut.close();*/
}



void QBetterTableWidget::saveToXlsxFile(const QString &fileName)
{

     if (xlsx != nullptr)
     {
         delete (xlsx);
         xlsx = nullptr;
     }
     // Create a new document
     xlsx = new QXlsx::Document(this);
     xlsx->addSheet(fileName);
     xlsx->selectSheet(fileName);
     QTableWidgetItem    *_item       = nullptr;
     QWidget             *_widget     = nullptr;
     for (int row = 0; row < rowCount(); ++row)
     {
         for (int col = 0; col < columnCount(); ++col)
         {
            _item = item(row,col);
            if (_item != nullptr)
            {
                QString str =   _item->text();
                xlsx->write(row+1, col+1,str);
                calculateXlsxTextSize(xlsx,col,str);

            }
            else
            {
               _widget = cellWidget(row, col);
               if (_widget != nullptr)
               {
                  if (!XlsxCheckForButton     (xlsx,_widget,row,col,fileName))
                  if (!XlsxCheckForSwitch     (xlsx,_widget,row,col,fileName))
                  if (!XlsxCheckForSpinBox    (xlsx,_widget,row,col,fileName))
                  if (!XlsxCheckForQComboBox  (xlsx,_widget,row,col,fileName))
                  if (!XlsxCheckForQSizeWidget(xlsx,_widget,row,col,fileName))
                  if (!XlsxCheckForQRectWidget(xlsx,_widget,row,col,fileName))
                  {
                  }
               }
            }
         }
      }
  xlsx->saveAs(fileName);
}


bool QBetterTableWidget::XlsxCheckForSwitch(QXlsx::Document *_xlsx,
                                            QWidget *_widget,
                                            int _row,
                                            int _col, QString fileName)
{
    QBetterSwitchButton *_switch     = nullptr;
    _switch = qobject_cast<QBetterSwitchButton*>(_widget);
    if (_switch)
    {
       _xlsx->write(_row+1,_col+1, QVariant(bool(_switch->getState())));
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _switch = qobject_cast<QBetterSwitchButton*>(objLst[i]);
          if (_switch)
          {
             _xlsx->write(_row+1,_col+1,QVariant(bool(_switch->getState())));
             return true;
          }
        }

    }
   return false;
}

bool QBetterTableWidget::XlsxCheckForButton(QXlsx::Document *_xlsx,
                                            QWidget *_widget,
                                            int _row,
                                            int _col,
                                            QString fileName)
{
    _xlsx->selectSheet(fileName);
    QPushButton *_btn     = nullptr;
    _btn = qobject_cast<QPushButton*>(_widget);
    if (_btn)
    {
       QString str = _btn->text();
        _xlsx->write(_row+1,_col+1,str);
       calculateXlsxTextSize(_xlsx,_col,str);
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _btn = qobject_cast<QPushButton*>(objLst[i]);
          if (_btn)
          {
              QString str = _btn->text();
               _xlsx->write(_row+1,_col+1,str);
              calculateXlsxTextSize(_xlsx,_col,str);
             return true;
          }
        }

    }
    return false;
}

bool QBetterTableWidget::XlsxCheckForSpinBox(QXlsx::Document *_xlsx,
                                             QWidget *_widget,
                                             int _row,
                                             int _col,
                                             QString fileName)
{
    _xlsx->selectSheet(fileName);
    QSpinBox *_spnb     = nullptr;
    _spnb = qobject_cast<QSpinBox*>(_widget);
    if (_spnb)
    {
        QXlsx::Format format;
        format.setNumberFormat("General");
       _xlsx->write(_row+1,_col+1,_spnb->value(),format);
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _spnb = qobject_cast<QSpinBox*>(objLst[i]);
          if (_spnb)
          {
              QXlsx::Format format;
              format.setNumberFormat("General");
             _xlsx->write(_row+1,_col+1,_spnb->value(),format);
             return true;
          }
        }

    }
    return false;
}

bool QBetterTableWidget::XlsxCheckForQSizeWidget(QXlsx::Document *_xlsx, QWidget *_widget, int _row, int _col, QString fileName)
{
    _xlsx->selectSheet(fileName);
    QSizeWidget *_sw     = nullptr;
    _sw = qobject_cast<QSizeWidget*>(_widget);
    if (_sw)
    {
        QSize size= _sw->getQSize();
        QString str = QString("%1 x %2").arg(QString::number(size.width()),
                                             QString::number(size.height()));
       _xlsx->write(_row+1,_col+1,str);
       calculateXlsxTextSize(_xlsx,_col,str);
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _sw = qobject_cast<QSizeWidget*>(objLst[i]);
          if (_sw)
          {
              QSize size= _sw->getQSize();
              QString str = QString("%1 x %2").arg(QString::number(size.width()),
                                                   QString::number(size.height()));
             _xlsx->write(_row+1,_col+1,str);
             calculateXlsxTextSize(_xlsx,_col,str);
             return true;
          }
        }

    }
    return false;
}

bool QBetterTableWidget::XlsxCheckForQComboBox(QXlsx::Document *_xlsx,
                                               QWidget *_widget,
                                               int _row,
                                               int _col,
                                               QString fileName)
{
    QComboBox *_cmbB     = nullptr;
    _cmbB = qobject_cast<QComboBox*>(_widget);
    if (_cmbB)
    {
        _xlsx->addSheet(_cmbB->objectName());
        _xlsx->selectSheet(_cmbB->objectName());
        QXlsx::Worksheet *worksheet =_xlsx->currentWorksheet();
        for (int i=0;i<_cmbB->count();++i)
        {
            worksheet->writeString(i+1,1,_cmbB->itemText(i));
        }
        QString sheet_reference_str = "'"+
                                      _cmbB->objectName()+
                                      "'!$A$1:$A$"+
                                      QString::number(_cmbB->count());

        QXlsx::DataValidation validation(QXlsx::DataValidation::List,
                                         QXlsx::DataValidation::Between,
                                          sheet_reference_str);



        validation.addCell(_row+1,_col+1);
        _xlsx->selectSheet(fileName);
        worksheet =_xlsx->currentWorksheet();
        worksheet->addDataValidation(validation);
        worksheet->write(_row+1,_col+1, _cmbB->currentText());
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _cmbB = qobject_cast<QComboBox*>(objLst[i]);
          if (_cmbB)
          {
              _xlsx->addSheet(_cmbB->objectName());
              _xlsx->selectSheet(_cmbB->objectName());
              QXlsx::Worksheet *worksheet =_xlsx->currentWorksheet();
              for (int i=0;i<_cmbB->count();++i)
              {
                  worksheet->writeString(i+1,1,_cmbB->itemText(i));
              }
              QString sheet_reference_str = "'"+
                                            _cmbB->objectName()+
                                            "'!$A$1:$A$"+
                                            QString::number(_cmbB->count());

              QXlsx::DataValidation validation(QXlsx::DataValidation::List,
                                               QXlsx::DataValidation::Between,
                                                sheet_reference_str);



              validation.addCell(_row+1,_col+1);
              _xlsx->selectSheet(fileName);
              worksheet =_xlsx->currentWorksheet();
              worksheet->addDataValidation(validation);
              worksheet->write(_row+1,_col+1, _cmbB->currentText());
             return true;
          }
        }

    }
    return false;
}



bool QBetterTableWidget::XlsxCheckForQRectWidget(QXlsx::Document *_xlsx,
                                                 QWidget *_widget,
                                                 int _row,
                                                 int _col,
                                                 QString fileName)
{
    _xlsx->selectSheet(fileName);
    QRectWidget *_rect     = nullptr;
    _rect = qobject_cast<QRectWidget*>(_widget);
    if (_rect)
    {
        QRect rect = _rect->rect();
        QString str = QString("Top: %1 ; Left: %2 ; Width: %3 ; Height: %4").
                      arg(QString::number(rect.top()),
                          QString::number(rect.left()),
                          QString::number(rect.width()),
                          QString::number(rect.height()));

       _xlsx->write(_row+1,_col+1,str);
       calculateXlsxTextSize(_xlsx,_col,str);
       return true;
    }
    else
    {
        QObjectList objLst = _widget->children();
        for (int i=0;i<objLst.count();++i)
        {
          _rect = qobject_cast<QRectWidget*>(objLst[i]);
          if (_rect)
          {
              QRect rect = _rect->rect();
              QString str = QString("Top : %1 Left: %2 \n Width: %3 Height: %4").
                            arg(QString::number(rect.top()),
                                QString::number(rect.left()),
                                QString::number(rect.width()),
                                QString::number(rect.height()));
             _xlsx->write(_row+1,_col+1,str);
             calculateXlsxTextSize(_xlsx,_col,str);
             return true;
          }
        }

    }
    return false;
}

void QBetterTableWidget::searchNumericBetweenOnAllTable(double minValue,
                                                        double maxValue)
{
    resetTableHighlightning();
    clearSelection();
    if (searchOnColOnly)
    {
       searchNumericBetweenOnCol(minValue,maxValue);
       return;
    }

    //pevent ressource exhausting while seraching
    setUpdatesEnabled(false);
    // Block signals to prevent unwanted behavior while table is being updated
    blockSignals(true);
    // Iterate through all rows and columns in the table
    for(int i = 0; i < rowCount(); ++i)
    {
        for(int j = 0; j < columnCount(); ++j)
        {
            // Get the current table item
            QTableWidgetItem *item = this->item(i, j);
            if(item != nullptr)
            {
                bool ok;
                // Convert item text to a double and check if conversion was successful
                double value = item->text().toDouble(&ok);
                if (ok && value >= minValue && value <= maxValue)
                {
                    // Set the background color to highlight the item
                    item->setBackground(tableHighLightColor);
                }
                else
                {
                    // Reset the background color
                    resetItemBackGround(item,i);
                }
            }
        }
    }
    //clear the selection in the table
    clearSelection();
    // Re-enable signals for the table
    blockSignals(false);
    setUpdatesEnabled(true);
    update();
}

void QBetterTableWidget::searchNumericValueOnAllTable(double searchValue,
                                                      NumericSearchType searchType)
{
    resetTableHighlightning();
    clearSelection();
    blockSignals(true);
    setUpdatesEnabled(false);

    if (searchOnColOnly)
    {
        searchNumericValueOnCol(searchValue,searchType);
        return;
    }

    blockSignals(true);
    for(int i = 0; i < rowCount(); ++i)
    {
        for(int j = 0; j < columnCount(); ++j)
        {

            QTableWidgetItem *item = this->item(i, j);
            if(item != nullptr)
            {
                bool conversionSucces;
                double value = item->text().toDouble(&conversionSucces);
                if (conversionSucces)
                {
                   switch (searchType)
                   {
                       case egal :
                       {
                           value == searchValue ? item->setBackground(tableHighLightColor)
                                                : resetItemBackGround(item,i);
                       }
                       break;
                       case inferiorOrEgal :
                       {
                           value <= searchValue ? item->setBackground(tableHighLightColor)
                                                : resetItemBackGround(item);
                       }
                       break;
                       case strictlyInferior:
                       {
                           value < searchValue ? item->setBackground(tableHighLightColor)
                                               : resetItemBackGround(item);

                       }
                       break ;
                      case superiorOrEgal:
                      {
                          value >= searchValue ? item->setBackground(tableHighLightColor)
                                                : resetItemBackGround(item);
                      }
                      break;
                      case strictlySuperior:
                      {
                          value > searchValue ? item->setBackground(tableHighLightColor)
                                              : resetItemBackGround(item);
                      }
                      break;
                     case between : break;

                   }
                }

            }
        }
    }

    clearSelection();
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
}

void QBetterTableWidget::searchNumericValueOnCol(double searchValue,
                                                 NumericSearchType searchType)
{
    resetTableHighlightning();
    clearSelection();
    blockSignals(true);
    setUpdatesEnabled(false);
    for(int i = 0; i < rowCount(); ++i)
    {
       QTableWidgetItem *item = this->item(i, searchColIndex);
       if(item != nullptr)
       {
           bool convertionSucces = false;
           double value = item->text().toDouble(&convertionSucces);
           if (convertionSucces)
           {
                switch (searchType)
                {
                    case egal :
                    {
                       value == searchValue ? item->setBackground(tableHighLightColor)
                                            : resetItemBackGround(item,i);
                    }
                    break;
                    case inferiorOrEgal :
                    {
                        value <= searchValue ? item->setBackground(tableHighLightColor)
                                             : resetItemBackGround(item,i);
                    }
                    break;
                    case strictlyInferior:
                    {
                        value < searchValue ? item->setBackground(tableHighLightColor)
                                            : resetItemBackGround(item,i);
                    }
                    break ;
                   case superiorOrEgal:
                   {
                       value >= searchValue ? item->setBackground(tableHighLightColor)
                                            : resetItemBackGround(item,i);
                   }
                   break;
                   case strictlySuperior:
                   {
                       value > searchValue   ? item->setBackground(tableHighLightColor)
                                             : resetItemBackGround(item,i);
                   }
                   break;
                  case between : break;
                }
            }
       }

    }
    clearSelection();
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
}

void QBetterTableWidget::searchNumericBetweenOnCol(double minValue, double maxValue)
{
    try {
            resetTableHighlightning();
            setUpdatesEnabled(false);
            blockSignals(true);
            for(int i = 0; i < rowCount(); ++i)
            {
                    QTableWidgetItem *item = this->item(i, searchColIndex);
                    if(item != nullptr)
                    {
                        double value = item->text().toDouble();
                        if(value >= minValue && value <= maxValue)
                        {
                            item->setBackground(tableHighLightColor);
                        }
                        else
                        {
                            resetItemBackGround(item,i);
                        }
                    }
            }
       clearSelection();
       blockSignals(false);
       setUpdatesEnabled(true);
       update();
       ensurePolished();
    }
    catch (const std::exception &e)
    {
        qDebug() << "Exception caught in function QBetterTableWidget::searchBetweenOnCol: " << e.what();
        clearSelection();
        blockSignals(false);
        setUpdatesEnabled(true);
        update();
        ensurePolished();
    }
}

void QBetterTableWidget::realignHeaderUnitComboxes(int headerIndex)
{
    for (int i=0;i<this->columnCount();++i)
    {
       if (i<unitComboboxList.count())
       {
           QOutputUnitCombobox *currentCb = unitComboboxList[i];
           if (currentCb)
           {
               QWidget *w = currentCb->parentWidget();
               if (w)
               {
                  w->setGeometry (headerList[headerIndex]->sectionViewportPosition(i),
                                  0,
                                  headerList[headerIndex] ->sectionSize(i)-2 ,
                                  headerList[headerIndex]->height()-2);
              }
           }
       }
    }
}


void QBetterTableWidget::realignHeaderLineEdits(int headerIndex)
{
   if (headerIndex>0)
   {
         QVector<QLineEdit   *> vect = headerLineEditList[headerIndex];
         //QVector<QLineEdit   *> vect = headerLineEditList[headerIndex-1];
         if (vect.count()>0)
         {
             for (int i=0;i<this->columnCount();++i)
             {
                QLineEdit *currentLe = vect[i];
                if (currentLe)
                {

                    currentLe->setGeometry (headerList[headerIndex]->sectionViewportPosition(i),
                                            0,
                                            headerList[headerIndex] ->sectionSize(i)-2 ,
                                            headerList[headerIndex]->height()-2);
                 }

             }
         }
   }
}





void QBetterTableWidget::resetTableHighlightning()
{
    blockSignals(true);
    setUpdatesEnabled(false);
    for (int i = 0; i < rowCount(); ++i)
        for (int j = 0; j < columnCount(); ++j)
         {
            QTableWidgetItem *_item = item(i, j);
            if (_item != nullptr)
            {

              _item->setBackground((useAlternateLineColor) ?
                                   ((i%2==0) ? alternateLineColor1 : alternateLineColor2)
                                   : tableBackgroundColor);
            }
         }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
}

void QBetterTableWidget::resetItemBackGround(QTableWidgetItem *item, int index)
{
    item->setBackground((useAlternateLineColor) ?
                        ((index%2==0) ? alternateLineColor1 : alternateLineColor2)
                          : tableBackgroundColor);
}

void QBetterTableWidget::resetItemBackGround(QTableWidgetItem *item)
{
  int _row =row(item);
  item->setBackground((useAlternateLineColor) ?
                      ((_row%2==0) ? alternateLineColor1 : alternateLineColor2)
                        : tableBackgroundColor);
}


void QBetterTableWidget::delSelection()
{
    QList<QTableWidgetItem*> selectedItems = this->selectedItems();
    for (QTableWidgetItem* item : selectedItems)
    {
        if (item != nullptr)
        {
            this->removeCellWidget(item->row(), item->column());
            delete item;
        }
    }
}



 /* QXlsx::Document xlsx;
  for (int row = 1; row < rowCount(); ++row)
  {
     for (int col = 0; col < columnCount(); ++col)
     {
         QTableWidgetItem* item = this->itemAt(row, col);
         if (item != nullptr)
         {
             QWidget* widget = this->cellWidget(row, col);
             if (widget != nullptr)
             {
                 QMessageBox msgBox;
                 QString str = QString("ROW %1 COL %2 IS WIDGET").arg(QString::number(row),
                                                                     QString::number(col));
                 msgBox.setText(str);
                 msgBox.exec();
            }
            else
            {
                QMessageBox msgBox;
                QString str = QString("ROW %1 COL %2 TEXT: %3").arg(QString::number(row),
                                                                    QString::number(col),
                                                                    item->text());
                msgBox.setText(str);
                msgBox.exec();

                xlsx.write(row, col, item->text());
           }
         }
         else
         {
             xlsx.write(row, col, "not an item");
         }
     }
  }
  xlsx.saveAs(fileName);*/
//}

/*void QBetterTableWidget::saveToXlsxFile(const QString &fileName)
{
   // Create a new document
   QXlsx::Document xlsx;
   // Iterate through the rows and columns of the QPropertyEditor
   int rowCount = this->rowCount();
   int colCount = this->columnCount();
   int colWidth;
   int rowHeight;
   int textWidth;
   int imgWidth;
   int imgHeight;
   for (int i = 0; i < rowCount; ++i)
   {
      for (int j = 0; j < colCount; ++j)
      {
         /* QWidget* widget = this->cellWidget(i, j);
          if (widget != nullptr)
          {
              //create a pixmap from the widget
              QPixmap pixmap = widget->grab();
              QImage image = pixmap.toImage();
              //insert the picture in the xlsx
              colWidth  = xlsx.columnWidth(j);
              rowHeight = xlsx.rowHeight(i);
              imgWidth  = image.width();
              imgHeight = image.height();
              if(imgWidth > colWidth)
              {
                  xlsx.setColumnWidth(j, imgWidth);
              }
              if(imgHeight>rowHeight)
              {
                  xlsx.setRowHeight(i,imgHeight);
              }
              xlsx.insertImage(i, j, image);
          }
          else
          {
             QTableWidgetItem* item = this->item(i, j);
             if (item != nullptr)
             {
                 qInfo()<<"FOUND:"<<item->text();
                 // check if cell is merged

                 /*int rowSpan = this->rowSpan(i, j);
                 int colSpan = this->columnSpan(i, j);
                 if (rowSpan > 1 || colSpan > 1)
                 if (rowSpan > 1 || colSpan > 1)
                 {
                     // merge cells in excel
                     xlsx.mergeCells(QXlsx::CellRange(i, j, i+rowSpan, j+colSpan));
                 }

                    //  colWidth  = xlsx.columnWidth(j);
                    //  textWidth = item->text().length()*1.5;
                     // if(textWidth > colWidth)
                     //     xlsx.setColumnWidth(j, textWidth);
                      xlsx.write(i, j, item->text());
                 }
             }
               else
               {
                   QString str = "not implemented";
                   colWidth  = xlsx.columnWidth(j);
                   textWidth = str.length()*1.5;
                   if(textWidth > colWidth)
                       xlsx.setColumnWidth(j, textWidth);
                   xlsx.write(i, j, str);
               }
           } //end for clocount
       } //end for rowcount

   // Save the document
   xlsx.saveAs(fileName);
}*/



/**
 * @brief This function creates a new horizontal header and adds it to the QBetterTableWidget.
 *
 * It creates a new horizontal header, sets it as the horizontal header for the QBetterTableWidget,
 * appends it to a headerList vector, and then connects the sectionResized signal of the new header and the horizontalHeader
 * to a lambda function that resizes all the headers' sections at the same time, this way all the cells width stay synchronized
 * when the user resize one.
 *
 * @return pointer to the new header
 */
QBetterHeaderView*  QBetterTableWidget::addNewHeader()
//QHeaderView*  QBetterTableWidget::addNewHeader()
{
    // Create new header
  //  QHeaderView *result = new QHeaderView(Qt::Horizontal);
    qInfo()<<"ENTER ADD WIDGET";
    QBetterHeaderView *result = new QBetterHeaderView(Qt::Horizontal);
    // Check if the header was created successfully
    if (!result)
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Failed to create new header";
        return nullptr;
    }
     qInfo()<<"RESULT OK";
    result->setParent(globalTableContainer);
    // Set the model of the new header to be the same as the horizontal header
     qInfo()<<"PARENT OK";
    result->setModel(this->horizontalHeader()->model());
     qInfo()<<"SET MODEL OK";
    // Check if the model was set successfully
    if (!result->model())
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Failed to set model for new header";
        return nullptr;
    }
     qInfo()<<"SET MODEL OK 2";
    //set height of the new header
//    result->setMaximumHeight(headerList[0]->height());
//    result->setMinimumHeight(headerList[0]->height());

     result->setMaximumHeight(this->horizontalHeader()->height());
     result->setMinimumHeight(this->horizontalHeader()->height());
     qInfo()<<"result->setMaximumHeight(headerList[0]->height()); OK";
    // Create new frame to be used as a spacer
    QFrame *leftFrame = new QFrame(globalTableContainer);
    qInfo()<<"QFrame OK";
    // Check if the frame was created successfully
    if (!leftFrame)
    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Failed to create new frame";
        return nullptr;
    }

    // Set frame style and size
    leftFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    leftFrame->setLineWidth(0);
    leftFrame->setMidLineWidth(0);
    qInfo()<<"leftFrame->setMidLineWidth(0); OK";
   // leftFrame->setFixedSize(verticalHeader()->width()+1,headerList[0]->height());
    leftFrame->setFixedSize(verticalHeader()->width()+1,horizontalHeader()->height());
    QPalette palette = leftFrame->palette();
    palette.setColor(QPalette::Window, this->palette().color(QPalette::Base));
    leftFrame->setAutoFillBackground(true);
    leftFrame->setPalette(palette);
    //add frame to list for easing destruction
    frameList.append(leftFrame);
    //remove globalContainer from Layout
    tableLayout->removeWidget(this);
    //update globalContainer position in layout
    headerList.append(result);
    //add new Header to layout*/
    tablePosInLayout++;
    tableLayout->setContentsMargins(0,0,0,0);
    tableLayout->setSpacing(0);
    qInfo()<<"tableLayout->setSpacing(0); OK";
    for (int i=0;i<headerList.count();++i)
    //for (int i=1;i<headerList.count();++i)
    {
        qInfo()<<"i="<<i;
        //QHeaderView *currentHeader =  headerList[i];
         QBetterHeaderView *currentHeader =  headerList[i];
         qInfo()<<currentHeader;
        // QFrame      *currentFrame  =  frameList[i-1];
         QFrame      *currentFrame  =  frameList[i];
         qInfo()<<"QFrame      *currentFrame  =  frameList[i-1]; OK";
         if (currentHeader!=nullptr && currentFrame!=nullptr)
         {
            qInfo()<<"currentHeader!=nullptr && currentFrame!=nullptr; OK";

            tableLayout->removeWidget(currentHeader);
            tableLayout->removeWidget(currentFrame);
            tableLayout->addWidget(currentFrame,i,0,1,1);
            tableLayout->addWidget(currentHeader,i,1,1,1);
            // Connect the sectionResized signal of the horizontal header
            //and the new header with a lambda function that resizes all
            //the headers' sections
            connect(horizontalHeader(),
                    &QBetterHeaderView::sectionResized,
                   // &QHeaderView::sectionResized,
                    this,
                    [i, this](int logicalIndex, int oldSize, int newSize)
                    {
                       headerList[i]->resizeSection(logicalIndex, newSize);
                       realignHeaderUnitComboxes(i);
                       realignHeaderLineEdits(i);
                    });

            connect(headerList[i],
                    //&QHeaderView::sectionResized, this,
                    &QBetterHeaderView::sectionResized, this,
                    [this](int logicalIndex,
                           int oldSize,
                           int newSize)
                    {
                        this->horizontalHeader()->resizeSection(logicalIndex,
                                                                newSize);

                        realignHeaderUnitComboxes(0);
                        realignHeaderLineEdits(0);
                    });
         }
    }

  tableLayout->addWidget(this,headerList.count(),0,1,2);
  QVector<QLineEdit *> vect;
  headerLineEditList.append(vect);
  return result;
}


void QBetterTableWidget::setHeaderModelFromCsvString(int headerIndex,
                                                     QString aCsvString,
                                                     QString separator)
{
    QStringList headerLabels = aCsvString.split(separator);
    if(headerIndex == 0)
    {
        setHorizontalHeaderLabels(headerLabels);
    }
    else if (headerIndex == 1 && horizontalHeader()->count() == 2)
    {
        horizontalHeader()->setModel(new QStringListModel(headerLabels));
    }
}


/*bool QBetterTableWidget::setRowFromCsvString(int aRow,
                                             QString aCsvHeaderString,
                                             QString aSeparator)
{
    QStringList strList;
    QString str="";
    int colCount = 0;
    strList  = aCsvHeaderString.split(aSeparator);
    colCount = strList.count();
    if (this->rowCount()-1)
        this->setRowCount(aRow+1);
    for (int col=0; col<colCount;++col )
    {
        str = strList[col];
        if (str=="true")
        {
           addSwitchWidget(aRow,col,true);
        }
        else if (str=="false")
        {
            addSwitchWidget(aRow,col,false);
        }
        else if (str.contains(QString("button=")))
        {
            QStringList splited = str.split("=");
            addPushBtn(aRow,col,splited.at(1));
        }
        else if (str.contains(QString("comboBox=")))
            {
                str.remove(0, 9);
                int selectedIndex;
                QStringList splited = str.split(comboBoxSeparator);
                selectedIndex = splited.last().toInt();
                splited.removeLast();
                QComboBox *cb = addSimpleComboBox(aRow,col,splited);
                cb->blockSignals(true);
                cb->setCurrentIndex(selectedIndex);
                cb->blockSignals(false);
            }
        else
        addText(aRow,col,strList.at(col));
    }
  return true;
}*/

/**
 * @brief Set the values of a row in the table widget from a CSV string.
 * @param aRow The row to set the values for.
 * @param aCsvHeaderString The CSV string to set the values from.
 * @param aSeparator The separator used in the CSV string.
 * @return True if the values were set successfully, false otherwise.
 * This function splits the CSV string into a list of strings using the separator,
 * and then adds widgets to the cells of the specified row based on the values in
 * the list of strings. If a string value is "true" or "false", a switch widget is
 * added to the cell. If the string value starts with "button=", a push button is
 * added to the cell. If the string value starts with "comboBox=", a combo box is
 * added to the cell. If none of these conditions are met, a text widget is added
 * to the cell.
 */
bool QBetterTableWidget::setRowFromCsvString(int aRow, QString aCsvHeaderString, QString aSeparator)
{
    // Define variables to store split header string and column count
    QStringList strList;
    int colCount = 0;

    // Split the header string based on the separator and store the result in strList
    strList = aCsvHeaderString.split(aSeparator);
    colCount = strList.count();

    // If the number of rows in the table is not enough, set the row count to aRow + 1
    if (this->rowCount()-1)
        this->setRowCount(aRow+1);

    // Loop through each column
    for (int col=0; col<colCount;++col )
    {
        // Get the string for current column
        QString str = strList[col];

        // Check if the string is "true"
        if (str == "true")
        {
            // Add a switch widget to the specified row and column with the state set to true
            if (!addSwitchWidget(aRow, col, true))
            {
                qInfo() << "QBetterTableWidget::" << __FUNCTION__ << "Failed to add switch widget with state true at row:" << aRow << "column:" << col;
                return false;
            }
        }
        // Check if the string is "false"
        else if (str == "false")
        {
            // Add a switch widget to the specified row and column with the state set to false
            if (!addSwitchWidget(aRow, col, false))
            {
                qInfo() << "QBetterTableWidget::" << __FUNCTION__ << "Failed to add switch widget with state false at row:" << aRow << "column:" << col;
                return false;
            }
        }
        // Check if the string starts with "button="
        else if (str.contains(QString("button=")))
        {
            // Split the string after "=" and add a push button to the specified row and column with the text set to the result
            QStringList splited = str.split("=");
            if (!addPushBtn(aRow, col, splited.at(1)))
            {
                qInfo() << "QBetterTableWidget::" << __FUNCTION__ << "Failed to add push button at row:" << aRow << "column:" << col;
                return false;
            }
        }
        // Check if the string starts with "comboBox="
        else if (str.contains(QString("comboBox=")))
        {
            // Remove "comboBox=" from the string, split the string based on the separator and add a combo box to the specified row and column with the items set to the result
            str.remove(0, 9);
            QStringList splited = str.split(comboBoxSeparator);
            int selectedIndex = splited.last().toInt();
            splited.removeLast();
            QComboBox *cb = addSimpleComboBox(aRow, col, splited);

            // If adding the combo box failed, log the error and return false
                if (!cb) {
                    qInfo() << "QBetterTableWidget::" << __FUNCTION__ << " - failed to add combo box widget";
                    return false;
                }
                cb->blockSignals(true);
                cb->setCurrentIndex(selectedIndex);
                cb->blockSignals(false);
            }
        else
        {
            if (!addText(aRow,col,strList.at(col))) {
                qInfo() << "QBetterTableWidget::" << __FUNCTION__ << " - failed to add text widget";
                return false;
            }
        }
    }
  return true;
}



void QBetterTableWidget::onButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int row = indexAt(btn->pos()).row();
    int col = indexAt(btn->pos()).column();
    emit buttonClicked(row, col);
}

void QBetterTableWidget::onStateChanged()
{
    QBetterSwitchButton *sw = qobject_cast<QBetterSwitchButton*>(sender());
    int row = indexAt(sw->pos()).row();
    int col = indexAt(sw->pos()).column();
    emit switchChanged(row, col,sw->getState());
}

void QBetterTableWidget::onComboBoxChanged(int index)
{
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    int row = indexAt(cb->pos()).row();
    int col = indexAt(cb->pos()).column();
    emit comboBoxChanged(row, col, index);
}

void QBetterTableWidget::onHorizontalScrollBarChanged(int value)
{
    int scrollBarOffset = horizontalHeader()->offset();
    //int scrollBarOffset = headerList[0]->offset();
    for(int i=0;i<headerList.count();++i)
   // for(int i=1;i<headerList.count();++i)
    {
        headerList[i]->setOffset(scrollBarOffset);
        realignHeaderUnitComboxes(i);
        realignHeaderLineEdits(i);
    }

}

void QBetterTableWidget::onItemChanged(QTableWidgetItem *item)
{
  int row = item->row();
  if (row % 2 == 0)
  {
      item->setBackground(alternateLineColor1);
  }
  else
  {
      item->setBackground(alternateLineColor2);
  }
}


/**
 * @brief Handles the change of unit for a specific column in the table widget
 *
 * This function is connected to the textChanged() signal of
 * the QOutputUnitCombobox class
 * and is called when the user changes the unit displayed in the combobox.
 * The function
 * updates all the cells in the specified column with the new unit.
 *
 * @param oldText The previous unit displayed in the combobox
 * @param newText The new unit that the user has selected
 */
void QBetterTableWidget::onUnitCbTextChanged(const QString &oldText,
                                             const QString &newText)
{
    QOutputUnitCombobox *ucb = qobject_cast<QOutputUnitCombobox*>(sender());
    if (ucb)
    {
        //get the rowcount
        int nbRow=rowCount();
        //get the col we need to update
        int colToUpdate = ucb->getHeaderColIndex();
        //get the new unit
        QString currentUnit = ucb->getCurrentUnit();
        //and the default unit
        QString defaultUnit = ucb->getDefaultUnit();
        qInfo()<<"unit change signal at Col:"
               <<ucb->getHeaderColIndex()
               <<"\nheader index:"
               <<ucb->getHeaderIndex()
               <<"\nnew unit is:"
               <<currentUnit
               <<"\nindex is:"
               <<ucb->currentIndex()
               <<"\ndefault unit is:"
               <<defaultUnit;
       //create a dispatcher
       QUnitDispatcher *unitDispatcher = new QUnitDispatcher();
       //save ressources
       blockSignals(true);
       setUpdatesEnabled(false);
       //convert all the values in the table
       for (int aRow=0;aRow<nbRow;++aRow)
       {

          QTableWidgetItem *_item = item(aRow, colToUpdate);
          if (_item != nullptr)
          {
              //it's an item, so go on !
              bool convertSucces = false;
              double inputValue  = _item->text().toDouble(&convertSucces);
              //security check
              if (convertSucces)
              {
                 //if it can be converted to double it can be converted
                 //from one unit to another (aka if it can bleed, it can die)
                  double outputValue = unitDispatcher->
                                       convFromUnitToUnit(inputValue,
                                                          0.0,
                                                          oldText,
                                                          newText,
                                                          ucb->getDefaultFamillyIndex());
                  //change the cell according to the value
                  _item->setText(QString::number(outputValue));
              }
          }
          else
          {
              //TO do later
          }

       }
     blockSignals(false);
     setUpdatesEnabled(true);
     update();
     ensurePolished();
     delete(unitDispatcher);
     unitDispatcher = nullptr;
   }

 }


void QBetterTableWidget::onSearchNumericAsked(NumericSearchType searchType,
                                              double minValue,
                                              double maxValue,
                                              bool   onCol)
{
   searchOnColOnly = onCol;
   if (searchType == between)
            searchNumericBetweenOnAllTable(minValue,maxValue);
        else
            searchNumericValueOnAllTable(minValue,searchType);
}

/**
 * @brief This function fills the table with data from a QAbstractItemModel.
 *
 * The function starts by blocking signals of the table to prevent unnecessary signal emissions,
 * disabling updates to improve the performance, clearing the table and setting the number of rows and columns to match the model.
 * It then loops through each cell in the model, creating a new QTableWidgetItem object for each cell,
 * setting its text to the data from the model and then adds the item to the corresponding cell in the table.
 * After the table is filled, it restores updates and unblocks signals.
 *
 * @param model QAbstractItemModel which data will be used to fill the table
 */
void QBetterTableWidget::setTableModel(QAbstractItemModel *model)
{
    blockSignals(true);
    setUpdatesEnabled(false);
    clear();
    setRowCount(model->rowCount());
    setColumnCount(model->columnCount());
    for (int row = 0; row < model->rowCount(); row++)
    {
        for (int col = 0; col < model->columnCount(); col++)
        {
            QModelIndex index = model->index(row, col);
            QString str = model->data(index).toString();
            if (autoWidgetOnModels)
            {
                if (str == "true" || str == "false")
                {
                   this->addSwitchWidget(row,col);
                }
                else
                {
                    QTableWidgetItem *item = new QTableWidgetItem(model->data(index).toString());
                    setItem(row, col, item);
                }
                //TODO
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(index).toString());
                setItem(row, col, item);
            }
        }
    }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
}

/**
 * @brief destroy all the widgets and items that have been added to the table.
 *
 * All widgets and items no matter they are directly parented to the table
 * or elsewhere, are added to QVector of typed pointers
 * this function delete all the objects in these lists , clear and squeeze lists
 */
void QBetterTableWidget::clearAllWidgets()
{
    qDeleteAll (pushButtonList        );
    qDeleteAll (betterTableWidgetList );
    qDeleteAll (spinBoxList           );
    qDeleteAll (sizeWidgetList        );
    qDeleteAll (sizeFWidgetList       );
    qDeleteAll (rectWidgetList        );
    qDeleteAll (rectFWidgetList       );
    qDeleteAll (lineEditList          );
    qDeleteAll (tableWidgetItemList   );
    qDeleteAll (colorButtonList       );
    qDeleteAll (betterSwitchButtonList);
    qDeleteAll (simpleComboBoxList    );
    qDeleteAll (unitComboboxList      );
    qDeleteAll (containerWidgetList   );
    qDeleteAll (frameList             );

    pushButtonList         .clear();
    betterTableWidgetList  .clear();
    spinBoxList            .clear();
    sizeWidgetList         .clear();
    sizeFWidgetList        .clear();
    rectWidgetList         .clear();
    rectFWidgetList        .clear();
    lineEditList           .clear();
    tableWidgetItemList    .clear();
    colorButtonList        .clear();
    betterSwitchButtonList .clear();
    simpleComboBoxList     .clear();
    unitComboboxList       .clear();
    containerWidgetList    .clear();
    frameList              .clear();


    pushButtonList         .squeeze();
    betterTableWidgetList  .squeeze();
    spinBoxList            .squeeze();
    sizeWidgetList         .squeeze();
    sizeFWidgetList        .squeeze();
    rectWidgetList         .squeeze();
    rectFWidgetList        .squeeze();
    lineEditList           .squeeze();
    tableWidgetItemList    .squeeze();
    colorButtonList        .squeeze();
    betterSwitchButtonList .squeeze();
    simpleComboBoxList     .squeeze();
    unitComboboxList       .squeeze();
    containerWidgetList    .squeeze();
    frameList              .squeeze();

    if (valueSearchBox != nullptr)
    {
        delete valueSearchBox;
        valueSearchBox = nullptr;
    }
}

/**
 * @brief Pastes the contents of the clipboard into the table widget.
 *
 * Pastes the contents of the clipboard into the table widget,
 * starting from the first selected cell.
 * The clipboard text is assumed to be tab-separated values,
 * with each row separated by a newline character.
 */
void QBetterTableWidget::pasteFromClipboard()
{
  QModelIndexList selection = selectedIndexes();
  if (!selection.isEmpty())
  {
      int row_anchor = selection[0].row();
      int column_anchor = selection[0].column();

      QClipboard *clipboard = QApplication::clipboard();

      QStringList rows = clipboard->text().split('\n');
      for (int indx_row = 0; indx_row < rows.size(); ++indx_row)
      {
        QStringList values = rows[indx_row].split('\t');
        for (int indx_col = 0; indx_col < values.size(); ++indx_col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(values[indx_col]);
            setItem(row_anchor + indx_row, column_anchor + indx_col, item);
        }
      }
  }
}

/**
 * @brief Copies the selected cells of the table widget to the clipboard.
 *
 * Copies the selected cells of the table widget to the clipboard,
 * using the tab character as a column separator and the newline character
 * as a row separator.
 */
void QBetterTableWidget::copyToClipboard()
{
    QModelIndexList selection = selectedIndexes();
    if (!selection.isEmpty())
    {
        QString copy_text;
        int first_row = selection[0].row();
        int first_col = selection[0].column();
        int last_row = selection[selection.size()-1].row();
        int last_col = selection[selection.size()-1].column();

        for (int row = first_row; row <= last_row; row++)
        {
            for (int col = first_col; col <= last_col; col++)
            {
                if (QTableWidgetItem* item = this->item(row, col))
                    copy_text += item->text();
                else
                    copy_text += ""; // add empty string if item is nullptr
                if (col != last_col)
                    copy_text += '\t';
            }
            if (row != last_row)
                copy_text += '\n';
        }
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(copy_text);
    }
}

bool QBetterTableWidget::eventFilter(QObject *obj, QEvent *event)
{

  // check if the event is a wheel event
  if (event->type() == QEvent::Wheel)
  {
      qInfo()<<"wheel";
      // check if the object is a QSpinBox
      QSpinBox* spinbox = qobject_cast<QSpinBox*>(obj);
      if(spinbox)
      {
          qInfo()<<"spinBox";
          // check if the spinbox has focus
          if(spinbox->hasFocus())
          {
              // if it has focus, let the event be handled by the spinbox as usual
              return QObject::eventFilter(obj, event);
          }else{
              // if it does not have focus, ignore the event and prevent it from being propagated further
              event->ignore();
              return true;
          }
      }
      else
      {
          // if the object is not a QSpinBox, let the event be handled by the parent class
          return QObject::eventFilter(obj, event);
      }
  }
  // if the event is not a wheel event, let the event be handled by the parent class
  return QObject::eventFilter(obj, event);

}

void QBetterTableWidget::keyPressEvent(QKeyEvent *event)
{
                       //CTRL                      +                       V
   if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_V)
   {
      pasteFromClipboard();
   }
                      //CTRL                            +                     C
   else if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_C)
   {
       copyToClipboard();
   }

   //CTRL                                                +                     X
   else if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_X)
   {
       copyToClipboard();
       delSelection();
   }

   //CTRL                                                +                     A
   else if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_A)
   {
       selectAll();
   }


   QWidget::keyPressEvent(event);
}

void QBetterTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // Get the position of the mouse click
    QPoint pos = event->pos();
    // Get the index of the column at the mouse click position
    int col = columnAt(pos.x());
    // Set the private variable searchColIndex with the column index
    searchColIndex = col;
    // Show the context menu
    m_contextMenu->exec(event->globalPos());
}

void QBetterTableWidget::showEvent(QShowEvent *event)
{
    QTableWidget::showEvent(event);
    QPalette palette = viewport()->palette();
    palette.setColor(QPalette::Highlight, highlightColor);
    viewport()->setPalette(palette);
}


QColor QBetterTableWidget::getContrastColor(QColor aColor)
{
   bool startFromWhite = false;

   uint8_t whiteR = 0xFF;
   uint8_t whiteG = 0xFF;
   uint8_t whiteB = 0xFF;

   uint8_t blackR = 0x00;
   uint8_t blackG = 0x00;
   uint8_t blackB = 0x00;
   //get color components
   int r  = aColor.red   ();
   int g  = aColor.green ();
   int b  = aColor.blue  ();
   //calculate luminance with this highly scientifc method
   qreal luminance = r * 0.2126 + g * 0.7152 + b * 0.0722;
   //chose the start color from luminance (black or white)
   startFromWhite=(luminance<140);
   //calculate int from rgb components
   int colorValue =  b          |
                     (g << 8)   |
                     (r << 16)  |
                     (0 << 24);
   //calculate opposite color
    QColor opposite( 0xffffffff ^ colorValue);
   //combine both
   if (startFromWhite) //start with white
   {
       whiteR -= static_cast<int>(opposite.red()   /4) ;
       whiteG -= static_cast<int>(opposite.green() /4) ;
       whiteB -= static_cast<int>(opposite.blue()  /4) ;
       return QColor(whiteR,whiteG,whiteB);
   }
   else //start with black
   {
       blackR += static_cast<int>(opposite.red()   /4) ;
       blackG += static_cast<int>(opposite.green() /4) ;
       blackB += static_cast<int>(opposite.blue()  /4) ;
       return QColor(blackR,blackG,blackB);
   }

}


void QBetterTableWidget::paintEvent(QPaintEvent *event)
{
    QTableWidget::paintEvent(event);
    if (useAlternateLineColor)
    {
       QPainter painter(viewport());
       QColor color;
       for (int row = 0; row < rowCount(); ++row)
       {

           for (int col = 0; col < columnCount(); ++col)
           {
               QTableWidgetItem *item = this->item(row, col);
               if (!item)
               {
                    QModelIndexList _selectedIndexes = this->selectedIndexes();
                    bool cellSelected = false;
                    for (const auto &index : _selectedIndexes)
                    {
                        if (index.row() == row && index.column() == col)
                        {
                            cellSelected = true;
                            break;
                        }
                    }
                    cellSelected ? color = highlightColor :
                                  (row % 2 == 0) ? color = alternateLineColor1
                                                 : color = alternateLineColor2;
                    QRect rect = visualRect(model()->index(row, col));

                    painter.fillRect(rect, color);
               }
           }
       }
    }
}

void QBetterTableWidget::focusOutEvent(QFocusEvent *event)
{
     QTableWidget::focusOutEvent(event);
     m_outOfFocusRefreshTimer->start();
}

void QBetterTableWidget::focusInEvent(QFocusEvent *event)
{
    QTableWidget::focusInEvent(event);
    m_outOfFocusRefreshTimer->stop();
}

const QColor &QBetterTableWidget::getHighlightColor() const
{
    return highlightColor;
}

void QBetterTableWidget::setHighlightColor(const QColor &newHighlightColor)
{
    if (highlightColor == newHighlightColor)
        return;
    highlightColor = newHighlightColor;
    QPalette palette = viewport()->palette();
    palette.setColor(QPalette::Highlight, highlightColor);
    viewport()->setPalette(palette);
    emit highlightColorChanged();
}

bool QBetterTableWidget::getUseAlternateLineColor() const
{
    return useAlternateLineColor;
}

void QBetterTableWidget::setUseAlternateLineColor(bool newUseAlternateLineColor)
{
    useAlternateLineColor = newUseAlternateLineColor;
    emit useAlternateLineColorChanged();
}


bool QBetterTableWidget::setHeaderFromCsvString(int headerIndex,
                                                QString aCsvHeaderString,
                                                QString aSeparator)
{

    if (headerIndex < 0 || headerIndex >= headerList.count())

    {
        qInfo() << metaObject()->className()
                << "::"
                << __FUNCTION__
                << " : Invalid indexes for header or column";
        return false;
    }

    QStringList strList;
    int colCount = 0;
    strList  = aCsvHeaderString.split(aSeparator);
    colCount = strList.count();

    this->setUpdatesEnabled(false);
    this->blockSignals(true);

    if (this->columnCount()!=colCount)
    this->setColumnCount(colCount);

    for (int col=0; col<colCount;++col )
    {
       //tricks the header
       QBetterHeaderView *currentHeader = headerList[headerIndex];
       currentHeader->setIsStdLineEdit(true);
       // Create new line edit
       QLineEdit *_lineEdit = new QLineEdit(this);
       // Set the value
       _lineEdit->setText(strList[col]);
       //set the alignment of lineEdit
       _lineEdit->setAlignment(Qt::AlignHCenter);
       // Add line edit to list
       headerLineEditList[headerIndex].append(_lineEdit);
       _lineEdit->setReadOnly(true);
       _lineEdit->setParent   (currentHeader);
       _lineEdit->setGeometry (currentHeader->sectionViewportPosition(col),
                               0,
                               this->horizontalHeader()->sectionSize(col)-2 ,
                               this->horizontalHeader()->height()-2);
    }
    this->blockSignals(false);
    this->setUpdatesEnabled(true);
    return true;
}

const QColor &QBetterTableWidget::getAlternateLineColor2() const
{
    return alternateLineColor2;
}

void QBetterTableWidget::setAlternateLineColor2(const QColor &newAlternateLineColor2)
{
    if (alternateLineColor2 == newAlternateLineColor2)
        return;
    alternateLineColor2 = newAlternateLineColor2;
    setUseAlternateLineColor(useAlternateLineColor);
    emit alternateLineColor2Changed();
}

const QColor &QBetterTableWidget::getAlternateLineColor1() const
{
    return alternateLineColor1;
}

void QBetterTableWidget::setAlternateLineColor1(const QColor &newAlternateLineColor1)
{
    if (alternateLineColor1 == newAlternateLineColor1)
        return;
    alternateLineColor1 = newAlternateLineColor1;
    setUseAlternateLineColor(useAlternateLineColor);
    emit alternateLineColor1Changed();
}

QWidget *QBetterTableWidget::getGlobalTableContainer() const
{
    return globalTableContainer;
}

void QBetterTableWidget::setGlobalTableContainer(QWidget *newGlobalTableContainer)
{
    if (globalTableContainer == newGlobalTableContainer)
        return;
    globalTableContainer = newGlobalTableContainer;
    emit globalTableContainerChanged();
}

const QColor &QBetterTableWidget::getColMinValueColor() const
{
    return colMinValueColor;
}

void QBetterTableWidget::setColMinValueColor(const QColor &newColMinValueColor)
{
    if (colMinValueColor == newColMinValueColor)
        return;
    colMinValueColor = newColMinValueColor;
    emit colMinValueColorChanged();
}

const QColor &QBetterTableWidget::getColMaxValueColor() const
{
    return colMaxValueColor;
}

void QBetterTableWidget::setColMaxValueColor(const QColor &newColMaxValueColor)
{
    if (colMaxValueColor == newColMaxValueColor)
        return;
    colMaxValueColor = newColMaxValueColor;
    emit colMaxValueColorChanged();
}


const QColor &QBetterTableWidget::getTableHighLightColor() const
{
    return tableHighLightColor;
}

void QBetterTableWidget::setTableHighLightColor(const QColor &newTableHighLightColor)
{
    if (tableHighLightColor == newTableHighLightColor)
        return;
    tableHighLightColor = newTableHighLightColor;
    emit tableHighLightColorChanged();
}

double QBetterTableWidget::findMaxValueOnCol(int index)
{
    int _index;
    index==std::numeric_limits<int>::min() ? _index=searchColIndex
                                           : _index=index;


    blockSignals(true);
    setUpdatesEnabled(false);
    clearSelection();
    double maxValue = std::numeric_limits<double>::min();
    QTableWidgetItem* _item   = nullptr;
    QTableWidgetItem* maxItem = nullptr;
    for (int row = 0; row < rowCount(); row++)
    {
        _item = this->item(row, _index);
        if (_item != nullptr)
        {
            bool conversionSuccess = false;
            double value = _item->text().toDouble(&conversionSuccess);
            if (conversionSuccess)
            {
                if (value > maxValue)
                {
                  /*  QMessageBox msgBox;
                    msgBox.setText("MaxValue found:"+QString::number(value));
                    msgBox.exec();*/

                    maxValue = value;
                    maxItem = _item;
                }
            }
            else
               resetItemBackGround(_item);
        }
    }
    if (maxItem != nullptr)
    {
       /* QMessageBox msgBox;
                           msgBox.setText("MaxValue found:"+maxItem->text());
                           msgBox.exec();*/

        maxItem->setBackground(colMaxValueColor);
    }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    return maxValue;
}


double QBetterTableWidget::findMinValueOnCol(int index)
{
    int _index;
    index==std::numeric_limits<int>::min() ? _index=searchColIndex
                                           : _index=index;
    blockSignals(true);
    setUpdatesEnabled(false);
    clearSelection();
    double minValue = std::numeric_limits<double>::max();
    QTableWidgetItem* _item = nullptr;
    QTableWidgetItem* minItem = nullptr;
    for (int row = 0; row < this->rowCount(); row++)
    {
        _item = this->item(row, _index);
        if (_item != nullptr)
        {
            bool conversionSuccess = false;
            double value = _item->text().toDouble(&conversionSuccess);
            if (conversionSuccess)
            {
                if (value < minValue)
                {
                    minValue = value;
                    minItem = _item;
                }
            }
            else
                resetItemBackGround(_item);

        }
    }
    if (minItem != nullptr)
    {
        minItem->setBackground(colMinValueColor);
    }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    return minValue;
}

double QBetterTableWidget::giveAverageOnCol(bool showIt, int index)
{
    clearSelection();
    int _index;
    index==std::numeric_limits<int>::min() ? _index=searchColIndex
                                           : _index=index;
    blockSignals(true);
    setUpdatesEnabled(false);
    clearSelection();
    QTableWidgetItem* _item = nullptr;
    double result       = 0;
    int    nbValidValue = 0;
    bool   canAverage=false;
    for (int row = 0; row < this->rowCount(); row++)
    {
        _item = this->item(row, _index);
        if (_item != nullptr)
        {
            bool conversionSuccess = false;
            double value = _item->text().toDouble(&conversionSuccess);
            if (conversionSuccess)
            {
               result += value;
               canAverage = true;
               nbValidValue++;
            }
            else
                resetItemBackGround(_item);
        }
    }

    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    if (canAverage && nbValidValue!=0)
    {
        result /= nbValidValue;
    }
    else
        result = -999.25;

    if (showIt)
    {
        QMessageBox msgBox;
        QPushButton *copyButton  = msgBox.addButton(tr("Copy"), QMessageBox::ActionRole);
        QPushButton *okButton    = msgBox.addButton(QMessageBox::Ok);
        msgBox.setText("Average: "+QString::number(result));
        msgBox.exec();

        if (msgBox.clickedButton() == copyButton)
        {
            // copy value
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(QString::number(result));

        }
        else if (msgBox.clickedButton() == okButton)
        {
            return result;
        }
    }

    return result;
}

double QBetterTableWidget::findMaxValueOnSelected()
{

    blockSignals(true);
    setUpdatesEnabled(false);
    double maxValue = std::numeric_limits<double>::min();
    QTableWidgetItem* maxItem = nullptr;
    for (auto &range : selectedRanges())
    {
        for (int row = range.topRow(); row <= range.bottomRow(); row++)
        {
            for (int col = range.leftColumn(); col <= range.rightColumn(); col++)
            {
                QTableWidgetItem* _item = this->item(row, col);
                if (_item != nullptr)
                {
                    bool conversionSuccess = false;
                    double value = _item->text().toDouble(&conversionSuccess);
                    if (conversionSuccess)
                    {
                        if (value > maxValue)
                        {

                           /* QMessageBox msgBox;
                            msgBox.setText("MaxValue found:"+QString::number(value));
                            msgBox.exec();*/
                            maxValue = value;
                            maxItem = _item;
                        }
                    }
                    else
                    {
                        resetItemBackGround(_item);
                    }
                }
            }
        }
    }
    if (maxItem != nullptr)
    {

        maxItem->setBackground(colMaxValueColor);
    }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    clearSelection();
    return maxValue;
}

double QBetterTableWidget::findMinValueOnSelected()
{
    blockSignals(true);
    setUpdatesEnabled(false);
    double minValue = std::numeric_limits<double>::max();
    QTableWidgetItem* minItem = nullptr;
    for (auto &range : selectedRanges())
    {
        for (int row = range.topRow(); row <= range.bottomRow(); row++)
        {
            for (int col = range.leftColumn(); col <= range.rightColumn(); col++)
            {
                QTableWidgetItem* _item = this->item(row, col);
                if (_item != nullptr)
                {
                    bool conversionSuccess = false;
                    double value = _item->text().toDouble(&conversionSuccess);
                    if (conversionSuccess)
                    {
                        if (value < minValue)
                        {
                            minValue = value;
                            minItem = _item;
                        }
                    }
                    else
                    {
                        resetItemBackGround(_item);
                    }
                }
            }
        }
    }
    if (minItem != nullptr)
    {
        minItem->setBackground(colMinValueColor);
    }
    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    clearSelection();
    return minValue;
}


double QBetterTableWidget::giveAverageOnSelected(bool showIt)
{
    blockSignals(true);
    setUpdatesEnabled(false);
    QTableWidgetItem* _item = nullptr;
    double result       = 0;
    int    nbValidValue = 0;
    bool   canAverage=false;
    for (auto &range : selectedRanges())
    {
        for (int row = range.topRow(); row <= range.bottomRow(); row++)
        {
            for (int col = range.leftColumn(); col <= range.rightColumn(); col++)
            {
                _item = this->item(row, col);
                if (_item != nullptr)
                {
                    bool conversionSuccess = false;
                    double value = _item->text().toDouble(&conversionSuccess);
                    if (conversionSuccess)
                    {
                       result += value;
                       canAverage = true;
                       nbValidValue++;
                    }
                    else
                    {
                        resetItemBackGround(_item);
                    }
                }
            }
        }
    }

    setUpdatesEnabled(true);
    blockSignals(false);
    update();
    ensurePolished();
    if (canAverage && nbValidValue!=0)
    {
        result /= nbValidValue;
    }
    else
        result = -999.25;

    if (showIt)
    {
        QMessageBox msgBox;
        QPushButton *copyButton  = msgBox.addButton(tr("Copy"), QMessageBox::ActionRole);
        QPushButton *okButton    = msgBox.addButton(QMessageBox::Ok);
        msgBox.setText("Average: "+QString::number(result));
        msgBox.exec();

        if (msgBox.clickedButton() == copyButton)
        {
            // copy value
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(QString::number(result));

        }
        else if (msgBox.clickedButton() == okButton)
        {
            return result;
        }
    }

    return result;
}



const QColor &QBetterTableWidget::getTableBackgroundColor() const
{
    return tableBackgroundColor;
}

void QBetterTableWidget::setTableBackgroundColor(const QColor &newTableBackgroundColor)
{
    if (tableBackgroundColor == newTableBackgroundColor)
        return;
    tableBackgroundColor = newTableBackgroundColor;
    emit tableBackgroundColorChanged();
}

double QBetterTableWidget::calculateXlsxTextSize(QXlsx::Document *_xlsx,
                                                 int _col,
                                                 QString str)
{
    double colWidth  = _xlsx->columnWidth(_col+1);
    double strWidth =  str.length()+8.43;
     if(strWidth > colWidth)
     _xlsx->setColumnWidth(_col+1, strWidth);
    return str.length()+8.43;
}

const QFont &QBetterTableWidget::getXlsxFont() const
{
    return XlsxFont;
}

void QBetterTableWidget::setXlsxFont(const QFont &newXlsxFont)
{
    if (XlsxFont == newXlsxFont)
        return;
    XlsxFont = newXlsxFont;
    emit XlsxFontChanged();
}






