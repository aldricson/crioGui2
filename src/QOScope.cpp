#include "QOScope.h"
#include <QtWidgets>
#include <QtCharts>

QOScope::QOScope(QWidget* parent)
    : QWidget(parent), maxSamples(100)
{
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(Qt::black));

    chart = new QChart;
    chart->setBackgroundBrush(QBrush(Qt::black));

    // Hide the legend
    chart->legend()->setVisible(false);

    lineSeries = new QLineSeries;
    QPen pen(Qt::green);
    pen.setWidth(2);
    lineSeries->setPen(pen);

    chart->addSeries(lineSeries);

    xAxis = new QValueAxis;
    xAxis->setRange(0, maxSamples);
    xAxis->setLabelsVisible(false);
    xAxis->setTitleText("Samples");

    yAxis = new QValueAxis;
    yAxis->setRange(0, 1); // Set an initial range, which will be adjusted later

    // Set the grid color to dark green
    yAxis->setGridLineColor(Qt::darkGreen);

    chart->setAxisX(xAxis, lineSeries);
    chart->setAxisY(yAxis, lineSeries);

    chartView->setChart(chart);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(chartView);
    setLayout(layout);
}

void QOScope::setVerticalScale(double min, double max)
{
    yAxis->setRange(min, max);
}

void QOScope::setHorizontalMaxSamples(int maxSamples)
{
    this->maxSamples = maxSamples;
    xAxis->setRange(0, maxSamples);
    samples.clear();
}

void QOScope::addSample(double sample)
{
    samples.append(sample);

    while (samples.size() > maxSamples)
    {
        samples.removeFirst();
    }

    lineSeries->clear();
    for (int i = 0; i < samples.size(); ++i)
    {
        lineSeries->append(i, samples[i]);
    }

    // Adjust the horizontal scale as needed
    xAxis->setRange(0, maxSamples);
    yAxis->setRange(yAxis->min(), yAxis->max());

    chart->scroll(chart->plotArea().width() / maxSamples, 0);
    chartView->repaint();
}

void QOScope::clearGraph()
{
    lineSeries->clear();
    samples.clear();
}


void QOScope::paintEvent(QPaintEvent* event)
{
    if (m_initialized)
    {
        //chartView->repaint();
    }
}

void QOScope::showEvent(QShowEvent *event)
{
    if (!m_initialized)
    {
        m_initialized = true;
    }
}
