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

Level* make_new_level(u32 pit_count, u32 level_idx);
void level_deinit(Level *level);

// TODO: Can we pass just the Render_commands OR produce some temp ones 
// !?!!?! Good design, first time honestly
b32 level_update(Level *level, Game_State *gs, f64 dt);
void level_render(Level *level, Game_State *gs);
Entity *level_add_entity(Level *level);


#endif
