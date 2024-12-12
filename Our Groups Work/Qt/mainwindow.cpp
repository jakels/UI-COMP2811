#include <QMessageBox>
#include "mainwindow.h"

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent) {

    QMessageBox::information(this, "Welcome","The database will take a minute to load. Press OK to start.");
    DB_Initialise();

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

    //setupNavigation();
    createNavigationBar();
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
    timeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({"All Time", "Last Month", "Last Year"});
    timeFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    timeFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    timeFilter->setMinimumWidth(150);
    timeFilter->setMinimumContentsLength(10);

    QLabel *regionLabel = new QLabel("Filter by Region:");
    regionLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({"All Regions", "Region A", "Region B", "Region C"});
    regionFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    regionFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    regionFilter->setMinimumWidth(150);
    regionFilter->setMinimumContentsLength(10); // adjusts width based on number of characters

    QPushButton *applyFilterButton = new QPushButton("Apply Filters");
    applyFilterButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #457B9D; color: white; border-radius: 5px; "
        "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1D3557; }"
        "QPushButton:pressed { background-color: #14213D; }"
    );

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
                        "  border: 1px solid #DEE2E6; border-radius: 10px; background-color: #F8F9FA; "
                        "  box-shadow: 0 5px 10px rgba(0, 0, 0, 0.1); padding: 15px; }"
                        "QFrame:hover {"
                        "  transform: scale(1.05); box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2); }";

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
        std::string status = "<b style='color: green;'>Status:</b> <span style='color: green;'>Above compliance threshold</span><br>";
        double percentage = 100 - (((double)SAMPLES_NumberOfUnsafePops()/(double)std::max(numberOfPops, 1)) * (double)100);
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>Status:</b> <span style='color: red;'>Below compliance threshold</span><br>";
        }
        QString formattedText = QString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>Number of POP samples:</b> <span style='color: green;'>%1</span><br>"
                "<b style='color: grey;'>Safety rate:</b> <span style='color: grey;'>%2 p.c</span><br>"
                "<b style='color: red;'>Number of fail POP samples:</b> <span style='color: red;'>%3</span><br>"
                "%4"
                "</p>"
        ).arg(numberOfPops).arg(percentage).arg(SAMPLES_NumberOfUnsafePops()).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == "Compliance")
    {
        std::string status = "<b style='color: green;'>Status:</b> <span style='color: green;'>Above compliance threshold</span><br>";
        double percentage = ((double)numberOfComplianceBasedSamplesUnsafe/(double)SAMPLES_NumberOfComplianceSamples()) * (double)100;
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>Status:</b> <span style='color: red;'>Below compliance threshold</span><br>";
        }
        QString formattedText = QString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>Number of Compliance samples:</b> <span style='color: green;'>%1</span><br>"
                "<b style='color: grey;'>Safety rate:</b> <span style='color: grey;'>%2 p.c</span><br>"
                "<b style='color: red;'>Number of fail compliance samples:</b> <span style='color: red;'>%3</span><br>"
                "%4"
                "</p>"
        ).arg(SAMPLES_NumberOfComplianceSamples()).arg(percentage).arg(numberOfComplianceBasedSamplesUnsafe).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    summaryLabel->setStyleSheet("font-size: 16px; color: #7f8c8d; text-align: center;");
    summaryLabel->setWordWrap(true);


    QPushButton *detailsButton = new QPushButton("View Details");
    detailsButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #457B9D; color: white; border-radius: 5px; "
        "  padding: 8px 15px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1D3557; }"
        "QPushButton:pressed { background-color: #14213D; }"
    );

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

