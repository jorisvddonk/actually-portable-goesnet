// GOESNET module compatibility defines, for easier porting of modules to Cosmopolitan libc!

// Compatibilty with Cosmopolitan libc:
#define _read read
#define _open open
#define _argc argc
#define _argv argv
#define _close close
#define strupr brtl_strupr
#define random brtl_random
#define srand brtl_srand

// Various noctis defines
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
