

#ifndef MACROS_H
#define MACROS_H

#if defined(_WIN64) || defined(__WIN32) || defined(__APPLE__)
#define RENDER_ENABLED 1
#define RENDER_DISABLED 0
#else
#define RENDER_ENABLED 0
#define RENDER_DISABLED 1
#endif // platform specifications

#endif //MACROS_H
