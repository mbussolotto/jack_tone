#ifndef __JACK_TONE_H
#define __JACK_TONE_H


#include <unistd.h>

    int process (size_t len, float *out);
    static void jack_shutdown(void *arg);
    float  frequency;
    float  offset;
    float  amp;
    int len;
    int totalLen;
    int fsamp;


#endif
