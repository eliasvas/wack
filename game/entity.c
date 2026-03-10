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
      e->anim_tile_pos = e->tile_pos;
      break;
    case ENTITY_KIND_HUMAN:
      e->anim_time -= dt;
      if (e->anim_time < 0) {
        e->anim_time = (f32)(rand() % 10) / 3;
      }
      //e->anim_tile_pos = v2_add(v2_sub(e->tile_pos, v2m(1, 0)), v2m(sin_f32(anim_time) ,0));
      break;
    default:
  }
}

void make_ground_entity(Entity *e, v2 tile_pos) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*3;
  e->kind = ENTITY_KIND_GROUND;
}

void make_pit_entity(Entity *e, v2 tile_pos) {
  e->tile_pos = tile_pos;
  e->tc.x = 8*2;
  e->kind = ENTITY_KIND_PIT;
}

void entity_render(Game_State *gs, Entity *e) {
  R2D_Quad quad = (R2D_Quad) {
      .src_rect = rec(e->tc.x, e->tc.y,8,8),
      .dst_rect = rec(e->dim_px*e->tile_pos.x, e->dim_px*e->tile_pos.y, e->dim_px, e->dim_px),
      .c = col(1,1,1,1),
      .tex = gs->atlas,
      .rot_deg = 0,
  };
  R2D_Cmd cmd = (R2D_Cmd){ .kind = R2D_CMD_KIND_ADD_QUAD, .q = quad};
  r2d_push_cmd(gs->frame_arena, &gs->cmd_list, cmd, 256);
}

