#include <gtest/gtest.h>

#include "Headers/Map_Building/ProceduralMapGenerator.h"
#include "test_helpers.h"

TEST(FileMapLoader, ParsesAllCellTypesAndPointsOfInterest)
{
    Map map;
    loadMapFromRows(map, {
        "#####",
        "#B.S#",
        "#.#D#",
        "#X.D#",
        "#####",
    });

    EXPECT_EQ(map.getCell(0, 0), CELLTYPE::WALL);
    EXPECT_EQ(map.getCell(1, 2), CELLTYPE::ROAD);
    EXPECT_EQ(map.getCell(3, 1), CELLTYPE::EMPTY_SPACE);
    EXPECT_EQ(map.getBase(), (POINT{1, 1}));
    ASSERT_EQ(map.getChargingStationLocations().size(), 1u);
    EXPECT_EQ(map.getChargingStationLocations()[0], (POINT{1, 3}));
    EXPECT_EQ(map.getClientLocations().size(), 2u);
    EXPECT_TRUE(map.integrityCheck());
}

TEST(FileMapLoader, RejectsUnknownCharacter)
{
    Map map;
    EXPECT_EQ(thrownCode([&] { loadMapFromRows(map, {"###", "#?#", "###"}, "bad_char.txt"); }), 2);
}

TEST(FileMapLoader, RejectsShortLine)
{
    Map map;
    EXPECT_EQ(thrownCode([&] { loadMapFromRows(map, {"####", "#B#", "####"}, "short_line.txt"); }), 3);
}

TEST(FileMapLoader, RejectsTruncatedFile)
{
    const auto path = writeMapFile({"###", "#B#", "###"}, "truncated.txt");

    Map map;
    map.setRows(5); // claim more rows than the file has
    map.setCols(3);

    FileMapLoader loader;
    loader.setMapAdress(path.string());
    EXPECT_EQ(thrownCode([&] { loader.generateMap(map); }), 4);
}

TEST(MapIntegrity, FailsWhenClientIsWalledOff)
{
    Map map;
    loadMapFromRows(map, {
        "#####",
        "#B.##",
        "#..#D",
        "#####",
    }, "walled_client.txt");

    EXPECT_FALSE(map.integrityCheck());
}

TEST(ProceduralMapGenerator, ProducesValidMapWithRequestedObjects)
{
    for (int attempt = 0; attempt < 10; ++attempt)
    {
        Map map;
        map.setRows(21);
        map.setCols(21);

        ProceduralMapGenerator generator(/*clients=*/8, /*charging_stations=*/3, /*randomness=*/20);
        ASSERT_NO_THROW(generator.generateMap(map));

        EXPECT_TRUE(map.integrityCheck());
        EXPECT_EQ(map.getClientLocations().size(), 8u);
        EXPECT_EQ(map.getChargingStationLocations().size(), 3u);

        // The outer border must stay solid wall
        for (int j = 0; j < map.getCols(); ++j)
        {
            EXPECT_EQ(map.getCell(0, j), CELLTYPE::WALL);
            EXPECT_EQ(map.getCell(map.getRows() - 1, j), CELLTYPE::WALL);
        }
        for (int i = 0; i < map.getRows(); ++i)
        {
            EXPECT_EQ(map.getCell(i, 0), CELLTYPE::WALL);
            EXPECT_EQ(map.getCell(i, map.getCols() - 1), CELLTYPE::WALL);
        }
    }
}

TEST(ProceduralMapGenerator, ThrowsWhenMapIsTooSmallForObjects)
{
    Map map;
    map.setRows(5);
    map.setCols(5);

    ProceduralMapGenerator generator(/*clients=*/100, /*charging_stations=*/100, /*randomness=*/0);
    EXPECT_EQ(thrownCode([&] { generator.generateMap(map); }), 2);
}
