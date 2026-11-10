#include <gtest/gtest.h>

#include <cstdlib>

#include "Headers/Pathfinder.h"
#include "test_helpers.h"

namespace
{
    // B at (1,1), D at (1,5) with a wall between them; ground units must detour via row 3.
    const std::vector<std::string> kDetourMap = {
        "#######",
        "#B.#.D#",
        "#..#..#",
        "#.....#",
        "#######",
    };

    bool isAdjacent(POINT a, POINT b)
    {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) == 1;
    }
}

TEST(Pathfinder, GroundPathAvoidsWallsAndIsShortest)
{
    Map map;
    loadMapFromRows(map, kDetourMap, "detour.txt");

    const POINT start{1, 1};
    const POINT end{1, 5};
    const auto path = Pathfinder::findPath(start, end, &map, MOVEMENTMODE::GROUND);

    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.back(), end);
    EXPECT_EQ(path.size(), 8u); // down 2, right 4, up 2

    POINT previous = start;
    for (const POINT& step : path)
    {
        EXPECT_TRUE(isAdjacent(previous, step));
        EXPECT_NE(map.getCell(step.x, step.y), CELLTYPE::WALL);
        previous = step;
    }
}

TEST(Pathfinder, AirPathFliesOverWalls)
{
    Map map;
    loadMapFromRows(map, kDetourMap, "detour_air.txt");

    const auto path = Pathfinder::findPath({1, 1}, {1, 5}, &map, MOVEMENTMODE::AIR);

    ASSERT_EQ(path.size(), 4u); // straight line across the wall
    EXPECT_EQ(path.back(), (POINT{1, 5}));
}

TEST(Pathfinder, ReturnsEmptyWhenUnreachableOnGround)
{
    Map map;
    loadMapFromRows(map, {
        "#####",
        "#B#D#",
        "#####",
    }, "unreachable.txt");

    EXPECT_TRUE(Pathfinder::findPath({1, 1}, {1, 3}, &map, MOVEMENTMODE::GROUND).empty());
    EXPECT_FALSE(Pathfinder::findPath({1, 1}, {1, 3}, &map, MOVEMENTMODE::AIR).empty());
}

TEST(Pathfinder, PathToSelfIsEmpty)
{
    Map map;
    loadMapFromRows(map, kDetourMap, "self.txt");

    EXPECT_TRUE(Pathfinder::findPath({1, 1}, {1, 1}, &map, MOVEMENTMODE::GROUND).empty());
}
