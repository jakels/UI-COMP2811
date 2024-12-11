#include "mainwindow.h"

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
            case 0: setWindowTitle("Dashboard"); break;
            case 1: setWindowTitle("Pollutant Overview"); break;
            case 2: setWindowTitle("POPs"); break;
            case 3: setWindowTitle("Fluorinated Compounds"); break;
            case 4: setWindowTitle("Compliance Overview"); break;
            default: setWindowTitle("Water Quality Monitor"); break;
        }
    });

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);

    setupNavigation();
    configureTabOrder();
}

QWidget *DashboardWindow::createDashboardPage() {
    QWidget *dashboardPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(dashboardPage);

    layout->addLayout(createFilters());
    layout->addWidget(createContent(), 1);
    layout->addStretch();

    return dashboardPage;
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

    const int columnCount = 2;

    for (int i = 0; i < cardTitles.size(); ++i) {
        QFrame *card = createCard(cardTitles[i], cardStyle);
        gridLayout->addWidget(card, i / columnCount, i % columnCount, Qt::AlignTop);
    }

    contentWidget->setLayout(gridLayout);
    scrollArea->setWidget(contentWidget);

    return scrollArea;
}

QFrame *DashboardWindow::createCard(const QString &title, const QString &style) {
    int numberOfSafe = SAMPLES_NumberOfSafeEntries();
    int numberOfCaution = SAMPLES_NumberOfCautionEntries();
    int numberOfDanger = SAMPLES_NumberOfDangerEntries();
    int numberOfSafeFluroEntries = SAMPLES_NumberOfSafeFluroEntries();


    QFrame *card = new QFrame();
    card->setStyleSheet(style);

    // Use QSizePolicy to make the card responsive
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50; text-align: center;");

    QLabel *summaryLabel = new QLabel();
    if (title.toStdString() == "Pollutant Overview") {
        QString formattedText = QString(
            "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
            "<b style='color: green;'>Number of Safe:</b> <span style='color: green;'>%1</span><br>"
            "<b style='color: orange;'>Number of Caution:</b> <span style='color: orange;'>%2</span><br>"
            "<b style='color: red;'>Number of Danger:</b> <span style='color: red;'>%3</span>"
            "</p>"
        ).arg(numberOfSafe).arg(numberOfCaution).arg(numberOfDanger);
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == "Fluorinated Compounds")
    {
        std::string status = "<b style='color: green;'>Status:</b> <span style='color: green;'>Above compliance threshold</span><br>";
        double percentage = ((double)numberOfSafeFluroEntries/(double)SAMPLES_NumberOfFluroEntries()) * (double)100;
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>Status:</b> <span style='color: red;'>Below compliance threshold</span><br>";
        }
        QString formattedText = QString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>Number of Safe Fluro-Samples:</b> <span style='color: green;'>%1</span><br>"
                "<b style='color: grey;'>Safety rate:</b> <span style='color: grey;'>%2 p.c</span><br>"
                "%3"
                "</p>"
        ).arg(numberOfSafeFluroEntries).arg(percentage).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == "POPs")
    {
        QString formattedText = QString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>Number of Safe:</b> <span style='color: green;'>Waiting for implementation</span><br>"
                "</p>"
        );
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == "Compliance")
    {
        QString formattedText = QString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>Number of Safe:</b> <span style='color: green;'>Waiting for implementation</span><br>"
                "</p>"
        );
        summaryLabel->setText(formattedText);
    }
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

    // Title Label
    QLabel *appTitle = new QLabel("Water Quality Monitor");
    appTitle->setStyleSheet(
        "font-size: 26px; "
        "font-weight: bold; "
        "color: #2c3e50; "
        "padding: 10px; "
        "margin-right: 20px;"
    );

    // Navigation Buttons
    QPushButton *dashboardButton = new QPushButton("Dashboard");
    QPushButton *pollutantOverviewButton = new QPushButton("Pollutant Overview");
    QPushButton *popsButton = new QPushButton("POPs");
    QPushButton *fluorinatedCompoundsButton = new QPushButton("Fluorinated Compounds");
    QPushButton *complianceOverviewButton = new QPushButton("Compliance Overview");

    QString buttonStyle =
        "QPushButton {"
        "  background-color: #3498db; "
        "  color: white; "
        "  border-radius: 10px; "
        "  padding: 10px 20px; "
        "  font-size: 14px; "
        "  font-weight: bold; "
        "  margin: 5px;"
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1c5a7a; }"
        "QPushButton:focus {"
        "  border: 2px solid #1abc9c; "
        "  outline: none;"
        "}";

    dashboardButton->setStyleSheet(buttonStyle);
    pollutantOverviewButton->setStyleSheet(buttonStyle);
    popsButton->setStyleSheet(buttonStyle);
    fluorinatedCompoundsButton->setStyleSheet(buttonStyle);
    complianceOverviewButton->setStyleSheet(buttonStyle);

    // Language Selector
    QComboBox *languageSelector = new QComboBox();
    languageSelector->addItems({"Arabic", "French", "English", "Mandarin"});
    languageSelector->setStyleSheet(
        "padding: 5px; "
        "font-size: 14px; "
        "color: #333; "
        "border: 1px solid #ccc; "
        "border-radius: 5px; "
        "background-color: white; "
        "margin-left: 20px;"
    );

    // Set Focus Policies
    dashboardButton->setFocusPolicy(Qt::StrongFocus);
    pollutantOverviewButton->setFocusPolicy(Qt::StrongFocus);
    popsButton->setFocusPolicy(Qt::StrongFocus);
    fluorinatedCompoundsButton->setFocusPolicy(Qt::StrongFocus);
    complianceOverviewButton->setFocusPolicy(Qt::StrongFocus);
    languageSelector->setFocusPolicy(Qt::StrongFocus);

    // Add widgets to the navigation layout
    navLayout->addWidget(appTitle);
    navLayout->addWidget(dashboardButton);
    navLayout->addWidget(pollutantOverviewButton);
    navLayout->addWidget(popsButton);
    navLayout->addWidget(fluorinatedCompoundsButton);
    navLayout->addWidget(complianceOverviewButton);
    navLayout->addWidget(languageSelector);

    // Connect buttons to page switches
    connect(dashboardButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(0); });
    connect(pollutantOverviewButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(1); });
    connect(popsButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(2); });
    connect(fluorinatedCompoundsButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(3); });
    connect(complianceOverviewButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(4); });

    // Create a navigation widget and set its layout
    QWidget *navWidget = new QWidget(this);
    navWidget->setLayout(navLayout);
    navWidget->setStyleSheet(
        "background-color: #ecf0f1; "
        "border-bottom: 2px solid #bdc3c7;"
    );

    // Insert navigation widget into the main layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(centralWidget()->layout());
    mainLayout->insertWidget(0, navWidget);

    // Add Footer
    QHBoxLayout *footerLayout = new QHBoxLayout();

    QLabel *userGuideLink = new QLabel("<a href='https://example.com/user-guide'>User Guide</a>");
    userGuideLink->setStyleSheet("font-size: 14px; color: #3498db;");
    userGuideLink->setTextFormat(Qt::RichText);
    userGuideLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    userGuideLink->setOpenExternalLinks(true);

    QLabel *helpResourcesLink = new QLabel("<a href='https://example.com/help-resources'>Help Resources</a>");
    helpResourcesLink->setStyleSheet("font-size: 14px; color: #3498db;");
    helpResourcesLink->setTextFormat(Qt::RichText);
    helpResourcesLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    helpResourcesLink->setOpenExternalLinks(true);

    QLabel *creditsLink = new QLabel("<a href='https://example.com/credits'>Credits</a>");
    creditsLink->setStyleSheet("font-size: 14px; color: #3498db;");
    creditsLink->setTextFormat(Qt::RichText);
    creditsLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    creditsLink->setOpenExternalLinks(true);

    footerLayout->addWidget(userGuideLink);
    footerLayout->addSpacing(10);
    footerLayout->addWidget(helpResourcesLink);
    footerLayout->addSpacing(10);
    footerLayout->addWidget(creditsLink);
    footerLayout->addStretch();

    QWidget *footerWidget = new QWidget(this);
    footerWidget->setLayout(footerLayout);

    mainLayout->addWidget(footerWidget);
}

