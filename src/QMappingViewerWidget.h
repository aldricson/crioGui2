#ifndef QMAPPINGVIEWERWIDGET_H
#define QMAPPINGVIEWERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QTimer>


class QMappingRowWidget;

class QMappingViewerWidget : public QWidget {
    Q_OBJECT

public:
    explicit QMappingViewerWidget(QWidget *parent = nullptr);
    ~QMappingViewerWidget();

private slots:
    void onInsertBefore();
    void onInsertAfter();
    void onAdd();
    void onSelectRow(QMappingRowWidget *row);

private:
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QVBoxLayout *containerLayout;
    QVector<QMappingRowWidget*> rowWidgets;
    QMappingRowWidget *selectedRow;
    QWidget *fillerWidget;

    QPushButton *insertBeforeBtn;
    QPushButton *insertAfterBtn;
    QPushButton *addBtn;

    void setupUi();
    QMappingRowWidget* createRowWidget();

    void updateRowIndices();
};

#endif // QMAPPINGVIEWERWIDGET_H
