#include "music_editor.h"

#include "gui/icons.h"
#include "gui/input.h"
#include "imgui.h"

namespace yaze {
namespace app {
namespace editor {

void MusicEditor::Update() {
  ImGui::Separator();
  if (ImGui::BeginTable("MusicEditorColumns", 2, music_editor_flags_,
                        ImVec2(0, 0))) {
    ImGui::TableSetupColumn("Assembly");
    ImGui::TableSetupColumn("Composition");
    ImGui::TableHeadersRow();
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    assembly_editor_.InlineUpdate();

    ImGui::TableNextColumn();
    DrawToolset();
    if (ImGuiID child_id = ImGui::GetID((void*)(intptr_t)9);
        ImGui::BeginChild(child_id, ImVec2(0, 250), false)) {
      DrawPianoStaff();
    }
    ImGui::EndChild();
    DrawPianoRoll();

    ImGui::EndTable();
  }
}

static const int NUM_KEYS = 25;
static bool keys[NUM_KEYS];

void MusicEditor::DrawPianoStaff() {
  const int NUM_LINES = 5;
  const int LINE_THICKNESS = 2;
  const int LINE_SPACING = 50;

  // Get the draw list for the current window
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Draw the staff lines
  ImVec2 canvas_p0 =
      ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
  ImVec2 canvas_p1 = ImVec2(canvas_p0.x + ImGui::GetContentRegionAvail().x,
                            canvas_p0.y + ImGui::GetContentRegionAvail().y);
  draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(32, 32, 32, 255));
  for (int i = 0; i < NUM_LINES; i++) {
    auto line_start = ImVec2(canvas_p0.x, canvas_p0.y + i * LINE_SPACING);
    auto line_end = ImVec2(canvas_p1.x + ImGui::GetContentRegionAvail().x,
                           canvas_p0.y + i * LINE_SPACING);
    draw_list->AddLine(line_start, line_end, IM_COL32(200, 200, 200, 255),
                       LINE_THICKNESS);
  }

