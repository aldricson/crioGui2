#ifndef QMAPPINGROWWIDGET_H
#define QMAPPINGROWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>
#include "globalEnumSpace.h" // Assuming ModuleType is defined in a separate header

using namespace globalEnumSpace;

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
    explicit QMappingRowWidget(QWidget *parent = nullptr);
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


private:
    QLabel      *m_indexLabel      ;
    QComboBox   *m_moduleTypeCB    ;
    QComboBox   *m_moduleAliasCB   ;
    QComboBox   *m_moduleChannelCB ;
    QLineEdit   *m_minSrcValueLE   ;
    QLineEdit   *m_maxSrcValueLE   ;
    QLineEdit   *m_minDestLE       ;
    QLineEdit   *m_maxDestLE       ;
    QLineEdit   *m_destChannelLE   ;
    QPushButton *m_upBtn           ;
    QPushButton *m_downBtn         ;
    QPushButton *m_delBtn          ;

    int          m_index           ;

    void createUi     () ;
    void createLayout () ;
    void connectAll   () ;

};

#endif // QMAPPINGROWWIDGET_H
