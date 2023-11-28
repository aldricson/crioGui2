#include "QBetterSwitchButton.h"

QBetterSwitchButton::QBetterSwitchButton(QString aFalseText ,
                                         QString aTrueText  ,
                                         QColor  aFalseColor,
                                         QColor  aTrueColor ,
                                         QColor aCursorColor,
                                         bool  initialState ,
                                         QWidget *parent)
    : QWidget{parent}
{
    trueContrastedColor  = getContrastColor(aTrueColor);
    falseContrastedColor = getContrastColor(aFalseColor);
    trueQss  = QString ("color: %1").arg(trueContrastedColor.name()) ;
    falseQss = QString ("color: %1").arg(falseContrastedColor.name());

    QFont font = falseLabel.font();
    font.setPointSize(10);
    font.setBold(true);
    falseLabel.setFont(font);
    trueLabel.setFont(font);

    falseText      = aFalseText                     ;
    trueText       = aTrueText                      ;
    falseLabel     . setText       (falseText)      ;
    falseLabel     . setParent     (this)           ;
    falseLabel     . setStyleSheet (falseQss)       ;
    trueLabel      . setText       (trueText)       ;
    trueLabel      . setParent     (this)           ;
    trueLabel      . setStyleSheet (trueQss)        ;
    falseColor     = aFalseColor                    ;
    trueColor      = aTrueColor                     ;
    cursorColor    = aCursorColor                   ;
    cursorPosition . setX          (0)              ;
    cursorPosition . setY          (0)              ;
    cursorSize     = 20                             ;
    state          = initialState                   ;
    if (state)
    {
        currentBackGroundColor  = trueColor;
        posInBackGroundGradient = 1.0;
    }
    else
    {
        currentBackGroundColor = falseColor;
        posInBackGroundGradient=0.0;
    }
    calculateGradients           ()               ;
    calculateInitialSizeAndPos   ()               ;
    falseTrueGradientMap.insert  (0.0,falseColor) ;
    falseTrueGradientMap.insert  (1.0,trueColor ) ;

    switchMover.setEasingCurve(QEasingCurve::Linear) ;
    switchMover.setDuration(100) ;
    switchMover.setTargetObject(this);
    switchMover.setPropertyName("cursorPosition");

    BackGroundColorMover.setEasingCurve(QEasingCurve::Linear) ;
    BackGroundColorMover.setDuration(95) ;
    BackGroundColorMover.setTargetObject(this);
    BackGroundColorMover.setPropertyName("posInBackGroundGradient");


}

QColor QBetterSwitchButton::getContrastColor(QColor aColor)
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

void QBetterSwitchButton::setDisabled(bool disable)
{
    isEnabled = !disable;
    emit enableStateChanged(isEnabled);
}

void QBetterSwitchButton::setEnabled(bool enable)
{
    isEnabled = enable;
    emit enableStateChanged(isEnabled);
}

const qsizetype &QBetterSwitchButton::getCursorSize() const
{
    return cursorSize;
}

void QBetterSwitchButton::setCursorSize(const qsizetype &newCursorSize)
{
    if (cursorSize == newCursorSize)
        return;
    cursorSize = newCursorSize;
    calculateGradients();
    emit cursorSizeChanged();
}

bool QBetterSwitchButton::getState() const
{
    return state;
}

void QBetterSwitchButton::setState(bool newState)
{
    if (state == newState)
        return;
    state = newState;
    moveSwitch(state,false);
    emit stateChanged();
}

void QBetterSwitchButton::paintCursor()
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter.setPen(pen);
    painter.setBrush(cursorColor);

    painter.drawEllipse(cursorPosition.x(), cursorPosition.y(), cursorSize, cursorSize);
    painter.setBrush(radialGradient);
    painter.drawEllipse(cursorPosition.x()+1, cursorPosition.y()+1, cursorSize-2, cursorSize-2);

    painter.setBrush(QColor(210, 210, 210));
    painter.drawEllipse(cursorPosition.x()+2, cursorPosition.y()+2, cursorSize-4, cursorSize-4);

    if (isEnabled)
      {
        painter.setBrush(linearGradient);
        painter.drawEllipse(cursorPosition.x()+3, cursorPosition.y()+3, cursorSize-6, cursorSize-6);
      }
      else
      {
        painter.setBrush(cursorDisabledGradient);
        painter.drawEllipse(cursorPosition.x()+3, cursorPosition.y()+3, cursorSize-6, cursorSize-6);
      }

    painter.end();
    falseLabel.move(cursorPosition.x()+cursorSize+1,0);
    trueLabel.move(cursorPosition.x()-labelTrueSize+7,0);
}

