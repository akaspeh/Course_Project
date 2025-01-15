cmake_minimum_required(VERSION 3.23)
project(Course_project)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Источники
set(SOURCE_FILES
        src/main.cpp
        TracyClient.cpp
        src/Server.cpp
        src/Server.h
        src/data_structure/ThreadPool.cpp
        src/data_structure/ThreadPool.h
        src/data_structure/TaskQueue.h
        src/data_structure/ThreadSafeHashMap.h src/data_structure/InvertedIndex.cpp src/data_structure/InvertedIndex.h src/FileStorageManager.cpp src/FileStorageManager.h src/utils/Request.h src/utils/Response.h src/utils/RequestType.h src/utils/Document.h)

# Опции компиляции и линковки
add_compile_definitions(TRACY_ENABLE)
# set(CMAKE_EXE_LINKER_FLAGS "-static")


# Добавление исполняемого файла
add_executable(Course_project ${SOURCE_FILES})

# Линковка библиотек
target_link_libraries(Course_project PRIVATE ws2_32 dbghelp stdc++fs)


