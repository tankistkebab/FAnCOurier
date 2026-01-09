#include "../Headers/HiveMind.h"
#include "../Headers/HiveMind.h"
#include <iostream>
#include <limits>

void HiveMind::update()
{
    //intai verificam daca s-au livrat pachete
    checkDeliveries();

    //verificam bateriile
    checkBatteryLevels();

    //alocam taskuirle
    assignTasks();
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
    int dist, buffer, consumption, safeBatteryLevel;
    int stepsPerTick, ticksNeeded;


    for (Agent* agent : fleet)
    {
        if (agent->isDead())
        {
            continue;
        }

        //calc distanta pana la cea mai apropiata statie
        POINT nearestStation = findNearestChargingStation(agent->getPosition(), agent->getMovementMode());
        dist = ManhattanDist(agent->getPosition(), nearestStation);

        //estimare consum
        if (agent->getMovementMode() == MOVEMENTMODE::AIR)
        {
            stepsPerTick = 3;
        }
        else
        {
            stepsPerTick = 1;
        }

        ticksNeeded = dist / stepsPerTick;

        consumption = agent->getBatteryConsumption();
       safeBatteryLevel = (ticksNeeded * consumption) + buffer;
       buffer = 20;

        //check daca avem suficienta baterie ca sa ne intoarcem + buffer de safety
        if (agent->getBattery() < safeBatteryLevel && agent->getState() != AGENTSTATE::CHARGING && agent->getState() != AGENTSTATE::DEAD)
        {
            //fortam robotul sa se intoarca
            cout << "ALERTA: Agent " << agent->getId() << " are bateria in critica! Se intoarce la statie." << std::endl;

            vector<POINT> pathToStation = Pathfinder::findPath(agent->getPosition(), nearestStation, map, agent->getMovementMode());

            agent->assignPath(pathToStation);
        }
    }
}

void HiveMind::assignTasks()
{
    int distToPickup, distToDeliver, totalDist, speed, batteryNeeded;
    int estimatedTicks;

    double operationalCost, score;


    vector<TaskBid> bids;
    //Dureroasa functie

    vector<int> idleAgentIndices;
    for (size_t i = 0; i < fleet.size(); ++i)
    {
        //check ca agenti sa fie IDLE si vii
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

    //partea de logica pentru a genera ofertele
    for (int agentIdx : idleAgentIndices)
    {
        Agent* agent = fleet[agentIdx];
        POINT agentPos = agent->getPosition();

        for (int pkgIdx : pendingPackageIndices)
        {
            Package& pkg = packages[pkgIdx];

            //cal distanta de la baza la client
            distToPickup = ManhattanDist(agentPos, map->baza);
            distToDeliver = ManhattanDist(map->baza, pkg.destination);

            totalDist = distToPickup + distToDeliver;

            //estimare distanta si cost
            if (agent->getMovementMode() == MOVEMENTMODE::AIR)
            {
                speed = 3; //Drona
            }
            else if (agent->getSymbol() == 'S') {
                speed = 2; //scuter
            }
            else {
                speed = 1; //robot
            }

            estimatedTicks = totalDist / speed;
            if (estimatedTicks == 0)
            {
                estimatedTicks = 1; //evit div 0
            }

            //check daca ajunge bateria cu o marja de eroare e 10%
            batteryNeeded = estimatedTicks * agent->getCost();
            if (agent->getBattery() < batteryNeeded * 1.1f)
            {
                continue;
            }

            // Profit = Reward - (CostOperare * Timp)
            operationalCost = estimatedTicks * agent->getCost();
            score = pkg.reward - operationalCost;


            //daca pachetul expira soon, creste artificial valoarea lui (ca sa evitam penalizarea care e cam naspa)
            if (pkg.deadlineTicks < estimatedTicks + 5)
            {
                score += 100.0;
            }

            // Adaugam oferta
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

        //calc ruta
        vector<POINT> pathPart1;

        //In cazul in care agentul nu e la baza
        if (ManhattanDist(agent->getPosition(), map->baza) > 0)
        {
            pathPart1 = Pathfinder::findPath(agent->getPosition(), map->baza, map, agent->getMovementMode());
        }

        vector<POINT> pathPart2 = Pathfinder::findPath(map->baza, pkg.destination, map, agent->getMovementMode());

        //concatenarea rutelor
        vector<POINT> fullPath = pathPart1;
        fullPath.insert(fullPath.end(), pathPart2.begin(), pathPart2.end());

        if (fullPath.empty())
        {
            cerr << "Eroare: Nu s-a gasit drum valid pentru pachetul " << pkg.id << std::endl;
            continue;
        }

        //setam starile noi si marcam ca ocupati
        agent->assignPath(fullPath);
        pkg.status = PACKAGESTATUS::ASSIGNED;
        pkg.assignedAgentId = agent->getId();

        agentTaken[bid.agentIndex] = true;
        packageTaken[bid.packageIndex] = true;

        cout << "DEBUG: HiveMind: Agent " << agent->getId() << " (" << agent->getSymbol() << ") a preluat pachetul " << pkg.id << " (Reward: " << pkg.reward << ")" << std::endl;
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