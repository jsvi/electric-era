// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Charging.h"
#include "Charger.h"

#include <iostream>

using std::cout;
using std::ostream;
using Availability::AvailabilityEvent;

namespace ChargingNodes {


Charger::Charger() = default;

Charger::Charger ( const Charger& other ) = default;

Charger::Charger ( chargerID_t chargerID ) {
    this->chargerID = chargerID;
}

Charger::~Charger() { // = default
    Debug( "~Charger\n" );
}

Charger::Charger(Charger&&) = default;

Charger& Charger::operator= (Charger&& other) = default;


chargerID_t Charger::getChargerID() const {
    return this->chargerID;
}

Charger& Charger::operator= ( const Charger& other ) = default;

bool Charger::operator== ( const Charger& other ) const {
    return (other.chargerID == this->chargerID );
}

bool Charger::operator!= ( const Charger& other ) const {
    return (other.chargerID != this->chargerID );
}

void Charger::insertAvailabilityEvent(shared_ptr<AvailabilityEvent> ae ){
    this->availabilityEvents.push_back(ae);
}

inline ostream& operator <<  (ostream& os, const Charger& c) {
    Debug(  "Charger:[" << c.getChargerID() << "]\n" );
    auto ae = c.getAvailabilityEvents();
    os << ae;
    return os;
}

ostream& operator << ( ostream& os, const vector<shared_ptr<Charger>>& vc ) {
    for (auto c : vc) {
        os << *c.get();
    }
    return os;
}

ostream& operator <<  (ostream& os, const vector<Charger>& vc) {
    for (const Charger& c : vc) {
        os << c;
    }
    return os;
}

auto operator <  (const Charger c, const Charger c2) {
    return c.getChargerID() < c2.getChargerID();
}

} //namespace ChargingNodes

