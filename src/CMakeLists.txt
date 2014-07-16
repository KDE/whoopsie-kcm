configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(kcm_SRCS
    main.cpp
    Module.cpp
)

qt5_add_dbus_interface(kcm_SRCS whoopsie-preferences.xml WhoopsiePreferences)

qt5_wrap_ui(kcm_SRCS Module.ui)
add_library(kcm_whoopsie MODULE ${kcm_SRCS})

target_link_libraries(kcm_whoopsie
    Qt5::DBus
    KF5::Auth
    KF5::ConfigWidgets
    KF5::I18n
    KF5::Service)

install(TARGETS kcm_whoopsie DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kcm_whoopsie.desktop DESTINATION ${SERVICES_INSTALL_DIR})
