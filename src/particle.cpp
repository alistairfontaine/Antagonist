#include "particle.h"

/*
   The Master Particle Ring Buffer Allocation Pool.
   Allocates exactly 128 continuous particle slots inside our data segment.
   Zeroed out at boot time; completely shielded from stack-frame collisions.
*/
static Particle particle_pool[MAX_PARTICLES] __attribute__((aligned(4)));
static uint32_t next_particle_slot = 0;

/*
   Clears out the particle pool elements at system startup.
*/
extern "C" void init_particle_system() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particle_pool[i].active = 0;
        particle_pool[i].lifetime_ticks = 0;
    }
    next_particle_slot = 0;
}

/*
   Spawns a new particle tracking element inside the static ring buffer.
   Overwrites the oldest slot in constant time O(1) if the buffer wraps around.
*/
extern "C" void spawn_particle(Vec3 pos, Vec3 vel, uint32_t color, int lifespan) {
    uint32_t slot = next_particle_slot;

    particle_pool[slot].active = 1;
    particle_pool[slot].position = pos;
    particle_pool[slot].velocity = vel;
    particle_pool[slot].color = color;
    particle_pool[slot].lifetime_ticks = lifespan;

    // Advance our circular ring index tracker pointer safely
    next_particle_slot = (next_particle_slot + 1) % MAX_PARTICLES;
}

/*
   The Master Particle Physics Simulation & Rasterization Loop.
   Updates vector coordinates, applies lifetime tick decay, and maps
   3D world positions directly onto the 2D staging double-buffer grid.
*/
extern "C" void update_and_render_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // Skip unallocated or dead tracking entries instantly
        if (!particle_pool[i].active) {
            continue;
        }

        // 1. Decay the lifetime parameter bounds
        particle_pool[i].lifetime_ticks--;
        if (particle_pool[i].lifetime_ticks <= 0) {
            particle_pool[i].active = 0;
            continue;
        }

        // 2. Execute velocity step integration maps across position coordinates
        particle_pool[i].position.x += particle_pool[i].velocity.x;
        particle_pool[i].position.y += particle_pool[i].velocity.y;
        particle_pool[i].position.z += particle_pool[i].velocity.z;

        /*
           3. 3D-to-2D Viewport Projection Mapping Transformation.
              Translates raw player map offsets into local screen pixels.
              Simplified raycast space translation approximation for fast CPU math.
        */
        float screen_x = (particle_pool[i].position.y * 40.0f) + (float)(SCREEN_WIDTH / 2);
        float screen_y = (particle_pool[i].position.z * 40.0f) + (float)(SCREEN_HEIGHT / 2);

        int px = (int)screen_x;
        int py = (int)screen_y;

        // 4. Render a clean 2x2 colored pixel block onto our hidden back-buffer matrix
        if (px >= 0 && px < SCREEN_WIDTH - 1 && py >= 0 && py < SCREEN_HEIGHT - 1) {
            uint32_t c = particle_pool[i].color;
            put_pixel_buffer(px,     py,     c);
            put_pixel_buffer(px + 1, py,     c);
            put_pixel_buffer(px,     py + 1, c);
            put_pixel_buffer(px + 1, py + 1, c);
        }
    }
}
