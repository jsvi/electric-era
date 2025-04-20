// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include <gtest/gtest.h>
#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <iostream>
using std::cout;

#include "ChargingNetwork.h"
#include "Station.h"

#include "AvailabilityEvent.h"

namespace Charging {

TEST ( Station, HelloTest ) {
    ASSERT_TRUE( 0xdeadbeef == 0xdeadbeef );
}

TEST ( ChargingNetwork, ConstructTest ) {
    const string chargingNetworkDataFile {"../data/input_1.txt"};
    ChargingNetwork cn {chargingNetworkDataFile};
}

TEST ( Station, ConstructTest ) {
    stationID_t stationID {1};
    Station s {stationID};
}

TEST ( Station, Construct2Test ) {
    stationID_t stationID {2343637323};
    Station s {stationID};
}

TEST ( Station, Construct3Test ) {
    stationID_t stationID {UINT32_MAX};
    Station s {stationID};
}

TEST ( Station, Construct4Test ) {
    stationID_t stationID {UINT32_MAX+1};
    Station s {stationID};
}

TEST ( Station, Construct5Test ) {
    stationID_t stationID {UINT32_MAX+1000};
    Station s {stationID};
}

TEST ( Station, CopyTest ) {
    stationID_t stationID {1};
    Station s {stationID};
    Station s2 {s};
}

TEST ( Station, EqualityTest ) {
    stationID_t stationID {1};
    Station s {stationID};
    Station s2 {stationID};
    ASSERT_TRUE( s == s2 );
}

TEST ( Station, InequalityTest ) {
    Station s {1};
    Station s2 {2};
    ASSERT_TRUE( s != s2 );
}

TEST ( Station, AssignmentTest ) {
    Station s {1};
    Station s2 {2};
    s = s2;
    ASSERT_TRUE( s.getStationID() == 2 );
    ASSERT_TRUE( s.getStationID() == 2 );
}

TEST ( Station, OperatorLeftShiftTest ) {
    Station s {1};
    cout << s;
}

//Test loading a container w/ max num of stations in US
TEST ( Station, LoadTest ) {
    static const int GAS_STATIONS_IN_US = 115370;
    vector<Station> stations;
    for (int i = 0; i<GAS_STATIONS_IN_US; i++ ) {
        stations.emplace_back(i);
    }
    cout << "total stations: " << stations.size() << "\n";

}

TEST ( AvailabilityEvent, ConstructTest ) {
    using namespace Availability;
    u_int64_t startTime {0};
    u_int64_t endTime {50000};
    string availableText {"true"};
    bool availableBool {false};

    nanoseconds_t startTimeObj {startTime};
    nanoseconds_t endTimeObj {endTime};
    availableBool = (availableText == "true") ? true : false;

    AvailabilityEvent ar ( startTimeObj, endTimeObj, availableBool );
}

TEST ( ChargingStationDataFile, ExistsTest ) {
    const string inputFile1 {"../data/input_1.txt"};
    ifstream ifs {inputFile1};
    ASSERT_TRUE(ifs); //File exists
}

TEST ( ChargingStationDataFile, NotExistTest ) {
    const string inputFile1 {"../data/input_notexist.txt"};
    ifstream ifs {inputFile1};
    ASSERT_FALSE(ifs); //File doesn't exist
}

TEST ( StationAvailabilityReport, ConstructTest ) {

    StationAvailabilityReport report;
    StationAvailabilityEntry entry {0, 12.5f/100};
    report += entry;

    vector<StationAvailabilityEntry> entries {{0, 12.5f/100},{0, 12.75f/100},{0, 12.15f/100},{0, 32.2f/100},{0, 32.5f/100},{0, 32.4999f/100},
        {7, 12.5f/100},{3, 2.75f/100},{2, 32.15f/100},{5, 32.2f/100},{5, 26.5f/100},{1, 32.4999f/100}};

    for (auto e : entries) {
        report += e;
    }

    cout << report;

    report.sort();

    cout << report;
}

TEST ( ChargersOperatorLeftBitShift, MultiTest ) {
    vector<shared_ptr<Charger>> v;
    auto x = std::make_shared<Charger>(1001);
    auto y = std::make_shared<Charger>(1002);
    v.push_back ( x ); v.push_back ( y );
    cout << v;
}

} //namespace Charging

