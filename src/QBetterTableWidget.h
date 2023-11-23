#ifndef QBETTERTABLEWIDGET_H
#define QBETTERTABLEWIDGET_H
#include <QCheckBox>
#include <QFocusEvent>
#include <QTimer>
#include <QStyledItemDelegate>
#include <QScrollBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QStringListModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMetaProperty>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include "xlsxdatavalidation.h"
#include "QBetterSwitchButton.h"
#include "QColorButton.h"
#include "QSizeWidget.h"
#include "QSizeFWidget.h"
#include "QRectWidget.h"
#include "QRectFWidget.h"
#include "globalEnumSpace.h"
#include "QValueSearchBox.h"
#include "QOutputUnitCombobox.h"
#include "QBetterHeaderView.h"


using namespace globalEnumSpace;


class QBetterTableWidget : public QTableWidget
{
    Q_OBJECT
    /**
     * @property QFont XlsxFont
     * to get or set the font of the excel file when exporting
     * not fully implemented. So better not use it at this time
     * or do it at your own risk
     */
    Q_PROPERTY(QFont    XlsxFont               READ getXlsxFont             WRITE setXlsxFont NOTIFY XlsxFontChanged)
    /**
     * @property QColor tableBackgroundColor
     * the global background color for the whole table
     * @note Changing this value will have no effect if a styleSheet is used
     * or if useAlternateLineColors is set to true
     */
    Q_PROPERTY(QColor   tableBackgroundColor   READ getTableBackgroundColor  WRITE setTableBackgroundColor NOTIFY tableBackgroundColorChanged)
    Q_PROPERTY(QColor   tableHighLightColor    READ getTableHighLightColor   WRITE setTableHighLightColor NOTIFY tableHighLightColorChanged)
    Q_PROPERTY(QColor   colMaxValueColor       READ getColMaxValueColor      WRITE setColMaxValueColor NOTIFY colMaxValueColorChanged)
    Q_PROPERTY(QColor   colMinValueColor       READ getColMinValueColor      WRITE setColMinValueColor NOTIFY colMinValueColorChanged)
    Q_PROPERTY(QWidget  *globalTableContainer  READ getGlobalTableContainer  WRITE setGlobalTableContainer NOTIFY globalTableContainerChanged)
    Q_PROPERTY(QColor   alternateLineColor1    READ getAlternateLineColor1   WRITE setAlternateLineColor1 NOTIFY alternateLineColor1Changed)
    Q_PROPERTY(QColor   alternateLineColor2    READ getAlternateLineColor2   WRITE setAlternateLineColor2   NOTIFY alternateLineColor2Changed)
    Q_PROPERTY(bool     useAlternateLineColor  READ getUseAlternateLineColor WRITE setUseAlternateLineColor NOTIFY useAlternateLineColorChanged)
    Q_PROPERTY(QColor   highlightColor         READ getHighlightColor        WRITE setHighlightColor        NOTIFY highlightColorChanged)
public:

    explicit QBetterTableWidget(QWidget *parent = nullptr);
    ~QBetterTableWidget();

    QPushButton         *addPushBtn           (int row,
                                               int col,
                                               QString text="");

    QCheckBox           *addCheckBox          (int row,
                                               int col,
                                               bool state);

    QBetterTableWidget  *addBetterTableWidget (int row,
                                               int col);

    QSpinBox            *addIntSpinEdit       (int row,
                                               int col,
                                               int value);

    QSizeWidget         *addSizeWidget        (int row,
                                               int col,
                                               QSize size);

    QSizeFWidget        *addSizeFWidget       (int row,
                                               int col,
                                               QSizeF sizeF);

    QRectWidget         *addRectWidget        (int row,
                                               int col,
                                               QRect rect);

    QRectFWidget        *addRectFWidget       (int row,
                                               int col,
                                               QRectF rectF);

    QLineEdit           *addIntLineEdit       (int row,
                                               int col,
                                               int value);

    QLineEdit           *addDoubleLineEdit    (int row,
                                               int col,
                                               double value);

    QLineEdit           *addSimpleLineEdit     (int row,
                                               int col, QString str);

    QTableWidgetItem    *addText              (int row,
                                               int col,
                                               QString text);

