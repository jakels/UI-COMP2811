#include "fluorinatedpage.h"
#include <QGridLayout>
#include <QFrame>
#include <QDebug>
#include <QtCharts>
#include <QDateTime>
#include <QToolTip>
#include "DatasetInterface.h"

QStringList fluroPollutantTypes = {
        "Trifluralin",
        "Fluoroxypyr",
        "FLUORENE",
        "Fluoranthene",
        "Diflurobnzrn",
        "Fluoride - F",
        "Fluazifopbut",
        "Flutriafol",
        "Cyfluthrin"
};

FluorinatedPage::FluorinatedPage(QWidget *parent)
    : QMainWindow(parent),
      summaryCardLayout(new QVBoxLayout),
      filterDropdownType(new QComboBox()),
      filterDropdownLocation(new QComboBox()),
      filterDropdownCompliance(new QComboBox())
{
    /*std::vector<WaterQualitySample> samples = DB_GetEntriesByChemical("", "");
    for (int i = 0; i < 1000; i++) {
        WaterQualitySample current = samples[i];*/


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

    filterDropdownType->addItems(fluroPollutantTypes);
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

    QStringList locations = {"Any"};
    for(auto location : DB_UniqueLocations())
    {
        locations.append(location.c_str());
    }
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

    //complianceFilterLayout->addWidget(filterLabel);
    //complianceFilterLayout->addWidget(filterDropdownCompliance);

    connect(filterDropdownCompliance, &QComboBox::currentTextChanged, this, &FluorinatedPage::updateComplianceStatus);

    return filterWidget;
}

// Update compliance status
void FluorinatedPage::updateComplianceStatus()
{
    QLayoutItem *child;
    while ((child = summaryCardLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QString pollutant = filterDropdownType->currentText();
    QString location = filterDropdownLocation->currentText();
    std::vector<WaterQualitySample> samplesOfTheFluroCompound = DB_GetEntriesByChemical(pollutant.toStdString());
    if(samplesOfTheFluroCompound.size() == 0)
    {
        QMessageBox::information(this, "No data","No data for the fluro-compound selected.");
        return;
    }
    QString safetyLevel = SAMPLE_GetSafetyLevel(samplesOfTheFluroCompound.back()).c_str();

    QString status = QString("Pollutant: %1\nLocation: %2\nStatus: %3").arg(pollutant, location, safetyLevel);
    QString color = "green";
    if(safetyLevel == "Caution")
    {
        color = "orange";
    }
    if(safetyLevel == "Danger")
    {
        color = "red";
    }


    summaryCardLayout->addWidget(createSummaryCard(status, color));
    updateChartData();
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

QLineSeries *seriesFluro;
QDateTimeAxis *axisXFluro;
QValueAxis *axisYFluro;

QWidget *FluorinatedPage::createVisualizationSection()
{
    QWidget *visualizationWidget = new QWidget();
    visualizationWidget->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc; border-radius: 10px;");
    visualizationWidget->setMinimumSize(800, 500);

    seriesFluro = new QLineSeries();
    seriesFluro->setName("Fluorinated Pollutant Levels");


    // Load in data from the backend, get all the samples that had the chemical
    std::vector<WaterQualitySample> query = OrderSamplesByDate(DB_GetEntriesByChemical("Fluoroxypyr"));
    int numberOfSamples = query.size();
    double maximumResult = 0;

    // Loop through the samples
    for (int i = 0; i < numberOfSamples; i++)
    {
        WaterQualitySample sample = query[i];
        auto sampleDate = sample.sampleDateTime.c_str();
        double sampleResult = atof(sample.result.c_str());

        if (sampleResult > maximumResult) {
            maximumResult = sampleResult;
        }

        // Add the sample data to the chart
        seriesFluro->append(QDateTime::fromString(sampleDate, Qt::ISODate).toMSecsSinceEpoch(), sampleResult);
    }

    QChart *chart = new QChart();
    chart->addSeries(seriesFluro);
    chart->setTitle("Fluorinated Pollutants Levels Over Time");
    chart->legend()->setAlignment(Qt::AlignBottom);

    axisXFluro = new QDateTimeAxis();
    axisXFluro->setFormat("yyyy-MM-dd HH:mm");
    axisXFluro->setTitleText("Time");
    axisXFluro->setTickCount(10);
    chart->addAxis(axisXFluro, Qt::AlignBottom);
    seriesFluro->attachAxis(axisXFluro);

    axisYFluro = new QValueAxis();
    std::string title = "Pollutant Levels " + query[0].determinandUnitLabel;
    axisYFluro->setTitleText(title.c_str());
    axisYFluro->setLabelFormat("%.9f");
    axisYFluro->setRange(0, maximumResult);
    chart->addAxis(axisYFluro, Qt::AlignLeft);
    seriesFluro->attachAxis(axisYFluro);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    connect(seriesFluro, &QLineSeries::hovered, this, &FluorinatedPage::showChartDataTooltip);

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

void FluorinatedPage::showChartDataTooltip(const QPointF &point, bool state)
{
    if (state) {
        QString tooltipText = QString("Time: %1\nLevel: %2")
                                  .arg(QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd HH:mm"))
                                  .arg(point.y(), 0, 'f', 2);
        QToolTip::showText(QCursor::pos(), tooltipText);
    } else {
        QToolTip::hideText();
    }
}

void FluorinatedPage::updateChartData()
{
    // Clear old data
    seriesFluro->clear();

    QString pollutant = filterDropdownType->currentText();
    QString location = filterDropdownLocation->currentText();
    QString compliance = filterDropdownCompliance->currentText();

    std::vector<WaterQualitySample> query;

    // If location is "Any", query by chemical only
    if (location == "Any") {
        query = OrderSamplesByDate(DB_GetEntriesByChemical(pollutant.toStdString()));
    } else {
        query = OrderSamplesByDate(DB_GetEntriesByChemicalAndLocation(pollutant.toStdString(), location.toStdString()));
    }

    double maximumResult = 0;

    for (auto &sample : query)
    {
        double sampleResult = atof(sample.result.c_str());
        if (sampleResult > maximumResult) {
            maximumResult = sampleResult;
        }
        seriesFluro->append(QDateTime::fromString(sample.sampleDateTime.c_str(), Qt::ISODate).toMSecsSinceEpoch(), sampleResult);
    }

    // Update the Y axis range if we have data
    if (!query.empty()) {
        axisYFluro->setRange(0, maximumResult);
    } else {
        // If no data, reset to default range
        axisYFluro->setRange(0, 1);
        QMessageBox::information(this, "No data", "There was no data for your query.");
    }
}
