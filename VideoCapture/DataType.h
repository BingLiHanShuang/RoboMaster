//
// Created by parallels on 3/25/17.
//

#ifndef VIDEOCAPTURE_DATATYPE_H
#define VIDEOCAPTURE_DATATYPE_H

#include <cstdlib>
#include <cstdint>

typedef struct BufferData{
    uint8_t * data;
    size_t len;
};
#endif //VIDEOCAPTURE_DATATYPE_H
