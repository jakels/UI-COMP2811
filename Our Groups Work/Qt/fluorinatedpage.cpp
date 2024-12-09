#include "fluorinatedpage.h"
#include <QGridLayout>
#include <QFrame>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QToolTip>


//QT_CHARTS_USE_NAMESPACE

FluorinatedPage::FluorinatedPage(QWidget *parent)
    : QMainWindow(parent),
    summaryCardLayout(new QVBoxLayout),
    filterDropdownType(new QComboBox()),
    filterDropdownLocation(new QComboBox()),
    filterDropdownCompliance(new QComboBox())
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);


    // Filters layout
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(createTypeFilterSection());
    filterLayout->addWidget(createLocationFilterSection());
    filterLayout->addWidget(createComplianceFilterSection());
    mainLayout->addLayout(filterLayout);

    // Visualization and summary layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->addWidget(createSummarySection(), 1);
    contentLayout->addWidget(createVisualizationSection(), 3);
    mainLayout->addLayout(contentLayout);

    resize(1200, 700);
    setWindowTitle("Fluorinated Compounds");
    setStyleSheet("background-color: #f5f5f5;");

    // Initialize the compliance status
    updateComplianceStatus();
}

FluorinatedPage::~FluorinatedPage() {}

// Create filter sections
QWidget *FluorinatedPage::createTypeFilterSection()
{
    QWidget *filterWidget = new QWidget();
    QVBoxLayout *typeFilterLayout = new QVBoxLayout(filterWidget);
    typeFilterLayout->setSpacing(8);

    QLabel *filterLabel = new QLabel("Select Type:");
    filterLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: black;");

    QStringList pollutantTypes = {"PFAS Compliance", "Fluorinated Compound A", "Fluorinated Compound B"};
    filterDropdownType->addItems(pollutantTypes);
    filterDropdownType->setStyleSheet(
        "QComboBox {"
        "  font-size: 14px; padding: 5px; color: black; background-color: white; border: 1px solid #ccc; border-radius: 5px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  color: black; background-color: white; selection-background-color: #3498db; selection-color: white;"
        "}"
        );

    typeFilterLayout->addWidget(filterLabel);
    typeFilterLayout->addWidget(filterDropdownType);

    connect(filterDropdownType, &QComboBox::currentTextChanged, this, &FluorinatedPage::updateComplianceStatus);

    return filterWidget;
}

QWidget *FluorinatedPage::createLocationFilterSection()
{
    QWidget *filterWidget = new QWidget();
    QVBoxLayout *locationFilterLayout = new QVBoxLayout(filterWidget);
    locationFilterLayout->setSpacing(8);

    QLabel *filterLabel = new QLabel("Select Location:");
    filterLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: black;");

    QStringList locations = {"North America", "Europe", "Asia", "Global"};
    filterDropdownLocation->addItems(locations);
    filterDropdownLocation->setStyleSheet(
        "QComboBox {"
        "  font-size: 14px; padding: 5px; color: black; background-color: white; border: 1px solid #ccc; border-radius: 5px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  color: black; background-color: white; selection-background-color: #3498db; selection-color: white;"
        "}"
        );

    locationFilterLayout->addWidget(filterLabel);
    locationFilterLayout->addWidget(filterDropdownLocation);

    connect(filterDropdownLocation, &QComboBox::currentTextChanged, this, &FluorinatedPage::updateComplianceStatus);

    return filterWidget;
}

QWidget *FluorinatedPage::createComplianceFilterSection()
{
    QWidget *filterWidget = new QWidget();
    QVBoxLayout *complianceFilterLayout = new QVBoxLayout(filterWidget);
    complianceFilterLayout->setSpacing(8);

    QLabel *filterLabel = new QLabel("Compliance Status:");
    filterLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: black;");

    QStringList complianceStatuses = {"Compliant", "Non-Compliant"};
    filterDropdownCompliance->addItems(complianceStatuses);
    filterDropdownCompliance->setStyleSheet(
        "QComboBox {"
        "  font-size: 14px; padding: 5px; color: black; background-color: white; border: 1px solid #ccc; border-radius: 5px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  color: black; background-color: white; selection-background-color: #3498db; selection-color: white;"
        "}"
        );

    complianceFilterLayout->addWidget(filterLabel);
    complianceFilterLayout->addWidget(filterDropdownCompliance);

    connect(filterDropdownCompliance, &QComboBox::currentTextChanged, this, &FluorinatedPage::updateComplianceStatus);

    return filterWidget;
}

