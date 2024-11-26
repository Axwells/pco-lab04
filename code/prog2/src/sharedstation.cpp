//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"

void SharedStation::arriveAtStation() {
        trainsWaiting++;
        if (trainsWaiting < totalTrains) {
            stationSemaphore.acquire(); // Wait for second train
        } else {
            trainsWaiting = 0;

            for(int i = 1; i < totalTrains; ++i){
                stationSemaphore.release(); // Let first train go
            }
        }
    }
