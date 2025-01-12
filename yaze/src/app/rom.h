#ifndef YAZE_APP_ROM_H
#define YAZE_APP_ROM_H

#include <SDL.h>
#include <asar/src/asar/interface-lib.h>

#include <algorithm>
#include <chrono>
#include <cstddef>  // for size_t
#include <cstdint>  // for uint32_t, uint8_t, uint16_t
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>     // for function
#include <iostream>       // for string, operator<<, basic_...
#include <map>            // for map
#include <memory>         // for shared_ptr, make_shared
#include <stack>          // for stack
#include <string>         // for hash, operator==
#include <unordered_map>  // for unordered_map
#include <vector>         // for vector

#include "SDL_render.h"                    // for SDL_Renderer
#include "absl/container/flat_hash_map.h"  // for flat_hash_map
#include "absl/status/status.h"            // for Status
#include "absl/status/statusor.h"          // for StatusOr
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"  // for string_view
#include "app/core/common.h"           // for SnesToPc
#include "app/core/constants.h"        // for Bytes, uchar, armorPalettes
#include "app/gfx/bitmap.h"            // for Bitmap, BitmapTable
#include "app/gfx/compression.h"
#include "app/gfx/snes_palette.h"  // for PaletteGroup, SNESColor
#include "app/gfx/snes_tile.h"

