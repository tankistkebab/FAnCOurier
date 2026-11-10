#include <gtest/gtest.h>

#include "Headers/CustomAgents.h"
#include "Headers/HiveMind.h"
#include "test_helpers.h"

namespace
{
    void loadSmallCity(Map& map)
    {
        loadMapFromRows(map, {
            "#######",
            "#B...D#",
            "#.....#",
            "#S...D#",
            "#######",
        }, "small_city.txt");
    }
}

TEST(HiveMind, AssignsPendingPackageToIdleAgent)
{
    Map map;
    loadSmallCity(map);
    Robot robot(1, map.getBase(), &map);
    std::vector<Agent*> fleet{&robot};
    std::vector<Package> packages;
    packages.emplace_back(1, POINT{1, 5}, 500, 0, 20);

    HiveMind hive(&map, fleet, packages);
    hive.update(0);

    EXPECT_EQ(packages[0].status, PACKAGESTATUS::ASSIGNED);
    EXPECT_EQ(packages[0].assignedAgentId, robot.getId());
    EXPECT_EQ(robot.getState(), AGENTSTATE::MOVING);
    EXPECT_EQ(robot.getFinalDestination(), (POINT{1, 5}));
}

TEST(HiveMind, MarksPackageDeliveredWhenAgentArrives)
{
    Map map;
    loadSmallCity(map);
    Robot robot(1, map.getBase(), &map);
    std::vector<Agent*> fleet{&robot};
    std::vector<Package> packages;
    packages.emplace_back(1, POINT{1, 5}, 500, 0, 20);

    HiveMind hive(&map, fleet, packages);
    for (int tick = 0; tick < 10 && packages[0].status != PACKAGESTATUS::DELIVERED; ++tick)
    {
        hive.update(tick);
        robot.update();
    }

    EXPECT_EQ(packages[0].status, PACKAGESTATUS::DELIVERED);
}

TEST(HiveMind, ExpiresPackagesPastDeadline)
{
    Map map;
    loadSmallCity(map);
    std::vector<Agent*> fleet; // nobody available to deliver
    std::vector<Package> packages;
    packages.emplace_back(1, POINT{1, 5}, 500, /*creation=*/0, /*deadline=*/5);

    HiveMind hive(&map, fleet, packages);
    hive.update(5);
    EXPECT_EQ(packages[0].status, PACKAGESTATUS::PENDING);

    hive.update(6);
    EXPECT_EQ(packages[0].status, PACKAGESTATUS::EXPIRED);
}

TEST(HiveMind, PrefersCheapAgentForNonUrgentPackage)
{
    Map map;
    loadSmallCity(map);
    Drone drone(1, map.getBase(), &map);
    Robot robot(2, map.getBase(), &map);
    std::vector<Agent*> fleet{&drone, &robot};
    std::vector<Package> packages;
    packages.emplace_back(1, POINT{3, 5}, 500, 0, /*deadline=*/100);

    HiveMind hive(&map, fleet, packages);
    hive.update(0);

    EXPECT_EQ(packages[0].assignedAgentId, robot.getId());
    EXPECT_TRUE(drone.isIdle());
}
