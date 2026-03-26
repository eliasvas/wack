#include "level.h"

// Bullshit, FIX it - shouldn't we have a good random utility??!??!?!?!?!?
#include "stdlib.h"
#include "time.h"

void level_deinit(Level *level) {
  arena_destroy(level->arena);
}

Level* make_new_level(u32 pit_count, u32 level_idx) {
  // 1. Allocate level in it's own arena
  Arena *arena = arena_make(GB(1));
  Level *level = arena_push_struct(arena, Level);
  level->arena = arena;

  // Generate a ~random level (lets say of _progressing difficulty)
  srand(time(0));
  level->pit_count_x = pit_count;
  level->pit_count_y = pit_count;
  level->pit_dim_px = 50;
  for (u32 i = 0; i < level->pit_count_x; i+=1) {
    for (u32 j = 0; j < level->pit_count_y; j+=1) {
      for (u32 mb_i = 0; mb_i < level->pit_count_x; mb_i+=1) {
        for (u32 mb_j = 0; mb_j < level->pit_count_x; mb_j+=1) {
          b32 entity_is_pit = (mb_i == 1 && mb_j == 1);
          if (entity_is_pit) {
            make_pit_entity(level_add_entity(level), v2m(i * 3 + mb_i, j * 3 + mb_j), level->pit_dim_px);
          } else {
            make_ground_entity(level_add_entity(level), v2m(i * 3 + mb_i, j * 3 + mb_j), level->pit_dim_px);
          }
        }
      }
    }
  }

  // TODO: Find a way to scale this with pit count! so we put the guys in the middle! maybe
  for (u32 i = 1; i < 9; i += 3) {
    for (u32 j = 1; j < 9; j += 3) {
      if ((rand()%10)+level_idx < 5) {
        make_human_entity(level_add_entity(level), v2m(i,j), level->pit_dim_px);
      }else {
        make_goblin_entity(level_add_entity(level), v2m(i,j), level->pit_dim_px);
      }
    }
  } 
  // X. Finally return the level
  return level;
}

Entity *level_add_entity(Level *level) {
  Entity *e = nullptr;
  if (level->entities.cap == 0) {
    level->entities.cap = 256;
    level->entities.used = arena_push(level->arena, sizeof(b32) * level->entities.cap);
    level->entities.e = arena_push(level->arena, sizeof(Entity) * level->entities.cap);
  }
  assert(level->entities.cap > level->entities.count);
  e = &level->entities.e[level->entities.count++];
  return e;
}

b32 level_update(Level *level, Game_State *gs, f64 dt) {
  if (level) {
    for (u32 entity_idx = 0; entity_idx < level->entities.count; entity_idx+=1) {
      Entity *e = &(level->entities.e[entity_idx]);
      entity_update(gs, e, dt);
    }
		if (input_key_pressed(&gs->input, KEY_SCANCODE_ESCAPE)) {
      return true;
    }
  }
  return false;
}

void level_render(Level *level, Game_State *gs) {
  if (level) {
    R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(gs->game_viewport.w/2,gs->game_viewport.h/2), .origin = v2m(level->pit_count_x * level->pit_dim_px * 3 / 2,level->pit_count_y * level->pit_dim_px * 3 / 2), .zoom = 1.0, .rot_deg = 0} };
    r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);

    for (u32 entity_idx = 0; entity_idx < level->entities.count; entity_idx+=1) {
      Entity *e = &(level->entities.e[entity_idx]);
      entity_render(gs, e);
    }
  }
}



