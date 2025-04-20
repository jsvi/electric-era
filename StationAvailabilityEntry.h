// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef STATIONAVAILABILITYENTRY_H
#define STATIONAVAILABILITYENTRY_H

#include "Station.h"
#include <ostream>

namespace Availability {

class StationAvailabilityReportFactory;

/**
 * @brief Encapsulates a single line of the station availability report.
 * Consists of a station ID and uptime percentage.
 *
 *      StationAvailabilityReport report;
 *      report += StationAvailabilityEntry({0, 12.5f/100});
 *      cout << report;
 *
 */
class StationAvailabilityEntry
{
public:
    /**
     * Default constructor. C++ default.
     */
    StationAvailabilityEntry();

    /**
     * Constructor.
     *
     * @param stationID The ID of the station which this enty is regarding.
     * @param uptimeFraction The fraction of time that any charger at this station was available.
     * See Spec Section 2.3. We store as a decimal fraction and display as a percentage.
     * For example, if a station was available 12.5% of the time, uptimeFraction would be 0.125.
     */
    StationAvailabilityEntry(ChargingNodes::stationID_t stationID, float uptimeFraction);

    /**
     * Copy constructor. C++ default.
     *
     * @param other object to copy
     */
    StationAvailabilityEntry(const StationAvailabilityEntry& other);

    /**
     * Destructor. C++ default.
     */
    ~StationAvailabilityEntry();

    /**
     * Assignment operator. C++ default.
     *
     * @param other object to copy from
     * @return object reference
     */
    StationAvailabilityEntry& operator=(const StationAvailabilityEntry& other);

    /**
     * @brief Comparison operator less than.
     *
     * Sorts primarily on stationID, then on uptimeFraction.
     *
     * @param other object to compare
     * @return bool
     */
    bool  operator<(const StationAvailabilityEntry& other) const noexcept;

    /**
     * @brief Spaceship comparison operator.
     *
     * Sorts primarily on stationID, then on uptimeFraction.
     *
     * @param other object to compare
     * @return std::partial_ordering
     */
    constexpr std::partial_ordering operator<=>(const StationAvailabilityEntry& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const StationAvailabilityEntry& sae);
    friend class StationAvailabilityReportFactory;

protected:
    /**
     * @brief The station ID for this station.
     * Each entry in the report pertains to one station. Each station has one entry.
     * See Spec Section 2.3.7.
     *
     */
    ChargingNodes::stationID_t stationID;
    /**
     * @brief The fraction of time that any charger at this station was available.
     * See Spec Section 2.3. We store as a decimal fraction. For example, if a station was
     * available 12.5% of the time, uptimeFraction would be 0.125.
     * We multiple by 100, truncate, and convert to integer to print.
     * So, 12.5% would print as 12.
     *
     * See Spec Section 2.3.7 and 2.3.8
     */
    float uptimeFraction;
};

/**
 * @brief Write to output stream.
 * Writes the station ID, a space, and then the uptime percentage.
 * See Spec Section 2.3.7
 * (Converts the uptimeFraction to percentage, truncates and converts to integer.)
 * So, 12.5% would print as 12. 12.79% would also print as 12.
 * See Spec Section 2.3.8
 * Doesn't emit a newline.
 *
 * @param os output stream
 * @param sae StationAvailabilityEntry to write
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& os, const StationAvailabilityEntry& sae);

} //namespace Availability

#endif // STATIONAVAILABILITYENTRY_H
