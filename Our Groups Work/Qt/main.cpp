#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QFrame>
#include <QScrollArea>
#include "datavisualizationpage.h"
#include "pollutantoverview.h"
#include "fluorinatedpage.h"
#include "compliancedashboard.h"
#include "DatasetInterface.h"
#include "popspage.h"

// Main application window class
class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    DashboardWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;

    QWidget *createDashboardPage();
    DataVisualizationPage *createDataVisualizationPage();
    QHBoxLayout *createFilters();
    QScrollArea *createContent();
    QFrame *createCard(const QString &title, const QString &style);

    void setupNavigation();
};

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    stackedWidget = new QStackedWidget(this);

    // Add pages to the stacked widget
    stackedWidget->addWidget(createDashboardPage());
    stackedWidget->addWidget(new Pollutantoverview(this));
    stackedWidget->addWidget(new PopsPage(this));
    stackedWidget->addWidget(new FluorinatedPage(this));
    stackedWidget->addWidget(new ComplianceDashboard(this));

    // Dynamically update the window title based on the current page
    connect(stackedWidget, &QStackedWidget::currentChanged, this, [=](int index) {
        switch (index) {
            case 0:
                setWindowTitle("Dashboard");
                break;
            case 1:
                setWindowTitle("Pollutant Overview");
                break;
            case 2:
                setWindowTitle("POPs");
                break;
            case 3:
                setWindowTitle("Fluorinated Compounds");
                break;
            case 4:
                setWindowTitle("Compliance Overview");
                break;
            default:
                setWindowTitle("Water Quality Monitor");
                break;
        }
    });
    // Main layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);

    setupNavigation();
}

QWidget *DashboardWindow::createDashboardPage() {
    QWidget *dashboardPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(dashboardPage);

    layout->addLayout(createFilters());
    layout->addWidget(createContent(), 1); // Make the content take most of the space
    layout->addStretch(); // Add stretch to ensure it expands dynamically

    return dashboardPage;
}

DataVisualizationPage *DashboardWindow::createDataVisualizationPage() {
    return new DataVisualizationPage(this);
}