QWidget *DashboardWindow::createNavigationBar() {
    // Instead of creating two separate widgets, just create one navBar
    QWidget *navBar = new QWidget(this);
    QHBoxLayout *navLayout = new QHBoxLayout(navBar);
    navLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *appTitle = new QLabel("Water Quality Monitor");
    appTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #F1FAEE;");

    // Create buttons
    QPushButton *dashboardButton = new QPushButton("Dashboard");
    QPushButton *pollutantOverviewButton = new QPushButton("Pollutant Overview");
    QPushButton *popsButton = new QPushButton("POPs");
    QPushButton *fluorinatedCompoundsButton = new QPushButton("Fluorinated Compounds");
    QPushButton *complianceOverviewButton = new QPushButton("Compliance");

    // Assign object names for tab order
    dashboardButton->setObjectName("DashboardButton");
    pollutantOverviewButton->setObjectName("PollutantOverviewButton");
    popsButton->setObjectName("PopsButton");
    fluorinatedCompoundsButton->setObjectName("FluorinatedCompoundsButton");
    complianceOverviewButton->setObjectName("ComplianceOverviewButton");

    QString buttonStyle =
        "QPushButton {"
        "  background-color: #F1FAEE; color: #1D3557; border-radius: 5px; "
        "  padding: 10px 20px; font-size: 14px; font-weight: bold; border: 1px solid #DEE2E6; }"
        "QPushButton:hover { background-color: #A8DADC; }"
        "QPushButton:pressed { background-color: #457B9D; color: white; }"
        "QPushButton:focus { background-color: #A8DADC;outline: none;}";

    dashboardButton->setStyleSheet(buttonStyle);
    pollutantOverviewButton->setStyleSheet(buttonStyle);
    popsButton->setStyleSheet(buttonStyle);
    fluorinatedCompoundsButton->setStyleSheet(buttonStyle);
    complianceOverviewButton->setStyleSheet(buttonStyle);

    // Language Selector
    QComboBox *languageSelector = new QComboBox();
    languageSelector->addItems({"Arabic", "French", "English", "Mandarin"});
    languageSelector->setStyleSheet(
        "padding: 8px; "
        "font-size: 14px; "
        "color: #333; "
        "border: 1px solid #ccc; "
        "border-radius: 5px; "
        "background-color: white; "
        "margin-left: 20px;"
    );
    languageSelector->setObjectName("LanguageSelector");
    languageSelector->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    languageSelector->setMinimumWidth(150);
    languageSelector->setMinimumContentsLength(10); // adjusts width based on number of characters

    //Set Focus Policies
        dashboardButton->setFocusPolicy(Qt::StrongFocus);
        pollutantOverviewButton->setFocusPolicy(Qt::StrongFocus);
        popsButton->setFocusPolicy(Qt::StrongFocus);
        fluorinatedCompoundsButton->setFocusPolicy(Qt::StrongFocus);
        complianceOverviewButton->setFocusPolicy(Qt::StrongFocus);
        languageSelector->setFocusPolicy(Qt::StrongFocus);


    navLayout->addWidget(appTitle);
    navLayout->addSpacing(20);
    navLayout->addWidget(dashboardButton);
    navLayout->addWidget(pollutantOverviewButton);
    navLayout->addWidget(popsButton);
    navLayout->addWidget(fluorinatedCompoundsButton);
    navLayout->addWidget(complianceOverviewButton);
    navLayout->addWidget(languageSelector);
    navLayout->addStretch();

    // Set the desired background color directly on navBar
    navBar->setStyleSheet(
        "background-color: #1D3557; "
        "border-bottom: 2px solid #A8DADC;"
    );

    // Connect buttons to page switches
    connect(dashboardButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(0); });
    connect(pollutantOverviewButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(1); });
    connect(popsButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(2); });
    connect(fluorinatedCompoundsButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(3); });
    connect(complianceOverviewButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(4); });

    // Insert navigation widget into the main layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(centralWidget()->layout());
    mainLayout->insertWidget(0, navBar);

    // Add Footer
    QHBoxLayout *footerLayout = new QHBoxLayout();


    QLabel *userGuideLink = new QLabel("User Guide");
    userGuideLink->setStyleSheet(
    "QLabel { font-size: 14px; color: #F1FAEE; }"
    "QLabel a { color: white; text-decoration: none; }"
    "QLabel a:hover { text-decoration: underline; }"
);
    QLabel *helpResourcesLink = new QLabel("Help Resources");
    helpResourcesLink->setStyleSheet(
    "QLabel { font-size: 14px; color: #F1FAEE; }"
    "QLabel a { color: white; text-decoration: none; }"
    "QLabel a:hover { text-decoration: underline; }"
);

    QLabel *creditsLink = new QLabel("Credits");
    creditsLink->setStyleSheet(
    "QLabel { font-size: 14px; color: #F1FAEE; }"
    "QLabel a { color: white; text-decoration: none; }"
    "QLabel a:hover { text-decoration: underline; }"
);

    footerLayout->addWidget(userGuideLink);
    footerLayout->addSpacing(10);
    footerLayout->addWidget(helpResourcesLink);
    footerLayout->addSpacing(10);
    footerLayout->addWidget(creditsLink);
    footerLayout->addStretch();

    QWidget *footerWidget = new QWidget(this);
    footerWidget->setLayout(footerLayout);
    footerWidget->setStyleSheet(
        "background-color: #1D3557; "
        "border-bottom: 2px solid #A8DADC;"
    );
    footerLayout->setContentsMargins(20, 20, 20, 20);

    mainLayout->addWidget(footerWidget);

    return navBar;
}

void DashboardWindow::configureTabOrder() {
    QPushButton *dashboardButton = findChild<QPushButton *>("DashboardButton");
    QPushButton *pollutantOverviewButton = findChild<QPushButton *>("PollutantOverviewButton");
    QPushButton *popsButton = findChild<QPushButton *>("PopsButton");
    QPushButton *fluorinatedCompoundsButton = findChild<QPushButton *>("FluorinatedCompoundsButton");
    QPushButton *complianceOverviewButton = findChild<QPushButton *>("ComplianceOverviewButton");
    QComboBox *languageSelector = findChild<QComboBox *>("LanguageSelector");
    QPushButton *applyFilterButton = findChild<QPushButton *>("Apply Filters");

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

    DashboardWindow window;
    window.resize(1200, 800);
    window.setWindowTitle("Dashboard");
    window.show();

    return app.exec();
}

