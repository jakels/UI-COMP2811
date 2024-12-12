/* DatasetInterface.cpp
    This file is the interface for the water dataset, it has functions for initialising the DB and querying it.
    You cannot use the database without first initialising it with DB_Initialise
*/

// Imports
#include "DatasetInterface.h"
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "sqlite3.h"
#include "Utils.h"
#include <cmath>
#include "fluorinatedpage.h"
#include "constants.h"

// Global variables
std::string CSVfilepath = "Water.csv";
std::vector<WaterQualitySample> cachedDataset;
int cachedDatasetSize = 0;
bool initialised = false;
std::vector<std::string> uniqueChemicals;
std::vector<std::string> uniqueLocations;
std::vector<WaterQualitySample> fluroSamples;
int numberOfComplianceBasedSamples = 0;
int numberOfComplianceBasedSamplesUnsafe = 0;
int fluroEntriesTotal = 0;
int safeFluroEntriesTotal = 0;
int safeEntriesTotal = 0;
int cautionEntriesTotal = 0;
int dangerEntriesTotal = 0;
int numberOfUnsafePOPs = 0;
int numberOfPops = 0;

// Print utility function
void WaterQualitySample::print() const
{
    std::cout << "Sample ID: " << id
              << ", Sampling Point: " << samplingPoint
              << ", Result: " << result
              << ", Date: " << sampleDateTime
              << std::endl;
}

std::vector<WaterQualitySample> DB_GetCachedEntries()
{
    Log("Getting cached entries");
    return cachedDataset;
}

std::vector<WaterQualitySample> DB_GetCachedEntriesSubset(int count)
{
    Log("Getting cached entries subset of size " + std::to_string(count));
    std::vector<WaterQualitySample> samples;
    for (int i = 0; i < count; i++)
    {
        samples.push_back(cachedDataset[i]);
    }
    return samples;
}


// Custom comparator to sort by the 'result' field
bool compareByResult(const WaterQualitySample& a, const WaterQualitySample& b) {
    return a.sampleDateTime > b.sampleDateTime;
}

std::vector<WaterQualitySample> OrderSamplesByDate(std::vector<WaterQualitySample> input)
{
    Log("Ordering vector of size " + std::to_string(input.size()));

    std::sort(input.begin(), input.end(), compareByResult);

    return input;
}

// Function to load the CSV file into a vector of WaterQualitySample objects
std::vector<WaterQualitySample> DB_GetAllEntries(const std::string& filePath)
{
    std::vector<WaterQualitySample> samples;
    csv::CSVReader reader(filePath);

    int maxLoad = 145000;
    int i = 0;
    int counter = 0;

    // Iterate through the rows of the CSV
    for (auto& row : reader)
    {
        if(i==maxLoad){break;}
        if(counter == 25000)
        {
            Log("Loaded " + std::to_string(i));
            counter = 0;
        }
        WaterQualitySample sample;
        sample.id = row["@id"].get<>();
        sample.samplingPoint = row["sample.samplingPoint"].get<>();
        sample.samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        sample.samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        sample.sampleDateTime = row["sample.sampleDateTime"].get<>();
        sample.determinandLabel = row["determinand.label"].get<>();
        sample.determinandDefinition = row["determinand.definition"].get<>();
        sample.determinandNotation = row["determinand.notation"].get<>();
        sample.resultQualifierNotation = row["resultQualifier.notation"].get<>();
        sample.result = row["result"].get<>();
        sample.codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
        sample.determinandUnitLabel = row["determinand.unit.label"].get<>();
        sample.sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        sample.isComplianceSample = row["sample.isComplianceSample"].get<>();
        sample.purposeLabel = row["sample.purpose.label"].get<>();
        sample.samplingPointEasting = row["sample.samplingPoint.easting"].get<>();
        sample.samplingPointNorthing = row["sample.samplingPoint.northing"].get<>();
        sample.headers = reader.get_col_names();
        sample.rawRow = row;
        sample.safeMax = 0.08;
        sample.cautionMax = 0.9;

        if(!(std::find(uniqueChemicals.begin(), uniqueChemicals.end(), sample.determinandLabel) != uniqueChemicals.end()))
        {
            uniqueChemicals.push_back(sample.determinandLabel);
        }

        if(!(std::find(uniqueLocations.begin(), uniqueLocations.end(), sample.samplingPointLabel) != uniqueLocations.end()))
        {
            uniqueLocations.push_back(sample.samplingPointLabel);
        }

        std::string safetyLevel = SAMPLE_GetSafetyLevel(sample);
        if(safetyLevel == "Safe")
        {
            safeEntriesTotal++;
        }
        if(safetyLevel == "Caution")
        {
            cautionEntriesTotal++;
        }
        if(safetyLevel == "Danger")
        {
            dangerEntriesTotal++;
        }

        if(sample.isComplianceSample == "true")
        {
            numberOfComplianceBasedSamples++;
            if(SAMPLE_GetSafetyLevel(sample) == "Danger")
            {
                numberOfComplianceBasedSamplesUnsafe++;
            }
        }

        if(popPollutants.contains(sample.determinandLabel.c_str()))
        {
            numberOfPops++;
            if(SAMPLE_GetSafetyLevel(sample) == "Danger")
            {
                numberOfUnsafePOPs++;
            }
        }

        if(fluroPollutantTypes.contains(sample.determinandLabel.c_str()))
        {
            fluroEntriesTotal++;
            fluroSamples.push_back(sample);
            if(SAMPLE_GetSafetyLevel(sample) == "Safe")
            {
                safeFluroEntriesTotal++;
            }
        }

        samples.push_back(sample);
        i++;
        counter++;
    }
    Log("Finished loading samples at " + std::to_string(i) + ", " + std::to_string(uniqueChemicals.size()) + " unique uniqueChemicals.");

    return samples;
}

