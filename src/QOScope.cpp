#include "QOScope.h"
#include <QtWidgets>
#include <QtCharts>

QOScope::QOScope(QWidget* parent)
    : QWidget(parent), maxSamples(100)
{
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(Qt::black));
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    chart = new QChart;
    chart->setBackgroundBrush(QBrush(Qt::black));
    chart->setMargins(QMargins(0, 0, 0, 0));


    // Hide the legend
    chart->legend()->setVisible(false);

    lineSeries = new QLineSeries;
    QPen pen(Qt::green);
    pen.setWidth(1);
    lineSeries->setPen(pen);

    chart->addSeries(lineSeries);

    xAxis = new QValueAxis;
    xAxis->setRange(0, maxSamples);
    xAxis->setLabelsVisible(false);
    xAxis->setTitleText("Samples");

    // Set the font and color for the X axis labels
    QFont xAxisFont;
    xAxisFont.setPointSize(6); // Set the font size to be smaller
    xAxis->setLabelsFont(xAxisFont);
    xAxis->setLabelsColor(Qt::darkGreen);

    yAxis = new QValueAxis;
    yAxis->setRange(0, 1); // Set an initial range, which will be adjusted later

    // Set the font and color for the Y axis labels
    QFont yAxisFont;
    yAxisFont.setPointSize(6); // Set the font size to be smaller
    yAxis->setLabelsFont(yAxisFont);
    yAxis->setLabelsColor(Qt::darkGreen);

    // Set the grid color to dark green
    yAxis->setGridLineColor(Qt::darkGreen);
    xAxis->setGridLineColor(Qt::darkGreen);

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    lineSeries->attachAxis(xAxis);
    lineSeries->attachAxis(yAxis);

    chartView->setChart(chart);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
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

    // Adjust the horizontal scale as needed
    xAxis->setRange(0, maxSamples);
    yAxis->setRange(yAxis->min(), yAxis->max());
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
