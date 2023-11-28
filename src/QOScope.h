#ifndef QOSCOPE_H
#define QOSCOPE_H

#include <QWidget>

class QChartView;
class QLineSeries;
class QChart;
class QValueAxis;

class QOScope : public QWidget
{
    Q_OBJECT

public:
    QOScope(QWidget* parent = nullptr);

    void setVerticalScale(double min, double max);
    void setHorizontalMaxSamples(int maxSamples);
    void addSample(double sample);
    void clearGraph();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent( QShowEvent* event ) override;

private:
    QChartView* chartView;
    QLineSeries* lineSeries;
    QChart* chart;
    QValueAxis* xAxis;
    QValueAxis* yAxis;
    int maxSamples;
    QVector<double> samples;
    bool m_initialized = false;
};

#endif // QOSCOPE_H
