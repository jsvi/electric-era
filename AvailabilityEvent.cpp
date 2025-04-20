// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Charging.h"
#include "AvailabilityEvent.h"
#include <iostream>

namespace Availability {

AvailabilityEvent::AvailabilityEvent() = default;

AvailabilityEvent::AvailabilityEvent ( const AvailabilityEvent& other ) = default;

AvailabilityEvent::AvailabilityEvent (  nanoseconds_t startTime, nanoseconds_t endTime, bool available){
    this->startTime = startTime;
    this->endTime = endTime;
    this->available = available;

}


AvailabilityEvent::~AvailabilityEvent() { // = default
     Debug( "~AvailabilityEvent()\n" );
}

AvailabilityEvent& AvailabilityEvent::operator= ( const AvailabilityEvent& other ) = default;

AvailabilityEvent::AvailabilityEvent(AvailabilityEvent&&) = default;
AvailabilityEvent& AvailabilityEvent::operator= (AvailabilityEvent&& other) = default;


bool AvailabilityEvent::operator== ( const AvailabilityEvent& other ) const {
    return this->startTime == other.startTime
        and this->endTime == other.endTime
        and this->available == other.available;
}

bool AvailabilityEvent::operator!= ( const AvailabilityEvent& other ) const {
    return this->startTime != other.startTime
        or  this->endTime != other.endTime
        or this->available != other.available;
}

std::partial_ordering AvailabilityEvent::operator<=>(const Availability::AvailabilityEvent& other) const noexcept {
    if (auto c = this->startTime <=> other.startTime; c != 0)
        return c;
    if (auto c = this->endTime <=> other.endTime; c != 0)
        return c;
    return this->available <=> other.available;
}

std::ostream& operator <<  (std::ostream& os, const AvailabilityEvent& ae) {
    os << ae.startTime
    << " " << ae.endTime
    << " " << ae.available
    << "\n"
    ;
    return os;
}

std::ostream& operator <<  (std::ostream& os, const vector<shared_ptr<AvailabilityEvent>> & aev){
    for (auto ae : aev) {
        os << *ae;
    }
    return os;
}

std::ostream& operator <<  (std::ostream& os, const vector<AvailabilityEvent> & aev) {
    for (auto& ae : aev) {
        os << ae;
    }
    return os;
}

} //namespace Availability
