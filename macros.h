

#ifndef MACROS_H
#define MACROS_H

#define FORCE_DISABLE_RENDER 0


#if (defined(_WIN64) || defined(__WIN32) || defined(__APPLE__)) && (FORCE_DISABLE_RENDER == 0)
#define RENDER_ENABLED 1
#define RENDER_DISABLED 0
#else
#define RENDER_ENABLED 0
#define RENDER_DISABLED 1
#endif // platform specifications

// settings
#if RENDER_ENABLED
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
#endif

#define GRAVITY_ACC 3.0f
#define ZNEAR 0.1f
#define ZFAR 1000.0f

#define GJK_MAX_NUM_ITERATIONS 64
#define EPA_TOLERANCE 0.0001f
#define EPA_MAX_NUM_FACES 64
#define EPA_MAX_NUM_LOOSE_EDGES 32
#define EPA_MAX_NUM_ITERATIONS 64
#define BOUNDARY 1000000.0f

#endif //MACROS_H