QHBoxLayout *DashboardWindow::createFilters() {
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *timeLabel = new QLabel("Filter by Time:");
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({"All Time", "Last Month", "Last Year"});
    timeFilter->setStyleSheet("padding: 5px; font-size: 14px; color: #333;");

    QLabel *regionLabel = new QLabel("Filter by Region:");
    regionLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({"All Regions", "Region A", "Region B", "Region C"});
    regionFilter->setStyleSheet("padding: 5px; font-size: 14px; color: #333;");

    QPushButton *applyFilterButton = new QPushButton("Apply Filters");
    applyFilterButton->setStyleSheet("QPushButton {"
                                     "  background-color: #3498db; color: white; border-radius: 15px; "
                                     "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                                     "QPushButton:hover { background-color: #2980b9; }"
                                     "QPushButton:pressed { background-color: #1c5a7a; }");

    connect(applyFilterButton, &QPushButton::clicked, this, [=]() {
        QString selectedTime = timeFilter->currentText();
        QString selectedRegion = regionFilter->currentText();
        qDebug() << "Filters applied: Time -" << selectedTime << ", Region -" << selectedRegion;
    });

    filterLayout->addWidget(timeLabel);
    filterLayout->addWidget(timeFilter);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(regionLabel);
    filterLayout->addWidget(regionFilter);
    filterLayout->addSpacing(20);
    filterLayout->addWidget(applyFilterButton);
    filterLayout->addStretch();

    return filterLayout;
}

QScrollArea *DashboardWindow::createContent() {
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    QWidget *contentWidget = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(contentWidget);
    gridLayout->setSpacing(20);

    QStringList cardTitles = {"Pollutant Overview", "POPs", "Fluorinated Compounds", "Compliance"};
    QString cardStyle = "QFrame {"
                        "  border: 1px solid rgba(0, 0, 0, 0.1); border-radius: 20px; background-color: #ffffff; "
                        "  box-shadow: 0 10px 15px rgba(0, 0, 0, 0.1); padding: 20px; transition: transform 0.3s ease, box-shadow 0.3s ease;}"
                        "QFrame:hover {"
                        "  transform: scale(1.02); box-shadow: 0 15px 20px rgba(0, 0, 0, 0.2); }";

    const int columnCount = 2; // Number of columns in the grid

    for (int i = 0; i < cardTitles.size(); ++i) {
        QFrame *card = createCard(cardTitles[i], cardStyle);
        gridLayout->addWidget(card, i / columnCount, i % columnCount, Qt::AlignTop);
    }

    contentWidget->setLayout(gridLayout);
    scrollArea->setWidget(contentWidget);

    return scrollArea;
}

QFrame *DashboardWindow::createCard(const QString &title, const QString &style) {
    QFrame *card = new QFrame();
    card->setStyleSheet(style);

    // Use QSizePolicy to make the card responsive
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50; text-align: center;");

    QLabel *summaryLabel = new QLabel("Summary Information...");
    summaryLabel->setStyleSheet("font-size: 16px; color: #7f8c8d; text-align: center;");
    summaryLabel->setWordWrap(true);

    QPushButton *detailsButton = new QPushButton("View Details");
    detailsButton->setStyleSheet("QPushButton {"
                                 "  background-color: #3498db; color: white; border-radius: 10px; "
                                 "  padding: 10px 15px; font-size: 14px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #2980b9; }"
                                 "QPushButton:pressed { background-color: #1c5a7a; }");

    // Connect the "View Details" button to the relevant page
    connect(detailsButton, &QPushButton::clicked, this, [this, title]() {
        if (title == "Pollutant Overview") {
            stackedWidget->setCurrentIndex(1); // Navigate to Pollutant Overview page
        } else if (title == "POPs") {
            stackedWidget->setCurrentIndex(2);
        } else if (title == "Fluorinated Compounds") {
            stackedWidget->setCurrentIndex(3);
        }else if (title == "Compliance") {
            stackedWidget->setCurrentIndex(4); // Navigate to Compliance Overview page
        }
    });

    cardLayout->addStretch();
    cardLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(summaryLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(detailsButton, 0, Qt::AlignCenter);
    cardLayout->addStretch();

    return card;
}

void DashboardWindow::setupNavigation() {
    QHBoxLayout *navLayout = new QHBoxLayout();

    QLabel *appTitle = new QLabel("Water Quality Monitor");
    appTitle->setStyleSheet("font-size: 26px; font-weight: bold; color: #333; padding: 10px;");

    QPushButton *dashboardButton = new QPushButton("Dashboard");
    QPushButton *pollutantOverviewButton = new QPushButton("Pollutant Overview");
    QPushButton *popsButton = new QPushButton("POPs");
    QPushButton *fluorinatedCompoundsButton = new QPushButton("Fluorinated Compounds");
    QPushButton *complianceOverviewButton = new QPushButton("Compliance Overview");

    QString buttonStyle = "QPushButton {"
                          "  background-color: #3498db; color: white; border-radius: 15px; "
                          "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                          "QPushButton:hover { background-color: #2980b9; }"
                          "QPushButton:pressed { background-color: #1c5a7a; }";
    dashboardButton->setStyleSheet(buttonStyle);
    pollutantOverviewButton->setStyleSheet(buttonStyle);
    popsButton->setStyleSheet(buttonStyle);
    fluorinatedCompoundsButton->setStyleSheet(buttonStyle);
    complianceOverviewButton->setStyleSheet(buttonStyle);


    QComboBox *languageSelector = new QComboBox();
    languageSelector->addItems({"Arabic", "French", "English", "Mandarin"});
    languageSelector->setStyleSheet("padding: 5px; font-size: 14px; color: #333;");


    navLayout->addWidget(appTitle);
    navLayout->addWidget(dashboardButton);
    navLayout->addWidget(pollutantOverviewButton);
    navLayout->addWidget(popsButton);
    navLayout->addWidget(fluorinatedCompoundsButton);
    navLayout->addWidget(complianceOverviewButton);
    navLayout->addWidget(languageSelector);

    connect(dashboardButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(pollutantOverviewButton, &QPushButton::clicked, this, [=]() {
       stackedWidget->setCurrentIndex(1);
   });
    connect(popsButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(fluorinatedCompoundsButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(complianceOverviewButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(4);
    });

    QWidget *navWidget = new QWidget(this);
    navWidget->setLayout(navLayout);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(centralWidget()->layout());
    mainLayout->insertWidget(0, navWidget);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    DB_Initialise();

    DashboardWindow window;
    window.resize(1200, 800);
    window.setWindowTitle("Dashboard");
    window.show();

    return app.exec();
}

#include "main.moc"
