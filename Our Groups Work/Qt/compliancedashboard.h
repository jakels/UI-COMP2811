#ifndef COMPLIANCEDASHBOARD_H
#define COMPLIANCEDASHBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QComboBox>
#include <vector>
#include <string>

struct MockSample {
    std::string location;
    std::string pollutant;
    std::string status;
};

class ComplianceDashboard : public QWidget
{
    Q_OBJECT

public:
    ComplianceDashboard(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override; // Handle resizing

private slots:
    void applyFilters();
    void showDetails(const MockSample &sample);

// protected:
//     void resizeEvent(QResizeEvent *event) override;

private:
    void displaySummaryCards(const std::vector<MockSample> &samples);
    int calculateColumns(); // Determine the number of columns dynamically

    QScrollArea *scrollArea;         // To make cards scrollable
    QWidget *scrollContent;          // Content widget inside the scroll area
    QGridLayout *cardContainer;      // Grid layout to hold the cards
    QComboBox *locationFilter;
    QComboBox *pollutantFilter;
    QComboBox *statusFilter;

    std::vector<MockSample> mockData;
    int cardWidth;  // Dynamically adjustable card width
    int cardHeight; // Dynamically adjustable card height
};

#endif // COMPLIANCEDASHBOARD_H

