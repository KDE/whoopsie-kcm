project(kcm-whoopsie)

find_package(Qt4 4.7.0 REQUIRED)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-std=c++0x)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

add_subdirectory(src)
