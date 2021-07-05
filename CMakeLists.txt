cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

set(URITEMPLATE_VERSION_MAJOR "1")
set(URITEMPLATE_VERSION_MINOR "2")
set(URITEMPLATE_VERSION_RELEASE "0")
set(URITEMPLATE_SUMMARY "C++ library")
set(URITEMPLATE_REPOSITORY_URL "https://github.com/TinkoffCreditSystems/uri-template")
set(URITEMPLATE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(URITEMPLATE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(URITEMPLATE_VERSION_STRING "${URITEMPLATE_VERSION_MAJOR}.${URITEMPLATE_VERSION_MINOR}.${URITEMPLATE_VERSION_RELEASE}")

## Make sure that custom modules are found
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

## Declare project
project("uri-template" LANGUAGES CXX
                       VERSION "${URITEMPLATE_VERSION_STRING}"
                       DESCRIPTION "URI Templates expansion and reverse-matching for C++"
)


##############################################
# Declare options and project-wide parameters

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra \
        -Wunused-variable -Wunused-parameter \
        -Wunused-function -Wunused -Wno-system-headers \
        -Wno-deprecated -Woverloaded-virtual -Wwrite-strings"
    )
endif()

# Compile in release with debug info mode by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Export symbols for shared libs on windows
if(WIN32 AND BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

option(URITEMPLATE_BUILD_TESTING "Build included unit-tests" OFF)
option(URITEMPLATE_BUILD_DOCS "Build sphinx generated docs" OFF)


##############################################
# Create target and set properties

set(UCONFIG_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UCONFIG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


set(UCONFIG_SOURCES ${UCONFIG_SRC_DIR}/Expander.cpp
                    ${UCONFIG_SRC_DIR}/Matcher.cpp
                    ${UCONFIG_SRC_DIR}/Modifier.cpp
                    ${UCONFIG_SRC_DIR}/Operator.cpp
                    ${UCONFIG_SRC_DIR}/Parser.cpp
                    ${UCONFIG_SRC_DIR}/Template.cpp
                    ${UCONFIG_SRC_DIR}/Variable.cpp
)

# liburi-template
add_library(${PROJECT_NAME} ${UCONFIG_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION "${URITEMPLATE_VERSION_STRING}"
    VERSION "${URITEMPLATE_VERSION_STRING}"
)


##############################################
# Installation instructions

include(GNUInstallDirs)

## Install targets
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

## Install headers
install(DIRECTORY ${UCONFIG_INC_DIR}/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

## Configure cmake helpers
include(CMakePackageConfigHelpers)

set(URITEMPLATE_VERSION_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/uri-template-config-version.cmake")
set(URITEMPLATE_PROJECT_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/uri-template-config.cmake")
set(URITEMPLATE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/uri-template/")

write_basic_package_version_file(${URITEMPLATE_VERSION_CONFIG}
    VERSION ${URITEMPLATE_VERSION_STRING}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(${PROJECT_SOURCE_DIR}/uri-template-config.cmake.in
    ${URITEMPLATE_PROJECT_CONFIG}
    INSTALL_DESTINATION ${URITEMPLATE_CMAKE_INSTALL_DIR}
)

## Install cmake helpers
install(FILES ${URITEMPLATE_PROJECT_CONFIG}
              ${URITEMPLATE_VERSION_CONFIG}
        DESTINATION ${URITEMPLATE_CMAKE_INSTALL_DIR}
)

install(EXPORT ${PROJECT_NAME}-targets
        DESTINATION ${URITEMPLATE_CMAKE_INSTALL_DIR}
        NAMESPACE ${PROJECT_NAME}::
)


##############################################
# Unit-tests

if(URITEMPLATE_BUILD_TESTING)
    include(CTest)

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        if(NOT APPLE)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -lgcov")
        endif()
    endif()

    add_subdirectory(test)
endif()


##############################################
# Docs

if(URITEMPLATE_BUILD_DOCS)
    add_subdirectory(docs)
endif()
