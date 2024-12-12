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
#include <QDebug>
#include "DatasetInterface.h"
#include "translation_manager.h"

ComplianceDashboard::ComplianceDashboard(QWidget *parent)
        : QWidget(parent), cardWidth(300), cardHeight(300) // Updated card dimensions
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Uniform padding
    mainLayout->setSpacing(10); // Tight spacing between sections

    // Create filter labels and combo boxes
    QLabel *locationLabel = new QLabel(t("Location:").c_str());
    locationLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    locationComboBox = new QComboBox();
    locationComboBox->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    locationComboBox->addItem(t("Any Location").c_str());
    locationComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    locationComboBox->setMinimumWidth(150);
    locationComboBox->setMinimumContentsLength(10);

    QLabel *pollutantLabel = new QLabel(t("Determinand :").c_str());
    pollutantLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    pollutantComboBox = new QComboBox();
    pollutantComboBox->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    pollutantComboBox->addItem(t("Any Determinand").c_str());
    pollutantComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    pollutantComboBox->setMinimumWidth(150);
    pollutantComboBox->setMinimumContentsLength(10);

    QLabel *statusLabel = new QLabel(t("Status:").c_str());
    statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    statusComboBox = new QComboBox();
    statusComboBox->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    statusComboBox->addItem(t("Any Status").c_str());
    statusComboBox->addItem(t("Safe").c_str());
    statusComboBox->addItem(t("Caution").c_str());
    statusComboBox->addItem(t("Danger").c_str());
    statusComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    statusComboBox->setMinimumWidth(150);
    statusComboBox->setMinimumContentsLength(10);

    QPushButton *applyFilterButton = new QPushButton(t("Apply Filters").c_str());
    applyFilterButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #457B9D; color: white; border-radius: 5px; "
            "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { background-color: #1D3557; }"
            "QPushButton:pressed { background-color: #14213D; }"
    );

    // Populate filter combo boxes from the database
    for (auto location : DB_UniqueLocations()) {
        locationComboBox->addItem(QString::fromStdString(location));
    }

    for (auto chemical : DB_UniqueChemicals()) {
        pollutantComboBox->addItem(QString::fromStdString(chemical));
    }

    // Layout for filters
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(locationLabel);
    filterLayout->addWidget(locationComboBox);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(pollutantLabel);
    filterLayout->addWidget(pollutantComboBox);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(statusLabel);
    filterLayout->addWidget(statusComboBox);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(applyFilterButton);
    filterLayout->addStretch();

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

    connect(applyFilterButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilters);

    setLayout(mainLayout);
}

void ComplianceDashboard::displaySummaryCards(const std::vector<Data> &samples)
{
    // Clear previous cards
    while (QLayoutItem *item = cardContainer->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    int columns = calculateColumns(); // Determine number of columns
    int row = 0, col = 0;

    for (const auto &sample : samples) {
        QFrame *card = new QFrame(this);
        card->setFrameShape(QFrame::StyledPanel);

        // Determine background color based on status
        QString cardColor;
        if (sample.status == "Safe") {
            cardColor = "#008000"; // Green for safe
        } else if (sample.status == "Caution") {
            cardColor = "#F39C12"; // Amber for caution
        } else if (sample.status == "Danger") {
            cardColor = "#E74C3C"; // Red for danger
        } else {
            cardColor = "#95A5A6"; // Default color (gray) for undefined
        }

        card->setStyleSheet(QString(R"(
            QFrame {
                border-radius: 10px;
                padding: 15px;
                background-color: %1;
                color: white;
                box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1);
            }
        )").arg(cardColor));

        card->setMinimumSize(cardWidth, cardHeight);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);

        // Pollutant Level
        QLabel *pollutantLabel = new QLabel((t("Pollutant: ").c_str() + QString::fromStdString(sample.pollutant)), card);
        pollutantLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white; margin-bottom: 12px;");
        pollutantLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(pollutantLabel);

        // Location
        QLabel *locationLabel = new QLabel((t("Location: ").c_str() + QString::fromStdString(sample.location)), card);
        locationLabel->setStyleSheet("font-size: 16px; color: #f0f0f0; margin-bottom: 10px;");
        locationLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(locationLabel);

        // Status
        QLabel *statusLabel = new QLabel((t("Status: ").c_str() + QString::fromStdString(sample.status)), card);
        statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: white; margin-bottom: 10px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(statusLabel);

        // Details Button
        QPushButton *detailsButton = new QPushButton(t("Details").c_str(), card);
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

        cardContainer->addWidget(card, row, col);

        col++;
        if (col >= columns) {
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
    int windowWidth = scrollArea->viewport()->width(); // Use the viewport width of the scroll area
    return std::max(1, windowWidth / (cardWidth + 20)); // Ensure at least 1 column
}

void ComplianceDashboard::showDetails(const Data &sample)
{
    std::string historicalData;
    auto dataEntries = DB_GetEntriesByChemicalAndLocation(sample.pollutant, sample.location);
    for (auto &entry : dataEntries) {
        historicalData += entry.sampleDateTime + " " + entry.result + "\n";
    }

    QMessageBox::information(this,
                             (t("Details for ").c_str() + QString::fromStdString(sample.pollutant)),
                             (t("Location: ").c_str() + QString::fromStdString(sample.location) + "\n"
                              + t("Pollutant: ").c_str() + QString::fromStdString(sample.pollutant) + "\n"
                              + t("Status: ").c_str() + QString::fromStdString(sample.status) + "\n\n"
                              + t("Historical Trends: ").c_str() + QString::fromStdString(historicalData))
    );
}

void ComplianceDashboard::applyFilters()
{
    QString selectedLocation = locationComboBox->currentText();
    QString selectedPollutant = pollutantComboBox->currentText();
    QString selectedStatus = statusComboBox->currentText();

    // Validate if both are set to "Any", show dataset too large message
    if (selectedLocation == t("Any Location").c_str() && selectedPollutant == t("Any Determinand").c_str()) {
        QMessageBox::information(this, t("Dataset too large").c_str(), t("Please refine your search.").c_str());
        return;
    }

    std::vector<WaterQualitySample> waterSamples;

    // Determine which database query to run based on filters
    if (selectedLocation != t("Any Location").c_str() && selectedPollutant != t("Any Determinand").c_str()) {
        waterSamples = DB_GetEntriesByChemicalAndLocation(selectedPollutant.toStdString(), selectedLocation.toStdString());
    } else if (selectedLocation == t("Any Location").c_str() && selectedPollutant != t("Any Determinand").c_str()) {
        waterSamples = DB_GetEntriesByChemical(selectedPollutant.toStdString());
    } else if (selectedLocation != t("Any Location").c_str() && selectedPollutant == t("Any Determinand").c_str()) {
        waterSamples = DB_GetEntriesByLocation(selectedLocation.toStdString());
    }

    // Filter by status if not "Any Status"
    std::vector<Data> filteredSamples;
    for (auto &sample : waterSamples) {
        std::string status = SAMPLE_GetSafetyLevel(sample);
        if (selectedStatus != t("Any Status").c_str() && status != selectedStatus.toStdString()) {
            continue;
        }
        filteredSamples.push_back({sample.samplingPointLabel, sample.determinandLabel + " (" +sample.result+" " + sample.determinandUnitLabel+")", status});
    }

    displaySummaryCards(filteredSamples);

    if (filteredSamples.empty()) {
        QMessageBox::information(this, t("No Results").c_str(), t("No data matches the selected filters.").c_str());
    }
}
