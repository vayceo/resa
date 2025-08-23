//
// Created on 21.09.2023.
//

#ifndef RUSSIA_RQVECTOR_H
#define RUSSIA_RQVECTOR_H


class RQVector {

    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };
};


#endif //RUSSIA_RQVECTOR_H
