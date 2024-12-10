#ifndef WATER_QUALITY_SAMPLE_H
#define WATER_QUALITY_SAMPLE_H

#include <string>
#include <vector>
#include "csv.hpp" // Include the csv-parser library

// Class to represent a water quality sample
class WaterQualitySample {
public:
    std::string id;
    std::string samplingPoint;
    std::string samplingPointNotation;
    std::string samplingPointLabel;
    std::string sampleDateTime;
    std::string determinandLabel;
    std::string determinandDefinition;
    std::string determinandNotation;
    std::string resultQualifierNotation;
    std::string result;
    std::string codedResultInterpretation;
    std::string determinandUnitLabel;
    std::string sampledMaterialTypeLabel;
    std::string isComplianceSample;
    std::string purposeLabel;
    std::string samplingPointEasting;
    std::string samplingPointNorthing;

    // Function to print the sample's details
    void print() const;
};

// Function to load the CSV file into a vector of WaterQualitySample objects
std::vector<WaterQualitySample> DB_GetAllEntries(const std::string& filePath);

// Function to debug the csv
int DebugData();
int MinimumResultRow(std::vector<WaterQualitySample> data, std::string determinand);
int FindNumberOfUniqueEntries(std::vector<WaterQualitySample> dataSetVector);


#endif // WATER_QUALITY_SAMPLE_H
