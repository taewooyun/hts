cmake_minimum_required(VERSION 3.19)
project(project1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Qt6 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup()

qt_add_executable(project1
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    root.cpp
    root.h
    source/home/home.cpp
    source/home/home.h
    source/home/home.ui
    source/login/login.cpp
    source/login/login.h
    source/login/login.ui
    source/signup/signup.cpp
    source/signup/signup.h
    source/signup/signup.ui
    source/chart/chart.cpp
    source/chart/chart.h
    source/chart/chart.ui
    source/chart/chart_model.h
    resource.qrc
)

target_link_libraries(project1
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)
target_link_libraries(project1 PRIVATE Qt6::Charts)
target_link_libraries(project1 PRIVATE Qt6::Network)

include(GNUInstallDirs)

install(TARGETS project1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET project1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
