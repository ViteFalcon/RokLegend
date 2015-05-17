SET(RO_MATH_SOURCES
    RoVector2f.cpp
    RoVector2f.h
    RoVector3f.cpp
    RoVector3f.h
)

ADD_LIBRARY(${RO_MATH_LIBRARY} STATIC ${RO_MATH_SOURCES})