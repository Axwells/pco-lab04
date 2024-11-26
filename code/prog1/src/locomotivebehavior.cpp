//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void swapSens(int &a, int &b){
    int tmp = a;
    a = b;
    b = tmp;
}

void LocomotiveBehavior::run() {
    int laps = 0;

    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Locomotive démarrée");

    while (true) {
        // Effectuer N tours définis pour cette locomotive
        while (laps < nbTours) {
            // Access the shared section.
            attendre_contact(accessPoint1); // Point before shared section.
            sharedSection->access(loco);

            dirigerAiguillages(aiguillages);

            attendre_contact(accessPoint2); // Exit point of shared section.
            sharedSection->leave(loco);

            attendre_contact(stationPoint);
            loco.afficherMessage("Tour complété");
            laps++;
        }

        loco.afficherMessage("Arrivée à la station");
        loco.arreter();
        sharedStation.arriveAtStation();

        // Attendre avant de redémarrer
        std::this_thread::sleep_for(std::chrono::seconds(2));
        loco.demarrer();

        // Inverser le sens après les tours
        loco.afficherMessage("Inversion de direction");
        loco.inverserSens();
        swapSens(accessPoint1, accessPoint2);
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

void LocomotiveBehavior::dirigerAiguillages(std::vector<std::pair<int, int>> locoAiguillages) {
    for (size_t i = 0; i < locoAiguillages.size(); ++i)
        diriger_aiguillage(locoAiguillages[i].first, locoAiguillages[i].second, 0);
}
