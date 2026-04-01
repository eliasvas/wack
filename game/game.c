// FIXME: Why is this needed, wont we get this definition from platform? seed?
#define BRAND_IMPLEMENTATION

#include "game.h"
#include "gui/gui.h"
#include "level.h"
#include "entity.h"

// TODO: read this
// https://www.gamedeveloper.com/programming/improved-lerp-smoothing-

// Game Idea:
// There are goblins and peasants coming out of each hole
// You want to find and WHACK as many goblins as possible
// they can fart though and create fog of war YOU must cleanup
// You can have hammer and cleaning product and drag them or become a cursor or?
// Maybe just scroll between them

// TODO: Audio synthesis for the _art noises :|
// TODO: Think more gameplay mechanics

void game_init(Game_State *gs) {
  gs->game_init_timestamp = gs->time_sec; 
  gs->next_level_idx = 0;

  // RNG test
#if 0
  for(u32 i = 0; i < 20; i+=1) {
    f64 r = brand_srange(-2,2);
    printf("r= %f\n", r);
  }
#endif
  // Zoom Stuff
  gs->zoom = 1.0;
  gs->zoom_damp_factor = 0.1;

}

void game_update(Game_State *gs, float dt) {
  static bool gui_initialized = false;
  if (!gui_initialized) {
    gui_context_init(gs->frame_arena, &gs->font);
    gui_initialized = true;
  }
  gs->game_viewport = rec(0,0,gs->screen_dim.x, gs->screen_dim.y);
  // TODO: This needs work, it should scroll faster when at higher zooms
  gs->zoom = clamp(gs->zoom + gs->zoom_damp_factor * input_get_scroll_delta(&gs->input).y, 1.0, 100.0);

  // If level_update returns true (level finished), we delete it
  if (level_update((Level*)gs->level, gs, dt)) {
    level_deinit((Level*)gs->level);
    gs->level = nullptr;
  }
}


void game_render(Game_State *gs, float dt) {
  // Push viewport, scissor and camera (we will not change these the whole frame except in UI pass)
  R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_VIEWPORT, .r = gs->game_viewport };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
  cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_SCISSOR, .r = gs->game_viewport };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
  //cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(gs->game_viewport.w/2.0, gs->game_viewport.h/2.0), .origin = v2m(0,0), .zoom = gs->zoom, .rot_deg = 0} };
  cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(0,0), .origin = v2m(0,0), .zoom = gs->zoom, .rot_deg = 0} };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);

  level_render((Level*)gs->level, gs);

  // GUI logic
  if (!gs->level) {
    gui_frame_begin(gs->screen_dim, &gs->input, &gs->cmd_list, dt);
    static Simple_Game_Options opt;
    gui_simple_game_options_menu(MAKE_STR("Main_Pane"), &opt);
    if (opt.start_btn_pressed) {
      gs->level = (void*)make_new_level(3, gs->next_level_idx++);

      // Calculate the default zoom for the level
      f32 zoom_squish_factor = 0.90;
      f32 tile_count_x = ((Level*)gs->level)->pit_count_x * 3;
      f32 xzoom = gs->game_viewport.w / tile_count_x;
      f32 tile_count_y = ((Level*)gs->level)->pit_count_y * 3;
      f32 yzoom = gs->game_viewport.h / tile_count_y;
      gs->zoom = minimum(xzoom, yzoom)*zoom_squish_factor;
    }
    if (opt.exit_btn_pressed) {
      gs->should_close = true;
    }
    gui_frame_end();
  }
}

void game_shutdown(Game_State *gs) {
  // This COULD be used for the persistent
  // GUI stuff outlined in game_update(!!)
}

