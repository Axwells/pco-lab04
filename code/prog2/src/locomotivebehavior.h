//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"
#include "sharedstation.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive& loco,
                           std::shared_ptr<SharedSectionInterface> sharedSection,
                           SharedStation& station, int nbTours,int contactRequest, int contactRequest2, int contactAccess, int contactLeave, int stationPoint, std::vector<std::pair<int, int>> locoSwitches)
            : loco(loco), sharedSection(sharedSection), sharedStation(station), nbTours(nbTours), contactRequest(contactRequest), contactRequest2(contactRequest2), contactAccess(contactAccess), contactLeave(contactLeave), stationPoint(stationPoint), locoSwitches(locoSwitches) {
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    int nbTours;
    int contactRequest;
    int contactRequest2;
    int contactAccess;
    int contactLeave;
    int stationPoint;

    std::vector<std::pair<int, int>> locoSwitches;
    SharedStation& sharedStation;

    /**
     * @brief directSwitches Direct the switches needed for the train to follow his course
     * @param locoSwitches Vector of pairs containing the id of the switch and the direction wanted
     */
    void directSwitches(std::vector<std::pair<int, int>> locoSwitches);

};

#endif // LOCOMOTIVEBEHAVIOR_H
