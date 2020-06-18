add_executable(main main.cxx)

target_include_directories(main PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(main ${PROJECT_NAME})