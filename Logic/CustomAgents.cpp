#include "../Headers/CustomAgents.h"

AGENTTYPE Drone::getType() const
{
    return AGENTTYPE::DRONE;
}

AGENTTYPE Robot::getType() const
{
    return AGENTTYPE::ROBOT;
}

AGENTTYPE Scooter::getType() const
{
    return AGENTTYPE::SCOOTER;
}
