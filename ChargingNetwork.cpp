// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Charging.h"
#include "ChargingNetwork.h"
#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <iostream>
using std::cout;

#include <sstream>
using std::istringstream;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <memory>

#include <filesystem>

#include <assert.h>

#include "Station.h"
#include "StationAvailabilityReportFactory.h"

namespace Charging {

ChargingNetwork::ChargingNetwork() = default;

ChargingNetwork::ChargingNetwork ( const ChargingNetwork& other ) = default;


ChargingNetwork::ChargingNetwork ( const std::filesystem::path& inputFile ){

    ifstream ifs {inputFile};
    if ( !ifs.is_open() ) { //throw exception if we can't open the file
        std::cout << ERROR_TEXT << "\n"; // See Spec Section 2.3.1
        static const std::string explanation { "Could not open file." };
        std::cerr << explanation << "\n"; // See Spec Section 2.3.2
        throw std::filesystem::filesystem_error ( explanation, inputFile,  std::error_code() );
    }
    Debug( "\n" );

    enum class Modes { NONE, STATIONS, AVAILABILITY_REPORTS};
    Modes mode {Modes::NONE};

    string line;
    auto counter = 0;

    while ( std::getline ( ifs, line ) ) {
        Debug( "\nLine " << counter++ << ": " << line << "\n" );
        if ( line == ChargingNetwork::STATIONS_HEADER ) {
            Debug( "Stations header\n" );
            mode = Modes::STATIONS;
        } else if ( line == ChargingNetwork::CHARGERAVAILABILITY_HEADER ) {
            Debug( "Availability header\n" );
            mode = Modes::AVAILABILITY_REPORTS;
        } else if ( line == "" ) {
            ; // Do nothing if blank line
        } else {

            istringstream iss ( line );
            stationID_t stationID;
            chargerID_t chargerID;

            switch ( mode ) {
            case Modes::NONE:
                // Have to account for the fact that if there is no header, by default
                // we would fall thru to the STATIONS case.
                break;
            case Modes::STATIONS:
                Debug( "case STATIONS:\n" );
                iss >>  stationID;
                Debug( "\nstationID: " << stationID << ":" );


                while ( iss >> chargerID ) {
                    Debug( chargerID << "," );

                    //create a Charger
                    auto c = std::make_shared<ChargingNodes::Charger>(chargerID);
                    this->chargers.insert({chargerID, c});

                    //insert a Charger pointer in the associated Station
                    auto result = this->stations.find(stationID);
                    if (result != this->stations.end()) { //if the Station already exists
                        auto& station = result->second;
                        station->insertCharger(c);
                    } else {    //if not, create the Station
                        auto station = std::make_shared<ChargingNodes::Station>(stationID);
                        station->insertCharger(c);
                        this->stations.insert({stationID, station});
                    }

                }
                break;
            case Modes::AVAILABILITY_REPORTS: {
                Debug( "case AVAILABILITY_REPORTS:\n" );
                iss >>  chargerID;
                Debug( "chargerID: " << chargerID << ":" );

                //create an AvailabilityEvent for the current line, and insert
                //it into the associated Charger
                u_int64_t startTime;
                u_int64_t endTime;
                string availableText;
                bool availableBool {false};

                iss >> startTime;
                iss >> endTime;

                //handle strange condition of startTime greater than endTime.
                //We set endTime to equal startTime.
                //See Spec Section 4.3
                if (startTime > endTime)
                    endTime = startTime;
                nanoseconds_t startTimeObj {startTime};
                nanoseconds_t endTimeObj {endTime};

                iss >> availableText;
                availableBool = ( availableText == "true" ) ? true : false;

                //find the associated Charger
                const auto result = chargers.find(chargerID);
                if (result == chargers.end()) {
                    assert(false); // Should never get here bc there should always be a Charger for this chargerID
                } else {
                    auto& charger = result->second;
                    charger->getChargerID();
                    auto ae = std::make_shared<AvailabilityEvent>( startTimeObj, endTimeObj, availableBool );
                    charger->insertAvailabilityEvent(ae );
                }

            }
                break;
            default:
                break;
            }
        }
    }

    Debug( "\n" );
    Debug( "cout << chargers;\n" );
    Debug( "cout << stations;\n" );
    Debug( stations );
    Debug( "\nC++ version: " << __cplusplus <<"\n" );
}

ChargingNetwork::~ChargingNetwork()= default;

ChargingNetwork& ChargingNetwork::operator= ( const ChargingNetwork& other ) = default;

ChargingNetwork::ChargingNetwork(ChargingNetwork&&) = default;

ChargingNetwork& ChargingNetwork::operator= (ChargingNetwork&& other) = default;

StationAvailabilityReport ChargingNetwork::getStationAvailabilityReport() const {
    auto factory = Availability::StationAvailabilityReportFactory(this->stations);
    return factory.getReport();
}

std::ostream& operator <<  (std::ostream& os, const map<ChargingNodes::stationID_t, shared_ptr<ChargingNodes::Station>>& m) {
    for (const auto&[ k, v ] : m) { //key is stationID, value is the Station (pointer to Station, actually)
        os << *v.get() << "\n";
        //os << "std::ostream& operator <<  (std::ostream& os, const map<stationID_t, shared_ptr<Station>>& m) {";
    }
    return os;
}

} //namespace Charging

