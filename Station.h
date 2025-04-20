// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef STATION_H
#define STATION_H

#include <cstdint>
#include <ostream>
using std::ostream;
#include <vector>

#include <algorithm>
#include <map>
#include <memory>

#include "Charger.h"

namespace Availability {
    class StationAvailabilityReportFactory; //forward declaration
}

namespace ChargingNodes {
class Charger;
using std::vector;
using std::unordered_map;
using std::shared_ptr;

/**
 * @brief Station ID type alias.
 * Station ID is a 32 bit unsigned integer. See Spec Section 2.1.6.
 *
 */
using stationID_t = uint32_t;


/**
 * @brief Encapsulates a single charging station.
 * Its identity lies in the station ID.
 * See Spec Section 2.1.2, 2.1.6
 *
 * Create the Station and add Charger pointers to it:
 *
 *      stationID_t stationID {3};
 *      Station station{ stationID };
 *
 *      chargerID_t chargerID{ 1003 };
 *      auto c = std::make_shared<Charger>(chargerID);
 *      station.insertCharger( c );
 *
 */
class Station
{

public:
    /**
     * Default constructor
     */
    Station();

    /**
     * Copy constructor
     *
     * @param other the object being copied from
     */
    Station ( const Station& other );

    /**
     * constructor from stationID
     *
     * @param other the station ID from which to construct this object
     */
    Station ( const stationID_t stationID );

    /**
     * Destructor
     */
    ~Station();

    /**
     * Assignment operator
     *
     * @param other the object being copied from
     * @return object reference
     */
    Station& operator= ( const Station& other );


    /**
     * @brief Move constructor.
     *
     * @param  The object to be moved from
     */
    Station(Station&&);

    /**
     * @brief Move assignment operator.
     *
     * @param other The object to be moved from
     * @return Station&
     */
    Station& operator= (Station&& other);

    /**
     *
     * Equality operator
     *
     * @param other the object being compared with
     * @return object reference
     */
    bool operator== ( const Station& other ) const;

    /**
     *
     * Inequality operator
     *
     * @param other the object being compared with
     * @return object reference
     */
    bool operator!= ( const Station& other ) const;

    stationID_t getStationID() const;
    void insertCharger( shared_ptr<ChargingNodes::Charger> charger );
    friend std::ostream& operator <<  (std::ostream& os, const Station& s);
    friend class Availability::StationAvailabilityReportFactory;


protected:
    /**
     * @brief Station ID.
     * Identity of a Station lies within the stationID.
     * 32 bit unsigned int. See Spec Section 2.1.6,
     *
     */
    stationID_t stationID;

    /**
     * @brief A vector of Charger's.
     * Mulitiplicity: Each Station can be associated from zero to many Chargers.
     *
     */
    vector<shared_ptr<ChargingNodes::Charger>> chargers;


};

/**
 * @brief Write to output stream
 * @param os output stream
 * @param s Station
 */
ostream& operator <<  (ostream& os, const Station& s);
/**
 * @brief Write to output stream
 * @param os output stream
 * @param vs vector of Station's
 */
ostream& operator <<  (ostream& os, const vector<Station>& vs);
} //namespace ChargingNodes

namespace std
{
template <>
/**
 * @brief Comparator to allow Station to be added to a map.
 */
struct less<ChargingNodes::Station>  {
    bool operator() ( const ChargingNodes::Station& a, const ChargingNodes::Station& b ) const
    {
        return a.getStationID() < b.getStationID();
    }
};
}


#endif // STATION_H
