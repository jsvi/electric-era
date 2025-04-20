// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#ifndef AVAILABILITYEVENT_H
#define AVAILABILITYEVENT_H

#include "Charger.h"

#include <memory>
#include <vector>

namespace Availability {
using std::shared_ptr;
using std::vector;

/**
 * @brief Type alias for start and end times.
 * A 64-bit unsigned int.
 *
 */
using nanoseconds_t = uint64_t;


/**
 * @brief Encapsulates a single line of the Charger Availability Reports section of the input data file.
 * See Spec Section 2.1.4, 2.1.10 to 2.1.13, 2.1,13.f to 2.1.13.m,  and 2.1 generally.
 */
class AvailabilityEvent
{
public:
    /**
     * Default constructor
     */
    AvailabilityEvent();

    /**
     * Copy constructor
     *
     * @param other object being copied
     */
    AvailabilityEvent ( const AvailabilityEvent& other );

    /**
     * Constructor
     */
    AvailabilityEvent (  nanoseconds_t startTime, nanoseconds_t endTime, bool available);


    /**
     * Destructor
     */
    ~AvailabilityEvent();

    /**
     * Assignment operator
     *
     * @param other the object being assigned from
     * @return object reference
     */
    AvailabilityEvent& operator= ( const AvailabilityEvent& other );

    /**
     * @brief Move constructor.
     *
     * @param  The object to be moved from
     */
    AvailabilityEvent(AvailabilityEvent&&);

    /**
     * @brief Move assignment operator.
     *
     * @param other The object to be moved from
     * @return AvailabilityEvent&
     */
    AvailabilityEvent& operator= (AvailabilityEvent&& other);

    /**
     * @brief Equality operator
     *
     * @param other the object being compared with
     * @return true if all members are equal
     */
    bool operator== ( const AvailabilityEvent& other ) const;

    /**
     * @brief Inequality operator.
     *
     * @param other the object being compared with
     * @return false if a single member is not equal
     */
    bool operator!= ( const AvailabilityEvent& other ) const;

    void insertAvailabilityEvent(shared_ptr<AvailabilityEvent> ae );

    /**
     * @brief Start time of the availability event in nanoseconds.
     * See Spec Section 2.1.8 and 2.1.13.g
     *
     */
    nanoseconds_t startTime {0};
    /**
     * @brief End time of the availability event in nanoseconds.
     * See Spec Section 2.1.8 and 2.1.13.g
     *
     */
    nanoseconds_t endTime {0};
    /**
     * @brief Whether the charger was available or not during this time period. true or false.
     * See Spec Section 2.1.9 and 2.1.13.g
     *
     */
    bool available {false};

    /**
     * @brief Spaceship comparison operator.
     *
     * Sorts on startTime, then endTime, then available
     *
     * @param other object to compare
     * @return std::partial_ordering
     */
    std::partial_ordering operator<=>(const AvailabilityEvent& other) const noexcept;

};

std::ostream& operator <<  (std::ostream& os, const AvailabilityEvent& ae);
std::ostream& operator <<  (std::ostream& os, const vector<shared_ptr<AvailabilityEvent>> & aev);
std::ostream& operator <<  (std::ostream& os, const vector<AvailabilityEvent> & aev);


} // namespace Availability
#endif // AVAILABILITYEVENT_H
