// COMP2811 Coursework 1 sample solution: Quake class

#pragma once

#include <string>
#include <iostream>

static const double MIN_LATITUDE = -90.0;
static const double MAX_LATITUDE = 90.0;
static const double MIN_LONGITUDE = -180.0;
static const double MAX_LONGITUDE = 180.0;

class Quake
{
  public:
    Quake(const std::string&, double, double, double, double);
    std::string getTime() const { return time; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    double getDepth() const { return depth; }
    double getMagnitude() const { return magnitude; }

  private:
    std::string time;
    double latitude;
    double longitude;
    double depth;
    double magnitude;
};

std::ostream& operator << (std::ostream&, const Quake&);
