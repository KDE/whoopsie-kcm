project(kcm-whoopsie)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.14 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 5.0.0 CONFIG REQUIRED DBus)

find_package(KF5Auth 5.0.0)
find_package(KF5ConfigWidgets 5.0.0)
find_package(KF5Service 5.0.0)

# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm-whoopsie\")

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
