#ifndef QDONUTGAUGE_H
#define QDONUTGAUGE_H

#include <QWidget>
#include <QPainter>
#include <QConicalGradient>
#include <QStyleOption>
#include <QLabel>
#include <QFile>

class QDonutGauge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double  min               READ min               WRITE setMin               NOTIFY minChanged)
    Q_PROPERTY(double  max               READ max               WRITE setMax               NOTIFY maxChanged)
    Q_PROPERTY(bool    clockwiseRotation READ clockwiseRotation WRITE setClockwiseRotation NOTIFY clockwiseRotationChanged)
    Q_PROPERTY(QString suffix            READ getSuffix         WRITE setSuffix            NOTIFY suffixChanged)

public:
    enum StartingPoint { Top, Right, Bottom, Left };
    Q_ENUM(StartingPoint)

    explicit QDonutGauge(QWidget *parent = nullptr);
    void setValue(double value);
    void setRange(double min, double max);
    void setStartingPoint(StartingPoint point);

    double min() const;
    void setMin(int newMin);

    double max() const;
    void setMax(int newMax);

    bool clockwiseRotation();
    void setClockwiseRotation(bool clockwise);

    const QString &getSuffix() const;
    void setSuffix(const QString &newSuffix);

signals:
    void minChanged              ();
    void maxChanged              ();
    void clockwiseRotationChanged();

    void suffixChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    double m_min;
    double m_max;
    double m_value;
    QLabel        *m_centerLabel  = nullptr;
    QColor         m_startColor   = Qt::blue;
    QColor         m_endColor     = Qt::red;
    QString        suffix         = "";
    StartingPoint m_startingPoint;
    bool m_clockwiseRotation;

    void updateLabelGeometry();
    int calculateStartAngle() const;
    QConicalGradient setupGradient(const QRectF &rect, int startAngle, double spanAngle) const;
    void loadStyleStringFromResource();


};

#endif // QDONUTGAUGE_H
