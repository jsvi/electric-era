// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2025 <copyright holder> <email>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef STATIONAVAILABILITYREPORTFACTORY_H
#define STATIONAVAILABILITYREPORTFACTORY_H

#include "StationAvailabilityReport.h"
#include "Station.h"
//#include "Charger.h"
#include <memory>
#include <map>

namespace Availability {
using std::shared_ptr;
using std::map;

/**
 * @brief Factory for creating StationAvailabilityReport.
 * It depends on a container of Station's. Inject via constructor:
 *
 *      map<stationID_t, shared_ptr<Station>> stations;
 *      ... add a bunch of Station's to the container with insert()
 *      auto factory = StationAvailabilityReportFactory(this->stations);
 *      StationAvailabilityReport report =  factory.getReport();
 *
 * Why not just pass the stations map to the StationAvailabilityReport object?
 * Well, the report doesn't need to concern itself with Stations and all of its attendant info
 * (such as each station's chargers and each charger's availability). The report only contains
 * entries for each station and its availability.
 *
 */
class StationAvailabilityReportFactory
{
public:
    /**
     * Default constructor. C++ default.
     */
    StationAvailabilityReportFactory();

    /**
     * Constructor
     *
     * @param stations a container of Station's on which to report.
     */
    StationAvailabilityReportFactory(map<ChargingNodes::stationID_t, shared_ptr<ChargingNodes::Station>> stations);

    /**
     * Copy constructor. C++ default.
     *
     * @param other object to copy.
     */
    StationAvailabilityReportFactory(const StationAvailabilityReportFactory& other);

    /**
     * Destructor. C++ default.
     */
    ~StationAvailabilityReportFactory();

    /**
     * Assignment operator. C++ default.
     *
     * @param other object to copy from.
     * @return object reference
     */
    StationAvailabilityReportFactory& operator=(const StationAvailabilityReportFactory& other);

    /**
     * @brief Get the station availability report.
     * Returns an object which contains StationAvailabilityEntry's, which are lines of the station
     * availability report.
     *
     * @return StationAvailabilityReport
     *
     *
     */
    StationAvailabilityReport getReport();

protected:
    /**
     * @brief A map of Station's, keyed by station ID.
     * The need for this is that this object will iterate over its Station's, generating
     * a report of availability by station. <code>stations</code> contains all the info needed
     * for creating the report.
     */
    map<ChargingNodes::stationID_t, shared_ptr<ChargingNodes::Station>> stations;

};

} //namespace Availability

#endif // STATIONAVAILABILITYREPORTFACTORY_H
