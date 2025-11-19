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
    resource.qrc

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
    source/util/AppManager/apimanager.cpp
    source/util/AppManager/apimanager.h
    source/util/AppManager/appmanager.cpp
    source/util/AppManager/appmanager.h
    source/util/AppManager/databasemanager.cpp
    source/util/AppManager/databasemanager.h
    source/dashboard/dashboard.h source/dashboard/dashboard.cpp source/dashboard/dashboard.ui
    source/dashboard/senddialog.h source/dashboard/senddialog.cpp source/dashboard/senddialog.ui
    source/dashboard/depositdialog.h source/dashboard/depositdialog.cpp source/dashboard/depositdialog.ui

    tab.h tab.cpp
    root.h root.cpp root.ui
    source/chart/detaildialog.h source/chart/detaildialog.cpp source/chart/detaildialog.ui


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
