#include "../Headers/HiveMind.h"
#include <cstdlib>
#include <iostream>
#include <limits>

void HiveMind::update(int tick)
{
    this->currentTick = tick; // Salvam tick-ul curent

    checkDeliveries();
    checkDeadlines(tick); // Folosim tick
    checkBatteryLevels();
    assignTasks(); // Acum assignTasks are acces la this->currentTick corect
}

void HiveMind::checkDeadlines(int currentTick)
{
    int expiryTick;

    for (auto& pkg : packages)
    {
        if (pkg.status == PACKAGESTATUS::PENDING || pkg.status == PACKAGESTATUS::ASSIGNED)
        {
            //calc momentul expirarii
            expiryTick = pkg.creationTime + pkg.deadlineTicks;

            if (currentTick > expiryTick)
            {
                pkg.status = PACKAGESTATUS::EXPIRED;
                cout << "[INFO] Pachetul " << pkg.id << " a EXPIRAT (Missed Deadline)." << endl;
            }
        }
    }
}

void HiveMind::checkDeliveries()
{
    //oentru fiecare pachet, verificam statusul si gasim agentul assignat
    for (Package& pkg : packages)
    {
        if (pkg.status == PACKAGESTATUS::ASSIGNED)
        {
            Agent* assignedAgent = nullptr;
            for(auto* a : fleet) //fancy variabilele astea auto
            {
                if(a->getId() == pkg.assignedAgentId)
                {
                    assignedAgent = a;
                    break;
                }
            }

            if (assignedAgent)
            {
                //check daca agentul este la client
                POINT agentPos = assignedAgent->getPosition();
                if (agentPos.x == pkg.destination.x && agentPos.y == pkg.destination.y)
                {

                    pkg.status = PACKAGESTATUS::DELIVERED;
                    cout << "Pachetul " << pkg.id << " livrat de Agent " << assignedAgent->getId() << endl;

                    //agentul devine IDLE automat la finalul path-ului
                }
            }
        }
    }
}