// Update compliance status
void FluorinatedPage::updateComplianceStatus()
{
    // Clear existing summary cards
    QLayoutItem *child;
    while ((child = summaryCardLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Determine compliance status based on filters
    QString pollutant = filterDropdownType->currentText();
    QString location = filterDropdownLocation->currentText();
    QString compliance = filterDropdownCompliance->currentText();

    QString status = QString("Pollutant: %1\nLocation: %2\nStatus: %3").arg(pollutant, location, compliance);
    QString color = (compliance == "Compliant") ? "green" : "red";

    // Add a summary card with compliance status
    summaryCardLayout->addWidget(createSummaryCard(status, color));
}

// Create summary card
QWidget *FluorinatedPage::createSummaryCard(const QString &title, const QString &color)
{
    QFrame *card = new QFrame();
    card->setStyleSheet(QString("background-color: %1; border-radius: 50px; padding: 50px;").arg(color));
    card->setFixedSize(400, 500);

    QVBoxLayout *cardContent = new QVBoxLayout(card);
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #fff;");
    titleLabel->setAlignment(Qt::AlignCenter);

    cardContent->addWidget(titleLabel);

    return card;
}

QWidget *FluorinatedPage::createVisualizationSection() {
    // Create the visualization widget
    QWidget *visualizationWidget = new QWidget();
    visualizationWidget->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc; border-radius: 10px;");
    visualizationWidget->setMinimumSize(800, 500);

    // Create a line series for pollutant levels
    QLineSeries *series = new QLineSeries();
    series->setName("Fluorinated Pollutant Levels");

    // Example data for fluorinated pollutants (time, pollutant level)
    series->append(QDateTime::fromString("2024-12-01T08:00:00", Qt::ISODate).toMSecsSinceEpoch(), 45.2);
    series->append(QDateTime::fromString("2024-12-01T12:00:00", Qt::ISODate).toMSecsSinceEpoch(), 47.8);
    series->append(QDateTime::fromString("2024-12-01T16:00:00", Qt::ISODate).toMSecsSinceEpoch(), 44.0);
    series->append(QDateTime::fromString("2024-12-01T20:00:00", Qt::ISODate).toMSecsSinceEpoch(), 49.3);
    series->append(QDateTime::fromString("2024-12-02T00:00:00", Qt::ISODate).toMSecsSinceEpoch(), 50.7);

    // Create the chart and configure it
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Fluorinated Pollutants Levels Over Time");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Configure the X-axis for time
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd HH:mm");
    axisX->setTitleText("Time");
    axisX->setTickCount(10);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configure the Y-axis for pollutant levels
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Pollutant Levels (ppm)");
    axisY->setLabelFormat("%.1f");
    axisY->setRange(40.0, 60.0); // Example range, can be adjusted based on actual data
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create the chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Connect the hovered signal to display tooltips
    connect(series, &QLineSeries::hovered, this, &FluorinatedPage::showChartDataTooltip);

    // Arrange the chart view in the layout
    QVBoxLayout *layout = new QVBoxLayout(visualizationWidget);
    layout->addWidget(chartView);

    return visualizationWidget;
}

// Create summary section
QWidget *FluorinatedPage::createSummarySection()
{
    QWidget *summaryWidget = new QWidget();
    summaryWidget->setLayout(summaryCardLayout);
    return summaryWidget;
}


void FluorinatedPage::showChartDataTooltip(const QPointF &point, bool state) {
    if (state) {
        // Show the tooltip at the current mouse position
        QString tooltipText = QString("Time: %1\nLevel: %2 ppm")
                                  .arg(QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd HH:mm"))
                                  .arg(point.y(), 0, 'f', 2); // Format the level with 2 decimal places
        QToolTip::showText(QCursor::pos(), tooltipText);
    } else {
        // Hide the tooltip when the mouse leaves the point
        QToolTip::hideText();
    }
}

