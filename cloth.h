#ifndef __CLOTH__
#define __CLOTH__

#include <vector>
#include "vec.h"
#include "spring.h"
#include "particle.h"
#include "sphere.h"

class Cloth {
    public:
        int width;
        int height;
        float spacing;
        float k;
        bool ver;
        vec3 c1;
        vec3 c2;
        vec3 c3;
        Sphere *sphere;
        int steps;
        bool fixed;
        vec3 wind;

        std::vector<Particle> particles;
        std::vector<Spring> springs;

        Cloth(int w, int h, float s, float kval, Sphere *sph, int stps);
        Cloth(float s, float kval, vec3 p1, vec3 p2, vec3 p3, Sphere *sph, int stps, bool f);
        void reset();
        void buildParticles();
        void buildParticles(vec3 &p1, vec3 &p2, vec3 &p3);
        void buildSprings();
        void computeForces();
        void integrate(float deltat);
        int convertIndex(int x, int y);
};

#endif