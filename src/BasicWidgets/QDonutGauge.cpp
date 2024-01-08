#include "QDonutGauge.h"


QDonutGauge::QDonutGauge(QWidget *parent) : QWidget(parent),
                                            m_min(0),
                                            m_max(100),
                                            m_value(0),
                                            m_startingPoint(Top),
                                            m_clockwiseRotation(true)
{
    m_centerLabel = new QLabel(this);
    m_centerLabel->setAlignment(Qt::AlignCenter);
    updateLabelGeometry();
    loadStyleStringFromResource();
}

void QDonutGauge::setValue(double value)
{
    m_value = value;
    m_centerLabel->setText(QString::number(m_value)+suffix);
    update(); // Triggers a repaint
}

void QDonutGauge::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
}

void QDonutGauge::setStartingPoint(StartingPoint point)
{
    m_startingPoint = point;
}


int QDonutGauge::calculateStartAngle() const
{
    switch (m_startingPoint)
    {
        case Top:    return 90;
        case Right:  return 0;
        case Bottom: return 270;
        case Left:   return 180;
        default:     return 90;
    }
}

QConicalGradient QDonutGauge::setupGradient(const QRectF &rect, int startAngle, double spanAngle) const
{
    // Gradient should start at the starting angle of the donut
    QConicalGradient gradient(rect.center(), startAngle);

    // Set the start and end colors of the gradient
    gradient.setColorAt(0.0, m_startColor); // Start color (blue) at minimum value
    gradient.setColorAt(1.0, m_endColor);   // End color (red) at maximum value

    return gradient;
}


/*QConicalGradient QDonutGauge::setupGradient(const QRectF &rect, int startAngle, double spanAngle) const
{
    // Gradient should start at the starting angle of the donut
    QConicalGradient gradient(rect.center(), startAngle);

    // Normalize the span angle to a range of 0 to 1
    double normalizedSpan = qAbs(spanAngle) / 360.0;

    // Set the start and end colors of the gradient

    if (!m_clockwiseRotation)
    {
        gradient.setColorAt(0.0, m_startColor);
        gradient.setColorAt(normalizedSpan, m_endColor);
    }
    else
    {
        gradient.setColorAt(0.0, m_endColor);
        gradient.setColorAt(normalizedSpan, m_startColor);
    }



    return gradient;
}*/


void QDonutGauge::paintEvent(QPaintEvent *event)
{
    updateLabelGeometry();
    QPainter painter(this);

    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

    painter.setRenderHint(QPainter::Antialiasing);
    if (m_max > m_min)
    {
        int outerRadius = qMin(width(), height()) / 2;
        int innerRadius = outerRadius - 20; // Adjust inner radius to make the donut look thicker or thinner
        QRectF outerRect(width() / 2 - outerRadius, height() / 2 - outerRadius, 2 * outerRadius, 2 * outerRadius);
        QRectF innerRect(width() / 2 - innerRadius, height() / 2 - innerRadius, 2 * innerRadius, 2 * innerRadius);

        int startAngle = calculateStartAngle();
        double spanAngle = 360.0 * (m_value - m_min) / (m_max - m_min);

        if (m_clockwiseRotation)
        {
            spanAngle = -spanAngle;
        }

        QConicalGradient gradient = setupGradient(outerRect, startAngle, spanAngle);


        // Draw the outer arc
        painter.setBrush(gradient);
        painter.drawPie(outerRect, startAngle * 16, static_cast<int>(spanAngle * 16));

        // Draw the inner circle to create a donut effect
        painter.setBrush(this->palette().window()); // Use the widget's background color
        painter.drawEllipse(innerRect);
    }
}


void QDonutGauge::resizeEvent(QResizeEvent *event)
{
    updateLabelGeometry(); // Update label position when the widget is resized
    QWidget::resizeEvent(event);
}

const QString &QDonutGauge::getSuffix() const
{
    return suffix;
}

void QDonutGauge::setSuffix(const QString &newSuffix)
{
    if (suffix == newSuffix)
        return;
    suffix = newSuffix;
    emit suffixChanged();
}

bool QDonutGauge::clockwiseRotation()
{
    return m_clockwiseRotation;
}

void QDonutGauge::setClockwiseRotation(bool clockwise)
{
    m_clockwiseRotation = clockwise;
    Q_EMIT clockwiseRotationChanged();
}

double QDonutGauge::max() const
{
    return m_max;
}

void QDonutGauge::setMax(int newMax)
{
    if (m_max == newMax)
        return;
    m_max = newMax;
    emit maxChanged();
}

double QDonutGauge::min() const
{
    return m_min;
}

void QDonutGauge::setMin(int newMin)
{
    if (m_min == newMin)
        return;
    m_min = newMin;
    emit minChanged();
}

void QDonutGauge::updateLabelGeometry()
{
    int side = qMin(width(), height());
    int labelSize = side / 2; // Adjust this size as needed
    int labelX = (width() - labelSize) / 2;
    int labelY = (height() - labelSize) / 2;
    m_centerLabel->setGeometry(labelX, labelY, labelSize, labelSize);

    // Calculate a suitable font size based on the widget size
    int fontSize = labelSize / 4.5;

    // Set the font size
    QFont labelFont = m_centerLabel->font();
    labelFont.setPointSize(fontSize);
    m_centerLabel->setFont(labelFont);
}

void QDonutGauge::loadStyleStringFromResource()
{
   //--- Modules ----

   QFile file(":/styles/darkDonutGauge.scss");
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       return;
   }

   QTextStream in(&file);
   QString content = in.readAll();
   file.close();

   this->setStyleSheet(content);
}

