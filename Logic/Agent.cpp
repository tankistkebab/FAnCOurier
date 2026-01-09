#include "../Headers/Agent.h"

Agent::Agent(int _id, POINT _startPos, const Map* _map)
    : id(_id), position(_startPos), map(_map)
{
    state = AgentState::IDLE;
}


bool Agent::isDead() const
{
    if (state == AgentState::DEAD)
    {
        return true;
    }
    return false;
}
bool Agent::isIdle() const
{
    if (state == AgentState::IDLE)
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
MovementMode Agent::getMovementMode() const
{
    return stats.mode;
}
char Agent::getSymbol() const
{
    return stats.symbol;
}


void Agent::update()
{
    int i;

    CELLTYPE cell;

    if (state == AgentState::DEAD)
    {
        return;
    }

    switch (state)
    {
        case AgentState::MOVING:
            //consumam baterie per tick
            currentBattery -= stats.batteryCons;

            //Verificam sa mai avem baterie
            if (currentBattery <= 0)
            {
                currentBattery = 0;
                state = AgentState::DEAD;
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
                    state = AgentState::IDLE;

                    //check daca am aterizat pe o statie de incarcat
                    cell = map->grid[position.x][position.y];
                    if (cell == CELLTYPE::BASE || cell == CELLTYPE::CHARGING_STATION)
                    {
                        state = AgentState::CHARGING;
                    }
                    break;
                }
            }
            break;

        case AgentState::CHARGING:
            chargeStep();
            break;

        case AgentState::IDLE:
            //check daca am aterizat pe o statie de incarcat
            cell = map->grid[position.x][position.y];

            if ((cell == CELLTYPE::BASE || cell == CELLTYPE::CHARGING_STATION) && currentBattery < stats.maxBattery)
            {
                state = AgentState::CHARGING;
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
        state = AgentState::IDLE; //baterie full
    }
}

void Agent::assignPath(const vector<POINT>& newPath)
{
    if (state == AgentState::DEAD)
    {
        return;
    }

    currentPath = newPath;
    if (!currentPath.empty())
    {
        state = AgentState::MOVING;
    }
}
