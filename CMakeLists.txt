cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_definitions(-DUNICODE -D_UNICODE)

project ("wxMarkdownCtrl")
add_library(wxMarkdownCtrl INTERFACE)
add_library(wxMarkdownCtrl::wxMarkdownCtrl ALIAS wxMarkdownCtrl)

target_sources (wxMarkdownCtrl PRIVATE "include/wxmarkdownctrl/wxMarkdownCtrl.h")

target_include_directories (wxMarkdownCtrl INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base html)
find_package(cppMarkdown REQUIRED)

target_link_libraries(wxMarkdownCtrl INTERFACE wx::core wx::base wx::html)
target_link_libraries(wxMarkdownCtrl INTERFACE cppMarkdown::cppMarkdown)

# Install

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(ConfigPackageLocation lib/cmake/wxMarkdownCtrl)

configure_package_config_file(
        cmake/wxMarkdownCtrlConfig.cmake.in
	    "${CMAKE_CURRENT_BINARY_DIR}/wxMarkdownCtrl/wxMarkdownCtrlConfig.cmake"
	    INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/wxMarkdownCtrl"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/wxMarkdownCtrl/wxMarkdownCtrlConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY AnyNewerVersion
)

install(
        TARGETS wxMarkdownCtrl EXPORT wxMarkdownCtrlTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(EXPORT wxMarkdownCtrlTargets
        FILE wxMarkdownCtrlTargets.cmake
        NAMESPACE wxMarkdownCtrl::
        DESTINATION ${ConfigPackageLocation}
)

install(FILES 
            "${CMAKE_CURRENT_BINARY_DIR}/wxMarkdownCtrl/wxMarkdownCtrlConfig.cmake" 
            "${CMAKE_CURRENT_BINARY_DIR}/wxMarkdownCtrl/wxMarkdownCtrlConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
)

export(PACKAGE wxMarkdownCtrl)