void QBetterSwitchButton::paintBackground()
{

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter.setPen(pen);
    if (isEnabled)
    {
      painter.setBrush(currentBackGroundColor);
      painter.drawRoundedRect(0, 0
        , width(), height()
        , 10, 10);
      painter.setCompositionMode(QPainter::CompositionMode_Multiply);
      painter.setBrush(bgLinearGradient);
      painter.drawRoundedRect(2, 2, width()-4, height()-4, 8, 8);
    }
    else
    {
      painter.setBrush(QColor(150, 150, 150));
      painter.drawRoundedRect(0, 0
        , width(), height()
        , 10, 10);

      painter.setBrush(cursorDisabledGradient);
      painter.drawRoundedRect(1, 1, width() - 2, height() - 2, 8, 8);
    }
    painter.end();
}

void QBetterSwitchButton::calculateGradients()
{
    //For now, hard coded
    const int center = static_cast<int>(cursorSize/ 2);
    radialGradient = QRadialGradient(center,
                                     center,
                                     center+2);

    radialGradient.setColorAt(0, QColor(255, 255, 255));
    radialGradient.setColorAt(0.6, QColor(255, 255, 255));
    radialGradient.setColorAt(1, QColor(205, 205, 205));

    linearGradient = QLinearGradient(3, cursorSize-2, cursorSize, 4);
    linearGradient.setColorAt(0, QColor(255, 255, 255));
    linearGradient.setColorAt(0.55, QColor(230, 230, 230));
    linearGradient.setColorAt(0.72, QColor(255, 255, 255));
    linearGradient.setColorAt(1, QColor(255, 255, 255));

    bgLinearGradient = QRadialGradient(0, 10, width()/3);//= QLinearGradient(3, cursorSize-2, cursorSize, 4);
    bgLinearGradient.setColorAt(0, QColor(160, 160, 160));
    bgLinearGradient.setColorAt(0.5, QColor(255, 255, 255));
    bgLinearGradient.setColorAt(1, QColor(200, 200, 200));
    //bgLinearGradient.setColorAt(1, QColor(255, 255, 255,255));

    cursorDisabledGradient = QLinearGradient(3, cursorSize-2, cursorSize, 4);
    cursorDisabledGradient.setColorAt(0, QColor(230, 230, 230));
    cursorDisabledGradient.setColorAt(0.55, QColor(210, 210, 210));
    cursorDisabledGradient.setColorAt(0.72, QColor(230, 230, 230));
    cursorDisabledGradient.setColorAt(1, QColor(230, 230, 230));
}

QColor QBetterSwitchButton::getColorGradientAt(qreal key) const
{
    // key must belong to [0,1]
    if (key<0.0) key = 0.0;
    else
    if (key>1.0) key = 1.0;

    // directly get color if known
    if(falseTrueGradientMap.contains(key))
    {
        return falseTrueGradientMap.value(key) ;
    }

    // else, emulate a linear gradient
    QPropertyAnimation interpolator ;
    const qreal granularite = 100.0 ;
    interpolator.setEasingCurve(QEasingCurve::Linear) ;
    interpolator.setDuration(granularite) ;
    foreach( qreal key, falseTrueGradientMap.keys() )
    {
        interpolator.setKeyValueAt(key, falseTrueGradientMap.value(key)) ;
    }
    interpolator.setCurrentTime(key*granularite) ;
    return interpolator.currentValue().value<QColor>() ;
}

qreal QBetterSwitchButton::getPosInBackGroundGradient() const
{
    return posInBackGroundGradient;
}

void QBetterSwitchButton::setPosInBackGroundGradient(qreal newPosInBackGroundGradient)
{
    posInBackGroundGradient = newPosInBackGroundGradient;
    currentBackGroundColor = getColorGradientAt(posInBackGroundGradient);
    emit posInBackGroundGradientChanged();
}