void DashboardWindow::configureTabOrder() {
    // Get references to the widgets using their object names or explicit pointers
    QPushButton *dashboardButton = findChild<QPushButton *>("Dashboard");
    QPushButton *pollutantOverviewButton = findChild<QPushButton *>("Pollutant Overview");
    QPushButton *popsButton = findChild<QPushButton *>("POPs");
    QPushButton *fluorinatedCompoundsButton = findChild<QPushButton *>("Fluorinated Compounds");
    QPushButton *complianceOverviewButton = findChild<QPushButton *>("Compliance Overview");
    QComboBox *languageSelector = findChild<QComboBox *>();
    QPushButton *applyFilterButton = findChild<QPushButton *>("Apply Filters");

    // Ensure all widgets exist before setting the tab order
    if (dashboardButton && pollutantOverviewButton && popsButton &&
        fluorinatedCompoundsButton && complianceOverviewButton &&
        languageSelector && applyFilterButton) {
        setTabOrder(dashboardButton, pollutantOverviewButton);
        setTabOrder(pollutantOverviewButton, popsButton);
        setTabOrder(popsButton, fluorinatedCompoundsButton);
        setTabOrder(fluorinatedCompoundsButton, complianceOverviewButton);
        setTabOrder(complianceOverviewButton, languageSelector);
        setTabOrder(languageSelector, applyFilterButton);
        } else {
            qDebug() << "One or more widgets not found for setting tab order.";
        }
}

void DashboardWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Left:
            qDebug() << "Left arrow pressed - navigating backward";
        focusPreviousChild(); // Navigate to the previous widget
        break;
        case Qt::Key_Right:
            qDebug() << "Right arrow pressed - navigating forward";
        focusNextChild(); // Navigate to the next widget
        break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            qDebug() << "Enter or Return pressed";
        if (QPushButton *button = qobject_cast<QPushButton *>(QApplication::focusWidget())) {
            button->click(); // Trigger focused button action
        }
        break;
        case Qt::Key_Escape:
            qDebug() << "Escape pressed - closing application";
        close(); // Exit the application
        break;
        default:
            QMainWindow::keyPressEvent(event); // Default handling
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qApp->setStyleSheet("QWidget:focus { border: 2px solid #3498db; outline: none; }");

    DB_Initialise();

    DashboardWindow window;
    window.resize(1200, 800);
    window.setWindowTitle("Dashboard");
    window.show();

    return app.exec();
}