# gui libraries ---------------------------------------------------------------
set(IMGUI_PATH  ${EXT_DIR}/src/lib/imgui)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp
    )
add_library("ImGui" STATIC ${IMGUI_SOURCES}
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp
    ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
    )
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_include_directories(ImGui PUBLIC ${SDL2_INCLUDE_DIR})
target_compile_definitions(ImGui PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL2/SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)
target_link_libraries(ImGui PUBLIC ${SDL_TARGETS})

set(IMGUI_FILE_DLG_PATH ${EXT_DIR}/src/lib/ImGuiFileDialog)
file(GLOB IMGUI_FILE_DLG_SOURCES ${IMGUI_FILE_DLG_PATH}/*.cpp)

add_library("ImGuiFileDialog" STATIC ${IMGUI_FILE_DLG_SOURCES})
target_include_directories(ImGuiFileDialog PUBLIC ${IMGUI_PATH})
target_compile_definitions(ImGuiFileDialog PUBLIC 
  IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL2/SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)

set(IMGUI_COLOR_TEXT_EDIT_PATH ${EXT_DIR}/src/lib/ImGuiColorTextEdit)
file(GLOB IMGUI_COLOR_TEXT_EDIT_SOURCES ${IMGUI_COLOR_TEXT_EDIT_PATH}/*.cpp)
add_library("ImGuiColorTextEdit" STATIC ${IMGUI_COLOR_TEXT_EDIT_SOURCES})
target_include_directories(ImGuiColorTextEdit PUBLIC ${IMGUI_PATH})
target_compile_definitions(ImGuiColorTextEdit PUBLIC 
  IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL2/SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)

set(
  IMGUI_SRC
  ${IMGUI_PATH}/imgui.cpp 
  ${IMGUI_PATH}/imgui_demo.cpp
  ${IMGUI_PATH}/imgui_draw.cpp 
  ${IMGUI_PATH}/imgui_widgets.cpp
  ${IMGUI_PATH}/imgui_tables.cpp
  ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
  ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp 
  ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
  ${IMGUI_FILE_DLG_PATH}/ImGuiFileDialog.cpp
  ${IMGUI_COLOR_TEXT_EDIT_PATH}/TextEditor.cpp
)
add_library("ImGui_impl"
    ${IMGUI_SRC}
    )
target_include_directories("ImGui_impl" PUBLIC ${IMGUI_PATH})
target_include_directories("ImGui_impl" PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries("ImGui_impl" PUBLIC
    ImGui ImGuiFileDialog ImGuiColorTextEdit ${SDL_TARGETS}
    )
#target_compile_definitions("ImGui_impl" PUBLIC
#  IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL2/SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)
#message(STATUS "SDL_TARGETS = ${SDL_TARGETS}")