namespace yaze {
namespace app {

struct VersionConstants {
  uint32_t kGgxAnimatedPointer;
  uint32_t kOverworldGfxGroups1;
  uint32_t kOverworldGfxGroups2;
  // long ptrs all tiles of maps[high/low] (mapid* 3)
  uint32_t compressedAllMap32PointersHigh;
  uint32_t compressedAllMap32PointersLow;
  uint32_t overworldMapPaletteGroup;
  uint32_t overlayPointers;
  uint32_t overlayPointersBank;
  uint32_t overworldTilesType;
  uint32_t kOverworldGfxPtr1;
  uint32_t kOverworldGfxPtr2;
  uint32_t kOverworldGfxPtr3;
  uint32_t kMap32TileTL;
  uint32_t kMap32TileTR;
  uint32_t kMap32TileBL;
  uint32_t kMap32TileBR;
  uint32_t kSpriteBlocksetPointer;
};

enum class Z3_Version {
  US = 1,
  JP = 2,
  SD = 3,
  RANDO = 4,
};

static constexpr uint32_t overworldMapPaletteGroup = 0x67E74;
static constexpr uint32_t overlayPointers = 0x3FAF4;
static constexpr uint32_t overlayPointersBank = 0x07;
static constexpr uint32_t overworldTilesType = 0x7FD94;

static const std::map<Z3_Version, VersionConstants> kVersionConstantsMap = {
    {Z3_Version::US,
     {
         0x10275,  // kGgxAnimatedPointer
         0x5D97,   // kOverworldGfxGroups1
         0x6073,   // kOverworldGfxGroups2
         0x1794D,  // compressedAllMap32PointersHigh
         0x17B2D,  // compressedAllMap32PointersLow
         0x75504,  // overworldMapPaletteGroup
         0x77664,  // overlayPointers
         0x0E,     // overlayPointersBank
         0x71459,  // overworldTilesType
         0x4F80,   // kOverworldGfxPtr1
         0x505F,   // kOverworldGfxPtr2
         0x513E,   // kOverworldGfxPtr3
         0x18000,  // kMap32TileTL
         0x1B400,  // kMap32TileTR
         0x20000,  // kMap32TileBL
         0x23400,  // kMap32TileBR
         0x5B57,   // kSpriteBlocksetPointer
     }},
    {Z3_Version::JP,
     {
         0x10624,  // kGgxAnimatedPointer
         0x5DD7,   // kOverworldGfxGroups1
         0x60B3,   // kOverworldGfxGroups2
         0x176B1,  // compressedAllMap32PointersHigh
         0x17891,  // compressedAllMap32PointersLow
         0x67E74,  // overworldMapPaletteGroup
         0x3FAF4,  // overlayPointers
         0x07,     // overlayPointersBank
         0x7FD94,  // overworldTilesType
         0x4FC0,   // kOverworldGfxPtr1
         0x509F,   // kOverworldGfxPtr2
         0x517E,   // kOverworldGfxPtr3
         0x18000,  // kMap32TileTL
         0x1B3C0,  // kMap32TileTR
         0x20000,  // kMap32TileBL
         0x233C0,  // kMap32TileBR
         0x5B97,   // kSpriteBlocksetPointer
     }}

};

constexpr int kOverworldGraphicsPos1 = 0x4F80;
constexpr int kOverworldGraphicsPos2 = 0x505F;
constexpr int kOverworldGraphicsPos3 = 0x513E;
constexpr int kTile32Num = 4432;
constexpr int kTitleStringOffset = 0x7FC0;
constexpr int kTitleStringLength = 20;
constexpr int kSNESToPCOffset = 0x138000;

constexpr uint32_t kNumGfxSheets = 223;
constexpr uint32_t kNormalGfxSpaceStart = 0x87000;
constexpr uint32_t kNormalGfxSpaceEnd = 0xC4200;
constexpr uint32_t kPtrTableStart = 0x4F80;
constexpr uint32_t kLinkSpriteLocation = 0x80000;
constexpr uint32_t kFontSpriteLocation = 0x70000;

const absl::flat_hash_map<std::string, uint32_t> paletteGroupAddresses = {
    {"ow_main", core::overworldPaletteMain},
    {"ow_aux", core::overworldPaletteAuxialiary},
    {"ow_animated", core::overworldPaletteAnimated},
    {"hud", core::hudPalettes},
    {"global_sprites", core::globalSpritePalettesLW},
    {"armors", core::armorPalettes},
    {"swords", core::swordPalettes},
    {"shields", core::shieldPalettes},
    {"sprites_aux1", core::spritePalettesAux1},
    {"sprites_aux2", core::spritePalettesAux2},
    {"sprites_aux3", core::spritePalettesAux3},
    {"dungeon_main", core::dungeonMainPalettes},
    {"grass", core::hardcodedGrassLW},
    {"3d_object", core::triforcePalette},
    {"ow_mini_map", core::overworldMiniMapPalettes},
};

const absl::flat_hash_map<std::string, uint32_t> paletteGroupColorCounts = {
    {"ow_main", 35},     {"ow_aux", 21},         {"ow_animated", 7},
    {"hud", 32},         {"global_sprites", 60}, {"armors", 15},
    {"swords", 3},       {"shields", 4},         {"sprites_aux1", 7},
    {"sprites_aux2", 7}, {"sprites_aux3", 7},    {"dungeon_main", 90},
    {"grass", 1},        {"3d_object", 8},       {"ow_mini_map", 128},
};

class ROM{
public:
    //Load functions
    absl::StatusOr<Bytes> Load2bppGraphics();
    absl::Status LoadAllGraphicsData();
    absl::Status LoadFromFile(const absl::string_view& filename,
                              bool z3_load = true);
    absl::Status LoadFromPointer(uchar* data, size_t length);
    absl::Status LoadFromBytes(const Bytes& data);
    void LoadAllPalettes();
    // Save functions
    absl::Status SaveToFile(bool backup, absl::string_view filename = "");
    absl::Status UpdatePaletteColor(const std::string& groupName,
                                    size_t paletteIndex, size_t colorIndex,
                                    const gfx::SNESColor& newColor);
    void SaveAllPalettes();
    // Read functions
    gfx::SNESColor ReadColor(int offset);
    gfx::SNESPalette ReadPalette(int offset, int num_colors);
    uint8_t ReadByte(int offset) { return rom_data_[offset]; }
    uint16_t ReadWord(int offset) {
        return (uint16_t)(rom_data_[offset] | (rom_data_[offset + 1] << 8));
    }
    uint16_t ReadShort(int offset) {
        return (uint16_t)(rom_data_[offset] | (rom_data_[offset + 1] << 8));
    }
    std::vector<uint8_t> ReadByteVector(uint32_t offset, uint32_t length) {
        std::vector<uint8_t> result;
        for (uint32_t i = offset; i < length; i++) {
            result.push_back(rom_data_[i]);
        }
        return result;
    }
    gfx::Tile16 ReadTile16(uint32_t tile16_id) {
        // Skip 8 bytes per tile.
        auto tpos = 0x78000 + (tile16_id * 0x08);
        gfx::Tile16 tile16;
        tile16.tile0_ = gfx::WordToTileInfo(ReadShort(tpos));
        tpos += 2;
        tile16.tile1_ = gfx::WordToTileInfo(ReadShort(tpos));
        tpos += 2;
        tile16.tile2_ = gfx::WordToTileInfo(ReadShort(tpos));
        tpos += 2;
        tile16.tile3_ = gfx::WordToTileInfo(ReadShort(tpos));
        return tile16;
    }
    void WriteTile16(int tile16_id, const gfx::Tile16& tile) {
        // Skip 8 bytes per tile.
        auto tpos = 0x78000 + (tile16_id * 0x08);
        WriteShort(tpos, gfx::TileInfoToWord(tile.tile0_));
        tpos += 2;
        WriteShort(tpos, gfx::TileInfoToWord(tile.tile1_));
        tpos += 2;
        WriteShort(tpos, gfx::TileInfoToWord(tile.tile2_));
        tpos += 2;
        WriteShort(tpos, gfx::TileInfoToWord(tile.tile3_));
    }

