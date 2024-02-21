#ifndef SPHERE_H
#define SPHERE_H

class Sphere {
public:
    void draw(const float radius);

private:
    unsigned int VBO, VAO;
    constexpr static float X { 0.525731112119133606f };
    constexpr static float Z { 0.850650808352039932f };

    constexpr static float vdata[12][3] {
        { -X, 0, Z }, { X, 0, Z }, { -X, 0, -Z }, { X, 0, -Z },
        { 0, Z, X }, { 0, Z, -X }, { 0, -Z, X }, { 0, -Z, -X },
        { Z, X, 0 }, { -Z, X, 0 }, { Z, -X, 0 }, { -Z, -X, 0 }
    };

    constexpr static unsigned int tindices[20][3] = {
        { 1, 4, 0 }, { 4, 9, 0 }, { 4, 9, 5 }, { 8, 5, 4 }, { 1, 8, 4 },
        { 1, 10, 8 }, { 10, 3, 8 }, { 8, 3, 5 }, { 3, 2, 5 }, { 3, 7, 2 },
        { 3, 10, 7 }, { 10, 6, 7 }, { 6, 11, 7 }, { 6, 0, 11 }, { 6, 1, 0 },
        { 10, 1, 6 }, { 11, 0, 9 }, { 2, 11, 9 }, { 5, 2, 9 }, { 11, 2, 7 }
    };
};

#endif // !
