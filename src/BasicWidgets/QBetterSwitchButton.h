#ifndef QBETTERSWITCHBUTTON_H
#define QBETTERSWITCHBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>


class QBetterSwitchButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint cursorPosition READ getCursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(qsizetype cursorSize READ getCursorSize WRITE setCursorSize NOTIFY cursorSizeChanged)
    Q_PROPERTY(bool state READ getState WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(qreal posInBackGroundGradient READ getPosInBackGroundGradient WRITE setPosInBackGroundGradient NOTIFY posInBackGroundGradientChanged)
    Q_PROPERTY(QString trueText READ getTrueText WRITE setTrueText NOTIFY trueTextChanged)
    Q_PROPERTY(QString falseText READ getFalseText WRITE setFalseText NOTIFY falseTextChanged)
    Q_PROPERTY(QColor falseColor READ getFalseColor WRITE setFalseColor NOTIFY falseColorChanged)
    Q_PROPERTY(QColor trueColor READ getTrueColor WRITE setTrueColor NOTIFY trueColorChanged)
    Q_PROPERTY(QColor cursorColor READ getCursorColor WRITE setCursorColor NOTIFY cursorColorChanged)
public:
    explicit QBetterSwitchButton(QString aFalseText   ,
                                 QString aTrueText    ,
                                 QColor  aFalseColor  ,
                                 QColor  aTrueColor   ,
                                 QColor  aCursorColor ,
                                 bool    initialState ,
                                 QWidget *parent = nullptr);
    void setDisabled(bool disable);
    void setEnabled (bool  enable);



    const qsizetype &getCursorSize() const;
    void setCursorSize(const qsizetype &newCursorSize);
    bool getOldState() const;
    bool getState() const;
    void setState(bool newState);

    qreal getPosInBackGroundGradient() const;
    void setPosInBackGroundGradient(qreal newPosInBackGroundGradient);

    const QString &getTrueText() const;
    void setTrueText(const QString &newTrueText);

    const QString &getFalseText() const;
    void setFalseText(const QString &newFalseText);

    const QColor &getFalseColor() const;
    void setFalseColor(const QColor &newFalseColor);

    const QColor &getTrueColor() const;
    void setTrueColor(const QColor &newTrueColor);

    const QColor &getCursorColor() const;
    void setCursorColor(const QColor &newCursorColor);

private:
    QString     falseText              ;
    QString     trueText               ;
    QColor      falseColor             ;
    QColor      trueColor              ;
    QColor      cursorColor            ;
    QPainter    painter                ;
    QPoint      cursorPosition         ;
    qsizetype   cursorSize             ;
    QLabel      falseLabel             ;
    QLabel      trueLabel              ;
    bool        isEnabled      = true  ;
    bool        m_oldState     = false ;
    bool        state          = false ;
    int         falsePosX      = 0     ;
    int         truePosX       = 0     ;
    qsizetype   labelFalseSize = 0     ;
    qsizetype   labelTrueSize  = 0     ;
    QString     trueQss        = ""    ;
    QString     falseQss       = ""    ;

    QRadialGradient radialGradient;
    QLinearGradient linearGradient;
    QRadialGradient bgLinearGradient;
    QLinearGradient cursorDisabledGradient;
    QColor          falseContrastedColor;
    QColor           trueContrastedColor;

    QColor getColorGradientAt(qreal key) const;
    QMap <qreal,QColor> falseTrueGradientMap;
    qreal posInBackGroundGradient = 0.0;
    QColor currentBackGroundColor;

    QPropertyAnimation switchMover          ;
    QPropertyAnimation BackGroundColorMover ;
    QColor getContrastColor(QColor aColor);

    void    paintCursor();
    void    paintBackground();
    void    calculateGradients();
    void    calculateInitialSizeAndPos();
    void    moveSwitch(bool aState, bool fromMouse);




protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *);

    QPoint getCursorPosition() const;
    void   setCursorPosition(QPoint newCursorPosition);
signals:

    void cursorPositionChanged();
    void enableStateChanged(bool state);
    void cursorSizeChanged();
    void stateChanged();
    void posInBackGroundGradientChanged();
    void trueTextChanged();
    void falseTextChanged();
    void falseColorChanged();
    void trueColorChanged();
    void cursorColorChanged();
};

#endif // QBETTERSWITCHBUTTON_H
