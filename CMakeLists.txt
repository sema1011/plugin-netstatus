cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)

project(netstatus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Xml DBus Network Test)
qt_standard_project_setup()

# LXQt
find_package(lxqt REQUIRED)

# KF6 NetworkManagerQt
find_package(KF6NetworkManagerQt REQUIRED)

# Set up source files
set(HEADERS
    src/netstatus.h
    src/netstatuswidget.h
    src/netstatussettings.h
    src/netstatussettingsdialog.h
)

set(SOURCES
    netstatus-plugin.cpp
    src/netstatus.cpp
    src/netstatuswidget.cpp
    src/netstatussettings.cpp
    src/netstatussettingsdialog.cpp
)

set(UI_FILES)

# Libraries to link
set(LIBRARIES
    Qt6::Widgets
    Qt6::Xml
    Qt6::DBus
    Qt6::Network
    KF6::NetworkManagerQt
    lxqt
)

# Build plugin as shared library
add_library(${PROJECT_NAME} MODULE
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lxqt-panel"
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lxqt-panel"
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/lxqt-panel"
)

# Install desktop file (install as .desktop, not .desktop.in)
install(FILES resources/netstatus.desktop.in
    RENAME netstatus.desktop
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/lxqt/lxqt-panel"
)

# Auto-compile all .ts files to .qm files for translations
file(GLOB TRANSLATION_TS_FILES "${CMAKE_SOURCE_DIR}/translations/*.ts")
set(TRANSLATION_QM_FILES "")
foreach(ts_file ${TRANSLATION_TS_FILES})
    get_filename_component(ts_name ${ts_file} NAME_WE)  # e.g. "ru" from "ru.ts"
    
    # Skip template files (they are just examples, not real translations)
    if(ts_name STREQUAL "template")
        continue()
    endif()
    
    qt_add_translation(QM_FILES ${ts_file})
    list(APPEND TRANSLATION_QM_FILES ${QM_FILES})
endforeach()

# Create a target to build all translations
if(TRANSLATION_QM_FILES)
    add_custom_target(translations ALL DEPENDS ${TRANSLATION_QM_FILES})
endif()

# Install compiled .qm files
if(TRANSLATION_QM_FILES)
    install(FILES ${TRANSLATION_QM_FILES}
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/lxqt/translations/lxqt-panel/netstatus"
    )
endif()

# Tests
enable_testing()
if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    
    add_executable(tst_netstatus
        tests/tst_netstatus.cpp
        tests/mock_plugin_settings.h
        tests/test_settings.cpp
        src/netstatuswidget.cpp
    )
    
    target_link_libraries(tst_netstatus PRIVATE
        Qt6::Widgets
        Qt6::Test
        Qt6::DBus
        KF6::NetworkManagerQt
        lxqt
    )
    
    target_include_directories(tst_netstatus PRIVATE ${CMAKE_SOURCE_DIR}/src)
    
    add_test(NAME tst_netstatus COMMAND tst_netstatus)
    
    # Set test environment - need DBus session for NetworkManagerQt
    set_tests_properties(tst_netstatus PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()
