#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdint.h>
#include "math3d.h"
#include "buffer.h"
#include "palette.h"

/*
   Total Concurrent Particle Storage Limits.
   Allocates a strict max boundary of 128 active visual tracers in a static global ring array,
   ensuring the engine completely caps memory overhead while tracking system trajectories.
*/
#define MAX_PARTICLES         128

struct Particle {
    uint8_t active;
    Vec3 position;
    Vec3 velocity;
    uint32_t color;
    int lifetime_ticks;
};

/* Expose our raw visual particle debugger routines cleanly to C linkage maps */
extern "C" {
    void init_particle_system();
    void spawn_particle(Vec3 pos, Vec3 vel, uint32_t color, int lifespan);
    void update_and_render_particles();
}

#endif
