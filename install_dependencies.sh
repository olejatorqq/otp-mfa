#!/bin/bash

# Обновляем список пакетов
echo "Обновление списка пакетов..."
sudo apt-get update

echo "Установка инструментов для сборки и CMake..."
sudo apt-get install -y build-essential cmake pkg-config

# Устанавливаем Qt5 и необходимые компоненты
echo "Установка Qt5 и необходимых компонентов..."
sudo apt-get install -y qt5-default qttools5-dev qt5-qmake qtbase5-dev qtbase5-dev-tools \
    libqt5multimedia5-dev libqt5multimedia5 libqt5sql5-dev libqt5sql5-sqlite libqt5svg5-dev libqt5network5t64 libqt5network5-dev

# Устанавливаем OpenSSL
echo "Установка OpenSSL..."
sudo apt-get install -y libssl-dev

echo "Все зависимости успешно установлены."