//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

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
class SharedSection final : public SharedSectionInterface
{
public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection() :sem(1) {
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée par une autre locomotive.
     * Si la locomotive et son thread ont été mis en attente,
     * le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive &loco) override {
        // Check if the shared section is available
        if (!isAvailable) {
            loco.arreter(); // Stop the locomotive only if the section is in use
            afficher_message(qPrintable(QString("Locomotive %1 waiting for shared section").arg(loco.numero())));
        }

        isAvailable = false; // Mark the shared section as in use
        sem.acquire(); // Block and acquire the semaphore

        loco.demarrer(); // Restart the locomotive
        afficher_message(qPrintable(QString("Locomotive %1 enters the shared section").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive &loco) override {
        afficher_message(qPrintable(QString("Locomotive %1 leaves the shared section").arg(loco.numero())));
        isAvailable = true;
        sem.release(); // Release access to the shared section.
    }

private:

    /* A vous d'ajouter ce qu'il vous faut */
    PcoSemaphore sem;
    bool isAvailable;

    // Méthodes privées ...
    // Attribut privés ...
};


#endif // SHAREDSECTION_H