  // Write functions
  void Write(int addr, int value) { rom_data_[addr] = value; }

  void WriteShort(uint32_t addr, uint16_t value) {
    rom_data_[addr] = (uint8_t)(value & 0xFF);
    rom_data_[addr + 1] = (uint8_t)((value >> 8) & 0xFF);
  }

  void WriteVector(int addr, std::vector<uint8_t> data) {
    for (int i = 0; i < data.size(); i++) {
      rom_data_[addr + i] = data[i];
    }
  }

  void WriteColor(uint32_t address, const gfx::SNESColor& color) {
    uint16_t bgr = ((color.GetSNES() >> 10) & 0x1F) |
                   ((color.GetSNES() & 0x1F) << 10) |
                   (color.GetSNES() & 0x7C00);

    // Write the 16-bit color value to the ROM at the specified address
    WriteShort(address, bgr);
  }

  void WriteTile32(int id, const gfx::Tile32& tile) {
    const uint32_t map32address[4] = {
        GetVersionConstants().kMap32TileTL, GetVersionConstants().kMap32TileTR,
        GetVersionConstants().kMap32TileBL, GetVersionConstants().kMap32TileBR};

    if (id < 0 || id >= 0x4540) {
      std::cout << "Invalid tile ID: " << id << std::endl;
      return;
    }

    // Helper lambda to avoid code repetition
    auto writeTilesToRom = [&](int base_addr, auto get_tile) {
      for (int j = 0; j < 4; ++j) {
        Write(base_addr + id + j, get_tile(tile) & 0xFF);
      }
      Write(base_addr + id + 4,
            ((get_tile(tile) >> 4) & 0xF0) | ((get_tile(tile) >> 8) & 0x0F));
      Write(base_addr + id + 5,
            ((get_tile(tile) >> 4) & 0xF0) | ((get_tile(tile) >> 8) & 0x0F));
    };

    writeTilesToRom(map32address[0],
                    [](const gfx::Tile32& t) { return t.tile0_; });
    writeTilesToRom(map32address[1],
                    [](const gfx::Tile32& t) { return t.tile1_; });
    writeTilesToRom(map32address[2],
                    [](const gfx::Tile32& t) { return t.tile2_; });
    writeTilesToRom(map32address[3],
                    [](const gfx::Tile32& t) { return t.tile3_; });
  }

  void Expand(int size) {
    rom_data_.resize(size);
    size_ = size;
  }

  void QueueChanges(std::function<void()> function) { changes_.push(function); }