const QString &QBetterSwitchButton::getTrueText() const
{
    return trueText;
}

void QBetterSwitchButton::setTrueText(const QString &newTrueText)
{
    if (trueText == newTrueText)
        return;
    trueText = newTrueText;
    emit trueTextChanged();
}

const QString &QBetterSwitchButton::getFalseText() const
{
    return falseText;
}

void QBetterSwitchButton::setFalseText(const QString &newFalseText)
{
    if (falseText == newFalseText)
        return;
    falseText = newFalseText;
    emit falseTextChanged();
}

const QColor &QBetterSwitchButton::getFalseColor() const
{
    return falseColor;
}

void QBetterSwitchButton::setFalseColor(const QColor &newFalseColor)
{
    if (falseColor == newFalseColor)
        return;
    falseColor = newFalseColor;
    emit falseColorChanged();
}

const QColor &QBetterSwitchButton::getTrueColor() const
{
    return trueColor;
}

void QBetterSwitchButton::setTrueColor(const QColor &newTrueColor)
{
    if (trueColor == newTrueColor)
        return;
    trueColor = newTrueColor;
    emit trueColorChanged();
}

const QColor &QBetterSwitchButton::getCursorColor() const
{
    return cursorColor;
}

void QBetterSwitchButton::setCursorColor(const QColor &newCursorColor)
{
    if (cursorColor == newCursorColor)
        return;
    cursorColor = newCursorColor;
    emit cursorColorChanged();
}


void QBetterSwitchButton::calculateInitialSizeAndPos()
{
    labelFalseSize = falseLabel.fontMetrics().boundingRect(falseLabel.text()).width()+10;
    labelTrueSize  = trueLabel .fontMetrics().boundingRect(trueLabel .text()).width()+10;
    qsizetype finalSize      = 0                           ;
    if (labelFalseSize<labelTrueSize)
    {
        finalSize  = labelFalseSize+cursorSize              ;
        falseLabel . setFixedWidth (labelFalseSize)         ;
        trueLabel  . setFixedWidth (labelFalseSize)         ;
    }
    else
    {
        finalSize  = labelTrueSize+cursorSize              ;
        falseLabel . setFixedWidth (labelTrueSize)         ;
        trueLabel  . setFixedWidth (labelTrueSize)         ;

    }
    this->setFixedSize         (finalSize,cursorSize)      ;
    falsePosX = 0                      ;
    truePosX  = finalSize-cursorSize                               ;
    state ? cursorPosition.setX(truePosX) : cursorPosition.setX(falsePosX);
    cursorPosition.setY(0);

    update();
}

void QBetterSwitchButton::moveSwitch(bool aState, bool fromMouse)
{
  if(fromMouse)
    {
      if (state)
        {
          switchMover.setStartValue(QPoint(falsePosX,0));
          switchMover.setEndValue  (QPoint(truePosX,0)) ;
          BackGroundColorMover.setStartValue(qreal(0.0));
          BackGroundColorMover.setEndValue  (qreal(1.0));
        }
      else
        {
          switchMover.setStartValue(QPoint(truePosX,0));
          switchMover.setEndValue  (QPoint(falsePosX,0)) ;
          BackGroundColorMover.setStartValue(qreal(1.0));
          BackGroundColorMover.setEndValue  (qreal(0.0));
        }
      switchMover.start();
      BackGroundColorMover.start();
     }
  else
     {
        if  (state)
         {
            setCursorPosition(QPoint(truePosX,0));
            currentBackGroundColor=trueColor;
         }
        else
        {
            setCursorPosition(QPoint(falsePosX,0));
            currentBackGroundColor=falseColor;
        }
     }
}



QPoint QBetterSwitchButton::getCursorPosition() const
{
    return cursorPosition;
}

void QBetterSwitchButton::setCursorPosition(QPoint newCursorPosition)
{
    if (cursorPosition == newCursorPosition)
        return;
    cursorPosition = newCursorPosition;
    update();
    emit cursorPositionChanged();
}

void QBetterSwitchButton::paintEvent(QPaintEvent *event)
{

    paintBackground();
    paintCursor();

}

void QBetterSwitchButton::mousePressEvent(QMouseEvent *)
{
    if (isEnabled) {
        state = !state;
        emit stateChanged();
        moveSwitch(state, true);
    }
}
