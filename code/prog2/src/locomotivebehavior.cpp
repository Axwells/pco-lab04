//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void LocomotiveBehavior::run() {
    int laps = 0;

    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Locomotive started");

    while(true) {
        while (laps < nbTours) {
            attendre_contact(contactRequest);
            sharedSection->request(loco, loco.priority);

            attendre_contact(contactAccess);
            sharedSection->access(loco, loco.priority);

            directSwitches(locoSwitches);

            attendre_contact(contactLeave);
            sharedSection->leave(loco);

            attendre_contact(stationPoint);
            loco.afficherMessage("Lap completed");
            laps++;
        }

        loco.afficherMessage("Arriving at station");
        loco.arreter();
        sharedStation.arriveAtStation();

        // Simulate station delay and toggle priority
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sharedSection->togglePriorityMode();
        loco.priority = rand() % 11;
        loco.demarrer();

        loco.afficherMessage("Reversing direction");
        loco.inverserSens();
        std::swap(contactAccess, contactLeave);
        std::swap(contactRequest, contactRequest2);

        laps = 0;
    }
}


void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

void LocomotiveBehavior::directSwitches(std::vector<std::pair<int, int>> locoAiguillages) {
    for (size_t i = 0; i < locoAiguillages.size(); ++i)
        diriger_aiguillage(locoAiguillages[i].first, locoAiguillages[i].second, 0);
}
