// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Station.h"
#include <ostream>
using std::ostream;

namespace ChargingNodes {

Station::Station() = default;

Station::Station ( const Station& other ) = default;

Station::Station ( const stationID_t stationID ) {
    this->stationID = stationID;
}

Station::~Station() = default;

Station& Station::operator= ( const Station& other ) = default;

Station::Station(Station&&) = default;

Station& Station::operator= (Station&& other) = default;


bool Station::operator== ( const Station& other ) const {
    return (other.stationID == this->stationID );
}

bool Station::operator!= ( const Station& other ) const {
    return (other.stationID != this->stationID );
}

stationID_t Station::getStationID() const {
    return this->stationID;
}

void Station::insertCharger( shared_ptr<ChargingNodes::Charger> charger ) {
    //this->chargers.insert( charger );
    this->chargers.push_back( charger);
}

//using ChargingNodes::operator<<;
ostream& operator <<  (ostream& os, const Station& s) {
    os << "Station:[" << s.getStationID() << "]\n";
    os << s.chargers;
    return os;
}

ostream& operator <<  (ostream& os, const vector<Station>& vs) {
    for (const Station& s : vs) {
        os << s;
    }
    return os;
}

} //namespace ChargingNodes
