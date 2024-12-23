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
    std::vector<std::string> headers;
    std::vector<std::string> rawRow;
    double safeMax;
    double cautionMax;

    // Function to print the sample's details
    void print() const;
};

// Function to load the CSV file into a vector of WaterQualitySample objects
std::vector<WaterQualitySample> DB_GetAllEntries(const std::string& filePath);
std::vector<WaterQualitySample> DB_GetCachedEntries();
std::vector<WaterQualitySample> DB_GetEntriesByLocation(const std::string& locationName);
std::vector<WaterQualitySample> DB_GetEntriesByChemical(const std::string& chemical);
std::vector<WaterQualitySample> DB_GetEntriesByChemicalAndLocation(const std::string& chemical, const std::string& location);
int DB_Initialise();
std::vector<WaterQualitySample> DB_GetCachedEntriesSubset(int count);
std::vector<WaterQualitySample> OrderSamplesByDate(std::vector<WaterQualitySample> input);
int NumberOfSafeEntries(std::vector<WaterQualitySample> samples);
int NumberOfCautionEntries(std::vector<WaterQualitySample> samples);
int NumberOfDangerEntries(std::vector<WaterQualitySample> samples);
int NumberOfEntriesWithLevelType(std::vector<WaterQualitySample> samples, std::string level);
std::string SAMPLE_GetSafetyLevel(WaterQualitySample sample);
std::vector<std::string> DB_UniqueLocations();
std::vector<std::string> DB_UniqueChemicals();
bool SAMPLE_CheckSampleThreshold(WaterQualitySample sample, std::string thresholdType);
int SAMPLES_NumberOfSafeEntries();
int SAMPLES_NumberOfCautionEntries();
int SAMPLES_NumberOfDangerEntries();
std::vector<WaterQualitySample> DB_GetEntriesAllFluro();
int SAMPLES_NumberOfSafeFluroEntries();
int SAMPLES_NumberOfFluroEntries();
int DB_GetEntriesByChemicalAndLeastResult(std::vector<WaterQualitySample> data, std::string determinand);
int SAMPLES_NumberOfComplianceSamples();
int SAMPLES_NumberOfUnsafePops();
bool SAMPLE_WithinLastMonth(WaterQualitySample sample);
bool SAMPLE_WithinLastYear(WaterQualitySample sample);

extern int numberOfComplianceBasedSamplesUnsafe;
extern int numberOfPops;

#endif // WATER_QUALITY_SAMPLE_H
