#ifndef YAZE_APPLICATION_DATA_TILE_H
#define YAZE_APPLICATION_DATA_TILE_H

#include <tile.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Palette.h"

namespace yaze {
namespace Application {
namespace Graphics {

using byte = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;

// vhopppcc cccccccc
// [0, 1]
// [2, 3]
class TileInfo {
 public:
  ushort id_;
  ushort over_;
  ushort vertical_mirror_;
  ushort horizontal_mirror_;
  byte palette_;
  TileInfo() {}
  TileInfo(ushort id, byte palette, ushort v, ushort h, ushort o)
      : id_(id),
        palette_(palette),
        vertical_mirror_(v),
        horizontal_mirror_(h),
        over_(o) {}
};

class Tile32 {
 public:
  ushort tile0_;
  ushort tile1_;
  ushort tile2_;
  ushort tile3_;

  Tile32(ushort t0, ushort t1, ushort t2, ushort t3)
      : tile0_(t0), tile1_(t1), tile2_(t2), tile3_(t3) {}
};

class Tile16 {
 public:
  TileInfo tile0_;
  TileInfo tile1_;
  TileInfo tile2_;
  TileInfo tile3_;
  std::vector<TileInfo> tiles_info;

  Tile16(TileInfo t0, TileInfo t1, TileInfo t2, TileInfo t3)
      : tile0_(t0), tile1_(t1), tile2_(t2), tile3_(t3) {
    tiles_info.push_back(tile0_);
    tiles_info.push_back(tile1_);
    tiles_info.push_back(tile2_);
    tiles_info.push_back(tile3_);
  }
};

class TilesPattern {
 public:
  TilesPattern();
  std::string name;
  std::string description;
  bool custom;
  unsigned int tilesPerRow;
  unsigned int numberOfTiles;

  void default_settings();

  static TilesPattern pattern(std::string name);
  static std::vector<std::vector<tile8> > transform(
      const TilesPattern& pattern, const std::vector<tile8>& tiles);

 protected:
  std::vector<std::vector<tile8> > transform(
      const std::vector<tile8>& tiles) const;
  std::vector<tile8> reverse(const std::vector<tile8>& tiles) const;
  std::vector<std::vector<int> > transformVector;
};

class TilePreset {
 public:
  TilePreset();

  bool save(const std::string& file);
  bool load(const std::string& file);

  bool paletteNoZeroColor;
  int pcTilesLocation;
  uint16_t SNESTilesLocation;
  uint16_t SNESPaletteLocation;
  uint32_t pcPaletteLocation;
  uint32_t length;
  uint32_t bpp;

  TilesPattern tilesPattern;
  std::string compression;
};

}  // namespace Graphics
}  // namespace Application
}  // namespace yaze

#endif