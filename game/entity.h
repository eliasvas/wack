#ifndef ENTITY_H__
#define ENTITY_H__
#include "game.h"

typedef enum {
  ENTITY_KIND_PIT,
  ENTITY_KIND_GROUND,
  ENTITY_KIND_GOBLIN,
  ENTITY_KIND_HUMAN,
} Entity_Kind;

typedef struct {
  f32 dim_px;
  v2 tc;
  Entity_Kind kind;

  v2 tile_pos;
  f32 anim_speed;
  f32 anim_time;
  v2 anim_tile_pos;

} Entity;

void make_pit_entity(Entity *e, v2 tile_pos, f32 dim);
void make_ground_entity(Entity *e, v2 tile_pos, f32 dim);
void make_human_entity(Entity *e, v2 tile_pos, f32 dim);
void make_goblin_entity(Entity *e, v2 tile_pos, f32 dim);
void entity_render(Game_State *gs, Entity *e);
void entity_update(Game_State *gs, Entity *e, f64 dt);

#endif