void HiveMind::checkBatteryLevels()
{
    for (Agent* agent : fleet) {
        if (agent->isDead()) continue;
        if (agent->getState() == AGENTSTATE::CHARGING) continue;

        // Ignoram daca e deja la baza/statie (safety check simplu)
        POINT pos = agent->getPosition();
        if (map->grid[pos.x][pos.y] == CELLTYPE::BASE ||
            map->grid[pos.x][pos.y] == CELLTYPE::CHARGING_STATION) {
            continue;
        }

        // --- PARAMETRI ---
        int consumption = agent->getBatteryConsumption();
        int speed = agent->getSpeed();
        double mazeFactor = (agent->getMovementMode() == MOVEMENTMODE::AIR) ? 1.0 : 1.4;
        int flatBuffer = 10; // Reducem bufferul "panicat" la 10 (1-2 pasi)

        // --- PASUL 1: VERIFICARE MISIUNE (COMMITMENT) ---
        // Daca agentul are o treaba (se misca spre ceva ce nu e statie), verificam daca o poate termina
        if (agent->getState() == AGENTSTATE::MOVING) {
            POINT currentDest = agent->getFinalDestination();

            // Verificam daca destinatia curenta este o Statie/Baza
            bool isGoingToStation = (map->grid[currentDest.x][currentDest.y] == CELLTYPE::BASE ||
                                     map->grid[currentDest.x][currentDest.y] == CELLTYPE::CHARGING_STATION);

            // Daca NU merge spre o statie (deci livreaza sau ridica pachet), aplicam logica triunghiulara
            if (!isGoingToStation) {
                // A. Costul sa ajunga la destinatie (Client)
                int distToDest = ManhattanDist(pos, currentDest);
                int costToFinishJob = ((distToDest * mazeFactor + speed - 1) / speed) * consumption;

                // B. Costul de la destinatie la cea mai apropiata statie (Future Safe Haven)
                // Gasim cea mai apropiata statie fata de CLIENT, nu fata de unde suntem acum!
                POINT nearestStationFromDest = findNearestChargingStation(currentDest, agent->getMovementMode());
                int distDestToStation = ManhattanDist(currentDest, nearestStationFromDest);
                int costAfterJob = ((distDestToStation * mazeFactor + speed - 1) / speed) * consumption;

                // C. Cost Total
                int totalMissionCost = costToFinishJob + costAfterJob + flatBuffer;

                // D. DECIZIA: Daca avem baterie pentru TOT circuitul, IGNORAM panica de moment
                if (agent->getBattery() >= totalMissionCost) {
                    continue; // <--- LASA-L SA MEARGA! E SAFE.
                }
            } else {
                 // Daca merge deja spre o statie, il lasam in pace (verificare existenta)
                 if (currentDest.x == findNearestChargingStation(pos, agent->getMovementMode()).x &&
                     currentDest.y == findNearestChargingStation(pos, agent->getMovementMode()).y) {
                     continue;
                 }
            }
        }

        // --- PASUL 2: FALLBACK (LOGICA DE PANICA) ---
        // Daca am ajuns aici, inseamna ca agentul NU poate termina misiunea (sau sta degeaba).
        // Trebuie sa vedem daca se poate salva ACUM.

        POINT nearestStation = findNearestChargingStation(pos, agent->getMovementMode());
        int distToStation = ManhattanDist(pos, nearestStation);
        int ticksReturn = (distToStation * mazeFactor + speed - 1) / speed;
        int costReturn = ticksReturn * consumption;

        // Aici folosim un buffer un pic mai mare pentru siguranta imediata
        int panicThreshold = costReturn + 15;

        if (agent->getBattery() < panicThreshold) {

             // Debug: Vedem cine intra in panica
             // cout << "Agent " << agent->getId() << " ABANDON MISSION. Bat: " << agent->getBattery() << endl;

             std::vector<POINT> pathToStation = Pathfinder::findPath(
                pos, nearestStation, map, agent->getMovementMode()
            );

            if (!pathToStation.empty()) {
                agent->assignPath(pathToStation);
            }
        }
    }
}

