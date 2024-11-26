//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //


#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface {
public:
    SharedSection() : sem(1), accessSemaphore(1), isAvailable(true), currentMode(PriorityMode::HIGH_PRIORITY) {}

    /**
     * @brief request Makes a request to access the shared section.
     * Stores the priority of the requesting locomotive.
     * @param loco The locomotive requesting access.
     * @param priority The priority of the locomotive making the request.
     */
    void request(Locomotive& loco, int priority) override {
        isPriorityModeChanged = false;
        accessSemaphore.acquire();
        locoPriorities[loco.numero()] = priority;
        loco.afficherMessage(QString("Request to shared section made with priority %1").arg(priority));
        accessSemaphore.release();
    }

    /**
     * @brief access Grants access to the shared section if available and no higher-priority requests exist.
     * If access is denied, the locomotive waits until the section is available.
     * @param loco The locomotive attempting to access the shared section.
     * @param priority The priority of the locomotive attempting to access.
     */
    void access(Locomotive &loco, int priority) override {
        while (true) {
            accessSemaphore.acquire();

            if (isAvailable && !hasHigherPriorityRequest(priority, loco.numero())) {
                isAvailable = false;
                accessSemaphore.release();
                break;
            }
            accessSemaphore.release();

            loco.arreter();
            loco.afficherMessage("Waiting for shared section access");

            // Small delay to avoid spinning and allow other threads to update state
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        sem.acquire();
        loco.afficherMessage("Accessing shared section");
        loco.demarrer();
    }

    /**
     * @brief leave Releases the shared section after a locomotive exits.
     * Marks the section as available and removes the locomotive's priority entry.
     * @param loco The locomotive leaving the shared section.
     */
    void leave(Locomotive& loco) override {
        accessSemaphore.acquire();

        isAvailable = true;
        locoPriorities.erase(loco.numero()); // Let the waiting train have the priority
        loco.afficherMessage("Leaving shared section");

        accessSemaphore.release();
        sem.release();
    }

    /**
     * @brief togglePriorityMode Toggles the priority mode between HIGH_PRIORITY and LOW_PRIORITY.
     * Ensures this action happens only once during a cycle.
     */
    void togglePriorityMode() override {
        accessSemaphore.acquire();
        if(!isPriorityModeChanged)
            currentMode = (currentMode == PriorityMode::HIGH_PRIORITY) ? PriorityMode::LOW_PRIORITY : PriorityMode::HIGH_PRIORITY;
        isPriorityModeChanged = true;
        accessSemaphore.release();
    }

private:
    PcoSemaphore sem;
    PcoSemaphore accessSemaphore;
    bool isAvailable;
    PriorityMode currentMode;
    bool isPriorityModeChanged;
    std::map<int, int> locoPriorities;

    /**
     * @brief hasHigherPriorityRequest Checks if a higher-priority request exists in the queue.
     * Evaluates based on the current priority mode (HIGH_PRIORITY or LOW_PRIORITY).
     * @param priority The priority of the requesting locomotive.
     * @param locoId The ID of the requesting locomotive.
     * @return True if a higher-priority request exists, false otherwise.
     */
    bool hasHigherPriorityRequest(int priority, int locoId) {
        for (const auto& entry : locoPriorities) {
            if (entry.first != locoId) {
                if ((currentMode == PriorityMode::HIGH_PRIORITY && entry.second > priority) ||
                    (currentMode == PriorityMode::LOW_PRIORITY && entry.second < priority)) {
                    return true;
                }
            }
        }
        return false;
    }
};


#endif // SHAREDSECTION_H
