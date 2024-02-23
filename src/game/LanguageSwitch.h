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

extern std::vector<Components<bool*>> radar_box_;
extern std::vector<Components<bool*>> skeleton_box_;
extern std::vector<Components<bool*>> setting_box_;

extern std::vector<ComponentsSlider<float*, float>> radar_slider_;
extern std::vector<ComponentsSlider<float*, float>> skeleton_slider_;
extern std::vector<ComponentsSlider<float*, float>> setting_slider_;
extern std::vector<ComponentsSlider<int*, int>> radar_slider_int_;
extern std::vector<ComponentsSlider<int*, int>> skeleton_slider_int_;
extern std::vector<ComponentsSlider<int*, int>> setting_slider_int_;

extern std::vector<Components<float*>> radar_color_;
extern std::vector<Components<float*>> skeleton_color_;
extern std::vector<Components<float*>> setting_color_;

static std::string array1[] = {"readConfig", "skeletonConfig", "setting"};
static std::string array2[] = {"box", "Slider", "SliderInt", "Color"};
static int nodalNumber = sizeof(array1) / sizeof(array1[0]);

static std::vector<std::vector<Components<bool*>>*> box = {&radar_box_, &skeleton_box_, &setting_box_};
static std::vector<std::vector<ComponentsSlider<float*, float>>*> slider = {&radar_slider_, &skeleton_slider_, &setting_slider_};
static std::vector<std::vector<ComponentsSlider<int*, int>>*> slider_int = {&radar_slider_int_, &skeleton_slider_int_, &setting_slider_int_};
static std::vector<std::vector<Components<float*>>*> color = {&radar_color_, &skeleton_color_, &setting_color_};

extern bool WriteToINI(const std::string& filename);
extern bool LoadSet();
extern bool SaveSet();

}  // namespace ComponentsLists
