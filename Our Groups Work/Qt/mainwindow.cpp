#include "mainwindow.h"
#include "translation_manager.h"

DashboardWindow::DashboardWindow(QWidget *parent)
        : QMainWindow(parent) {

    QMessageBox::information(this, t("Welcome").c_str(), t("The database will take a minute to load. Press OK to start.").c_str());
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

    // Update the window title when switching pages
    connect(stackedWidget, &QStackedWidget::currentChanged, this, [=](int index) {
        switch (index) {
            case 0: setWindowTitle(t("Dashboard").c_str()); break;
            case 1: setWindowTitle(t("Pollutant Overview").c_str()); break;
            case 2: setWindowTitle(t("POPs").c_str()); break;
            case 3: setWindowTitle(t("Fluorinated Compounds").c_str()); break;
            case 4: setWindowTitle(t("Compliance Overview").c_str()); break;
            default: setWindowTitle(t("Water Quality Monitor").c_str()); break;
        }
    });

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);

    createNavigationBar();
    configureTabOrder();
}

QWidget *DashboardWindow::createDashboardPage() {
    QWidget *dashboardPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(dashboardPage);

    // Add filters section and main content area
    layout->addLayout(createFilters());
    layout->addWidget(createContent(), 1);
    layout->addStretch();

    return dashboardPage;
}

