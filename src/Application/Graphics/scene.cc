#include "scene.h"

#include <SDL2/SDL.h>
#include <tile.h>

#include <iostream>
#include <vector>

#include "Core/renderer.h"
#include "Graphics/tile.h"

namespace yaze {
namespace Application {
namespace Graphics {

void Scene::buildSurface(const std::vector<tile8>& tiles, SNESPalette& mPalette,
                         const TilesPattern& tp) {
  arrangedTiles = TilesPattern::transform(tp, tiles);
  tilesPattern = tp;
  allTiles = tiles;

  for (unsigned int j = 0; j < arrangedTiles.size(); j++) {
    for (unsigned int i = 0; i < arrangedTiles[0].size(); i++) {
      tile8 tile = arrangedTiles[j][i];
      // SDL_PIXELFORMAT_RGB888 ?
      SDL_Surface* newImage = SDL_CreateRGBSurfaceWithFormat(
          0, 8, 8, SDL_BITSPERPIXEL(3), SDL_PIXELFORMAT_RGB444);
      SDL_PixelFormat* format = newImage->format;
      format->palette = mPalette.GetSDL_Palette();

      char* ptr = (char*)newImage->pixels;

      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          ptr[i * 8 + j] = (char)tile.data[i * 8 + j];
        }
      }

      SDL_Texture* texture =
          SDL_CreateTextureFromSurface(Core::renderer, newImage);
      imagesCache[tile.id] = texture;
    }
  }
}

void Scene::buildScene(const std::vector<tile8>& tiles,
                       const SNESPalette mPalette, const TilesPattern& tp) {
  arrangedTiles = TilesPattern::transform(tp, tiles);
  tilesPattern = tp;
  allTiles = tiles;
  for (unsigned int j = 0; j < arrangedTiles.size(); j++) {
    for (unsigned int i = 0; i < arrangedTiles[0].size(); i++) {
      tile8 tile = arrangedTiles[j][i];
      // QImage newImage(8, 8, QImage::Format_Indexed8);
      // newImage.setColorCount(mPalette.size);
      // for (int i = 0; i < mPalette.size; i++) {
      //   newImage.setColor(i, mPalette.colors.at(i).rgb);
      // }
      // for (int i = 0; i < 8; i++) {
      //   for (int j = 0; j < 8; j++)
      //     newImage.setPixel(i, j, tile.data[i + j * 8]);
      // }
      // QPixmap m;
      // m.convertFromImage(newImage);
      // imagesCache[tile.id] = m;
      // GraphicsTileItem *newTileItem = new GraphicsTileItem(m, tile);
      // addItem(newTileItem);
      // newTileItem->setTileZoom(tilesZoom);
      // newTileItem->setPos(i * newTileItem->boundingRect().width() + i,
      //                     j * newTileItem->boundingRect().width() + j);
    }
  }
  // unsigned max_w = items()[0]->boundingRect().width() *
  // arrangedTiles[0].size() + arrangedTiles[0].size(); unsigned max_h =
  // items()[0]->boundingRect().width() * arrangedTiles.size() +
  // arrangedTiles.size(); setSceneRect(QRect(0, 0, max_w, max_h));
}

void Scene::updateScene() {
  std::cout << "Update scene";
  unsigned int itemCpt = 0;
  for (unsigned int j = 0; j < arrangedTiles.size(); j++) {
    for (unsigned int i = 0; i < arrangedTiles[0].size(); i++) {
      tile8 tile = arrangedTiles[j][i];
      // QPixmap m = imagesCache[tile.id];
      // GraphicsTileItem *tileItem = (GraphicsTileItem *)items()[itemCpt];
      // tileItem->image = m;
      // tileItem->rawTile = tile;
      // tileItem->setTileZoom(tilesZoom);
      // tileItem->setPos(i * tileItem->boundingRect().width() + i,
      //                  j * tileItem->boundingRect().width() + j);
      itemCpt++;
    }
  }
  // unsigned max_w =
  //     items()[0]->boundingRect().width() * arrangedTiles[0].size() +
  //     arrangedTiles[0].size();
  // unsigned max_h = items()[0]->boundingRect().width() * arrangedTiles.size()
  // +
  //                  arrangedTiles.size();
  // setSceneRect(QRect(0, 0, max_w, max_h));
}

void Scene::setTilesZoom(unsigned int tileZoom) {
  tilesZoom = tileZoom;
  // if (!items().isEmpty()) updateScene();
}

void Scene::setTilesPattern(TilesPattern tp) { tilesPattern = tp; }

}  // namespace Graphics
}  // namespace Application
}  // namespace yaze
