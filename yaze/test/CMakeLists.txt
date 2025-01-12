# GoogleTest ------------------------------------------------------------------

add_subdirectory(googletest)
#include(googletest/)

# set(SNAPPY_DIR /home/heaven7/heaven7/env/linux/google/snappy)
# include_directories(${SNAPPY_DIR}/include)
# set(GTEST_LIBS ${SNAPPY_DIR}/lib/libgmock_main.a 
#   ${SNAPPY_DIR}/lib/libgmock.a
#   ${SNAPPY_DIR}/lib/libgtest.a
#   ${SNAPPY_DIR}/lib/libgtest_main.a 
# )

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
# )

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
  yaze_test
  yaze_test.cc
  z3ed_test.cc
  cpu_test.cc
  spc700_test.cc
  ../src/cli/patch.cc
  ../src/cli/command_handler.cc
  compression_test.cc
  snes_palette_test.cc
  ../src/app/rom.cc
  ../src/app/emu/cpu.cc
  ../src/app/emu/audio/apu.cc
  ../src/app/emu/video/ppu.cc
  ../src/app/emu/audio/dsp.cc
  ../src/app/emu/audio/spc700.cc
  ../src/app/gfx/bitmap.cc
  ../src/app/gfx/snes_tile.cc
  ../src/app/gfx/snes_palette.cc
  ../src/app/gfx/compression.cc
  ../src/app/core/common.cc
  # ${ASAR_STATIC_SRC}
)

target_include_directories(
  yaze_test PUBLIC
  ../src/
  ../src/lib/
  # ../src/lib/asar/src/asar/
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(
  yaze_test
  ${ABSL_TARGETS}
  SDL2::SDL2
  ${PNG_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${CMAKE_DL_LIBS}
  # asar-static
  # snes_spc
  ImGui
  gmock_main
  gmock
  gtest_main
  gtest
  #${GTEST_LIBS}
)
target_compile_definitions(yaze_test PRIVATE "linux")
target_compile_definitions(yaze_test PRIVATE "stricmp=strcasecmp")

include(GoogleTest)
gtest_discover_tests(yaze_test)