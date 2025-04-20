// SPDX-FileCopyrightText: 2025 Jaspreet Dha git@jsvi.org
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Charging.h"
#include "StationAvailabilityReportFactory.h"
#include "StationAvailabilityEntry.h"
#include "AvailabilityEvent.h"
#include "Charger.h"
#include <iostream>
#include <algorithm>
#include <ranges>
#include <assert.h>

using ChargingNodes::stationID_t;
//class ChargingNodes::Station;

namespace Availability {

StationAvailabilityReportFactory::StationAvailabilityReportFactory() = default;

StationAvailabilityReportFactory::StationAvailabilityReportFactory(map<stationID_t, shared_ptr<ChargingNodes::Station>> stations) : stations{stations} {
}

StationAvailabilityReportFactory::StationAvailabilityReportFactory(const StationAvailabilityReportFactory& other) = default;

StationAvailabilityReportFactory::~StationAvailabilityReportFactory() = default;

StationAvailabilityReportFactory& StationAvailabilityReportFactory::operator=(const StationAvailabilityReportFactory& other) = default;

/**
 * \internal
 * Create and return the station availability report.
 *
 * Strategy: Iterate over the cached Stations.
 * Every Station has multiple Chargers, which have multiple AvailabilityEvent's.
 * So, for every Station,
 * create a consolidated vector of AvailabilityEvent's. This may have duplicates.
 * To calculate the uptimeFraction, we need to know the numerator and denominator.
 * The denominator is simply the latestEndTime-earliestStartTime.
 * The numerator is more complicated:
 * Remove the overlapping AvailabilityEvent's in the consolidated vector of AvailabilityEvent's.
 * Then calculate uptime from the vector of non-overlapping AvailabilityEvent's.
 * Add entries to a StationAvailabilityReport and return the report.
 *
 * See Spec Section 2.3
 *
 * @return StationAvailabilityReport
 *
 * \endinternal
 */
StationAvailabilityReport StationAvailabilityReportFactory::getReport() {

    Debug( "StationAvailabilityReport StationAvailabilityReportFactory::getReport() {\n" );
    StationAvailabilityReport report;

    //Iterate over Station's. Note the earliest start time and latest end time, so we can know
    //the max time span we are calculating uptime for. Create a consolidated vector of all
    //AvailabilityEvent's for this Station (not broken up by Charger). This vector will not contain
    //downtime (non-available) events.
    for (const auto& [k,station] : this->stations) { //key is stationID, value is Station

        vector<AvailabilityEvent> vaeConsolidated; //Consolidated vector of AvailabilityEvent's

        nanoseconds_t earliestStartTime {UINT64_MAX};  // start with the highest value, and work downards.
        nanoseconds_t latestEndTime {0};               //  start with the lowest value and work upwards.

        Debug( "Charger loop:\n" );

        //It's possible the algorithm could be made even more efficient, but let's keep it simple for future maintenance's sake
        for (const auto& charger : station->chargers) {

            Debug( charger << "\n" );
            //Debug( charger->availabilityEvents );

            Debug( "availabilityEvent loop:\n" );
            for (const auto& availabilityEvent : charger->availabilityEvents ) {
                Debug( *availabilityEvent );

                if (earliestStartTime > availabilityEvent->startTime )
                    earliestStartTime = availabilityEvent->startTime ;

                if (latestEndTime < availabilityEvent->endTime)
                    latestEndTime = availabilityEvent->endTime;

                if (! false==availabilityEvent->available) //we discard the downtime events
                    vaeConsolidated.push_back( *availabilityEvent );
            }
        }

        Debug( "End Charger loop\n" );
        Debug( "vaeConsolidated: \n" << vaeConsolidated << "end vaeConsolidated\n");

        const nanoseconds_t denominator = latestEndTime - earliestStartTime;

        auto calculateUptime = [] ( const vector<AvailabilityEvent>& vaeNoOverlaps, const nanoseconds_t denominator ) {
            Debug( "calculateUptime()\n" );
            //Calculate the fraction of time that any charger at a station was available,
            //(by adding up the time that any charger was available
            //out of the entire time period that any charger at that station was reporting in.
            //(known by the denominator we're passed)
            //See Spec Section 2.3.
            //We return the fraction as a float; callers can calculate percent by multipying by 100 as desired.
            //This function assumes we're passed a vector with no overlapping AvailabilityEvent's. If there are
            //overlaps, the fraction will be wrong and may even be more than 1.
            Debug( "vaeNoOverlaps in calculateUptime() \n" << vaeNoOverlaps );
            nanoseconds_t availableDurationCumulative {0};
            for (const auto& availabilityEvent : vaeNoOverlaps) {
                availableDurationCumulative += (availabilityEvent.endTime - availabilityEvent.startTime);
            }
            const nanoseconds_t numerator = availableDurationCumulative; //superfluous, but it in code bc comments get out of synch. Compiler will remove
            const auto uptimeFraction = static_cast<float>(numerator)/denominator;
            return uptimeFraction;
        };


        auto removeOverlaps = [&vaeConsolidated] () {
            Debug( "removeOverlaps()\n" );
            /*

            Remove overlapping AvailabilityEvent's in vaeConsolidated and write
            non-overlapping durations as AvailabilityEvent's to vaeNoOverlaps, which is returned.

            Out strategy is to sort vaeConsolidated, iterate over it, compare the current
            AvailabilityEvent to the previous one. According to the logic below, either
            add the current AvailabilityEvent to vaeNoOverlaps, add a new AvailabilityEvent
            which has its startTime reset to the previous AvailabilityEvent's endTime, or do
            nothing.

            If comparing two AvailabilityEvent's A and B, there are 5 possible cases:

            Case 0:
            A. ---------
            B.            --------

            Case 1:
            A. --------
            B.     -------

            Case 2:
            A.      --------
            B. --------

            Case 3:
            A. --------------'
            B.     ------

            Case 4:
            A.       ------
            B.  ---------------

            Bc we sort the items,  there should only be cases 0, 1, and 3. But if we handle
            case 1 as below, that can result in there being a case 2 or 4 for following iterations.:
            0. Add B to vaeNoOverlaps. (A is already in)
            1, 2, 4. Set B.start = A.end and add B.
            3. Drop B.

            */

            //sort vaeConsolidated, move the first item to a new vector of AvailabilityEvent's: vaeNoOverlaps.
            //Tho, if empty, return
            //It's expensive to remove the first element, so we don't remove it.
            //We just iterate fr the 2nd element in the for loop in next paragraph.
            vector<AvailabilityEvent> vaeNoOverlaps;
            if (vaeConsolidated.empty())
                return vaeNoOverlaps;
            std::ranges::sort(vaeConsolidated, std::less());
            vaeNoOverlaps.push_back( vaeConsolidated.at(0) );
            Debug ( "vaeConsolidated, sorted " << vaeConsolidated );
            Debug ( "vaeNoOverlaps " << vaeNoOverlaps );
            if (vaeConsolidated.size() == 1) //bail if there was only one element
                return vaeNoOverlaps;

            //iterate fr the 2nd element forward, bc we already added 1st to vaeNoOverlaps
            //in each iteration, a is the previous AvailabilityEvent. I.e., the last element in vaeNoOverlaps.
            //b is the current AvailabilityEvent
            for (const auto& b : std::ranges::drop_view{vaeConsolidated, 1}) {
                const auto& a = vaeNoOverlaps.at(vaeNoOverlaps.size()-1);
                Debug( "a: " << a );
                Debug( "b: " << b );

                if (a.endTime <= b.startTime) {
                    Debug( "case 0\n" );
                    // Add b
                    vaeNoOverlaps.push_back( b );
                } else if (a.endTime > b.startTime && b.startTime > a.startTime && a.endTime < b.endTime) {
                    Debug( "case 1\n" );
                    // Set b.start = a.end. Add b.
                    // The = case doesn't need to be handled here bc it was handled in 0.
                    AvailabilityEvent aeNew{ b };
                    aeNew.startTime = a.endTime; Debug( "aeNew: " << aeNew );
                    if (not (aeNew.startTime == aeNew.endTime))
                        vaeNoOverlaps.push_back( aeNew );
                } else if (a.startTime > b.startTime and a.endTime >= b.endTime) {
                    Debug( "case 2\n" );
                    // Set b.start = a.end. Add b.
                    AvailabilityEvent aeNew{ b };
                    aeNew.startTime = a.endTime; Debug( "aeNew: " << aeNew );
                    if (not (aeNew.startTime == aeNew.endTime))
                        vaeNoOverlaps.push_back( aeNew );
                } else if (b.startTime >= a.startTime and a.endTime >= b.endTime) {
                    Debug( "case 3\n" );
                    // Dont add b
                } else if (a.startTime > b.startTime and a.endTime <= b.endTime) {
                    Debug( "case 4\n" );
                    // Set b.start = a.end. Add b.
                    AvailabilityEvent aeNew{ b };
                    aeNew.startTime = a.endTime; Debug( "aeNew: " << aeNew );
                    if (not (aeNew.startTime == aeNew.endTime))
                        vaeNoOverlaps.push_back( aeNew );
                } else {
                    Debug( "Shouldn't get here\n" );
                    assert(false);
                }
            }

            Debug( "vaeNoOverlaps (return value): \n" << vaeNoOverlaps );
            return vaeNoOverlaps;

        }; //removeOverlaps()

        vector<AvailabilityEvent> vaeNoOverlaps = removeOverlaps();
        auto uptimeFraction = calculateUptime( vaeNoOverlaps, denominator );

        Debug( "uptimeFraction: " << uptimeFraction << "\n" );
        StationAvailabilityEntry entry(station->getStationID(), uptimeFraction);
        report += entry;

    }

    report.sort(); //See Spec Section 2.3.9
    return report;

}


} //namespace Availability
