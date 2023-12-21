#ifndef QMAPPINGVIEWERWIDGET_H
#define QMAPPINGVIEWERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QTimer>
#include "Extractors/QCrioModulesDataExtractor.h"

class QMultiLineTextVisualizer ;
class QMappingRowWidget        ;
class QScrollBar               ;
class QLineEdit                ;
class QLabel                   ;
class QPushButton              ;

class QMappingViewerWidget : public QWidget {
    Q_OBJECT

public:
    explicit QMappingViewerWidget(QString modbusMappingPath, QCrioModulesDataExtractor *moduleExtractor, QWidget *parent = nullptr);
    ~QMappingViewerWidget();
    void enableAllControls(bool enabled);
    bool checkFields();

private slots:
    void onInsertBefore();
    void onInsertAfter();
    void onAdd();
    void onSelectRow   (QMappingRowWidget *row);
    void onDeleteRow   (QMappingRowWidget *row);
    void onMoveRowUp   (QMappingRowWidget *row);
    void onMoveRowDown (QMappingRowWidget *row);

public slots:
    void onLoad();
    void onSave();


private:


    QVector<QMappingRowWidget*> rowWidgets;
    QScrollArea                *scrollArea      = nullptr;
    QWidget                    *containerWidget = nullptr;
    QVBoxLayout                *containerLayout = nullptr;
    QMappingRowWidget          *selectedRow     = nullptr;
    QWidget                    *fillerWidget    = nullptr;
    QMultiLineTextVisualizer   *csvVizualizer   = nullptr;

    QPushButton *insertBeforeBtn = nullptr;
    QPushButton *insertAfterBtn  = nullptr;
    QPushButton *addBtn          = nullptr;

    QPushButton *reloadBtn      = nullptr;
    QPushButton *saveBtn        = nullptr;
    QPushButton *uploadBtn      = nullptr;

    QCrioModulesDataExtractor *m_extractor = nullptr;
    void setupUi();
    QMappingRowWidget* createRowWidget();

    QString m_modbusMappingPath;

    void updateRowIndices();
    void deleteAllRows();
    void blockAllSignals(bool blocked);
    void loadStyleStringFromResource();

};

#endif // QMAPPINGVIEWERWIDGET_H
