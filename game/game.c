#include "game.h"
#include "gui/gui.h"
#include "level.h"
#include "entity.h"
#include "stdlib.h"

static Level level;

// https://www.gamedeveloper.com/programming/improved-lerp-smoothing-

// There are goblins and peasants coming out of each hole
// You want to find and WHACK as many goblins as possible
// they can fart though and create fog of war YOU must cleanup
// You can have hammer and cleaning product and drag them or become a cursor or?
// Maybe just scroll between them

void game_init(Game_State *gs) {
  gs->game_init_timestamp = gs->time_sec; 
  level = make_new_level();


  // Generate the map
  level.pit_count_x = 3;
  level.pit_count_y = 3;
  level.pit_dim_px = 50;
  for (u32 i = 0; i < level.pit_count_x; i+=1) {
    for (u32 j = 0; j < level.pit_count_y; j+=1) {
      for (u32 mb_i = 0; mb_i < level.pit_count_x; mb_i+=1) {
        for (u32 mb_j = 0; mb_j < level.pit_count_x; mb_j+=1) {
          b32 entity_is_pit = (mb_i == 1 && mb_j == 1);
          Entity *e = level_add_entity(&level);
          *e = (Entity) {
            .tile_pos = v2m(i * 3 + mb_i, j * 3 + mb_j),
            .kind =  entity_is_pit ? ENTITY_KIND_PIT : ENTITY_KIND_GROUND,
            .tc = v2m(entity_is_pit ? 8*2 : 8*1, 0),
            .dim_px = level.pit_dim_px,
          };
        }
      }
    }
  }

  // Spawn humans
  u32 humans_to_spawn = 2;
  for (u32 human_idx = 0; human_idx < humans_to_spawn; human_idx+=1) {
    v2 tp = v2m(human_idx, human_idx);
    Entity *e = level_add_entity(&level);

    *e = (Entity) {
      .tile_pos = tp,
      .kind = ENTITY_KIND_HUMAN, 
      .tc = v2m(8*7, 0),
      .dim_px = level.pit_dim_px,
    };
  }


}

void game_update(Game_State *gs, float dt) {
  static bool gui_initialized = false;
  if (!gui_initialized) {
    gui_context_init(gs->frame_arena, &gs->font);
    gui_initialized = true;
  }
  gs->game_viewport = rec(0,0,gs->screen_dim.x, gs->screen_dim.y);
  level_update(&level, gs, dt);
}


void game_render(Game_State *gs, float dt) {
  // Push viewport, scissor and camera (we will not change these the whole frame except in UI pass)
  R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_VIEWPORT, .r = gs->game_viewport };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
  cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_SCISSOR, .r = gs->game_viewport };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
  //cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(gs->game_viewport.w/2.0, gs->game_viewport.h/2.0), .origin = v2m(0,0), .zoom = gs->zoom, .rot_deg = 0} };
  cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(0,0), .origin = v2m(0,0), .zoom = 1.0, .rot_deg = 0} };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);


  level_render(&level, gs);

  // ..
  // ..
  // In the end, perform a UI pass (TBA)
  // Right now: Print debug hero info stuff
  static u32 squish_count = 0;
  if (input_mkey_pressed(&gs->input, INPUT_MOUSE_RMB)) {
    squish_count+=1;
  }
  gui_frame_begin(gs->screen_dim, &gs->input, &gs->cmd_list, dt);

	gui_set_next_child_layout_axis(GUI_AXIS_X);
  gui_set_next_pref_height((Gui_Size){GUI_SIZE_KIND_PARENT_PCT, 1.0, 1.0});
  gui_set_next_pref_width((Gui_Size){GUI_SIZE_KIND_PARENT_PCT, 1.0, 1.0});
  Gui_Signal debug_pane = gui_pane(MAKE_STR("Main_Pane"));
  gui_push_parent(debug_pane.box);
  {
    gui_set_next_bg_color(col(0.1, 0.2, 0.4, 0.5));
    gui_set_next_text_alignment(GUI_TEXT_ALIGNMENT_CENTER);
    gui_set_next_pref_width((Gui_Size){GUI_SIZE_KIND_TEXT_CONTENT, 10.0, 1.0});
    gui_set_next_text_color(col(0.7, 0.8, 0.1, 0.9));
    buf hero_info = arena_sprintf(gs->frame_arena, "Press RMB for a nice squish");
    gui_label(hero_info);
  }
  gui_pop_parent();

	gui_set_next_child_layout_axis(GUI_AXIS_X);
  gui_set_next_pref_height((Gui_Size){GUI_SIZE_KIND_PARENT_PCT, 1.0, 1.0});
  gui_set_next_pref_width((Gui_Size){GUI_SIZE_KIND_PARENT_PCT, 1.0, 1.0});
  Gui_Signal debug_pane2 = gui_pane(MAKE_STR("Main_Pane2"));
  {
    gui_push_parent(debug_pane2.box);
    gui_set_next_bg_color(col(0.3, 0.1, 0.7, 0.5));
    gui_set_next_text_color(col(0.7, 0.8, 0.1, 0.9));
    gui_set_next_pref_width((Gui_Size){GUI_SIZE_KIND_TEXT_CONTENT, 10.0, 1.0});
    buf high_entity_info = arena_sprintf(gs->frame_arena, "squish count: %d", squish_count);
    gui_label(high_entity_info);

    /*
    buf choices[4] = {
      [0] = arena_sprintf(gs->frame_arena, "ease_in_quad"),
      [1] = arena_sprintf(gs->frame_arena, "ease_out_quad"),
      [2] = arena_sprintf(gs->frame_arena, "ease_in_qubic"),
      [3] = arena_sprintf(gs->frame_arena, "ease_out_qubic"),
    };
    gui_choice_box(buf_make("ease func", 6), choices, 4);
    */
  }
  gui_pop_parent();
  //--------------
  gui_frame_end();
}

void game_shutdown(Game_State *gs) {
  // This COULD be used for the persistent
  // GUI stuff outlined in game_update(!!)
}

