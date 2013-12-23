#ifndef __TYPES__
#define __TYPES__

#define NULL ((void *) 0)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned int size_t;

typedef uint32_t phyaddr_t;
typedef uint32_t pginfo_t;

#define ROUNDDOWN(a, n)                     \
({                              \
    uint32_t __a = (uint32_t) (a);              \
    (typeof(a)) (__a - __a % (n));              \
})
// Round up to the nearest multiple of n
#define ROUNDUP(a, n)                       \
({                              \
    uint32_t __n = (uint32_t) (n);              \
    (typeof(a)) (ROUNDDOWN((uint32_t) (a) + __n - 1, __n)); \
})


#endif