    QColorButton        *addColorButton       (int row,
                                               int col,
                                               QColor color);

    QBetterSwitchButton *addSwitchWidget      (int row,
                                               int col);

    QBetterSwitchButton *addSwitchWidget      (int row,
                                               int col,
                                               bool state);

    /**
    *  @brief  add a simple combobox to the table
    *  @param  row - row of the cell where the combobox will be added
    *  @param  col - column of the cell where the combobox will be added
    *  @param  lst - list of items to be added to the combobox
    *  @param  startIndex - index of the item that will be set as current
    *  @return pointer to the created combobox
    */
    QComboBox           *addSimpleComboBox    (int row,
                                               int col,
                                               QStringList lst,
                                               int startIndex);
    /**
    * @brief Adds a QOutputUnitCombobox widget to the header at the specified index and column.
    *
    * @param headerIndex Index of the header in the headerList.
    * @param colIndex Index of the column.
    *
    * @return A pointer to the QOutputUnitCombobox added, or nullptr if the indexes are invalid.
    */
    QOutputUnitCombobox  *addUnitComboBoxToHeader(int headerIndex,
                                                  int colIndex,
                                                  TunitsFamilyType famillyType,
                                                  QString defaultUnit);


    QVector<QPushButton         *>  pushButtonList         ;
    QVector<QBetterTableWidget  *>  betterTableWidgetList  ;
    QVector<QSpinBox            *>  spinBoxList            ;
    QVector<QSizeWidget         *>  sizeWidgetList         ;
    QVector<QSizeFWidget        *>  sizeFWidgetList        ;
    QVector<QRectWidget         *>  rectWidgetList         ;
    QVector<QRectFWidget        *>  rectFWidgetList        ;
    QVector<QLineEdit           *>  lineEditList           ;
    QVector<QVector<QLineEdit   *>> headerLineEditList     ;
    QVector<QLineEdit           *>  intLineEditList        ;
    QVector<QLineEdit           *>  doubleLineEditList     ;
    QVector<QTableWidgetItem    *>  tableWidgetItemList    ;
    QVector<QColorButton        *>  colorButtonList        ;
    QVector<QBetterSwitchButton *>  betterSwitchButtonList ;
    QVector<QComboBox           *>  simpleComboBoxList     ;
    QVector<QOutputUnitCombobox *>  unitComboboxList       ;
    QVector<QWidget             *>  containerWidgetList    ;
    QVector<QFrame              *>  frameList              ;
    //QVector<QHeaderView         *>  headerList             ;
    QVector<QBetterHeaderView   *>  headerList             ;
    QVector<QCheckBox           *>  checkBoxList           ;

    /**
    *  @brief  merge cells in a row
    *  @param  row - the row to be merged
    *  @return true if the merge is done successfully, false otherwise
    */
    bool mergeRowCells(int row);
    /**
    *  @brief  merge cells in a square range
    *  @param  rowStart - start row of the square
    *  @param  colStart - start column of the square
    *  @param  rowSpan - width the square in cells
    *  @param  colEnd -  height of the square in cells
    *  @return true if the merge is done successfully, false otherwise
    */
    bool mergeSquareCells(int rowStart,
                          int colStart,
                          int _rowSpan,
                          int _colSpan);

    /**
     * @brief Unmerges cells in a square range
     * @param rowStart - start row of the square
     * @param colStart - start column of the square
     * @param rowSpan - width of the square in cells
     * @param colSpan - height of the square in cells
     * @return true if the unmerge is done successfully, false otherwise
     */
    bool unmergeSquareCells(int rowStart,
                            int colStart,
                            int rowSpan,
                            int colSpan);

    /**
    * @brief Add a title row to the table with a given text
    * @param row The index of the row to add the title to
    * @param text The text to display in the title row
    * @return true if the row is in range and the operation was successful, false otherwise
    */
    bool addTitleRow(int row, const QString& text);

    void    verifyOrInitializeCell(int aRow, int aCol);
    QString setSpecialWidgetCsvText(int aRow, int aCol);
    virtual void saveToXlsxFile(const QString &fileName);
    virtual void saveToCsv(QString aFileName, QString aSeparator);



