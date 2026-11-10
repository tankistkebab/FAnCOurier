#include <gtest/gtest.h>

#include "Headers/CustomAgents.h"
#include "test_helpers.h"

namespace
{
    std::vector<POINT> straightPath(POINT from, int steps)
    {
        std::vector<POINT> path;
        for (int i = 1; i <= steps; ++i)
        {
            path.push_back({from.x, from.y + i});
        }
        return path;
    }

    // 1x40 corridor: base on the far left, charging station on the far right.
    void loadCorridor(Map& map)
    {
        loadMapFromRows(map, {
            std::string(42, '#'),
            "#B......................................S#",
            std::string(42, '#'),
        }, "corridor.txt");
    }
}

TEST(Agent, MovesSpeedStepsPerTickAndDrainsBattery)
{
    Map map;
    loadCorridor(map);
    Drone drone(1, map.getBase(), &map);

    drone.assignPath(straightPath(map.getBase(), 10));
    ASSERT_EQ(drone.getState(), AGENTSTATE::MOVING);

    drone.update();

    EXPECT_EQ(drone.getPosition(), (POINT{1, 1 + drone.getSpeed()}));
    EXPECT_FLOAT_EQ(drone.getBattery(), drone.getMaxBattery() - drone.getBatteryConsumption());
}

TEST(Agent, BecomesIdleAtEndOfPath)
{
    Map map;
    loadCorridor(map);
    Robot robot(1, map.getBase(), &map);

    robot.assignPath(straightPath(map.getBase(), 2));
    robot.update(); // step 1
    robot.update(); // step 2
    robot.update(); // path empty -> idle

    EXPECT_EQ(robot.getPosition(), (POINT{1, 3}));
    EXPECT_TRUE(robot.isIdle());
}

TEST(Agent, DiesWhenBatteryRunsOut)
{
    Map map;
    loadCorridor(map);
    Drone drone(1, map.getBase(), &map); // 100 battery, 10 per tick

    drone.assignPath(straightPath(map.getBase(), 39));
    for (int i = 0; i < 9; ++i)
    {
        drone.update();
    }
    EXPECT_EQ(drone.getState(), AGENTSTATE::MOVING);

    drone.update();
    EXPECT_TRUE(drone.isDead());
    EXPECT_FLOAT_EQ(drone.getBattery(), 0.0f);

    // Dead agents ignore new orders
    drone.assignPath(straightPath(drone.getPosition(), 1));
    EXPECT_TRUE(drone.isDead());
}

TEST(Agent, RechargesAtChargingStation)
{
    Map map;
    loadCorridor(map);
    const POINT station = map.getChargingStationLocations().at(0);
    const POINT start{station.x, station.y - 2};
    Scooter scooter(1, start, &map);

    scooter.assignPath(straightPath(start, 2));
    scooter.update(); // speed 2: moves onto the station
    scooter.update(); // path done -> starts charging
    ASSERT_EQ(scooter.getPosition(), station);
    EXPECT_EQ(scooter.getState(), AGENTSTATE::CHARGING);

    for (int i = 0; i < 4 && scooter.getState() == AGENTSTATE::CHARGING; ++i)
    {
        scooter.update();
    }
    EXPECT_FLOAT_EQ(scooter.getBattery(), scooter.getMaxBattery());
    EXPECT_TRUE(scooter.isIdle());
}
