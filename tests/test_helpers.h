#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "Headers/Map_Building/FileMapLoader.h"
#include "Headers/Map_Building/Map.h"

// Writes the given rows to a temporary map file and loads them into `map`
// through the real FileMapLoader, so tests exercise the same parsing path as the app.
inline std::filesystem::path writeMapFile(const std::vector<std::string>& rows, const std::string& name)
{
    const auto path = std::filesystem::temp_directory_path() / ("fancourier_" + name);
    std::ofstream out(path, std::ios::binary);
    for (const auto& row : rows)
    {
        out << row << '\n';
    }
    return path;
}

inline void loadMapFromRows(Map& map, const std::vector<std::string>& rows, const std::string& name = "test_map.txt")
{
    const auto path = writeMapFile(rows, name);

    map.setRows(static_cast<int>(rows.size()));
    map.setCols(rows.empty() ? 0 : static_cast<int>(rows[0].size()));

    FileMapLoader loader;
    loader.setMapAdress(path.string());
    loader.generateMap(map);
}

// Runs `fn` and returns the int error code it throws, or -1 if it does not throw.
template <typename Fn>
int thrownCode(Fn&& fn)
{
    try
    {
        fn();
    }
    catch (int code)
    {
        return code;
    }
    return -1;
}
