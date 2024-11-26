#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>

class SharedStation
{
public:
    SharedStation(int nbTrains) : stationSemaphore(0), trainsWaiting(0), totalTrains(nbTrains) {}

    /**
     * @brief arriveAtStation Checks if both trains arrived at the station to continue the program
     */
    void arriveAtStation();

private:
    PcoSemaphore stationSemaphore;
    int trainsWaiting;
    int totalTrains;
};

#endif // SHARED_STATION_H
