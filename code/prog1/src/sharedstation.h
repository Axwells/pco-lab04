#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>

class SharedStation
{
public:
    SharedStation(int nbTrains) : stationSemaphore(0), trainsWaiting(0), totalTrains(nbTrains) {}

    /* Implémentez toute la logique que vous avez besoin pour que les locomotives
     * s'attendent correctement à la station */

    void arriveAtStation();

private:
    PcoSemaphore stationSemaphore; // Semaphore to sync the locomotives.
    int trainsWaiting;
    int totalTrains;
};

#endif // SHARED_STATION_H
