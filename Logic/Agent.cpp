#include "../Headers/Agent.h"

Agent::Agent(int _id, POINT _startPos, const Map* _map)
    : id(_id), position(_startPos), map(_map)
{
    state = AGENTSTATE::IDLE;
}


bool Agent::isDead() const
{
    if (state == AGENTSTATE::DEAD)
    {
        return true;
    }
    return false;
}
bool Agent::isIdle() const
{
    if (state == AGENTSTATE::IDLE)
    {
        return true;
    }
    return false;
}
POINT Agent::getPosition() const
{
    return position;
}
int Agent::getId() const
{
    return id;
}
float Agent::getBattery() const
{
    return currentBattery;
}
int Agent::getCapacity() const
{
    return stats.capacity;
}
int Agent::getCost() const
{
    return stats.costPerTick;
}
MOVEMENTMODE Agent::getMovementMode() const
{
    return stats.mode;
}
char Agent::getSymbol() const
{
    return stats.symbol;
}

AGENTSTATE Agent::getState() const
{
    return state;
}

int Agent::getBatteryConsumption() const
{
    return stats.batteryCons;
}

float Agent::getMaxBattery() const
{
    return stats.maxBattery;
}

int Agent::getSpeed() const
{
    return stats.speed;
}

POINT Agent::getFinalDestination() const
{
    if (!currentPath.empty())
    {
        return currentPath.back();
    }
    return position;
}

void Agent::update()
{
    int i;

    CELLTYPE cell;

    if (state == AGENTSTATE::DEAD)
    {
        return;
    }

    switch (state)
    {
        case AGENTSTATE::MOVING:
            //consumam baterie per tick
            currentBattery -= stats.batteryCons;

            //Verificam sa mai avem baterie
            if (currentBattery <= 0)
            {
                currentBattery = 0;
                state = AGENTSTATE::DEAD;
                cout << "DEBUG: Agent " << id << " (" << stats.name << ") nu mai are baterie" << endl;
                return;
            }

            // Incepem miscarea
            for (i = 0; i < stats.speed; ++i)
            {
                if (!currentPath.empty())
                {
                    moveStep();
                }
                else
                {
                    //am terminat drumul
                    state = AGENTSTATE::IDLE;

                    //check daca am aterizat pe o statie de incarcat
                    cell = map->grid[position.x][position.y];
                    if (cell == CELLTYPE::BASE || cell == CELLTYPE::CHARGING_STATION)
                    {
                        state = AGENTSTATE::CHARGING;
                    }
                    break;
                }
            }
            break;

        case AGENTSTATE::CHARGING:
            chargeStep();
            break;

        case AGENTSTATE::IDLE:
            //check daca am aterizat pe o statie de incarcat
            cell = map->grid[position.x][position.y];

            if ((cell == CELLTYPE::BASE || cell == CELLTYPE::CHARGING_STATION) && currentBattery < stats.maxBattery)
            {
                state = AGENTSTATE::CHARGING;
            }
            break;
        default:
            cerr << "Sincer, nush cum ai reusit sa strici asta. AgentState ne-existent. Bafta la Debug" << endl;
            exit(EXIT_FAILURE);
    }
}

void Agent::moveStep()
{
    POINT nextPos = currentPath.front();
    currentPath.erase(currentPath.begin());

    position = nextPos;
}

void Agent::chargeStep() {
    //bateria se incarca cu 25% per tick
    float chargeAmount = stats.maxBattery * 0.25f;
    currentBattery += chargeAmount;

    if (currentBattery >= stats.maxBattery)
    {
        currentBattery = stats.maxBattery;
        state = AGENTSTATE::IDLE; //baterie full
    }
}

void Agent::assignPath(const vector<POINT>& newPath)
{
    if (state == AGENTSTATE::DEAD)
    {
        return;
    }

    currentPath = newPath;
    if (!currentPath.empty())
    {
        state = AGENTSTATE::MOVING;
    }
}
