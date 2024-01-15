cmake_minimum_required(VERSION 3.9)
project(PlugInUI VERSION 1.0.7 LANGUAGES C CXX)


option(UPDATE_EXE "Whether to start the upgrade program" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(/utf-8)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /we4013 /we4431 /we4133 /we4716 /we6244 /we6246 /we4457 /we4456 /we4172 /we4700 /we4477 /we4018 /we4047")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /we4013 /we4431 /we4133 /we4716 /we6244 /we6246 /we4457 /we4456 /we4172 /we4700 /we4477 /we4018 /we4047")
endif()
if (MSVC)
    # 使用Microsoft Visual C++编译器时
    add_compile_options(/wd4200)  # 禁用 warning C4200
endif()

# find_package(OpenMP REQUIRED)
# if(OpenMP_FOUND)
#     message(STATUS "found openmp") 
#     set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}  ${OPENMP_C_FLAGS})
#     set(CMAKE_CXX_FLAGS "-std=c++11" ${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS})
#     add_definitions(-D_OPENMP)
# else()
#     message(FATAL_ERROR "openmp not found!")
# endif()

configure_file(${CMAKE_SOURCE_DIR}/config/version.h.in ${CMAKE_SOURCE_DIR}/include/version.h)
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/MemProcFS/include/)
include_directories(${CMAKE_SOURCE_DIR}/lib/stb_image/)
include_directories(${CMAKE_SOURCE_DIR}/lib/ImGui/)
include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include/)
include_directories(${CMAKE_SOURCE_DIR}/lib/glad/include/)

aux_source_directory(${CMAKE_SOURCE_DIR}/src/UI/ SRC_UI_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/game/ SRC_GAME_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/vmm/ SRC_VMM_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/ue4math/ SRC_UE4MATH_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utils/ SRC_UTILS_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/lib/ImGui/ SRC_IMGUI_FILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/lib/glad/ SRC_GLAD_FILES)

link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib/MemProcFS/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib/ImGui)
link_directories(${CMAKE_SOURCE_DIR}/lib/glfw)


set(SRC_FILES  ${SRC_IMGUI_FILES} ${SRC_GAME_FILES} ${SRC_UTILS_FILES} ${SRC_UE4MATH_FILES}  ${SRC_UI_FILES} ${SRC_VMM_FILES} ${SRC_GLAD_FILES})

add_executable(${PROJECT_NAME}   ${SRC_FILES} )
target_link_libraries(${PROJECT_NAME}  legacy_stdio_definitions vmm leechcore d3d11 DirectXTex glfw3)

if(UPDATE_EXE)
include_directories(${CMAKE_SOURCE_DIR}/lib/zlib/include/)
link_directories(${CMAKE_SOURCE_DIR}/lib/zlib/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/curl/include/)
link_directories(${CMAKE_SOURCE_DIR}/lib/curl/lib)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/ProgramUpdate/ UPDATE_SRCS)
add_executable(UPdate   ${UPDATE_SRCS} )
target_link_libraries(UPdate libcurl bzip2 libminizip)
endif()



