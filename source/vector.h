#ifndef VECTOR_H
#define VECTOR_H

class Vector {
    public:
        float x;
};

class Vector2: public Vector{
    public:
        float y;
};

class Vector3: public Vector2{
    public:
        float z;
        void SetPositon(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }
};

#endif