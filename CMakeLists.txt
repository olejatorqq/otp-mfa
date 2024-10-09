cmake_minimum_required(VERSION 3.5)

project(otp-mfa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(OpenSSL REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        otp_window.cpp
        otp_window.h
        otp_window.ui
        otp_generator.h
        otp_generator.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(otp-mfa
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        account_manager.h account_manager.cpp
        add_account.ui
        add_account_dialog.h add_account_dialog.cpp
    )
else()
    if(ANDROID)
        add_library(otp-mfa SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(otp-mfa
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Линкуем библиотеки Qt и OpenSSL
target_link_libraries(otp-mfa PRIVATE Qt${QT_VERSION_MAJOR}::Widgets OpenSSL::SSL OpenSSL::Crypto)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.otp-mfa)
endif()
set_target_properties(otp-mfa PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS otp-mfa
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(otp-mfa)
endif()
