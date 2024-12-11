#include "compliancedashboard.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFrame>
#include "DatasetInterface.h"
#include "Utils.h"

ComplianceDashboard::ComplianceDashboard(QWidget *parent)
    : QWidget(parent), cardWidth(300), cardHeight(300) // Updated card dimensions
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Uniform padding
    mainLayout->setSpacing(10); // Tight spacing between sections

    // Title
    QLabel *titleLabel = new QLabel("Compliance Dashboard", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 24px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Filters
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(10); // Reduce spacing between filters

    locationFilter = new QComboBox(this);
    pollutantFilter = new QComboBox(this);
    statusFilter = new QComboBox(this);
    QPushButton *applyFilterButton = new QPushButton("Apply Filters", this);

    locationFilter->addItem("Any Location");
    pollutantFilter->addItem("Any Pollutant");

    for(auto location : DB_UniqueLocations()){
        locationFilter->addItem(location.c_str());
    }

    for(auto chemical : DB_UniqueChemicals()){
        pollutantFilter->addItem(chemical.c_str());
    }

    statusFilter->addItems({"Any Status", "Safe", "Caution", "Danger"});

    filterLayout->addWidget(new QLabel("Location:", this));
    filterLayout->addWidget(locationFilter);
    filterLayout->addWidget(new QLabel("Pollutant:", this));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Status:", this));
    filterLayout->addWidget(statusFilter);
    filterLayout->addWidget(applyFilterButton);
    mainLayout->addLayout(filterLayout);

    // Scrollable Area for Cards
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    scrollContent = new QWidget(this);
    cardContainer = new QGridLayout(scrollContent);
    cardContainer->setSpacing(15); // Space between cards
    cardContainer->setContentsMargins(0, 0, 0, 0); // Remove card container margins

    scrollContent->setLayout(cardContainer);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Mock data setup
    mockData = { };

    connect(applyFilterButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilters);

    displaySummaryCards(mockData);

    setLayout(mainLayout);
}

void ComplianceDashboard::displaySummaryCards(const std::vector<MockSample> &samples)
{
    // Clear previous cards
    while (QLayoutItem *item = cardContainer->takeAt(0))
    {
        delete item->widget();
        delete item;
    }

    int columns = calculateColumns(); // Determine number of columns
    int row = 0, col = 0;

    for (const auto &sample : samples)
    {
        QFrame *card = new QFrame(this);
        card->setFrameShape(QFrame::StyledPanel);
        card->setStyleSheet(QString(R"(
            QFrame {
                border-radius: 10px;
                padding: 15px;
                background-color: %1;
                color: white;
                box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1);
            }
        )").arg(sample.status == "Safe" ? "#2ECC71" : "#E74C3C"));

        card->setMinimumSize(cardWidth, cardHeight);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);

        // Pollutant Name
        QLabel *pollutantLabel = new QLabel(QString::fromStdString(sample.pollutant), card);
        pollutantLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white; margin-bottom: 12px;");
        pollutantLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(pollutantLabel);

        // Location
        QLabel *locationLabel = new QLabel("Location: " + QString::fromStdString(sample.location), card);
        locationLabel->setStyleSheet("font-size: 16px; color: #f0f0f0; margin-bottom: 10px;");
        locationLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(locationLabel);

        // Status
        QLabel *statusLabel = new QLabel("Status: " + QString::fromStdString(sample.status), card);
        statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: white; margin-bottom: 10px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(statusLabel);

        // Details Button
        QPushButton *detailsButton = new QPushButton("Details", card);
        detailsButton->setStyleSheet(R"(
            QPushButton {
                background-color: white;
                color: black;
                font-size: 14px;
                padding: 10px;
                border-radius: 5px;
                border: none;
            }
            QPushButton:hover {
                background-color: #D6DBDF;
            }
        )");
        connect(detailsButton, &QPushButton::clicked, this, [this, sample]() {
            showDetails(sample);
        });
        cardLayout->addWidget(detailsButton);

        // Add card to grid layout
        cardContainer->addWidget(card, row, col);

        // Update row and column based on the number of columns
        col++;
        if (col >= columns)
        {
            col = 0;
            row++;
        }
    }
}

void ComplianceDashboard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Recalculate the grid layout when the window is resized
    displaySummaryCards(mockData);
}

int ComplianceDashboard::calculateColumns()
{
    // Dynamically calculate columns based on window width
    int windowWidth = scrollArea->viewport()->width(); // Use the viewport width of the scroll area
    return std::max(1, windowWidth / (cardWidth + 20)); // Ensure at least 1 column
}

void ComplianceDashboard::showDetails(const MockSample &sample)
{
    std::string historicalData;
    for (auto sample : DB_GetEntriesByChemicalAndLocation(sample.pollutant, sample.location)){
        historicalData += sample.sampleDateTime + " " + sample.result + "\n";
    }
    QMessageBox::information(this, "Details for " + QString::fromStdString(sample.pollutant),
                             "Location: " + QString::fromStdString(sample.location) + "\n"
                                                                                      "Pollutant: " + QString::fromStdString(sample.pollutant) + "\n"
                                                                              "Status: " + QString::fromStdString(sample.status) + "\n\n"
                                                                           "Historical Trends: " + historicalData.c_str());
}

void ComplianceDashboard::applyFilters()
{
    QString selectedLocation = locationFilter->currentText();
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedStatus = statusFilter->currentText();

    std::vector<MockSample> filteredSamples;
    std::vector<WaterQualitySample> waterSamples;
    if(selectedLocation.toStdString() != "Any Location" && selectedPollutant.toStdString() != "Any Pollutant")
    {
        waterSamples = DB_GetEntriesByChemicalAndLocation(selectedPollutant.toStdString(), selectedLocation.toStdString());
    }
    if(selectedLocation.toStdString() == "Any Location" && selectedPollutant.toStdString() != "Any Pollutant")
    {
        waterSamples = DB_GetEntriesByChemical(selectedPollutant.toStdString());
    }
    if(selectedLocation.toStdString() != "Any Location" && selectedPollutant.toStdString() == "Any Pollutant")
    {
        waterSamples = DB_GetEntriesByLocation(selectedLocation.toStdString());
    }
    if(selectedLocation.toStdString() == "Any Location" && selectedPollutant.toStdString() == "Any Pollutant")
    {
        QMessageBox::information(this, "Dataset too large", "Please refine your search.");
        return;
    }
    for (auto sample : waterSamples)
    {
        auto status = SAMPLE_GetSafetyLevel(sample);
        if(status != selectedStatus.toStdString() && selectedStatus.toStdString() != "Any Status")
        {
            continue;
        }
        filteredSamples.push_back({sample.samplingPointLabel, sample.result, status});
    }

    displaySummaryCards(filteredSamples);

    if (filteredSamples.empty())
    {
        QMessageBox::information(this, "No Results", "No data matches the selected filters.");
    }
}

