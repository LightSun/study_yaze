#ifndef YAZE_APPLICATION_GRAPHICS_SCENE_H
#define YAZE_APPLICATION_GRAPHICS_SCENE_H

#include <SDL2/SDL.h>
#include <tile.h>

#include <iostream>
#include <vector>

#include "Graphics/tile.h"

namespace yaze {
namespace application {
namespace Graphics {

class Scene {
 public:
  Scene() = default;
  void buildSurface(const std::vector<tile8>& tiles, SNESPalette& mPalette,
                    const TilesPattern& tp);

  void updateScene();
  void setTilesZoom(unsigned int tileZoom);
  void setTilesPattern(TilesPattern tp);

  std::unordered_map<unsigned int, SDL_Texture*> imagesCache;

 private:
  unsigned int tilesZoom;
  TilesPattern tilesPattern;
  std::vector<tile8> allTiles;
  std::vector<std::vector<tile8>> arrangedTiles;
};

}  // namespace Graphics
}  // namespace application
}  // namespace yaze

#endif