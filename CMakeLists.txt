cmake_minimum_required(VERSION 3.15)
project(AsyncWebServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Boost REQUIRED)

find_package(Threads REQUIRED)


include_directories(${HEADERS_DIR})

set(SOURCES
    ${SOURCES_DIR}/HttpRequest.cpp
    ${SOURCES_DIR}/HttpResponse.cpp
    ${SOURCES_DIR}/HttpServer.cpp
    ${SOURCES_DIR}/Middleware.cpp
    ${SOURCES_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Boost::headers
    Threads::Threads
)

# Print status message
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Boost Include Dirs: ${Boost_INCLUDE_DIRS}")