    //QHeaderView* addNewHeader();
    QBetterHeaderView* addNewHeader();


    bool setHeaderFromCsvString(int headerIndex,QString aCsvHeaderString, QString aSeparator);
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
    bool setRowFromCsvString(int aRow,
                             QString aCsvHeaderString,
                             QString aSeparator);

    /*!
     * \brief Adds Unit ComboBoxes to the specified header row.
     * \param headerIndex The index of the header row where the ComboBoxes will be added.
     * \param famillyTypes A vector containing the default family type for each ComboBox.
     * \param defaultUnits A vector containing the default unit for each ComboBox.
     * \return True if the ComboBoxes were successfully added, false otherwise.
     *
     * This function adds a unit ComboBox to each cell in the specified header row. The default family type and default unit for each ComboBox are specified in the `famillyTypes` and `defaultUnits` vectors, respectively.
     */
    bool addUnitComboBoxesToHeader(int headerIndex,
                                   const QVector<TunitsFamilyType> &famillyTypes,
                                   const QVector<QString> &defaultUnits);


    void setHeaderModelFromCsvString(int headerIndex,
                                     QString aCsvString,
                                     QString separator);


    void setTableModel(QAbstractItemModel *model);
    /**
     * @brief destroy all the widgets and items that have been added to the table.
     *
     * All widgets and items no matter they are directly parented to the table
     * or elsewhere, are added to QVector of typed pointers
     * this function delete all the objects in these lists , clear and squeeze lists
     */
    void clearAllWidgets();
    /**
     * @brief Pastes the contents of the clipboard into the table widget.
     *
     * Pastes the contents of the clipboard into the table widget, starting from the first selected cell.
     * The clipboard text is assumed to be tab-separated values, with each row separated by a newline character.
     */
    void pasteFromClipboard();
    /**
     * @brief Copies the selected cells of the table widget to the clipboard.
     *
     * Copies the selected cells of the table widget to the clipboard,
     * using the tab character as a column separator and the newline character
     *  as a row separator.
     */
    void copyToClipboard();

    const QFont &getXlsxFont() const;
    void setXlsxFont(const QFont &newXlsxFont);


    const QColor &getTableBackgroundColor() const;
    void setTableBackgroundColor(const QColor &newTableBackgroundColor);

    const QColor &getTableHighLightColor() const;
    void setTableHighLightColor(const QColor &newTableHighLightColor);

    double findMaxValueOnCol(int index=-2147483648); //-2147483648 is
                                                     //std::numeric_limits<int>::min()
    double findMinValueOnCol(int index=-2147483648);
    double giveAverageOnCol(bool showIt,int index=-2147483648);

    double findMaxValueOnSelected();
    double findMinValueOnSelected();
    double giveAverageOnSelected(bool showIt);



    const QColor &getColMaxValueColor() const;
    void setColMaxValueColor(const QColor &newColMaxValueColor);

    const QColor &getColMinValueColor() const;
    void setColMinValueColor(const QColor &newColMinValueColor);

    QWidget *getGlobalTableContainer() const;
    void setGlobalTableContainer(QWidget *newGlobalTableContainer);

    const QColor &getAlternateLineColor1() const;
    void setAlternateLineColor1(const QColor &newAlternateLineColor1);

    const QColor &getAlternateLineColor2() const;
    void setAlternateLineColor2(const QColor &newAlternateLineColor2);

    bool getUseAlternateLineColor() const;
    void setUseAlternateLineColor(bool newUseAlternateLineColor);

    const QColor &getHighlightColor() const;
    void setHighlightColor(const QColor &newHighlightColor);

signals:
    void buttonClicked(int row, int col);
    void switchChanged(int row, int col, bool  state);
    void comboBoxChanged(int row, int col, int index);
    void XlsxFontChanged();
    void tableBackgroundColorChanged();
    void tableHighLightColorChanged();
    void colMaxValueColorChanged();
    void colMinValueColorChanged();
    void globalTableContainerChanged();
    void alternateLineColor1Changed();
    void alternateLineColor2Changed();
    void useAlternateLineColorChanged();

