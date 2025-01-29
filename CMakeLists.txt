# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_definitions(-DUNICODE -D_UNICODE)

project ("wxMarkdownCtrl")
add_library(wxMarkdownCtrl)

set(WXMDCTRL_BUILD_EDITOR ON CACHE BOOL "Build example Markdown editor")

set_property(TARGET wxMarkdownCtrl PROPERTY CXX_STANDARD 17)

include(cmake/CPM.cmake)

target_include_directories (wxMarkdownCtrl PUBLIC "${CMAKE_SOURCE_DIR}/include")

CPMAddPackage("gh:wxWidgets/wxWidgets@3.2.6")
CPMAddPackage("gh:rwypior/cppMarkdown@0.5")

target_link_libraries(wxMarkdownCtrl PUBLIC wx::core wx::base wx::html)
target_link_libraries(wxMarkdownCtrl PUBLIC cppMarkdown::cppMarkdown)

# Include sub-projects.
add_subdirectory ("src")
add_subdirectory ("include")

if(WXMDCTRL_BUILD_EDITOR)
	add_subdirectory ("editor")
endif()
