#ifndef VADD_H
#define VADD_H

#define SIZE 256

extern "C" {
    void vadd(int* c,
        const int* a,
        const int* b);
}

#endif  // VADD_H