void HiveMind::assignTasks()
{//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    vector<TaskBid> bids;

    int speed, costPerTick, distToPickup, distToDeliver, realDistPickup, realDistDeliver, totalDist, estimatedTicks;
    int expiryTick, timeRemaining, batteryCost;
    double mazeFactor;

    vector<int> idleAgentIndices;
    for (size_t i = 0; i < fleet.size(); ++i)
    {
        if (fleet[i]->isIdle() && !fleet[i]->isDead())
        {
            idleAgentIndices.push_back(i);
        }
    }

    vector<int> pendingPackageIndices;
    for (size_t i = 0; i < packages.size(); ++i)
    {
        if (packages[i].status == PACKAGESTATUS::PENDING)
        {
            pendingPackageIndices.push_back(i);
        }
    }

    if (idleAgentIndices.empty() || pendingPackageIndices.empty())
    {
        return;
    }

    //gen oferte
    for (int agentIdx : idleAgentIndices)
    {
        Agent* agent = fleet[agentIdx];
        POINT agentPos = agent->getPosition();

        // parametrii agentului
        speed = agent->getSpeed();
        costPerTick = agent->getCost();

        //intr-uncat distanta Manhattan pe care o calculam nu e precisa, aleg un factor de corectie arbritar de 40%
        if (agent->getMovementMode() == MOVEMENTMODE::AIR)
        {
            mazeFactor = 1.0;
        }
        else
        {
            mazeFactor = 1.4;
        }

        for (int pkgIdx : pendingPackageIndices)
        {
            Package& pkg = packages[pkgIdx];

            //estimare realista a distantelor
            distToPickup = ManhattanDist(agentPos, map->baza);
            distToDeliver = ManhattanDist(map->baza, pkg.destination);

            realDistPickup = distToPickup * mazeFactor;
            realDistDeliver = distToDeliver * mazeFactor;

            totalDist = realDistPickup + realDistDeliver;

            // (Distanta + Viteza - 1) / Viteza = rotunjire in sus
            estimatedTicks = (totalDist + speed - 1) / speed;

            //verificare dealine
            expiryTick = pkg.creationTime + pkg.deadlineTicks;
            timeRemaining = expiryTick - this->currentTick;

            //daca nu ajung, pas la o drona
            if (estimatedTicks >= timeRemaining)
            {
                continue;
            }


            //check pt baterie (daca are suficiena ca sa il duca)
            batteryCost = estimatedTicks * agent->getBatteryConsumption();
            if (agent->getBattery() < batteryCost + 10)
            {
                continue;
            }

            //cal profit
            double operationalCost = estimatedTicks * costPerTick;
            double score = pkg.reward - operationalCost;

            //bonus pt urgenta
            if (timeRemaining < estimatedTicks + 10)
            {
                score += 150.0; //bonus mai generos pt a bate costul dronei
            }

            //penalizam umpic drona daca pachetele nu sunt urgente, ca sa le ia dronele
            if (agent->getMovementMode() == MOVEMENTMODE::AIR && timeRemaining > 30)
            {
                score -= 50.0;
            }

            bids.push_back({agentIdx, pkgIdx, score});
        }
    }

    sort(bids.begin(), bids.end(), std::greater<TaskBid>());

    vector<bool> agentTaken(fleet.size(), false);
    vector<bool> packageTaken(packages.size(), false);

    for (const auto& bid : bids)
    {
        if (agentTaken[bid.agentIndex] || packageTaken[bid.packageIndex])
        {
            continue;
        }

        Agent* agent = fleet[bid.agentIndex];
        Package& pkg = packages[bid.packageIndex];


        //cal drumul pana la baza
        vector<POINT> pathToBase;

        //check daca agentul e deja in baza
        if (ManhattanDist(agent->getPosition(), map->baza) > 0)
        {
            pathToBase = Pathfinder::findPath(agent->getPosition(), map->baza, map, agent->getMovementMode());

            //safety check
            if (pathToBase.empty())
            {
                continue;
            }
        }

        //calc drumul de la baza la client
        vector<POINT> pathToClient = Pathfinder::findPath(map->baza, pkg.destination, map, agent->getMovementMode());

        // Safety Check
        if (pathToClient.empty())
        {
            cerr << "HiveMind: Client inaccesibil pentru pachetul " << pkg.id << std::endl;
            continue;
        }

        //concatenam cele 2 rute
        vector<POINT> fullPath = pathToBase;
        fullPath.insert(fullPath.end(), pathToClient.begin(), pathToClient.end());

        agent->assignPath(fullPath);

        pkg.status = PACKAGESTATUS::ASSIGNED;
        pkg.assignedAgentId = agent->getId();

        agentTaken[bid.agentIndex] = true;
        packageTaken[bid.packageIndex] = true;

        std::cout << "[HIVEMIND] Agent " << agent->getId() << " (" << agent->getSymbol() << ") a preluat pachetul " << pkg.id << " (Reward: " << pkg.reward << ")" << std::endl;
    }
}

POINT HiveMind::findNearestChargingStation(POINT currentPos, MOVEMENTMODE mode)
{
    POINT bestStation = map->baza;
    int minCost, dist;

    minCost = ManhattanDist(currentPos, map->baza);

    for (const auto& station : map->chargingStationsLocations)
    {
        //A* cal rutele adevarate, aici fac doar o estimare (for efficiency sake)
        dist = ManhattanDist(currentPos, station);
        if (dist < minCost)
        {
            minCost = dist;
            bestStation = station;
        }
    }
    return bestStation;
}

int HiveMind::ManhattanDist(POINT a, POINT b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}