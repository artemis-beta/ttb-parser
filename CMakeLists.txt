CMAKE_MINIMUM_REQUIRED( VERSION 3.17 )

project( TTBParser VERSION 0.0.0 LANGUAGES CXX )

# assume built-in pthreads on MacOS
IF(APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
file( GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.cxx )
list( REMOVE_ITEM SRCS ${CMAKE_SOURCE_DIR}/src/main.cxx )

find_package( Boost COMPONENTS system filesystem date_time REQUIRED )

add_library( ${PROJECT_NAME} ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${LIBS} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include
                            ${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} )

add_subdirectory(tests)