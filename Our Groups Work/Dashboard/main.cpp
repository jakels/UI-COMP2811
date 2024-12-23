#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QDebug>

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    DashboardWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        mainLayout->addLayout(createHeader());
         mainLayout->addLayout(createFilters());
        mainLayout->addWidget(createContent());
        mainLayout->addLayout(createFooter());

        resize(800, 700);
        setWindowTitle("Water Quality Monitor");
        setStyleSheet("background-color: #f5f5f5;"); // Light gray background
    }

private:
    // Function to create the header
    QHBoxLayout *createHeader() {
        QHBoxLayout *headerLayout = new QHBoxLayout();

        QLabel *appTitle = new QLabel("Water Quality Monitor");
        appTitle->setStyleSheet("font-size: 26px; font-weight: bold; color: #333; padding: 10px;");



        QString buttonStyle = "QPushButton {"
                              "  background-color: #3498db; color: white; border-radius: 15px; "
                              "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                              "QPushButton:hover { background-color: #2980b9; }"
                              "QPushButton:pressed { background-color: #1c5a7a; }";

        QPushButton *navButton1 = createNavButton("Pollutant Overview", buttonStyle);
        QPushButton *navButton2 = createNavButton("POPs", buttonStyle);
        QPushButton *navButton3 = createNavButton("Fluorinated Compounds", buttonStyle);
        QPushButton *navButton4 = createNavButton("Compliance Overview", buttonStyle);
        QPushButton *navButton5 = createNavButton("Data Visualtion", buttonStyle);

        headerLayout->addWidget(appTitle);
        headerLayout->addWidget(navButton1);
        headerLayout->addWidget(navButton2);
        headerLayout->addWidget(navButton3);
        headerLayout->addWidget(navButton4);
        headerLayout->addWidget(navButton5);

        return headerLayout;
    }

    // Function to create the filters
    QHBoxLayout *createFilters() {
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
                                         "QPushButton:pressed { background-color: #1c5a7a }");

        connect(applyFilterButton, &QPushButton::clicked, this, [=]() {
            QString selectedTime = timeFilter->currentText();
            QString selectedRegion = regionFilter->currentText();
            qDebug() << "Filters applied: Time -" << selectedTime << ", Region -" << selectedRegion;
            // Logic for filter can be added here
        });

        QComboBox *languageSelector = new QComboBox();
        languageSelector->addItems({"English", "Arabic", "Mandarin"});
        languageSelector->setStyleSheet("padding: 5px; font-size: 14px; color: #333");

        filterLayout->addWidget(timeLabel);
        filterLayout->addWidget(timeFilter);
        filterLayout->addSpacing(20);
        filterLayout->addWidget(regionLabel);
        filterLayout->addWidget(regionFilter);
        filterLayout->addSpacing(20);
        filterLayout->addWidget(applyFilterButton);
        filterLayout->addStretch();
        filterLayout->addSpacing(100);
        filterLayout->addWidget(languageSelector);

        return filterLayout;
    }

    // Function to create a navigation button
    QPushButton *createNavButton(const QString &text, const QString &style) {
        QPushButton *button = new QPushButton(text);
        button->setStyleSheet(style);
        return button;
    }

    // Function to create the scrollable content
    QScrollArea *createContent() {
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);

        QWidget *contentWidget = new QWidget();
        QGridLayout *gridLayout = new QGridLayout(contentWidget);
        gridLayout->setSpacing(20); // Add spacing between cards

        QStringList cardTitles = {"Pollutant Overview", "POPs", "Fluorinated Compounds", "Compliance"};
        QString cardStyle = "QFrame {"
                            "  border: 1px solid rgba(0, 0, 0, 0.1); border-radius: 20px; background-color: #ffffff; "
                            "  box-shadow: 0 10px 15px rgba(0, 0, 0, 0.1); padding: 20px; transition: transform 0.3s ease, box-shadow 0.3s ease;}"
                            "QFrame:hover {"
                            "  transform: scale(1.02); box-shadow: 0 15px 20px rgba(0, 0, 0, 0.2); }";

        for (int i = 0; i < cardTitles.size(); ++i) {
            QFrame *card = createCard(cardTitles[i], cardStyle);
            gridLayout->addWidget(card, i / 2, i % 2, Qt::AlignCenter); // Center-align cards
        }

        contentWidget->setLayout(gridLayout);
        scrollArea->setWidget(contentWidget);

        return scrollArea;
    }

    // Function to create a single modern card
    QFrame *createCard(const QString &title, const QString &style) {
        QFrame *card = new QFrame();
        card->setStyleSheet(style);
        // Set fixed size for the card
        card->setFixedSize(400, 300); // Adjust size as needed (width x height)

        QVBoxLayout *cardLayout = new QVBoxLayout(card);

        // Icon (top)
        // QLabel *iconLabel = new QLabel();
        // iconLabel->setPixmap(QPixmap(":/icons/sample-icon.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        // iconLabel->setAlignment(Qt::AlignCenter);

        // Title
        QLabel *titleLabel = new QLabel(title);
        titleLabel->setStyleSheet("font-size: 20px; font-weight: 600; color: #34495e; text-align: center; margin-top: 10px;");

        // Summary Text
        QLabel *summaryLabel = new QLabel("Summary Information...");
        summaryLabel->setStyleSheet("font-size: 14px; color: #7f8c8d; text-align: center; margin: 15px 0;");
        summaryLabel->setWordWrap(true);

        // Action Button
        QPushButton *detailsButton = new QPushButton("View Details");
        detailsButton->setStyleSheet("QPushButton {"
                                     "  background-color: #3498db; color: white; border-radius: 15px; "
                                     "  padding: 10px 20px; font-size: 14px; font-weight: bold; }"
                                     "QPushButton:hover { background-color: #2980b9; }"
                                     "QPushButton:pressed { background-color: #1c5a7a; }");

        connect(detailsButton, &QPushButton::clicked, this, [=]() {
            qDebug("Navigating to details for %s", qPrintable(title));
        });

        // Add Components to Card Layout
      //  cardLayout->addWidget(iconLabel);        // Icon
        cardLayout->addWidget(titleLabel);       // Title
        cardLayout->addWidget(summaryLabel);     // Summary Text
        cardLayout->addWidget(detailsButton);    // Button
        cardLayout->setAlignment(Qt::AlignCenter);
        cardLayout->setSpacing(10);              // Spacing between elements

        return card;
    }


    // Function to create the footer
    QHBoxLayout *createFooter() {
        QHBoxLayout *footerLayout = new QHBoxLayout();

        QLabel *helpLabel = new QLabel("<a href='https://help.website' style='color: #0078d7;'>Help</a> | "
                                       "<a href='https://credits.website' style='color: #0078d7;'>Credits</a>");
        helpLabel->setOpenExternalLinks(true);

        footerLayout->addStretch();
        footerLayout->addWidget(helpLabel);

        return footerLayout;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DashboardWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
