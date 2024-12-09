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

set(WXWIDGETS_INCLUDE_PATH CACHE PATH "Path to WxWidgets include directory")
set(WXWIDGETS_LIB_PATH CACHE PATH "Path to WxWidgets lib directory")

target_include_directories (wxMarkdownCtrl PUBLIC "${WXWIDGETS_INCLUDE_PATH}" "${CMAKE_SOURCE_DIR}/include")
if (MSVC)
	target_include_directories (wxMarkdownCtrl PUBLIC "${WXWIDGETS_INCLUDE_PATH}/msvc")
endif()
target_link_directories (wxMarkdownCtrl PUBLIC "${WXWIDGETS_LIB_PATH}")

# Include sub-projects.
add_subdirectory ("src")
add_subdirectory ("include")
add_subdirectory ("editor")
