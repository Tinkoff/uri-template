cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(URITEMPLATE_VERSION_MAJOR "1")
set(URITEMPLATE_VERSION_MINOR "1")
set(URITEMPLATE_VERSION_RELEASE "1")
set(URITEMPLATE_VERSION_STRING "${URITEMPLATE_VERSION_MAJOR}.${URITEMPLATE_VERSION_MINOR}.${URITEMPLATE_VERSION_RELEASE}")
set(URITEMPLATE_LIB_VERSION ${URITEMPLATE_VERSION_STRING})
set(URITEMPLATE_LIB_SOVERSION "${URITEMPLATE_VERSION_MAJOR}")
mark_as_advanced(URITEMPLATE_VERSION_MAJOR URITEMPLATE_VERSION_MINOR URITEMPLATE_VERSION_RELEASE URITEMPLATE_VERSION_STRING URITEMPLATE_LIB_VERSION URITEMPLATE_LIB_SOVERSION)

project(uri-template VERSION "${URITEMPLATE_VERSION_STRING}" LANGUAGES CXX)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra \
    -Wunused-variable -Wunused-parameter \
    -Wunused-function -Wunused -Wno-system-headers \
    -Wno-deprecated -Woverloaded-virtual -Wwrite-strings")

# compile in release with debug info mode by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

option(BUILD_SHARED_LIBS "Build shared library" ON)

# If uri-template is being bundled in another project, we don't want to
# install anything.
if(NOT DEFINED URITEMPLATE_BUNDLED_MODE)
  if("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    set(URITEMPLATE_BUNDLED_MODE OFF)
  else()
    set(URITEMPLATE_BUNDLED_MODE ON)
  endif()
endif()
set(URITEMPLATE_BUNDLED_MODE ${URITEMPLATE_BUNDLED_MODE} CACHE BOOL "" FORCE)
mark_as_advanced(URITEMPLATE_BUNDLED_MODE)

# Allow people to choose whether to build shared or static libraries
# via the BUILD_SHARED_LIBS option unless we are in bundled mode, in
# which case we always use static libraries.
include(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON "NOT URITEMPLATE_BUNDLED_MODE" OFF)

set(SOURCES
    src/Expander.cpp
    src/Matcher.cpp
    src/Modifier.cpp
    src/Operator.cpp
    src/Parser.cpp
    src/Template.cpp
    src/Variable.cpp)

# liburi-template
add_library(uri-template ${SOURCES})
add_library(${PROJECT_NAME}::uri-template ALIAS uri-template)

target_include_directories(uri-template
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(uri-template PROPERTIES
    SOVERSION "${URITEMPLATE_VERSION_STRING}"
    VERSION "${URITEMPLATE_VERSION_STRING}"
)

if(NOT URITEMPLATE_BUNDLED_MODE)
    include(GNUInstallDirs)

    install(TARGETS uri-template
        EXPORT uritemplateTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    install(DIRECTORY include/uri-template
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    include(CMakePackageConfigHelpers)

    set(uritemplate_VERSION_CONFIG "${PROJECT_BINARY_DIR}/uritemplateConfigVersion.cmake")
    set(uritemplate_PROJECT_CONFIG "${PROJECT_BINARY_DIR}/uritemplateConfig.cmake")
    set(uritemplate_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/uritemplate/")
    write_basic_package_version_file(${uritemplate_VERSION_CONFIG}
        VERSION ${URITEMPLATE_VERSION_STRING}
        COMPATIBILITY AnyNewerVersion)
    configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/uritemplateConfig.cmake.in
        ${uritemplate_PROJECT_CONFIG}
        INSTALL_DESTINATION ${uritemplate_CONFIG_INSTALL_DIR})


    install(FILES ${uritemplate_PROJECT_CONFIG} ${uritemplate_VERSION_CONFIG}
        DESTINATION ${uritemplate_CONFIG_INSTALL_DIR})
    install(EXPORT uritemplateTargets
        DESTINATION ${uritemplate_CONFIG_INSTALL_DIR}
        NAMESPACE ${PROJECT_NAME}::)
endif(NOT URITEMPLATE_BUNDLED_MODE)

set(uritemplate_TARGETS_CONFIG "${PROJECT_BINARY_DIR}/uritemplateTargets.cmake")
export(EXPORT uritemplateTargets
    FILE ${uritemplate_TARGETS_CONFIG}
    NAMESPACE ${PROJECT_NAME}::)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

    include(CTest)
    add_subdirectory(test)
endif()
