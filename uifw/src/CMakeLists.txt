
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(SDL_TARGETS SDL2::SDL2)

#if(WIN32 OR MINGW)
#    list(PREPEND SDL_TARGETS SDL2::SDL2main)
#    add_definitions(-DSDL_MAIN_HANDLED)
#endif()

set(
  APP_CORE_SRC
  app/core/controller.cc
  app/core/pipeline.cc
  app/core/common.cc

  app/core/controller.h
  app/core/pipeline.h
  app/core/common.h
  app/rom.h
  app/rom.cc
  app/main.cpp
)

set(
    APP_GUI_SRC
    app/gui/canvas.h
    app/gui/canvas.cc
    app/gui/color.h
    app/gui/color.cc
    app/gui/input.h
    app/gui/input.cc
    app/gui/style.h
    app/gui/style.cc
    app/gui/icons.h
    app/gui/widgets.h
    app/gui/widgets.cc
    )

FILE (GLOB APP_GFX "app/gfx/*.h" "app/gfx/*.cc" )
FILE (GLOB APP_EDITOR "app/editor/*.h" "app/editor/*.cc" )

set(EXE_NAME test_uifw)
add_executable(
    ${EXE_NAME}
    ${APP_CORE_SRC}
    ${APP_GUI_SRC}
    ${APP_GFX}
    ${APP_EDITOR}
    )
target_include_directories(
  ${EXE_NAME} PUBLIC
  lib/
  ${CMAKE_SOURCE_DIR}/src/
  ${PNG_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIR}
  lib/SDL_mixer/include/
)

target_link_libraries(
  ${EXE_NAME} PUBLIC
  ${ABSL_TARGETS}
  ${SDL_TARGETS}
  ${PNG_LIBRARIES}
  ${CMAKE_DL_LIBS}
  SDL2_mixer
 # ImGui
  ImGui_impl
)

#######################################
if (UNIX)
  target_compile_definitions(${EXE_NAME} PRIVATE "linux")
  target_compile_definitions(${EXE_NAME} PRIVATE "stricmp=strcasecmp")
endif()

if(MACOS)
 # set(MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/yaze.ico)
  set_target_properties(${EXE_NAME}
      PROPERTIES
      BUNDLE True
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
     # MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/yaze.plist.in
  )
elseif(UNIX)
  set_target_properties(${EXE_NAME}
    PROPERTIES
    BUNDLE True
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
else()
set_target_properties(${EXE_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
   # LINK_FLAGS "${CMAKE_CURRENT_SOURCE_DIR}/yaze.res"
)
endif()
