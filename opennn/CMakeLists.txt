cmake_minimum_required(VERSION 2.8.12)

project(opennn)

set(version 6.0.4)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(opennn ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS opennn
    EXPORT opennnTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION opennn
    INCLUDES DESTINATION opennn
)

include (CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/opennnConfig.cmake"
    INSTALL_DESTINATION cmake
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/opennnConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/opennnConfigVersion.cmake"
    DESTINATION cmake
)
install(EXPORT opennnTargets DESTINATION cmake)
