#ifndef _Utilities_h
#define _Utilities_h

// get number of arguments with __NARG__
#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

// definition for FOO
#define SerialPrintln(...) VFUNC(SerialPrintln, __VA_ARGS__)

#define SerialPrintln1(first) \
    Serial.println(first)

#define SerialPrintln2(first, second) \
    Serial.print(first); \
    Serial.println(second)

#define SerialPrintln3(first, second, third) \
    Serial.print(first); \
    Serial.print(second); \
    Serial.println(third)

#define SerialPrintln4(first, second, third, fourth) \
    Serial.print(first); \
    Serial.print(second); \
    Serial.print(third); \
    Serial.println(fourth)

#define SerialPrintln5(first, second, third, fourth, fifth) \
    Serial.print(first); \
    Serial.print(second); \
    Serial.print(third); \
    Serial.print(fourth); \
    Serial.println(fifth);

#define SerialPrintln6(first, second, third, fourth, fifth, sixth) \
    Serial.print(first); \
    Serial.print(second); \
    Serial.print(third); \
    Serial.print(fourth); \
    Serial.print(fifth); \
    Serial.println(sixth);

#define SerialPrintln7(first, second, third, fourth, fifth, sixth, seventh) \
    Serial.print(first); \
    Serial.print(second); \
    Serial.print(third); \
    Serial.print(fourth); \
    Serial.print(fifth); \
    Serial.print(sixth); \
    Serial.println(seventh)

#endif // _Utilities_h