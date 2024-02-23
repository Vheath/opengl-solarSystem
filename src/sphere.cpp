#include <glad/glad.h>

#include "include/sphere.h"
#include <GLFW/glfw3.h>
#include <cmath>

Sphere::Sphere()
{
    genNorm();

    buildArr();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(float), &arr[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)+(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Sphere::draw(float radius)
{

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, tindices);
}

void Sphere::icoNormVec(int i)
{

    float d1[3], d2[3];
    for (int k { 0 }; k < 3; k++) {
        d1[k] = vdata[tindices[i][0]][k]
            - vdata[tindices[i][1]][k];
        d2[k] = vdata[tindices[i][1]][k]
            - vdata[tindices[i][2]][k];
    }
    double absval;
    n[0] = d1[1] * d2[2] - d1[2] * d2[1];
    n[1] = d1[2] * d2[0] - d1[0] * d2[2];
    n[2] = d1[0] * d2[1] - d1[1] * d2[0];

    absval = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    n[0] /= absval;
    n[1] /= absval;
    n[2] /= absval;
}

void Sphere::genNorm()
{
    for (int i { 0 }; i < 12; ++i) {
        icoNormVec(i);
        normals[i][0] = n[0];
        normals[i][1] = n[1];
        normals[i][2] = n[2];
    }
}

void Sphere::buildArr()
{
    arr.clear();
    for (int i { 0 }; i < 12; ++i) {
        arr.push_back(vdata[i][0]);
        arr.push_back(vdata[i][1]);
        arr.push_back(vdata[i][2]);

        arr.push_back(normals[i][0]);
        arr.push_back(normals[i][1]);
        arr.push_back(normals[i][2]);
    }
}
