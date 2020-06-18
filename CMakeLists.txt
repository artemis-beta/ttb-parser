CMAKE_MINIMUM_REQUIRED( VERSION 3.17 )

set(PROJECT_NAME TTBParser)

project(${PROJECT_NAME}  VERSION 0.0.0 LANGUAGES CXX )

add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable Coverage" OFF)

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
set( CMAKE_CXX_FLAGS "-DROOT_DIR=\\\"${CMAKE_SOURCE_DIR}\\\"")

if(CODE_COVERAGE)
    target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
    )

	find_program( LCOV_PATH  NAMES lcov lcov.bat lcov.exe lcov.perl)
	find_program( GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat )
	find_program( GCOV_PATH gcov )
	
	set(LCOV_EXCLUDES "build/*" "/usr*")
	
	add_custom_target(coverage
	
	        # Cleanup lcov
	        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -directory . -b ${PROJECT_SOURCE_DIR} --zerocounters
	
	        # Create baseline to make sure untouched files show up in the report
		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -c -i -d . -b ${PROJECT_SOURCE_DIR} -o ${PROJECT_NAME}.base
	
	        # Run executables
		COMMAND ${CMAKE_SOURCE_DIR}/build/bin/Covid19EERAModel
		COMMAND ${CMAKE_SOURCE_DIR}/build/bin/Covid19EERAModel-unit_tests
	
	        # Capturing lcov counters and generating report
		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . -b ${PROJECT_SOURCE_DIR} --capture --output-file ${PROJECT_NAME}.capture
	
	        # add baseline counters
		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -a ${PROJECT_NAME}.base -a ${PROJECT_NAME}.capture --output-file ${PROJECT_NAME}.total
	
	        # filter collected data to final coverage report and merge outputs
		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --remove ${PROJECT_NAME}.total ${LCOV_EXCLUDES} --output-file ${PROJECT_NAME}.info
	
	        # Generate HTML output
		
		COMMAND ${GENHTML_PATH} -o ${PROJECT_NAME}_coverage ${PROJECT_NAME}.info
	
	        # Set output files as GENERATED (will be removed on 'make clean')
	        BYPRODUCTS
	            ${PROJECT_NAME}.base
	            ${PROJECT_NAME}.capture
	            ${PROJECT_NAME}.total
	            ${PROJECT_NAME}.info
	            ${PROJECT_NAME}  # report directory
	
	        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	        VERBATIM # Protect arguments to commands
	        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
	)
endif(CODE_COVERAGE)

find_package( Boost COMPONENTS system filesystem date_time REQUIRED )

add_library( ${PROJECT_NAME} ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${LIBS} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include
                            ${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} )

add_subdirectory(tests)