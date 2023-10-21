# SDL2, SDL2_image and SDL2_mixer
add_subdirectory(${EXT_DIR}/src/lib/SDL SDL)
#if (UNIX)
#    add_subdirectory(${EXT_DIR}/src/lib/SDL SDL)
#else()
#  find_package(SDL2)
#endif()
include_directories(${EXT_DIR}/src/lib/SDL/include)
set(SDL2MIXER_OPUS OFF)
set(SDL2MIXER_FLAC OFF)
set(SDL2MIXER_MOD OFF)
set(SDL2MIXER_MIDI_FLUIDSYNTH OFF)
set(SDL2MIXER_WAVE OFF)
SET(SDL2MIXER_WAVPACK OFF)
find_library(SDL_MIXER_LIBRARY
  NAMES SDL_mixer
  HINTS
    ENV SDLMIXERDIR
    ENV SDLDIR
  PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
  )
add_subdirectory(${EXT_DIR}/src/lib/SDL_mixer SDL_mixer)
find_package(SDL2_image)
message(STATUS "SDL2_INCLUDE_DIR = ${SDL2_INCLUDE_DIR}")
