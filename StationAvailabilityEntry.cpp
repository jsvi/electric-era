// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "StationAvailabilityEntry.h"
#include <iostream>
#include <cmath>

namespace Availability {
StationAvailabilityEntry::StationAvailabilityEntry() = default;

StationAvailabilityEntry::StationAvailabilityEntry(ChargingNodes::stationID_t stationID, float uptimeFraction) :
    stationID {stationID}, uptimeFraction {uptimeFraction} {
}

StationAvailabilityEntry::StationAvailabilityEntry(const StationAvailabilityEntry& other) = default;


StationAvailabilityEntry::~StationAvailabilityEntry() = default;

StationAvailabilityEntry& StationAvailabilityEntry::operator=(const StationAvailabilityEntry& other) = default;

bool StationAvailabilityEntry::operator<(const StationAvailabilityEntry& other) const noexcept {
    //sort primarily on stationID, secondarily on uptimeFraction
    bool result = false;
    if  (this->stationID < other.stationID) result = true;
    else if  (this->stationID > other.stationID) result = false;
    else if  (this->uptimeFraction < other.uptimeFraction) result = true;
    else result = false;
    return result;
}

constexpr std::partial_ordering StationAvailabilityEntry::operator<=>(const StationAvailabilityEntry& other) const noexcept {
    if (auto c = this->stationID <=> other.stationID; c != 0)
        return c;
    return this->uptimeFraction <=> other.uptimeFraction;
}

std::ostream& operator<<(std::ostream& os, const StationAvailabilityEntry& sae) {
    std::cout << sae.stationID << " ";
    //truncate and convert to int. C++ automatically truncates during the conversion.
    const int intPercentage = static_cast<int> (sae.uptimeFraction*100);
    std::cout << intPercentage;
    return os;
}
} //namespace Availability
