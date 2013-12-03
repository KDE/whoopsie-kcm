configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(kcm_SRCS
    main.cpp
    Module.cpp
)

qt4_add_dbus_interface(kcm_SRCS whoopsie-preferences.xml WhoopsiePreferences)

kde4_add_ui_files(kcm_SRCS Module.ui)
kde4_add_plugin(kcm_whoopsie ${kcm_SRCS})

target_link_libraries(kcm_whoopsie
    ${KDE4_KDEUI_LIBRARY}
)

install(TARGETS kcm_whoopsie DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kcm_whoopsie.desktop DESTINATION ${SERVICES_INSTALL_DIR})
