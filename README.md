# otp-mfa

**Язык программирования:** C++  
**Среда разработки:** Qt 5.15.13

## Зависимости

Для сборки и запуска **OTP-MFA** необходимы следующие зависимости:

### Компилятор и инструменты сборки:

- **Qt: версия 5.15.13**
- **CMake: версия 3.28.3**  
- **GCC/G++: версия 13.3.0**  

### Qt библиотеки:
- **libQt5Core.so.5** — базовый модуль Qt (версия 5.15.13)
- **libQt5Gui.so.5** — модуль для графического интерфейса (версия 5.15.13)
- **libQt5Widgets.so.5** — модуль для виджетов GUI (версия 5.15.13)
- **libQt5Network.so.5** — модуль для сетевых операций (версия 5.15.13)
- **libQt5Sql.so.5** — модуль для работы с базами данных (версия 5.15.13)

### Криптографические библиотеки:
- **libcrypto.so.3** — библиотека OpenSSL для криптографических операций (версия 3.0.13)

### Системные библиотеки:
- **libstdc++.so.6** — стандартная библиотека C++ (версия 14.2.0)
- **libgcc_s.so.1** — поддержка исключений и базовые операции для GCC (версия 14.2.0)
- **libm.so.6** — математическая библиотека для операций с плавающей точкой
- **libc.so.6** — стандартная библиотека C для системных вызовов

## Установка (Через установщик)

### 1. Клонирование установщика
```bash
wget https://github.com/olejatorqq/otp-mfa/releases/download/v0.2.0/OTPMFA_Installer.run
```

### 2. Запуск установщика
```bash
chmod +x ./OTPMFA_Installer.run
./OTPMFA_Installer.run
```

### 3. Запуск приложения
Перейдите в директорию, которую указали при установке, затем
```bash
cd bin
./otp-mfa
```

### (Опционально) создание установщика

### 1. Установка QT Installer Framework:
[Ссылка на скачивание](https://download.qt.io/official_releases/qt-installer-framework/4.8.1/)

После сборки проекта (см. п.4 Установка (Ручной вариант))

### 2. Создание установщик (после сборки проекта)

```bash
cd build/deploy/
rm -rf ../../installer_files/packages/com.olejatorqq.otp-mfa/data/bin
cp -r  * ../../installer_files/packages/com.olejatorqq.otp-mfa/data/
cd ../../installer_files/
binarycreator   --offline-only   --config config/config.xml   --packages packages   OTPMFA_Installer.run
```
### 3. Запуск установщика

```bash
./OTPMFA_Installer.run
```

## Установка (Ручной вариант)

### 1. Клонирование репозитория

```bash
git clone https://github.com/oorlovsk/otp-mfa.git
cd otp-mfa
```

### 2. Установка зависимостей
Выполните следующие команды:

*Установку необходимых пакетов можно выполнить путем запуска install_dependencies.sh*
```bash
sudo apt update
chmod +x ./install_dependencies.sh
./install_dependencies.sh
```

### 3. Сборка проекта
Выполните следующие команды:

```bash
chmod +x ./start.sh
./start.sh
```

### 4. Установка приложения
Выполните следующие команды:

```bash
sudo make install
```

### 5. Запуск приложения
Запустить приложение можно из терминала или окна приложений. (Пример запуска через терминал)

```bash
./deploy/bin/otp-mfa
```