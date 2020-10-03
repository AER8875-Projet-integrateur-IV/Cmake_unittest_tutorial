add_executable(${CMAKE_PROJECT_NAME} main.cpp)

add_subdirectory(InterestingDirectory)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Division)