  // Draw the ledger lines
  const int NUM_LEDGER_LINES = 3;
  for (int i = -NUM_LEDGER_LINES; i <= NUM_LINES + NUM_LEDGER_LINES; i++) {
    if (i % 2 == 0) continue;  // skip every other line
    auto line_start = ImVec2(canvas_p0.x, canvas_p0.y + i * LINE_SPACING / 2);
    auto line_end = ImVec2(canvas_p1.x + ImGui::GetContentRegionAvail().x,
                           canvas_p0.y + i * LINE_SPACING / 2);
    draw_list->AddLine(line_start, line_end, IM_COL32(150, 150, 150, 255),
                       LINE_THICKNESS);
  }
}

void MusicEditor::DrawPianoRoll() {
  // Render the piano roll
  float key_width = ImGui::GetContentRegionAvail().x / NUM_KEYS;
  float white_key_height = ImGui::GetContentRegionAvail().y * 0.8f;
  float black_key_height = ImGui::GetContentRegionAvail().y * 0.5f;
  ImGui::Text("Piano Roll");
  ImGui::Separator();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Draw the staff lines
  ImVec2 canvas_p0 =
      ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
  ImVec2 canvas_p1 = ImVec2(canvas_p0.x + ImGui::GetContentRegionAvail().x,
                            canvas_p0.y + ImGui::GetContentRegionAvail().y);
  draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(200, 200, 200, 255));

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.f, 0.f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
  for (int i = 0; i < NUM_KEYS; i++) {
    // Calculate the position and size of the key
    ImVec2 key_pos = ImVec2(i * key_width, 0.0f);
    ImVec2 key_size;
    ImVec4 key_color;
    ImVec4 text_color;
    if (i % 12 == 1 || i % 12 == 3 || i % 12 == 6 || i % 12 == 8 ||
        i % 12 == 10) {
      // This is a black key
      key_size = ImVec2(key_width * 0.6f, black_key_height);
      key_color = ImVec4(0, 0, 0, 255);
      text_color = ImVec4(255, 255, 255, 255);
    } else {
      // This is a white key
      key_size = ImVec2(key_width, white_key_height);
      key_color = ImVec4(255, 255, 255, 255);
      text_color = ImVec4(0, 0, 0, 255);
    }

    ImGui::PushID(i);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_Button, key_color);
    ImGui::PushStyleColor(ImGuiCol_Text, text_color);
    if (ImGui::Button(kSongNotes[i].data(), key_size)) {
      keys[i] ^= 1;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImVec2 button_pos = ImGui::GetItemRectMin();
    ImVec2 button_size = ImGui::GetItemRectSize();
    if (keys[i]) {
      ImVec2 dest;
      dest.x = button_pos.x + button_size.x;
      dest.y = button_pos.y + button_size.y;
      ImGui::GetWindowDrawList()->AddRectFilled(button_pos, dest,
                                                IM_COL32(200, 200, 255, 200));
    }
    ImGui::PopID();
    ImGui::SameLine();
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
}

void MusicEditor::DrawSongToolset() {
  if (ImGui::BeginTable("DWToolset", 8, toolset_table_flags_, ImVec2(0, 0))) {
    ImGui::TableSetupColumn("#1");
    ImGui::TableSetupColumn("#play");
    ImGui::TableSetupColumn("#rewind");
    ImGui::TableSetupColumn("#fastforward");
    ImGui::TableSetupColumn("volumeController");

    ImGui::EndTable();
  }
}

void MusicEditor::DrawToolset() {
  static bool is_playing = false;
  static int selected_option = 0;
  static int current_volume = 0;
  const int MAX_VOLUME = 100;
  gui::ItemLabel("Select a song to edit: ", gui::ItemLabelFlags::Left);
  ImGui::Combo("#controls", &selected_option, kGameSongs, 30);
  if (ImGui::BeginTable("SongToolset", 5, toolset_table_flags_, ImVec2(0, 0))) {
    ImGui::TableSetupColumn("#play");
    ImGui::TableSetupColumn("#rewind");
    ImGui::TableSetupColumn("#fastforward");
    ImGui::TableSetupColumn("#volume");
    ImGui::TableSetupColumn("#slider");

    ImGui::TableNextColumn();
    if (ImGui::Button(is_playing ? ICON_MD_STOP : ICON_MD_PLAY_ARROW)) {
      is_playing = !is_playing;
    }

    BUTTON_COLUMN(ICON_MD_FAST_REWIND)
    BUTTON_COLUMN(ICON_MD_FAST_FORWARD)
    BUTTON_COLUMN(ICON_MD_VOLUME_UP)
    ImGui::TableNextColumn();
    ImGui::SliderInt("Volume", &current_volume, 0, 100);
    ImGui::EndTable();
  }

  const int SONG_DURATION = 120;  // duration of the song in seconds
  static int current_time = 0;    // current time in the song in seconds

  // Display the current time in the song
  ImGui::Text("Current Time: %d:%02d", current_time / 60, current_time % 60);

  // Display the song duration/progress using a progress bar
  ImGui::ProgressBar((float)current_time / SONG_DURATION);

  // Allow the user to seek to a specific time in the song using a slider
  ImGui::SliderInt("Seek Time", &current_time, 0, SONG_DURATION);

  if (ImGui::BeginTable("ChannelToolset", 9, toolset_table_flags_,
                        ImVec2(0, 0))) {
    ImGui::TableSetupColumn("#Channels");
    for (int i = 0; i < 3; i++) {
      ImGui::TableSetupColumn(absl::StrFormat("#%d", i).data());
    }

    for (int i = 0; i < 8; i++) {
      ImGui::TableNextColumn();
      ImGui::Button(absl::StrFormat("%d", i).data());
    }

    ImGui::EndTable();
  }
}

}  // namespace editor
}  // namespace app
}  // namespace yaze