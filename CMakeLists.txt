cmake_minimum_required(VERSION 3.5)

project(otp-mfa VERSION 0.1 LANGUAGES CXX)

# Установка типа сборки по умолчанию на Release, если не задано
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Multimedia Sql Svg Network)

# Находим OpenSSL
find_package(OpenSSL REQUIRED)

# Находим PkgConfig (если используется, например, для дополнительных зависимостей)
find_package(PkgConfig REQUIRED)

# Добавляем директории включаемых файлов
include_directories(${CMAKE_SOURCE_DIR}/include)

# Список исходных файлов проекта
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
    encryption_utils.h
    encryption_utils.cpp
    master_password_dialog.h
    master_password_dialog.cpp
    master_password_dialog.ui
    settings_dialog.h
    settings_dialog.cpp
    settings_dialog.ui
)

# Создаём основной исполняемый файл
add_executable(otp-mfa
    ${PROJECT_SOURCES}
    recovery_manager.h recovery_manager.cpp

)

# Линкуем необходимые библиотеки для основного приложения
target_link_libraries(otp-mfa PRIVATE
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::Sql
    Qt5::Svg
    Qt5::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Создаём отдельный исполняемый файл для тестирования генератора OTP
add_executable(otp_generator_test
    otp_generator_test.cpp
    otp_generator.cpp
    otp_generator.h
)

# Линкуем необходимые библиотеки для тестового приложения
target_link_libraries(otp_generator_test PRIVATE
    Qt5::Core
    OpenSSL::Crypto
)

# Устанавливаем свойства для macOS и Windows
set_target_properties(otp-mfa PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Подключаем GNUInstallDirs для стандартных директорий установки
include(GNUInstallDirs)

# Правила установки для основного исполняемого файла
install(TARGETS otp-mfa
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}       # Для Linux: bin/
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}        # Для macOS: bin/
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}       # Для Linux: lib/
)

# (Опционально) Правила установки для тестового исполняемого файла
install(TARGETS otp_generator_test
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/tests
)

# Установка Qt платформенных плагинов
# Получаем путь к Qt плагинам относительно местоположения Qt5::Widgets
get_target_property(Qt5Widgets_location Qt5::Widgets LOCATION)
get_filename_component(Qt5Widgets_dir ${Qt5Widgets_location} DIRECTORY)
set(Qt5_plugins_dir "${Qt5Widgets_dir}/../plugins")

# Устанавливаем папку platforms
install(DIRECTORY "${Qt5_plugins_dir}/platforms"
    DESTINATION plugins/platforms
    FILES_MATCHING PATTERN "libqxcb.so*"
)

# Установка файлов данных (например, база данных)
# install(FILES "${CMAKE_SOURCE_DIR}/accounts.db"
#     DESTINATION data
# )

# Директории (например, translations), установите их аналогично
# install(DIRECTORY "${CMAKE_SOURCE_DIR}/translations/"
#     DESTINATION translations
#     FILES_MATCHING PATTERN "*.qm"
# )