// Initialise the database by loading all the CSV data into a SQL db
int DB_Initialise()
{
    Log("Initialising database...");
    cachedDataset = DB_GetAllEntries(GetAbsoloutePathFromRelative(CSVfilepath));
    cachedDatasetSize = cachedDataset.size();
    initialised = true;

    if(cachedDataset.size() == 0)
    {
        Log("All entries size was 0, returning error code.");
        return 1;
    }

    Log("Initialised database.");
    return 0;
}

std::vector<WaterQualitySample> DB_GetEntriesByLocation(const std::string& specificLocation)
{
    Log("Getting all entries with location of " + specificLocation);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.samplingPointLabel != specificLocation)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}


std::vector<WaterQualitySample> DB_GetEntriesByChemical(const std::string& chemical)
{
    Log("Getting all entries with chemical of " + chemical);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.determinandLabel != chemical)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}

std::vector<WaterQualitySample> DB_GetEntriesByChemicalAndLocation(const std::string& chemical, const std::string& location)
{
    Log("Getting all entries with location of " + location + " and chemical of " + chemical);
    std::vector<WaterQualitySample> samples;

    // Iterate through the rows of the CSV
    for (int i = 0; i < cachedDatasetSize; i++)
    {
        WaterQualitySample sample = cachedDataset[i];

        if(sample.determinandLabel != chemical || sample.samplingPointLabel != location)
        {
            continue;
        }

        samples.push_back(sample);
    }

    Log("Returned a vector with " + std::to_string( samples.size() ) + " elements.");
    return samples;
}

int DB_GetEntriesByChemicalAndLeastResult(std::vector<WaterQualitySample> dataSetVector, std::string determinand)
{
    double minimumResult = 999.999;
    int minimumRow = 0;
    for (int i = 0; i < std::min(dataSetVector.size(), static_cast<size_t>(140733)); ++i) {
        WaterQualitySample current = dataSetVector[i];
        if (current.determinandLabel != determinand) {
            continue;
        }
        if (stod(current.result) < minimumResult) {
            minimumRow = i;
            minimumResult = stod(current.result);
        }
    }

    dataSetVector[minimumRow].print();
    return minimumRow;
}

bool SAMPLE_CheckSampleThreshold(WaterQualitySample sample, std::string thresholdType)
{
    if(thresholdType == "Safe")
    {
        return atof(sample.result.c_str()) < sample.safeMax;
    }
    else if(thresholdType == "Caution")
    {
        return atof(sample.result.c_str()) < sample.cautionMax;
    }
    else if (thresholdType == "Danger")
    {
        return atof(sample.result.c_str()) > sample.cautionMax;
    }
}

int SAMPLES_NumberOfEntriesWithLevelType(std::vector<WaterQualitySample> samples, std::string level)
{
    int total = 0;
    for(int i = 0; i < samples.size(); i++)
    {
        if(SAMPLE_CheckSampleThreshold(samples[i], level))
        {
            total++;
        }
    }

    return total;
}

int SAMPLES_NumberOfSafeEntries()
{
    return safeEntriesTotal;
}

int SAMPLES_NumberOfCautionEntries()
{
    return cautionEntriesTotal;
}

int SAMPLES_NumberOfDangerEntries()
{
    return dangerEntriesTotal;
}

std::string SAMPLE_GetSafetyLevel(WaterQualitySample sample)
{
    if(SAMPLE_CheckSampleThreshold(sample, "Safe"))
    {
        return "Safe";
    }

    if(SAMPLE_CheckSampleThreshold(sample, "Caution"))
    {
        return "Caution";
    }

    if(SAMPLE_CheckSampleThreshold(sample, "Danger"))
    {
        return "Danger";
    }

    return "Unknown";
}

std::vector<std::string> DB_UniqueLocations()
{
    Log("Getting all unique locations..");
    return uniqueLocations;
}

std::vector<std::string> DB_UniqueChemicals()
{
    Log("Getting all unique chemicals..");
    return uniqueChemicals;
}

std::vector<WaterQualitySample> DB_GetEntriesAllFluro()
{
    return fluroSamples;
}

int SAMPLES_NumberOfSafeFluroEntries()
{
    return safeFluroEntriesTotal;
}

int SAMPLES_NumberOfFluroEntries()
{
    return fluroEntriesTotal;
}

int SAMPLES_NumberOfComplianceSamples()
{
    return numberOfComplianceBasedSamples;
}

int SAMPLES_NumberOfUnsafePops()
{
    return numberOfUnsafePOPs;
}

bool SAMPLE_WithinLastMonth(WaterQualitySample sample)
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string currentTime = std::ctime(&now);
    std::string lastMonth = std::to_string(std::stoi(currentTime.substr(4, 2)) - 1);
    std::string currentMonth = currentTime.substr(4, 2);
;
    if(sample.sampleDateTime.substr(5, 2) == currentMonth || sample.sampleDateTime.substr(5, 2) == lastMonth)
    {
        return true;
    }
    return false;
}

bool SAMPLE_WithinLastYear(WaterQualitySample sample)
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string currentTime = std::ctime(&now);
    std::string lastYear = std::to_string(std::stoi(currentTime.substr(20, 4)) - 1);
    std::string currentYear = currentTime.substr(20, 4);

    if(sample.sampleDateTime.substr(0, 4) == currentYear || sample.sampleDateTime.substr(0, 4) == lastYear)
    {
        return true;
    }
    return false;
}