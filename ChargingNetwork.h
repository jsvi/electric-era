// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once //this is not part of the C++ Standard, so we still use header guards
#ifndef CHARGINGNETWORK_H
#define CHARGINGNETWORK_H

#include <string>
using std::string;

#include "Charger.h"
#include "Station.h"
#include "AvailabilityEvent.h"

#include <map>
using std::map;

#include <filesystem>

#include "StationAvailabilityReport.h"

#include <memory>
using std::unique_ptr;
using std::shared_ptr;

namespace Charging {
using namespace Availability;
using namespace ChargingNodes;
}

namespace Charging {
/**
 * @brief Encapsulates a charging network.
 * Usual usage is to pass the input data file location via parameter to the constructor (Constructor DI):
 *
 *      const std::filesystem::path  chargingNetworkDataFile {"/path/to/data/file"};
 *      ChargingNetwork cn {chargingNetworkDataFile};
 *
 */
class ChargingNetwork
{
public:
    /**
     * Default constructor. Using Rule of Zero for five basic methods.
     */
    ChargingNetwork();

    /**
     * Copy constructor. Default.
     *
     * @param other the object being copied from
     *
     */
    ChargingNetwork ( const ChargingNetwork& other );

    /**
     * @brief Constructor with dependency passed in.
     * Pass the location of the input data file. See Spec Section 2.1 and 3.4
     * Throws std::filesystem::filesystem_error if the specified input data file is not found.
     *
     *      const std::filesystem::path  chargingNetworkDataFile {"/path/to/data/file"};
     *      ChargingNetwork cn {chargingNetworkDataFile};
     *
     * @param inputFile The path to the input data file
     * \internal
     * @brief Construct the ChargingNetwork object graph by reading data from the passed inputFile.
     *
     * Try to read the file. If we can't show error and throw exception.
     * There are two modes in which the method can operate (actually, three, the third is NONE).
     * After we detect a line
     *
     *      [Stations]
     *
     * we are in Stations mode. We read in data to create Station objects and save auto pointers to them. Each  Station will
     * have zero or more Charger objects, auto pointers to which are saved in the Station object.
     * After we detect a line
     *
     *      [Charger Availability Reports]
     *
     * we are in AVAILABILITY_REPORTS mode and read in availability events (whether a given
     * Charger was available and from what start time to what end time. AvailabilityEvent auto pointers
     * are stored in the associated Charger.
     *
     * \endinternal
     */
    ChargingNetwork ( const ::std::filesystem::path& inputFile );

    /**
     * Destructor. Default.
     */
    ~ChargingNetwork();

    /**
     * @brief Assignment operator.
     * Default.
     *
     * @param other the object being assigned from
     * @return object reference
     */
    ChargingNetwork& operator= ( const ChargingNetwork& other );

    /**
     * @brief Move constructor.
     *
     * @param  The object to be moved from
     */
    ChargingNetwork(ChargingNetwork&&);

    /**
     * @brief Move assignment operator.
     *
     * @param other The object to be moved from
     * @return ChargingNetwork&
     */
    ChargingNetwork& operator= (ChargingNetwork&& other);

    /**
     * @brief Get the station availabilty report.
     *
     *      ChargingNetwork cn {chargingNetworkDataFile};
     *      StationAvailabilityReport report = cn.getStationAvailabilityReport();
     *      cout << report;
     *
     *
     * @return StationAvailabilityReport
     */
    StationAvailabilityReport getStationAvailabilityReport() const;
    /**
     * @brief Text to print when there is an error.
     * See Spec Section 2.3.1.
     *
     */
    inline static const ::std::string_view ERROR_TEXT {"ERROR"};
protected:


    map<stationID_t, shared_ptr<Station>> stations;
    map<chargerID_t, shared_ptr<Charger>> chargers;
    //unique_ptr<StationAvailabilityReport> report {nullptr};
    StationAvailabilityReport report;

    /**
     * @brief The heading in the data file preceding the info on stations.
     * After this heading is the list of stations and their associated chargers.
     */
    inline static const string STATIONS_HEADER{"[Stations]"};
    /**
     * @brief The heading in the data file preceding the info on uptime.
     * After this heading is a list of chargers and their uptimes.
     */
    inline static const string CHARGERAVAILABILITY_HEADER{"[Charger Availability Reports]"};
};


/**
 * @brief Write to output stream
 *
 * @param os output stream
 * @param m a map of Station's, keyed by stationID
 * @return std::ostream&
 */
std::ostream& operator <<  (std::ostream& os, const map<stationID_t, shared_ptr<Station>>& m);

} //namespace Charging

#endif // CHARGINGNETWORK_H
