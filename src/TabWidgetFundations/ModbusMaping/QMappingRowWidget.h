#ifndef QMAPPINGROWWIDGET_H
#define QMAPPINGROWWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QPainter>
#include "Extractors/QCrioModulesDataExtractor.h"

using namespace globalEnumSpace;

class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;

class QMappingRowWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY ( QLabel      *indexLabel      READ indexLabel      WRITE setIndexLabel      NOTIFY indexLabelChanged      )
    Q_PROPERTY ( QComboBox   *moduleTypeCB    READ moduleTypeCB    WRITE setModuleTypeCB    NOTIFY moduleTypeCBChanged    )
    Q_PROPERTY ( QComboBox   *moduleAliasCB   READ moduleAliasCB   WRITE setModuleAliasCB   NOTIFY moduleAliasCBChanged   )
    Q_PROPERTY ( QComboBox   *moduleChannelCB READ moduleChannelCB WRITE setModuleChannelCB NOTIFY moduleChannelCBChanged )
    Q_PROPERTY ( QLineEdit   *minSrcValueLE   READ minSrcValueLE   WRITE setMinSrcValueLE   NOTIFY minSrcValueLEChanged   )
    Q_PROPERTY ( QLineEdit   *maxSrcValueLE   READ maxSrcValueLE   WRITE setMaxSrcValueLE   NOTIFY maxSrcValueLEChanged   )
    Q_PROPERTY ( QLineEdit   *minDestLE       READ minDestLE       WRITE setMinDestLE       NOTIFY minDestLEChanged       )
    Q_PROPERTY ( QLineEdit   *maxDestLE       READ maxDestLE       WRITE setMaxDestLE       NOTIFY maxDestLEChanged       )
    Q_PROPERTY ( QLineEdit   *destChannelLE   READ destChannelLE   WRITE setDestChannelLE   NOTIFY destChannelLEChanged   )
    Q_PROPERTY ( QPushButton *upBtn           READ upBtn           WRITE setUpBtn           NOTIFY upBtnChanged           )
    Q_PROPERTY ( QPushButton *downBtn         READ downBtn         WRITE setDownBtn         NOTIFY downBtnChanged         )
    Q_PROPERTY ( QPushButton *delBtn          READ delBtn          WRITE setDelBtn          NOTIFY delBtnChanged          )
    Q_PROPERTY ( int         index            READ index           WRITE setIndex           NOTIFY indexChanged           )

public:
    explicit QMappingRowWidget(QCrioModulesDataExtractor *extractor,QWidget *parent = nullptr);
    ~QMappingRowWidget();

    // Getters and setters
    int  index() const;
    void setIndex(int index);

    ModuleType moduleType    () const         ;
    void       setModuleType (ModuleType type);

    QString moduleAlias    () const                ;
    void    setModuleAlias (  const QString &alias);

    QString moduleChannel    () const                  ;
    void    setModuleChannel (  const QString &channel);

    float minSource() const;
    void  setMinSource(float value);

    float maxSource() const;
    void  setMaxSource(float value);

    uint16_t minDest() const;
    void     setMinDest(uint16_t value);

    uint16_t maxDest() const;
    void     setMaxDest(uint16_t value);

    int    modbusChannel() const;
    void   setModbusChannel(int channel);

    QLabel *indexLabel() const;
    void setIndexLabel(QLabel *newIndexLabel);

    QComboBox *moduleTypeCB() const;
    void setModuleTypeCB(QComboBox *newModuleTypeCB);

    QComboBox *moduleAliasCB() const;
    void setModuleAliasCB(QComboBox *newModuleAliasCB);

    QComboBox *moduleChannelCB() const;
    void setModuleChannelCB(QComboBox *newModuleChannelCB);

    QLineEdit *minSrcValueLE() const;
    void setMinSrcValueLE(QLineEdit *newMinSrcValueLE);

    QLineEdit *maxSrcValueLE() const;
    void setMaxSrcValueLE(QLineEdit *newMaxSrcValueLE);

    QLineEdit *minDestLE() const;
    void setMinDestLE(QLineEdit *newMinDestLE);

    QLineEdit *maxDestLE() const;
    void setMaxDestLE(QLineEdit *newMaxDestLE);

    QLineEdit *destChannelLE() const;
    void setDestChannelLE(QLineEdit *newDestChannelLE);

    QPushButton *upBtn() const;
    void setUpBtn(QPushButton *newUpBtn);

    QPushButton *downBtn() const;
    void setDownBtn(QPushButton *newDownBtn);

    QPushButton *delBtn() const;
    void setDelBtn(QPushButton *newDelBtn);

    QString getCsvLine(bool &ok);
    void setCsvLine(const QString &csvLine);

    void blockAllSignals(bool blocked);
    void enableAllControls(bool enabled);

signals:
    void indexChanged         (const int                   &index      );
    void moduleTypeChanged    (globalEnumSpace::ModuleType moduleType  );
    void moduleAliasChanged   (const QString               &moduleAlias);
    void moduleChannelChanged (const QString               &channelName);
    void minSourceChanged     (float                       srcMin      );
    void maxSourceChanged     (float                       srcMax      );
    void minDestChanged       (uint16_t                    destMin     );
    void maxDestChanged       (uint16_t                    destMax     );
    void modbusChannelChanged (int                         modbusIndex );
    void rowSelectedSignal    (QMappingRowWidget *rowWidget)            ;
    void deleteRowSignal      (QMappingRowWidget *rowWidget)            ;
    void moveUpRowSignal      (QMappingRowWidget *rowWidget)            ;
    void moveDownRowSignal    (QMappingRowWidget *rowWidget)            ;

    void indexLabelChanged();
    void moduleTypeCBChanged();
    void moduleAliasCBChanged();
    void moduleChannelCBChanged();
    void minSrcValueLEChanged();
    void maxSrcValueLEChanged();
    void minDestLEChanged();
    void maxDestLEChanged();
    void destChannelLEChanged();
    void upBtnChanged();
    void downBtnChanged();
    void delBtnChanged();

protected:

    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent( QShowEvent* event ) override;
    void paintEvent(QPaintEvent *event) override;


private:
    QLabel      *m_indexLabel      = nullptr;
    QComboBox   *m_moduleTypeCB    = nullptr;
    QComboBox   *m_moduleAliasCB   = nullptr;
    QComboBox   *m_moduleChannelCB = nullptr;
    QLineEdit   *m_minSrcValueLE   = nullptr;
    QLineEdit   *m_maxSrcValueLE   = nullptr;
    QLineEdit   *m_minDestLE       = nullptr;
    QLineEdit   *m_maxDestLE       = nullptr;
    QLineEdit   *m_destChannelLE   = nullptr;
    QPushButton *m_upBtn           = nullptr;
    QPushButton *m_downBtn         = nullptr;
    QPushButton *m_delBtn          = nullptr;
    QCrioModulesDataExtractor *m_extractor = nullptr;

    int          m_index           ;

    void createUi     () ;
    void createLayout () ;
    void connectAll   () ;
    void loadStyleStringFromResource();
    bool loadAndApplyStyleSheet(const QString &resourcePath, QWidget *widget);

public slots:
    void onModuleTypeChanged  (int index);
    void onModuleAliasChanged (int index);
};

#endif // QMAPPINGROWWIDGET_H
