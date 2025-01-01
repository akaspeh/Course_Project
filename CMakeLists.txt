cmake_minimum_required(VERSION 3.23)
project(Course_project)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Источники
set(SOURCE_FILES
        main.cpp
        TracyClient.cpp
        Server.cpp
        Server.h
        data_structure/ThreadPool.cpp
        data_structure/ThreadPool.h
        data_structure/TaskQueue.h
        )

# Опции компиляции и линковки
add_compile_definitions(TRACY_ENABLE)
# set(CMAKE_EXE_LINKER_FLAGS "-static")


# Добавление исполняемого файла
add_executable(Course_project ${SOURCE_FILES})

# Линковка библиотек
target_link_libraries(Course_project PRIVATE ws2_32 dbghelp)


