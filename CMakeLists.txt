cmake_minimum_required(VERSION 3.5)

project(otp-mfa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно используем Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(ZXing REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Qt5 COMPONENTS Multimedia MultimediaWidgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    otp_window.cpp
    otp_window.h
    otp_window.ui
    otp_generator.h
    otp_generator.cpp
    account_manager.h
    account_manager.cpp
    add_account.ui
    add_account_dialog.h
    add_account_dialog.cpp
    qr_generator.h
    qr_generator.cpp
    /home/oorlovsk/otp-mfa/include/qrcodegen.cpp
    /home/oorlovsk/otp-mfa/include/qrcodegen.hpp
)

# Конфигурация сборки для Qt5
if(ANDROID)
    add_library(otp-mfa SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(otp-mfa
        ${PROJECT_SOURCES}
        encryption_utils.h encryption_utils.cpp
        master_password_dialog.h master_password_dialog.cpp
        master_password_dialog.ui

    )
endif()

# Линкуем необходимые библиотеки
target_link_libraries(otp-mfa PRIVATE Qt5::Widgets)
target_link_libraries(otp-mfa PRIVATE ZXing::ZXing)
target_link_libraries(otp-mfa PRIVATE Qt5::Multimedia)
target_link_libraries(otp-mfa PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(otp-mfa PRIVATE Qt5::Multimedia Qt5::MultimediaWidgets)
target_link_libraries(otp-mfa PRIVATE ${QREncode_LIBRARIES})
target_link_libraries(otp-mfa PRIVATE Qt5::Widgets ZXing::ZXing Qt5::Svg)
target_link_libraries(otp-mfa PRIVATE ZXing)
target_link_libraries(otp-mfa PRIVATE ${OpenCV_LIBS})
target_link_libraries(otp-mfa PRIVATE Qt5::Widgets Qt5::Sql)
target_link_libraries(otp-mfa PRIVATE OpenSSL::SSL OpenSSL::Crypto)

pkg_check_modules(QREncode REQUIRED libqrencode)
include_directories(${QREncode_INCLUDE_DIRS})
include_directories("/home/oorlovsk/otp-mfa/include")

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# Свойства для macOS и Windows
set_target_properties(otp-mfa PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Инсталляция
include(GNUInstallDirs)
install(TARGETS otp-mfa
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
