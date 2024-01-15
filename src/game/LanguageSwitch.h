#pragma once
#include <unordered_map>
#include "Data.h"
#include <vector>
#include "imgui.h"
#include "utils/INIReader.h"
#include <cstdio>
#include <fstream>

namespace ComponentsLists {

template <typename T>
struct Components {
    std::string ChineseName;
    std::string EnglishName;
    T value;
};

template <typename T, typename U>
struct ComponentsSlider {
    std::string ChineseName;
    std::string EnglishName;
    T value;
    U min;
    U max;
};

extern std::vector<Components<bool*>> ShboxLists;

extern std::vector<ComponentsSlider<float*, float>> SliderLists;
extern std::vector<ComponentsSlider<int*, int>> SliderIntLists;

extern std::vector<Components<float*>> ColorList;

extern bool WriteToINI(const std::string& filename);
extern bool LoadSet();
extern bool SaveSet();

}  // namespace ComponentsLists
