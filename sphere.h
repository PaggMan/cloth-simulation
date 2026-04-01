#ifndef __SPHERE__
#define __SPHERE__

#include <vector>
#include "vec.h"

constexpr int sectorCount = 72;
constexpr int stackCount = 24;

class Sphere {
    public:
        vec3 center;
        float radius;
        std::vector<vec3> particles;
        std::vector<int> triangleIndices;
        std::vector<int> lineIndices;
        Sphere(vec3 c, float r) {
            center = c;
            radius = r;

            float sectorStep = 2 * pi / sectorCount;
            float stackStep = pi / stackCount;
            float sectorAngle, stackAngle;

            for(int i = 0; i <= stackCount; i++) {
                stackAngle = pi / 2 - i * stackStep;
                float xy = radius * cosf(stackAngle);
                float z = radius * sinf(stackAngle);

                for(int j = 0; j <= sectorCount; j++) {
                    sectorAngle = j * sectorStep;
                    float x = xy * cosf(sectorAngle);
                    float y = xy * sinf(sectorAngle);
                    particles.emplace_back(x, y, z);
                }
            }

            for(int i = 0; i < stackCount; i++) {
                int k1 = i * (sectorCount + 1);
                int k2 = k1 + sectorCount + 1;

                for(int j = 0; j < sectorCount; j++, k1++, k2++) {
                    if(i != 0) {
                        triangleIndices.push_back(k1);
                        triangleIndices.push_back(k2);
                        triangleIndices.push_back(k1+1);
                    }
                    if(i != stackCount-1) {
                        triangleIndices.push_back(k1+1);
                        triangleIndices.push_back(k2);
                        triangleIndices.push_back(k2+1);
                    }
                    lineIndices.push_back(k1);
                    lineIndices.push_back(k2);
                    if(i != 0) {
                        lineIndices.push_back(k1);
                        lineIndices.push_back(k1+1);
                    }
                }
            }

            changeUpAxis(3, 2);
        }
        void changeUpAxis(int from, int to) {
            float tx[] = {1.0f, 0.0f, 0.0f};
            float ty[] = {0.0f, 0.0f, -1.0f};
            float tz[] = {0.0f, 1.0f, 0.0f};

            std::size_t i;
            std::size_t count = particles.size();
            float vx, vy, vz;
            for(i = 0; i < count; i += 1) {
                vx = particles[i][0];
                vy = particles[i][1];
                vz = particles[i][2];
                particles[i][0] = tx[0] * vx + ty[0] * vy + tz[0] * vz;
                particles[i][1] = tx[1] * vx + ty[1] * vy + tz[1] * vz;
                particles[i][2] = tx[2] * vx + ty[2] * vy + tz[2] * vz;
            }

            for(auto &p : particles) {
                p[0] += center[0];
                p[1] += center[1];
                p[2] += center[2];
            }
        }
};

#endif