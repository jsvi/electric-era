// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef CHARGER_H
#define CHARGER_H

#include <cstdint>

#include <ostream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>



namespace Availability {
    class AvailabilityEvent; //forward declaration
    class StationAvailabilityReportFactory; //forward declaration
}
using namespace Availability;

#include "AvailabilityEvent.h"

namespace ChargingNodes {
using std::vector;
using std::map;
using std::shared_ptr;



/**
 * @brief Charger ID type alias.
 * Charger ID is a 32 bit unsigned integer. See Section 2.1.7.
 */
using chargerID_t = uint32_t ;

/**
 * @brief Encapsulates a single charger.
 * Its identity lies in the ChargerID of the charger.
 *
 * Usual usage:
 *
 *      chargerID_t chargerID = 1003;
 *      Charger c( chargerID );
 *      cout << c;
 *
 * @todo write additional usage info.
 */
class Charger
{
public:
    /**
     * Default constructor. Using Rule of Zero, but we specify C++ default.
     */
    Charger();

    /**
     * Copy constructor. C++ default.
     *
     * @param other the object being copied from
     */
    Charger ( const Charger& other );

    Charger ( chargerID_t chargerID );

    /**
     * Destructor. C++ default.
     */
    ~Charger();

    /**
     * Assignment operator. C++ default.
     *
     * @param other the object being copied from
     * @return Charger&
     */
    Charger& operator= ( const Charger& other );

    /**
     * @brief Move constructor. C++ default.
     *
     * @param  The object to be moved from
     */
    Charger(Charger&&);

    /**
     * @brief Move assignment operator. C++ default.
     *
     * @param other The object to be moved from
     * @return Charger&
     */
    Charger& operator= (Charger&& other);


    /**
     * Equality operator.
     *
     * @param other the object being compared with
     * @return bool
     */
    bool operator== ( const Charger& other ) const;

    /**
     * Inequality operator.
     *
     * @param other the object being compared with
     * @return bool
     */
    bool operator!= ( const Charger& other ) const;

    //Might have to simply allow public access to chargerID if performance becomes a problem
    /**
     * @brief Returns chargerID for this Charger.
     *
     * @return chargerID_t
     */
    chargerID_t getChargerID() const;

    /**
     * @brief Insert an AvailabilityEvent.
     * Pass a shared_ptr to an AvailabilityEvent and this object will cache a copy
     * of the pointer to use later.
     *
     * @param ae shared_ptr to an AvailabilityEvent.
     */
    void insertAvailabilityEvent(shared_ptr<AvailabilityEvent> ae );

    /**
     * @brief Returns a vector of shared_ptr's to AvailabilityEvent's for this Charger
     *
     * @return std::vector< std::shared_ptr< AvailabilityEvent > >
     */
    vector<shared_ptr<AvailabilityEvent>> getAvailabilityEvents() const {
        return this->availabilityEvents;
    }

    friend class Availability::StationAvailabilityReportFactory;

protected:
    /**
     * @brief The ID for this charger.
     * This is a 32-bit unsigned integer.
     */
    chargerID_t chargerID;
    /**
     * @brief A container of AvailabilityEvent's for this Charger.
     * An AvailabilityEvent encapsulates a single line of the [Charger Availability Reports]
     * section of the input data file. I.e., it has a charger ID, start time, end time, and
     * availability.
     */
    vector<shared_ptr<AvailabilityEvent>> availabilityEvents;
};


/**
 * @brief Write output stream
 *
 * @param os output stream
 * @param c object to write
 * @return std::ostream&
 */
std::ostream& operator << ( std::ostream& os, const Charger& c );
/**
 * @brief Write to output stream
 *
 * @param os output stream
 * @param vc vector of Chargers to write
 * @return std::ostream&
 */
std::ostream& operator << ( std::ostream& os, const vector<Charger>& vc );
/**
 * @brief Write to output stream
 *
 * @param os output stream
 * @param vc vector of Charger pointers
 * @return std::ostream&
 */
std::ostream& operator << ( std::ostream& os, const vector<shared_ptr<Charger>>& vc );
/**
 * @brief Write to output stream
 *
 * @param os output stream
 * @param mc map of Station's, keyed by Charger
 * @return std::ostream&
 */
//std::ostream& operator <<  (std::ostream& os, const map<Charger, Station>& mc);
/**
 * @brief operator less-than
 *
 * @param c First object
 * @param c2 Second object
 * @return bool
 */
auto operator < ( const Charger c, const Charger c2 );

} //namespace ChargingNodes

namespace std
{
template <>
/**
 * @brief Comparator to allow Charger to be added to a map.
 */
struct less<ChargingNodes::Charger>  {
    bool operator() ( const ChargingNodes::Charger& a, const ChargingNodes::Charger& b ) const
    {
        return a.getChargerID() < b.getChargerID();
    }
};
};


#endif // CHARGER_H
