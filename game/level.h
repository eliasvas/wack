#ifndef LEVEL_H__
#define LEVEL_H__
#include "entity.h"

typedef struct {
  b32 *used;
  Entity *e;
  u32 count;
  u32 cap;
} Entities;

typedef struct {
  Arena *arena;
  Entities entities;

  f32 pit_dim_px;
  u32 pit_count_x;
  u32 pit_count_y;

} Level;
static Level make_new_level() {
  Level level = {};
  level.arena = arena_make(GB(1));

  return level;
}

static Entity *level_add_entity(Level *level) {
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

static void level_update(Level *level, Game_State *gs, f64 dt) {
  for (u32 entity_idx = 0; entity_idx < level->entities.count; entity_idx+=1) {
    Entity *e = &(level->entities.e[entity_idx]);
    entity_update(gs, e, dt);
  }
}

static void level_render(Level *level, Game_State *gs) {
  R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_SET_CAMERA, .c = (R2D_Cam){ .offset = v2m(gs->game_viewport.w/2,gs->game_viewport.h/2), .origin = v2m(level->pit_count_x * level->pit_dim_px * 3 / 2,level->pit_count_y * level->pit_dim_px * 3 / 2), .zoom = 1.0, .rot_deg = 0} };
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);

  for (u32 entity_idx = 0; entity_idx < level->entities.count; entity_idx+=1) {
    Entity *e = &(level->entities.e[entity_idx]);
    entity_render(gs, e);
  }


}


#endif
