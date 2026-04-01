#include "entity.h"
#include "stdlib.h"

void entity_update(Game_State *gs, Entity *e, f64 dt) {
  switch(e->kind) {
    case ENTITY_KIND_GROUND:
      e->anim_tile_pos = e->tile_pos;
      break;
    case ENTITY_KIND_PIT:
      e->anim_tile_pos = e->tile_pos;
      break;
    case ENTITY_KIND_GOBLIN:
    case ENTITY_KIND_HUMAN:
      e->anim_time += dt*e->anim_speed;
      f32 anim_factor = maximum(sin_f32(e->anim_time*2*PI), 0);
      e->anim_tile_pos = v2_add(v2_add(e->tile_pos, v2m(0,1)), v2m(0, -anim_factor));
      break;
    default:
  }
}

void make_goblin_entity(Entity *e, v2 tile_pos, f32 dim) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*10;
  e->kind = ENTITY_KIND_GOBLIN;
  e->dim_px = dim;
  e->anim_speed = 1.0 + (rand()%10)/10.0;
}

void make_human_entity(Entity *e, v2 tile_pos, f32 dim) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*7;
  e->kind = ENTITY_KIND_HUMAN;
  e->dim_px = dim;
  e->anim_speed = 0.5 + (rand()%10)/10.0;
}

void make_ground_entity(Entity *e, v2 tile_pos, f32 dim) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*1;
  e->kind = ENTITY_KIND_GROUND;
  e->dim_px = dim;
  e->anim_speed = 1.0;
}

void make_pit_entity(Entity *e, v2 tile_pos, f32 dim) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*2;
  e->kind = ENTITY_KIND_PIT;
  e->dim_px = dim;
  e->anim_speed = 1.0;
}

void entity_render(Game_State *gs, Entity *e) {
  f32 anim_factor = maximum(sin_f32(e->anim_time*2*PI), 0);
  f32 visibility_factor = (e->kind == ENTITY_KIND_HUMAN || e->kind == ENTITY_KIND_GOBLIN) ? anim_factor : 1;
  R2D_Quad quad = (R2D_Quad) {
      .src_rect = rec(e->tc.x, e->tc.y,8,8*visibility_factor),
      .dst_rect = rec(e->dim_px*e->anim_tile_pos.x, e->dim_px*e->anim_tile_pos.y, e->dim_px, e->dim_px*visibility_factor),
      .c = col(1,1,1,1),
      .tex = gs->atlas,
      .rot_deg = 0,
  };
  R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_ADD_QUAD, .q = quad};
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
}

