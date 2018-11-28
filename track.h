#ifndef TRACK_H
#define TRACK_H

#include <Box2D/Box2D.h>

class Track {
public:
    Track(b2World *world);
    ~Track();

    b2Body *getBody();

private:
    void generate();

    constexpr static int LENGTH = 1000;
    constexpr static float TRACK_THICK = 0.15;


    b2World *b2world;
    b2Body *trackBody;
};

#endif // TRACK_H
