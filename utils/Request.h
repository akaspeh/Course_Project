//
// Created by PC on 14.01.2025.
//

#ifndef COURSE_PROJECT_REQUEST_H
#define COURSE_PROJECT_REQUEST_H

#include "RequestType.h"

struct Request {
    RequestType type;
    std::string data;
};

#endif //COURSE_PROJECT_REQUEST_H
