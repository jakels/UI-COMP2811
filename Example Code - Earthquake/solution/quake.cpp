// COMP2811 Coursework 1 sample solution: Quake class

#include <stdexcept>
#include <sstream>
#include "quake.hpp"

using namespace std;


Quake::Quake(const string& tm, double lat, double lon, double dep, double mag):
  time(tm), latitude(lat), longitude(lon), depth(dep), magnitude(mag)
{
  ostringstream error;

  if (latitude < MIN_LATITUDE or latitude > MAX_LATITUDE) {
    error << "Invalid latitude: " << latitude;
    throw out_of_range(error.str());
  }

  if (longitude < MIN_LONGITUDE or longitude > MAX_LONGITUDE) {
    error << "Invalid longitude: " << longitude;
    throw out_of_range(error.str());
  }

  if (depth < 0.0) {
    error << "Invalid depth: " << depth;
    throw out_of_range(error.str());
  }

  if (magnitude < 0.0) {
    error << "Invalid magnitude: " << magnitude;
    throw out_of_range(error.str());
  }
}


ostream& operator<<(ostream& out, const Quake& quake)
{
  return out << "Time: " << quake.getTime()
             << "\nLatitude: " << quake.getLatitude() << " deg"
             << "\nLongitude: " << quake.getLongitude() << " deg"
             << "\nDepth: " << quake.getDepth() << " km"
             << "\nMagnitude: " << quake.getMagnitude() << endl;
}