QHBoxLayout *DashboardWindow::createFilters() {
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *timeLabel = new QLabel(t("Filter by Time:").c_str());
    timeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({t("All Time").c_str(), t("Last Month").c_str(), t("Last Year").c_str()});
    timeFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    timeFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    timeFilter->setMinimumWidth(150);
    timeFilter->setMinimumContentsLength(10);

    QLabel *regionLabel = new QLabel(t("Filter by Region:").c_str());
    regionLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1D3557;");
    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({t("All Regions").c_str()});
    for(auto region : DB_UniqueLocations())
    {
        regionFilter->addItem(QString::fromStdString(region));
    }
    regionFilter->setStyleSheet("padding: 8px; font-size: 14px; border: 1px solid #457B9D; border-radius: 5px; color: #333;");
    regionFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    regionFilter->setMinimumWidth(150);
    regionFilter->setMinimumContentsLength(10); // adjusts width based on number of characters

    QPushButton *applyFilterButton = new QPushButton(t("Apply Filters").c_str());
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

        qDebug() << t("Filters applied: Time -").c_str() << selectedTime << t(", Region -").c_str() << selectedRegion;
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

    QStringList cardTitles = {t("Pollutant Overview").c_str(), t("POPs").c_str(), t("Fluorinated Compounds").c_str(), t("Compliance").c_str()};
    QString cardStyle = "QFrame {"
                        "  border: 1px solid #DEE2E6; border-radius: 10px; background-color: #F8F9FA; "
                        "  box-shadow: 0 5px 10px rgba(0, 0, 0, 0.1); padding: 15px; }"
                        "QFrame:hover {"
                        "  transform: scale(1.05); box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2); }";

    // Add cards for each category
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

    //  conditions to show relevant info per card
    QLabel *summaryLabel = new QLabel();
    if (title.toStdString() == t("Pollutant Overview")) {
        QString formattedText = QString::fromStdString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>" + t("Number of Safe:") + "</b> <span style='color: green;'>%1</span><br>"
                                                                             "<b style='color: orange;'>" + t("Number of Caution:") + "</b> <span style='color: orange;'>%2</span><br>"
                                                                                                                                              "<b style='color: red;'>" + t("Number of Danger:") + "</b> <span style='color: red;'>%3</span>"
                                                                                                                                                                                                           "</p>"
        ).arg(numberOfSafe).arg(numberOfCaution).arg(numberOfDanger);
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == t("Fluorinated Compounds"))
    {
        std::string status = "<b style='color: green;'>" + std::string(t("Status:").c_str()) + "</b> <span style='color: green;'>" + t("Above compliance threshold").c_str() + "</span><br>";
        double percentage = ((double)numberOfSafeFluroEntries/(double)SAMPLES_NumberOfFluroEntries()) * (double)100;
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>" + std::string(t("Status:").c_str()) + "</b> <span style='color: red;'>" + t("Below compliance threshold").c_str() + "</span><br>";
        }
        QString formattedText = QString::fromStdString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>" + t("Number of Safe Fluro-Samples:") + "</b> <span style='color: green;'>%1</span><br>"
                                                                                           "<b style='color: grey;'>" + t("Safety rate:") + "</b> <span style='color: grey;'>%2 " + t("p.c") + "</span><br>"
                                                                                                                                                                                                               "%3"
                                                                                                                                                                                                               "</p>"
        ).arg(numberOfSafeFluroEntries).arg(percentage).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    if(title == t("POPs"))
    {
        std::string status = "<b style='color: green;'>" + std::string(t("Status:").c_str()) + "</b> <span style='color: green;'>" + t("Above compliance threshold").c_str() + "</span><br>";
        double percentage = 100 - (((double)SAMPLES_NumberOfUnsafePops()/(double)std::max(numberOfPops, 1)) * (double)100);
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>" + std::string(t("Status:").c_str()) + "</b> <span style='color: red;'>" + t("Below compliance threshold").c_str() + "</span><br>";
        }
        QString formattedText = QString::fromStdString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>" + t("Number of POP samples:") + "</b> <span style='color: green;'>%1</span><br>"
                                                                                    "<b style='color: grey;'>" + t("Safety rate:") + "</b> <span style='color: grey;'>%2 " + t("p.c") + "</span><br>"
                                                                                                                                                                                                        "<b style='color: red;'>" + t("Number of fail POP samples:") + "</b> <span style='color: red;'>%3</span><br>"
                                                                                                                                                                                                                                                                               "%4"
                                                                                                                                                                                                                                                                               "</p>"
        ).arg(numberOfPops).arg(percentage).arg(SAMPLES_NumberOfUnsafePops()).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    if(title.toStdString() == t("Compliance"))
    {
        std::string status = "<b style='color: green;'>" + std::string(t("Status").c_str()) + ":</b> <span style='color: green;'>" + t("Above compliance threshold").c_str() + "</span><br>";
        double percentage = ((double)numberOfComplianceBasedSamplesUnsafe/(double)SAMPLES_NumberOfComplianceSamples()) * (double)100;
        if(percentage < (double)90)
        {
            status = "<b style='color: red;'>" + std::string(t("Status:").c_str()) + "</b> <span style='color: red;'>" + t("Below compliance threshold").c_str() + "</span><br>";
        }
        QString formattedText = QString::fromStdString(
                "<p style='font-size: 16px; color: #7f8c8d; text-align: center;'>"
                "<b style='color: green;'>" + t("Number of Compliance samples:") + "</b> <span style='color: green;'>%1</span><br>"
                                                                                           "<b style='color: grey;'>" + t("Safety rate:") + "</b> <span style='color: grey;'>%2 " + t("p.c") + "</span><br>"
                                                                                                                                                                                                               "<b style='color: red;'>" + t("Number of fail compliance samples:") + "</b> <span style='color: red;'>%3</span><br>"
                                                                                                                                                                                                                                                                                             "%4"
                                                                                                                                                                                                                                                                                             "</p>"
        ).arg(SAMPLES_NumberOfComplianceSamples()).arg(percentage).arg(numberOfComplianceBasedSamplesUnsafe).arg(status.c_str());
        summaryLabel->setText(formattedText);
    }
    summaryLabel->setStyleSheet("font-size: 16px; color: #7f8c8d; text-align: center;");
    summaryLabel->setWordWrap(true);


    QPushButton *detailsButton = new QPushButton(t("View Details").c_str());
    detailsButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #457B9D; color: white; border-radius: 5px; "
            "  padding: 8px 15px; font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { background-color: #1D3557; }"
            "QPushButton:pressed { background-color: #14213D; }"
    );

    // Connect the "View Details" button to the relevant page
    connect(detailsButton, &QPushButton::clicked, this, [this, title]() {
        if (title == t("Pollutant Overview").c_str()) {
            stackedWidget->setCurrentIndex(1); // Navigate to Pollutant Overview page
        } else if (title == t("POPs").c_str()) {
            stackedWidget->setCurrentIndex(2);
        } else if (title == t("Fluorinated Compounds").c_str()) {
            stackedWidget->setCurrentIndex(3);
        }else if (title == t("Compliance").c_str()) {
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

QStringList GetLanguages()
{
    QStringList languages;
    if(language == "English")
    {
        languages.append("English");
        languages.append("Arabic");
        languages.append("German");
    }
    if(language == "Arabic")
    {
        languages.append("Arabic");
        languages.append("English");
        languages.append("German");
    }
    if(language == "German")
    {
        languages.append("German");
        languages.append("English");
        languages.append("Arabic");
    }
    return languages;
}

QWidget *DashboardWindow::createNavigationBar() {

    QWidget *navBar = new QWidget(this);
    QHBoxLayout *navLayout = new QHBoxLayout(navBar);
    navLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *appTitle = new QLabel(t("Water Quality Monitor").c_str());
    appTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #F1FAEE;");

    // Create buttons
    QPushButton *dashboardButton = new QPushButton(t("Dashboard").c_str());
    QPushButton *pollutantOverviewButton = new QPushButton(t("Pollutant Overview").c_str());
    QPushButton *popsButton = new QPushButton(t("POPs").c_str());
    QPushButton *fluorinatedCompoundsButton = new QPushButton(t("Fluorinated Compounds").c_str());
    QPushButton *complianceOverviewButton = new QPushButton(t("Compliance").c_str());

    // Assign object names for tab order
    dashboardButton->setObjectName("DashboardButton");
    pollutantOverviewButton->setObjectName("PollutantOverviewButton");
    popsButton->setObjectName(t("PopsButton").c_str());
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
    languageSelector->addItems(GetLanguages());
    languageSelector->setStyleSheet(
            "padding: 8px; "
            "font-size: 14px; "
            "color: #333; "
            "border: 1px solid #ccc; "
            "border-radius: 5px; "
            "background-color: white; "
            "margin-left: 20px;"
    );
    languageSelector->setObjectName(t("LanguageSelector").c_str());
    languageSelector->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    languageSelector->setMinimumWidth(150);
    languageSelector->setMinimumContentsLength(10); // adjusts width based on number of characters
    connect(languageSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DashboardWindow::onLanguageChanged);

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


    QLabel *userGuideLink = new QLabel(t("User Guide").c_str());
    userGuideLink->setStyleSheet(
            "QLabel { font-size: 14px; color: #F1FAEE; }"
            "QLabel a { color: white; text-decoration: none; }"
            "QLabel a:hover { text-decoration: underline; }"
    );
    QLabel *helpResourcesLink = new QLabel(t("Help Resources").c_str());
    helpResourcesLink->setStyleSheet(
            "QLabel { font-size: 14px; color: #F1FAEE; }"
            "QLabel a { color: white; text-decoration: none; }"
            "QLabel a:hover { text-decoration: underline; }"
    );

    QLabel *creditsLink = new QLabel(t("Credits").c_str());
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
    QPushButton *dashboardButton = findChild<QPushButton *>(t("DashboardButton").c_str());
    QPushButton *pollutantOverviewButton = findChild<QPushButton *>(t("PollutantOverviewButton").c_str());
    QPushButton *popsButton = findChild<QPushButton *>(t("PopsButton").c_str());
    QPushButton *fluorinatedCompoundsButton = findChild<QPushButton *>(t("FluorinatedCompoundsButton").c_str());
    QPushButton *complianceOverviewButton = findChild<QPushButton *>(t("ComplianceOverviewButton").c_str());
    QComboBox *languageSelector = findChild<QComboBox *>(t("LanguageSelector").c_str());
    QPushButton *applyFilterButton = findChild<QPushButton *>(t("Apply Filters").c_str());

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
        qDebug() << t("One or more widgets not found for setting tab order.").c_str();
    }
}

void DashboardWindow::keyPressEvent(QKeyEvent *event) {
    // Keyboard navigation
    switch (event->key()) {
        case Qt::Key_Left:
            qDebug() << t("Left arrow pressed - navigating backward").c_str();
            focusPreviousChild(); // Navigate to the previous widget
            break;
        case Qt::Key_Right:
            qDebug() << t("Right arrow pressed - navigating forward").c_str();
            focusNextChild(); // Navigate to the next widget
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            qDebug() << t("Enter or Return pressed").c_str();
            if (QPushButton *button = qobject_cast<QPushButton *>(QApplication::focusWidget())) {
                button->click(); // Trigger focused button action
            }
            break;
        case Qt::Key_Escape:
            qDebug() << t("Escape pressed - closing application").c_str();
            close(); // Exit the application
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void DashboardWindow::restartApplication() {
    // Store necessary state if needed

    // Hide the current window
    this->hide();

    // Create a new instance of DashboardWindow
    DashboardWindow *newWindow = new DashboardWindow();
    newWindow->show();

    // Close the current window
    this->close();
}

void DashboardWindow::onLanguageChanged(int index) {
    QComboBox *languageSelector = findChild<QComboBox *>(t("LanguageSelector").c_str());
    if(!languageSelector) return;

    QStringList langs = GetLanguages();

    // Update the language in the TranslationManager
    language = langs[index].toStdString();

    // Restart the main window to apply language changes
    restartApplication();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Highlight focused widget with a border
    qApp->setStyleSheet("QWidget:focus { border: 2px solid #3498db; outline: none; }");

    DashboardWindow window;
    window.resize(1200, 800);
    window.setWindowTitle(t("Dashboard").c_str());
    window.show();

    return app.exec();
}
