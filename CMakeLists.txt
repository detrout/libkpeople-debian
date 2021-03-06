project (kpeople)

option(ENABLE_EXAMPLES "Enable building of examples" OFF)
option(KDE4_BUILD_TESTS "Enable building of tests" OFF)

set (CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
     ${CMAKE_MODULE_PATH}
)

set (KDE_MIN_VERSION "4.10.1")

set(KPEOPLE_VERSION_MAJOR 0)
set(KPEOPLE_VERSION_MINOR 3)
set(KPEOPLE_VERSION_PATCH 0)

set(KPEOPLE_VERSION "${KPEOPLE_VERSION_MAJOR}.${KPEOPLE_VERSION_MINOR}.${KPEOPLE_VERSION_PATCH}")
set(KPEOPLE_SOVERSION 4)
set(KPEOPLE_LIBVERSION "${KPEOPLE_SOVERSION}.${KPEOPLE_VERSION_MINOR}.${KPEOPLE_VERSION_PATCH}")

find_package (KDE4 ${KDE_MIN_VERSION} REQUIRED)
find_package (KdepimLibs REQUIRED)
find_package(Baloo)


include (KDE4Defaults)
include (MacroLibrary)

include_directories (${KDE4_INCLUDES}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${KDEPIMLIBS_INCLUDE_DIRS}
)

if (${Baloo_FOUND})
    include_directories(${BALOO_INCLUDE_DIR})
endif()

if(KdepimLibs_VERSION_MINOR GREATER 11)
    add_definitions(-DHAVE_KDEPIM_ATLEAST_412)
endif()

add_subdirectory (src)
add_subdirectory (pixmaps)

configure_file(cmake/KPeopleConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/KPeopleConfig.cmake @ONLY)

macro_write_basic_cmake_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/KPeopleConfigVersion.cmake
        ${KPEOPLE_VERSION_MAJOR} ${KPEOPLE_VERSION_MINOR} ${KPEOPLE_VERSION_PATCH})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/KPeopleConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/cmake/KPeopleConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KPeople)
find_package(Msgfmt REQUIRED)
find_package(Gettext REQUIRED)
add_subdirectory( po )
