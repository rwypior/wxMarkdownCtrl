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

if (MSVC)
    SET(OPTIONS WIN32)
endif()

project ("MarkdownEditor")
add_executable(MarkdownEditor ${OPTIONS})

target_include_directories (MarkdownEditor PUBLIC "${WXWIDGETS_INCLUDE_PATH}" "${CMAKE_SOURCE_DIR}/include" ${PROJECT_SOURCE_DIR})
if (MSVC)
	target_include_directories (MarkdownEditor PUBLIC "${WXWIDGETS_INCLUDE_PATH}/msvc")
endif()
target_link_directories (MarkdownEditor PUBLIC "${WXWIDGETS_LIB_PATH}")

target_sources(MarkdownEditor PRIVATE 
	"main.cpp" "markdowneditor.cpp" "markdowneditor.h" "MarkdownEditorMainWindow.cpp" "MarkdownEditorMainWindow.h"
)

add_custom_command(
        TARGET MarkdownEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR}/res
)