    void highlightColorChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

private:
    QTimer    *m_outOfFocusRefreshTimer   = nullptr;
    QMenu     *m_contextMenu              = nullptr;
    //copy/paste/cut section
    QAction   *m_copyAction               = nullptr;
    QAction   *m_pasteAction              = nullptr;
    QAction   *m_cutAction                = nullptr;
    //search section
    QAction   *m_searchValueAction        = nullptr;
    QAction   *m_searchTextAction         = nullptr;
    //statistical tool section
    QAction   *m_giveMinValueOnCol        = nullptr;
    QAction   *m_giveMaxValueOnCol        = nullptr;
    QAction   *m_giveAverageCol           = nullptr;

    QAction   *m_giveMinValueOnSelected   = nullptr;
    QAction   *m_giveMaxValueOnSelected   = nullptr;
    QAction   *m_giveAverageOnSelected    = nullptr;

    bool      searchOnColOnly = true;
    int       searchColIndex  = 0;


    QWidget *globalTableContainer  = nullptr;
    QGridLayout *tableLayout = nullptr;
    int tablePosInLayout = 10;
    const QString                 comboBoxSeparator   = "⇚";

    QXlsx::Document       *xlsx = nullptr;
    QFont XlsxFont = QFont("Calibri", 11);
    QColor tableBackgroundColor    = Qt::white;
    QColor tableHighLightColor     = QColor (135,206,250);
    QColor colMaxValueColor        = QColor (0xFF7F50);
    QColor colMinValueColor        = QColor (0x32CD32);
    QColor alternateLineColor1     = QColor (0xFEFBEA);
    QColor alternateLineColor2     = QColor (0xC8E9E9);
    QColor highlightColor          = QColor (0xDFB7FF);
    QColor getContrastColor  (QColor aColor);
    bool   useAlternateLineColor   = false;

    QValueSearchBox *valueSearchBox;
    double calculateXlsxTextSize(QXlsx::Document *_xlsx,int _col,QString str);
    bool autoWidgetOnModels   = true;

    bool XlsxCheckForSwitch    (QXlsx::Document *_xlsx,
                                QWidget *_widget,
                                int _row,
                                int _col,
                                QString fileName);

    bool XlsxCheckForButton     (QXlsx::Document *_xlsx,
                                 QWidget *_widget,
                                 int _row,
                                 int _col,
                                 QString fileName);

    bool XlsxCheckForSpinBox     (QXlsx::Document *_xlsx,
                                  QWidget *_widget,
                                  int _row,
                                  int _col,
                                  QString fileName);

    bool XlsxCheckForQSizeWidget (QXlsx::Document *_xlsx,
                                  QWidget *_widget,
                                  int _row,
                                  int _col,
                                  QString fileName);

    bool XlsxCheckForQComboBox   (QXlsx::Document *_xlsx,
                                  QWidget *_widget,
                                  int _row,
                                  int _col,
                                  QString fileName);

    bool XlsxCheckForQRectWidget (QXlsx::Document *_xlsx,
                                  QWidget *_widget,
                                  int _row,
                                  int _col,
                                  QString fileName);


    void searchNumericBetweenOnAllTable(double minValue,
                                        double maxValue);

    void searchNumericValueOnAllTable(double searchValue,
                                      NumericSearchType searchType);

    void searchNumericValueOnCol(double searchValue,
                                 NumericSearchType searchType);


    void searchNumericBetweenOnCol(double minValue,
                                   double maxValue);

    void realignHeaderUnitComboxes(int headerIndex);
    void realignHeaderLineEdits   (int headerIndex);

    void resetTableHighlightning();
    void resetItemBackGround(QTableWidgetItem *item, int index);
    void resetItemBackGround(QTableWidgetItem *item);

    void delSelection();


private slots:
    void onButtonClicked();
    void onStateChanged();
    void onComboBoxChanged            (int index);
    void onHorizontalScrollBarChanged (int value);
    void onItemChanged                (QTableWidgetItem *item);
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
    void onUnitCbTextChanged      (const QString &oldText,
                                   const QString &newText);

    void onSearchNumericAsked  (globalEnumSpace::NumericSearchType searchType,
                                double minValue,
                                double maxValue,
                                bool   onCol);

};

#endif