  VersionConstants GetVersionConstants() const {
    return kVersionConstantsMap.at(version_);
  }
  int GetGraphicsAddress(const uchar* data, uint8_t addr) const {
    auto part_one = data[GetVersionConstants().kOverworldGfxPtr1 + addr] << 16;
    auto part_two = data[GetVersionConstants().kOverworldGfxPtr2 + addr] << 8;
    auto part_three = data[GetVersionConstants().kOverworldGfxPtr3 + addr];
    auto snes_addr = (part_one | part_two | part_three);
    return core::SnesToPc(snes_addr);
  }
  uint32_t GetPaletteAddress(const std::string& groupName, size_t paletteIndex,
                             size_t colorIndex) const;
  gfx::PaletteGroup GetPaletteGroup(const std::string& group) {
    return palette_groups_[group];
  }
  Bytes GetGraphicsBuffer() const { return graphics_buffer_; }
  gfx::BitmapTable GetGraphicsBin() const { return graphics_bin_; }

  auto title() const { return (uchar*)title_; }
  auto size() const { return size_; }
  auto begin() { return rom_data_.begin(); }
  auto end() { return rom_data_.end(); }
  auto data() { return rom_data_.data(); }
  auto vector() const { return rom_data_; }
  auto filename() const { return filename_; }
  auto isLoaded() const { return is_loaded_; }
  auto char_data() { return reinterpret_cast<char*>(rom_data_.data()); }

  auto push_back(uchar byte) { rom_data_.push_back(byte); }

  auto version() const { return version_; }

  void malloc(int n_bytes) {
    rom_data_.clear();
    rom_data_.reserve(n_bytes);
    rom_data_.resize(n_bytes);
    for (int i = 0; i < n_bytes; i++) {
      rom_data_.push_back(0x00);
    }
    size_ = n_bytes;
  }

  uchar& operator[](int i) {
    if (i > size_) {
      std::cout << "ROM: Index " << i << " out of bounds, size: " << size_
                << std::endl;
      return rom_data_[0];
    }
    return rom_data_[i];
  }
  uchar& operator+(int i) {
    if (i > size_) {
      std::cout << "ROM: Index " << i << " out of bounds, size: " << size_
                << std::endl;
      return rom_data_[0];
    }
    return rom_data_[i];
  }
  const uchar* operator&() { return rom_data_.data(); }

  ushort toint16(int offset) {
    return (ushort)((rom_data_[offset + 1]) << 8) | rom_data_[offset];
  }

  void SetupRenderer(std::shared_ptr<SDL_Renderer> renderer) {
    renderer_ = renderer;
  }

  void RenderBitmap(gfx::Bitmap* bitmap) const {
    bitmap->CreateTexture(renderer_);
  }

  void UpdateBitmap(gfx::Bitmap* bitmap) const {
    bitmap->UpdateTexture(renderer_);
  }

 private:
  long size_ = 0;
  bool is_loaded_ = false;
  bool has_header_ = false;
  char title_[21] = "ROM Not Loaded";
  std::string filename_;

  Bytes rom_data_;
  Bytes graphics_buffer_;

  Z3_Version version_ = Z3_Version::US;
  gfx::BitmapTable graphics_bin_;

  std::stack<std::function<void()>> changes_;
  std::shared_ptr<SDL_Renderer> renderer_;
  std::unordered_map<std::string, gfx::PaletteGroup> palette_groups_;
};

class SharedROM {
 public:
  SharedROM() = default;
  virtual ~SharedROM() = default;

  std::shared_ptr<ROM> shared_rom() {
    if (!shared_rom_) {
      shared_rom_ = std::make_shared<ROM>();
    }
    return shared_rom_;
  }

  auto rom() {
    if (!shared_rom_) {
      shared_rom_ = std::make_shared<ROM>();
    }
    ROM* rom = shared_rom_.get();
    return rom;
  }

 private:
  static std::shared_ptr<ROM> shared_rom_;
};

}  // namespace app
}  // namespace yaze

#endif
