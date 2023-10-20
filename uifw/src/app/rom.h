#pragma once

#include <SDL.h>
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

namespace h7 {
    using namespace yaze::app;
    class ROM{
    public:
        const char * title(){return "Test uifw";}
        size_t size(){return 10801920;}
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
         std::shared_ptr<SDL_Renderer> renderer_;
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
}
