// src/xpt.h
#ifndef _XPT_H
#define _XPT_H

#ifdef XPT_ON

extern int xpt_flg;

#define XPT_ERR 0x0001
#define XPT_WRN 0x0002
#define XPT_SNP 0x0004
#define XPT_TST 0x0008

#define XPT(A, ...) {		\
    if (xpt_flg & A) {		\
        fprintf(stderr, __VA_ARGS__);	\
    }				\
}

#define XPT_VER(version) printf("version: %s\n", version)

#define XPT_INIT() {					\
    const char *xpt_env = getenv("XPT_FLG");		\
    if (xpt_env) {					\
        xpt_flg = (int)strtol(xpt_env, NULL, 16);	\
    }							\
} 

#else  // XPT_OFF

#define XPT(A, B)
#define XPT_VER(version)
#define XPT_INIT()

#endif  // XPT_ON

#endif  // _XPT_H
