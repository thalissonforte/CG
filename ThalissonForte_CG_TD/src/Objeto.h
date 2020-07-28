#ifndef OBJETO_H
#define OBJETO_H

class Objeto{

public:
    float x, y, z;

    Objeto(float _x, float _y, float _z){
        x = _x;
        y = _y;
        z = _z;
    }

    void Set_pos(float _x, float _y, float _z){
        x = _x;
        y = _y;
        z = _z;
    }
};

#endif
