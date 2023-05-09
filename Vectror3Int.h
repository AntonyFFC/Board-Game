#pragma once
#ifndef VECTOR3INT_H
#define VECTOR3INT_H

struct Vector3Int {
    int x, y, z;
    Vector3Int(int ix = 0, int iy = 0, int iz = 0) : x(ix), y(iy), z(iz) {}
};

#endif