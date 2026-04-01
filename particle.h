#ifndef __PARTICLE__
#define __PARTICLE__

#include "vec.h"

class Particle {
    public:
        vec3 position;
        vec3 prevp;
        vec3 velocity;
        vec3 force;
        float mass;
        bool fixed;
        Particle(vec3 pos, bool f) {
            position = pos;
            prevp = pos;
            velocity = vec3();
            force = vec3();
            mass = 0.5f;
            fixed = f;
        }
};

#endif