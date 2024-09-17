cmake_minimum_required(VERSION 3.0)
project(CellGame)

# Указание стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXEC src/main.cpp src/screen.cpp src/celltable.cpp)

# Поиск пакета SFML
# Укажите SFML_DIR, если SFML установлена в нестандартном месте
find_package(SFML 2.6 COMPONENTS graphics REQUIRED) # дополните нужными вам модулями

# Добавление исполняемого файла
add_executable(CellGame ${EXEC})

# Связывание с библиотеками SFML
target_link_libraries(CellGame sfml-graphics)  # дополните нужными вам модулями
include_directories(./include)

# Если проект собирается на Windows, копируем DLL-файлы в выходную папку
if(WIN32)
    # Предполагается, что DLL-файлы находятся в подпапке 'bin' в папке установки SFML
    # Укажите правильный путь к DLL-файлам SFML
	add_custom_command(TARGET CellGame POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
					   ${SFML_DIR}/bin $<TARGET_FILE_DIR:CellGame>)
endif()
