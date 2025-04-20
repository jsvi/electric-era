// Input a data file about a charging network and output availability by charging station.
// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later
#include <iostream>
using std::cout;

#include <string>
using std::string;

#include <filesystem>

#include "Charging.h"
#include "ChargingNetwork.h"
#include "StationAvailabilityReport.h"

using namespace Charging;

/**
 * @brief Starts the program.
 *
 * Constructs a ChargingNetwork object passing it a path to the data file.
 * This creates the object graph and in so doing, reads data into the ChargingNetwork.
 * main gets a StationAvailabilityReport from the ChargingNetwork and prints it to
 * stdout.
 *
 *
 * @param argc The number of arguments passed on the command line. intut
 * @param argv The arguments. A pointer to char pointers
 * @return int
 */
int main(int argc, char **argv) {

    Debug( "Electric Era Charging Network Uptime Report Display Program" << std::endl );
#ifdef _DEBUG
    Debug( "_DEBUG\n" );
#endif
#ifdef NDEBUG
    //cout << "NDEBUG\n";
#endif

    if (1 == argc ) { //if no arguments specified, only name of currently running program
        std::cout << ChargingNetwork::ERROR_TEXT << "\n"; //Note: std::endl is not required bc we don't need to flush the stream
        std::cerr << "No data file specified\n"; // Output detailed error to stderr, not stdout. See Spec Section 2.3.2
        std::cerr << "Usage: " << argv[0] << " path_to_data_file\n";
        return EXIT_FAILURE;
    }

    const std::filesystem::path  chargingNetworkDataFile {argv[1]};
    Debug( "data file path: " << chargingNetworkDataFile << "\n" );

    int returnCode = EXIT_SUCCESS; //default

    try {
        ChargingNetwork cn {chargingNetworkDataFile};
        StationAvailabilityReport report = cn.getStationAvailabilityReport();
        cout << report;
    } catch (std::exception& ex) {
        returnCode = EXIT_FAILURE;
    }


    return returnCode; //success/failure See Spec Section 2.3.3

}
