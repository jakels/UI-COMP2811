#include "fluorinatedpage.h"
#include <QGridLayout>
#include <QFrame>
#include <QDebug>

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

    // Add navigation bar
    mainLayout->addLayout(createNavigationBar());

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

// Create the navigation bar
QHBoxLayout *FluorinatedPage::createNavigationBar()
{
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *appTitle = new QLabel("Fluorinated Compounds");
    appTitle->setStyleSheet("font-size: 26px; font-weight: bold; color: #333; padding: 5px;");

    QString buttonStyle = "QPushButton {"
                          "  background-color: #3498db; color: white; border-radius: 15px; "
                          "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                          "QPushButton:hover { background-color: #2980b9; }"
                          "QPushButton:pressed { background-color: #1c5a7a; }";

    QPushButton *navButton1 = createNavigationButton("Pollutant Overview", buttonStyle);
    QPushButton *navButton2 = createNavigationButton("POPs", buttonStyle);
    QPushButton *navButton3 = createNavigationButton("Fluorinated Compounds", buttonStyle);
    QPushButton *navButton4 = createNavigationButton("Compliance Overview", buttonStyle);
    QPushButton *navButton5 = createNavigationButton("Data Visualization", buttonStyle);

    headerLayout->addWidget(appTitle);
    headerLayout->addWidget(navButton1);
    headerLayout->addWidget(navButton2);
    headerLayout->addWidget(navButton3);
    headerLayout->addWidget(navButton4);
    headerLayout->addWidget(navButton5);

    return headerLayout;
}

// Helper to create a navigation button
QPushButton *FluorinatedPage::createNavigationButton(const QString &text, const QString &style)
{
    QPushButton *button = new QPushButton(text);
    button->setStyleSheet(style);
    return button;
}

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
    card->setStyleSheet(QString("background-color: %1; border-radius: 10px; padding: 20px;").arg(color));
    card->setFixedSize(300, 150);

    QVBoxLayout *cardContent = new QVBoxLayout(card);
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #fff;");
    titleLabel->setAlignment(Qt::AlignCenter);

    cardContent->addWidget(titleLabel);

    return card;
}

// Create visualization section
QWidget *FluorinatedPage::createVisualizationSection()
{
    QWidget *visualizationWidget = new QWidget();
    visualizationWidget->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc; border-radius: 10px;");
    visualizationWidget->setMinimumSize(800, 500);

    QLabel *placeholder = new QLabel("Visualization Area (Map/Time-Series)");
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet("font-size: 18px; color: #555;");

    QVBoxLayout *layout = new QVBoxLayout(visualizationWidget);
    layout->addWidget(placeholder);

    return visualizationWidget;
}

// Create summary section
QWidget *FluorinatedPage::createSummarySection()
{
    QWidget *summaryWidget = new QWidget();
    summaryWidget->setLayout(summaryCardLayout);
    return summaryWidget;
}
