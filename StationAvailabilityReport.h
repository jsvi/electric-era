// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef STATIONAVAILABILITYREPORT_H
#define STATIONAVAILABILITYREPORT_H

#include "StationAvailabilityEntry.h"
#include <vector>
using std::vector;

#include <ostream>

namespace Availability {

/**
 * @brief Encapsulates a station availabilty report.
 * See Spec Section 2.3.4 to 2.3.9
 *
 *      StationAvailabilityReport report;
 *      report += StationAvailabilityEntry({0, 12.5f/100});
 *      cout << report;
 *
 * Normally, you will not create a report manually, but rather will get one from the
 * StationAvailabilityReportFactory .
 *
 */
class StationAvailabilityReport
{
public:
    /**
     * Default constructor. C++ default.
     */
    StationAvailabilityReport();

    /**
     * Copy constructor. C++ default.
     *
     * @param other object to be copied
     */
    StationAvailabilityReport(const StationAvailabilityReport& other);

    /**
     * Destructor
     */
    ~StationAvailabilityReport();

    /**
     * Assignment operator. C++ default.
     *
     * @param other object to be copied from
     * @return object reference.
     */
    StationAvailabilityReport& operator=(const StationAvailabilityReport& other);


    StationAvailabilityReport(StationAvailabilityReport&& other);

    StationAvailabilityReport& operator=(StationAvailabilityReport&& other);

    /**
     * @brief Insert a StationAvailabilityEntry.
     * The StationAvailabilityReport consists of a series of StationAvailabilityEntry's.
     *
     * @param sae StationAvailabilityEntry to insert
     * @return StationAvailabilityReport&
     */
    StationAvailabilityReport& operator+=(const StationAvailabilityEntry& sae);

    /**
     * @brief Sort the report entries by stationID.
     *
     */
    void sort();

    friend std::ostream& operator<< (std::ostream& os, const StationAvailabilityReport& sar);


protected:
    /**
     * @brief A vector of StationAvailabilityEntry's.
     * They include the stationID and the uptime fraction (the fraction of time that any charger
     * at a given station was availabile.
     *
     */
    vector<StationAvailabilityEntry> stationAvailabilityEntries;

};

/**
 * @brief Write to output stream.
 * Only emits a newline after entries <em>other than</em> the first one.
 * This means there is no newline after the last entry.
 * There is no newline if there are no entries.
 * See Spec output sample files.
 *
 * @param os Output stream to write to.
 * @param sar StationAvailabilityReport to write
 *
 *      StationAvailabilityReport report;
 *      report += StationAvailabilityEntry({0, 12.5f/100});
 *      cout << report;
 *
 * @return std::ostream&
 */
std::ostream& operator<< (std::ostream& os, const StationAvailabilityReport& sar);

} //namespace Availability
#endif // STATIONAVAILABILITYREPORT_H
