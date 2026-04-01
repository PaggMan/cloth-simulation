#ifndef __SPRING__
#define __SPRING__

class Spring {
    public:
        int particle1;
        int particle2;
        float length;
        float k;
        Spring(int p1, int p2, float l, float kval) {
            particle1 = p1;
            particle2 = p2;
            length = l;
            k = kval;
        }
};
#endif