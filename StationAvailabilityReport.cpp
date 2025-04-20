// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Charging.h"
#include "StationAvailabilityReport.h"
#include <iostream>
#include <algorithm>

namespace Availability {

StationAvailabilityReport::StationAvailabilityReport() = default;

StationAvailabilityReport::StationAvailabilityReport(const StationAvailabilityReport& other) = default;

StationAvailabilityReport::~StationAvailabilityReport() = default;

StationAvailabilityReport& StationAvailabilityReport::operator=(const StationAvailabilityReport& other)= default;

StationAvailabilityReport::StationAvailabilityReport(StationAvailabilityReport&& other) = default;

StationAvailabilityReport& StationAvailabilityReport::operator=(StationAvailabilityReport&& other) = default;

StationAvailabilityReport& StationAvailabilityReport::operator+=(const StationAvailabilityEntry& sae) {
    stationAvailabilityEntries.push_back(sae);
    return *this;
}

void StationAvailabilityReport::sort() {

    namespace ranges = std::ranges; //namespace shortcut
    ranges::sort(this->stationAvailabilityEntries, std::less());

}

std::ostream& operator<< (std::ostream& os, const StationAvailabilityReport& sar) {
    Debug( "StationAvailabilityReport\n" );

    //Unfortunately, we have to NOT emit a newline at the end of the uptime entries.
    //This complicates the logic to support that. See the sample output files in Specification.
    int count = sar.stationAvailabilityEntries.size();
    int newlinesRemaining = count - 1;
    for (auto i : sar.stationAvailabilityEntries) {
        std::cout << i;
        if (newlinesRemaining-- > 0) {
            std::cout << "\n";
        }
    }
    return os;
}


} //namespace Availability
