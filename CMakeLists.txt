cmake_minimum_required(VERSION 3.5)

project(otp-mfa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты
find_package(Qt5 REQUIRED COMPONENTS Widgets Multimedia Sql Svg Network REQUIRED)
find_package(ZXing REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(QREncode REQUIRED libqrencode)

# Добавляем директории включаемых файлов
include_directories(${QREncode_INCLUDE_DIRS})
include_directories("/home/oorlovsk/otp-mfa/include")
include_directories(${OpenCV_INCLUDE_DIRS})

# Источники для основного приложения
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
    encryption_utils.h
    encryption_utils.cpp
    master_password_dialog.h
    master_password_dialog.cpp
    master_password_dialog.ui
    settings_dialog.h
    settings_dialog.cpp
    settings_dialog.ui
    /home/oorlovsk/otp-mfa/include/qrcodegen.cpp
    /home/oorlovsk/otp-mfa/include/qrcodegen.hpp
)

# Создаём основной исполняемый файл
add_executable(otp-mfa
    ${PROJECT_SOURCES}
)

# Линкуем библиотеки для основного приложения
target_link_libraries(otp-mfa PRIVATE Qt5::Widgets Qt5::Multimedia Qt5::Sql Qt5::Svg Qt5::Network)
target_link_libraries(otp-mfa PRIVATE ZXing::ZXing)
target_link_libraries(otp-mfa PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(otp-mfa PRIVATE ${OpenCV_LIBS})

# Создаём отдельный исполняемый файл для тестирования
add_executable(otp_generator_test
    otp_generator_test.cpp
    otp_generator.cpp
    otp_generator.h
)

# Линкуем библиотеки для тестового приложения
target_link_libraries(otp_generator_test PRIVATE Qt5::Core)
target_link_libraries(otp_generator_test PRIVATE OpenSSL::Crypto)

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
