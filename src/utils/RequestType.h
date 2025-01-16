//
// Created by PC on 14.01.2025.
//

#ifndef COURSE_PROJECT_REQUESTTYPE_H
#define COURSE_PROJECT_REQUESTTYPE_H

#include <string>
#include <cstdint>


enum class RequestType : std::uint8_t {
    FUPLOAD = 0,
    FSEARCH = 1,
    FDELETE = 2
};

#endif //COURSE_PROJECT_REQUESTTYPE_H
