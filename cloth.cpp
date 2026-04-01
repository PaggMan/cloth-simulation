#include "cloth.h"
#include <iostream>

Cloth::Cloth(int w, int h, float s, float kval, Sphere* sph, int stps) {
    width = w;
    height = h;
    spacing = s;
    k = kval;
    ver = true;
    sphere = sph;
    steps = stps;
    fixed = true;
    buildParticles();
    buildSprings();
}

Cloth::Cloth(float s, float kval, vec3 p1, vec3 p2, vec3 p3, Sphere* sph, int stps, bool f) {
    spacing = s;
    ver = false;
    k = kval;
    c1 = p1;
    c2 = p2;
    c3 = p3;
    sphere = sph;
    steps = stps;
    fixed = f;
    buildParticles(p1, p2, p3);
    buildSprings();
}

void Cloth::reset() {
    particles.clear();
    springs.clear();
    if(ver) {
        buildParticles();
    }
    else {
        buildParticles(c1, c2, c3);
    }
    buildSprings();
}

void Cloth::buildParticles() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            vec3 position(x*spacing, -y*spacing, 0);
            bool f = (y == 0 && (x == 0 || x == width-1));
            Particle p(position, f);
            particles.push_back(p);
        }
    }
}

void Cloth::buildParticles(vec3 &p1, vec3 &p2, vec3 &p3) {
    width = int((p2 - p1).magnitude() / spacing) + 1;
    height = int((p3 - p1).magnitude() / spacing) + 1;
    vec3 x_step = (p2 - p1) / (width - 1);
    vec3 y_step  = (p3 - p1) / (height - 1);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            vec3 position = p1 + x_step * x + y_step * y;
            bool f;
            if(fixed) f = (y == 0 && (x == 0 || x == width-1));
            else f = false;
            Particle p(position, f);
            particles.push_back(p);
        }
    }
}

void Cloth::buildSprings() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            for(int i = 1; x < width-i && i <= 2; i++) {
                springs.emplace_back(convertIndex(x, y), convertIndex(x+i, y), spacing*i, k);
            }
            for(int i = 1; y < height-i && i <= 2; i++) {
                springs.emplace_back(convertIndex(x, y), convertIndex(x, y+i), spacing*i, k);
            }
            if(x < width-1 && y < height-1) {
                springs.emplace_back(convertIndex(x, y), convertIndex(x+1, y+1), spacing*sqrt(2), k);
                springs.emplace_back(convertIndex(x+1, y), convertIndex(x, y+1), spacing*sqrt(2), k);
            }
        }
    }
}

void Cloth::computeForces() {
    for(auto &p : particles) {
        p.force = vec3(0, -9.8 * p.mass, 0);
    }
    for(auto &s : springs) {
        Particle &p1 = particles[s.particle1];
        Particle &p2 = particles[s.particle2];
        float deltax = (p2.position - p1.position).magnitude();
        vec3 dir = (p2.position - p1.position).normalized();
        vec3 force = dir * (deltax - s.length) * s.k;
        if(p1.fixed || p2.fixed) force *= 5.0f;
        p1.force += force;
        p2.force -= force;
    }
    for(auto &p : particles) {
        float damping = 1.0f;
        p.force -= p.velocity * damping;
    }
    for(int y = 0; y < height-1; y++) {
        for(int x = 0; x < width-1; x++) {
            auto &p1 = particles[convertIndex(x, y)];
            auto &p2 = particles[convertIndex(x+1, y)];
            auto &p3 = particles[convertIndex(x, y+1)];
            auto &p4 = particles[convertIndex(x+1, y+1)];

            vec3 normal1 = cross(p2.position - p1.position, p3.position - p1.position).normalized();
            float strength1 = dot(normal1, wind.normalized());
            // if(strength1 < 0) strength1 = 0;
            vec3 windForce1 = normal1 * strength1 * wind.magnitude();
            p1.force += windForce1;
            p2.force += windForce1;
            p3.force += windForce1;

            vec3 normal2 = cross(p4.position - p2.position, p2.position - p2.position).normalized();
            float strength2 = dot(normal2, wind.normalized());
            // if(strength2 < 0) strength2 = 0;
            vec3 windForce2 = normal2 * strength2 * wind.magnitude();
            p2.force += windForce2;
            p3.force += windForce2;
            p4.force += windForce2;
        }
    }
}

void Cloth::integrate(float deltat) {
    for(auto &p : particles) {
        if(p.fixed) continue;
        vec3 acceleration = p.force / p.mass;
        // p.velocity += acceleration * deltat;
        // p.position += p.velocity * deltat;

        // p.position * 2 - p.prevp

        float damping = 0.95f;
        vec3 new_p = p.position + (p.position - p.prevp) * damping + acceleration * deltat * deltat;
        p.prevp = p.position;
        p.position = new_p;
        p.velocity = (p.position - p.prevp) / deltat;
        if(sphere) {
            float dist = (sphere->center - p.position).magnitude();
            if(dist < sphere->radius) {
                p.position = sphere->center + (p.position - sphere->center).normalized() * sphere->radius + 1e-5;
            }
        }
        if(p.position[1] < -15) p.position[1] = -15;
    }
}

int Cloth::convertIndex(int x, int y) {
    return x + y * width;
}