cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message("Release Build")
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
else(CMAKE_BUILD_TYPE MATCHES Release)
	message("Debug Build")
	message("Debug Build")
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
endif(CMAKE_BUILD_TYPE MATCHES Release)

project(Projet CXX C)
add_executable(Projet include/CLI11.hpp src/helpers.cpp include/helpers.h src/Camera.cpp include/Camera.h include/CameraSettings.h src/main.cpp src/streamMain.cpp src/watchMain.cpp src/recordMain.cpp src/morseMain.cpp src/SongParser.cpp src/PWMSongParser.cpp include/constants.h)

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(MSVC)

target_include_directories(Projet PUBLIC $(CMAKE_CURRENT_SOURCE_DIR)/include)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(Projet PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(Projet "${CMAKE_THREAD_LIBS_INIT}")
endif()

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(Projet ${OpenCV_LIBRARIES})
else(OpenCV_FOUND)
    message(FATAL ERROR "Librarie OpenCV introuvable!")
endif(OpenCV_FOUND)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	find_package(PkgConfig)
	if(PkgConfig_FOUND)
		pkg_check_modules(ZBar IMPORTED_TARGET zbar)
		target_link_libraries(Projet PkgConfig::ZBar)
	else(PkgConfig_FOUND)
		message(FATAL ERROR "Librarie PkgConfig introuvable!")
	endif(PkgConfig_FOUND)
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
		include_directories(${ZBARCV_SOURCE_DIR})
		target_link_libraries(Projet ${ZBARCV_SOURCE_LIBRARIES} zbar)
	endif ()
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_INPUT ${SOURCE_FILES})
	set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	add_custom_command(
			OUTPUT ${DOXYGEN_OUTPUT}
			COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
			COMMAND ${CMAKE_COMMAND} -E echo "Done."
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			DEPENDS ${DOXYGEN_INPUT}
	)

	add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

	add_custom_target(apidoc_forced
			COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
			COMMAND ${CMAKE_COMMAND} -E echo "Done."
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
else(DOXYGEN_FOUND)
	message(FATAL ERROR "Librarie Doxygen introuvable!")
endif(DOXYGEN_FOUND)