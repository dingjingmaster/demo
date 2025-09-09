#ifndef C_COMMON_STD_MACROS_H
#define C_COMMON_STD_MACROS_H
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef C_IN
#define C_IN
#endif

#ifndef C_OUT
#define C_OUT
#endif

#ifdef __GNUC__
#define C_GNUC_CHECK_VERSION(major, minor)                                      ((__GNUC__ > (major)) || ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define C_GNUC_CHECK_VERSION(major, minor)                                      0
#endif


// 检测 编译器 是否支持 c11 标准
#ifndef C_SUPPORTED_C11
#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#define C_SUPPORTED_C11 1
#elif defined(__clang__) && __clang_major__ >= 3 && __clang_minor__ >= 0
#define C_SUPPORTED_C11 1
#else
#define C_SUPPORTED_C11 0
#endif
#endif

/**
 * @brief 是否支持C++11标准
 */
#ifndef C_SUPPORTED_CXX11
#if defined(__cplusplus) && __cplusplus >= 201103L
#define C_SUPPORTED_CXX11 1
#else
#define C_SUPPORTED_CXX11 0
#endif
#endif

#ifndef C_SUPPORTED_CXX14
#if defined(__cplusplus) && __cplusplus >= 201304L
#define C_SUPPORTED_CXX14 1
#else
#define C_SUPPORTED_CXX14 0
#endif
#endif

#if C_SUPPORTED_CXX11
#define C_DECL_NOEXCEPT noexcept
#define C_DECL_NOEXCEPT_EXPR(x) noexcept(x)
#else
#define C_DECL_NOEXCEPT
#define C_DECL_NOEXCEPT_EXPR(x)
#endif

#if C_SUPPORTED_CXX14
# define C_DECL_CONSTEXPR constexpr
# define C_DECL_RELAXED_CONSTEXPR constexpr
# define C_CONSTEXPR constexpr
# define C_RELAXED_CONSTEXPR constexpr
#else
# define C_DECL_CONSTEXPR
# define C_DECL_RELAXED_CONSTEXPR
# define C_CONSTEXPR
# define C_RELAXED_CONSTEXPR
#endif

/**
 * @brief 
 *  warning_push
 *  warning_pop
 */
#define C_DO_PRAGMA(text)                      _Pragma(#text)
#if defined(__INTEL_COMPILER) && defined(_MSC_VER)
# undef QT_DO_PRAGMA
# define C_WARNING_PUSH                         __paragma(warning(push))
# define C_WARNING_POP                          __pragma(warning(pop))
# define C_WARNING_DISABLE_MSVC(number)
# define C_WARNING_DISABLE_INTEL(number)        __pragma(warning(disable: number))
# define C_WARNING_DISABLE_CLANG(text)
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_DISABLE_DEPRECATED           C_WARNING_DISABLE_INTEL(1478 1786)
#elif defined(__INTEL_COMPILER)
# define C_WARNING_PUSH                         C_DO_PRAGMA(warning(push))
# define C_WARNING_POP                          C_DO_PRAGMA(warning(pop))
# define C_WARNING_DISABLE_INTEL(number)        C_DO_PRAGMA(warning(disable: number))
# define C_WARNING_DISABLE_MSVC(number)
# define C_WARNING_DISABLE_CLANG(text)
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_DISABLE_DEPRECATED           C_WARNING_DISABLE_INTEL(1478 1786)
#elif defined(_MSC_VER) && !defined(__clang__)
# undef QT_DO_PRAGMA
# define C_WARNING_PUSH                         __pragma(warning(push))
# define C_WARNING_POP                          __pragma(warning(pop))
# define C_WARNING_DISABLE_MSVC(number)         __pragma(warning(disable: number))
# define C_WARNING_DISABLE_INTEL(number)
# define C_WARNING_DISABLE_CLANG(text)
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_DISABLE_DEPRECATED           C_WARNING_DISABLE_MSVC(4996)
#elif defined(__clang__) && ((__clang_major__ * 100) + __clang_minor__)
# define C_WARNING_PUSH                         C_DO_PRAGMA(clang diagnostic push)
# define C_WARNING_POP                          C_DO_PRAGMA(clang diagnostic pop)
# define C_WARNING_DISABLE_CLANG(text)          C_DO_PRAGMA(clang diagnostic ignored text)
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_DISABLE_INTEL(number)
# define C_WARNING_DISABLE_MSVC(number)
# define C_WARNING_DISABLE_DEPRECATED           C_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
# define C_WARNING_PUSH                         C_DO_PRAGMA(GCC diagnostic push)
# define C_WARNING_POP                          C_DO_PRAGMA(GCC diagnostic pop)
# define C_WARNING_DISABLE_GCC(text)            C_DO_PRAGMA(GCC diagnostic ignored text)
# define C_WARNING_DISABLE_CLANG(text)
# define C_WARNING_DISABLE_INTEL(number)
# define C_WARNING_DISABLE_MSVC(number)
# define C_WARNING_DISABLE_DEPRECATED           C_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#else
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_PUSH
# define C_WARNING_POP
# define C_WARNING_DISABLE_INTEL(number)
# define C_WARNING_DISABLE_MSVC(number)
# define C_WARNING_DISABLE_CLANG(text)
# define C_WARNING_DISABLE_GCC(text)
# define C_WARNING_DISABLE_DEPRECATED
#endif

#ifndef C_IGNORE_DEPRECATIONS
#define C_IGNORE_DEPRECATIONS(statement) \
    C_WARNING_PUSH \
    C_WARNING_DISABLE_DEPRECATED \
    statement \
    C_WARNING_POP
#endif

#if defined(__GNUC__)
#ifndef C_ALIGNOF
#define C_ALIGNOF(type)         __alignof__ (type)
#endif
#ifndef C_TYPEOF
#define C_TYPEOF(expr)          __typeof__(expr)
#endif
#ifndef C_DECL_UNUSED
#define C_DECL_UNUSED           __attribute__ ((__unused__))
#endif
#ifndef C_DECL_ALIGN
#define C_DECL_ALIGN(n)         __attribute__((__aligned__(n)))
#endif
#ifndef C_NORETURN
#define C_NORETURN              __attribute__((__noreturn__))
#endif
#ifndef C_DECL_DEPRECATED
#define C_DECL_DEPRECATED       __attribute__((__deprecated__))
#endif
#ifndef C_DECL_CONST_FUNCTION
#define C_DECL_CONST_FUNCTION   __attribute__((const))
#endif
#ifndef C_REQUIRED_RESULT
#define C_REQUIRED_RESULT       __attribute__((__warn_unused_result__))
#endif
#ifndef C_DECL_PURE_FUNCTION
#define C_DECL_PURE_FUNCTION    __attribute__((pure))
#endif
#ifndef C_DECL_COLD_FUNCTION
#define C_DECL_COLD_FUNCTION    __attribute__((cold))
#endif
#else
#ifndef C_ALIGNOF
#define C_ALIGNOF(type)     
#endif
#ifndef C_TYPEOF
#define C_TYPEOF(expr)      
#endif
#ifndef C_DECL_UNUSED
#define C_DECL_UNUSED       
#endif
#ifndef C_DECL_ALIGN
#define C_DECL_ALIGN(n)     
#endif
#ifndef C_NORETURN
#define C_NORETURN          
#endif
#ifndef C_DECL_DEPRECATED
#define C_DECL_DEPRECATED   
#endif
#ifndef C_DECL_CONST_FUNCTION
#define C_DECL_CONST_FUNCTION 
#endif
#ifndef C_REQUIRED_RESULT
#define C_REQUIRED_RESULT
#endif
#ifndef C_DECL_PURE_FUNCTION
#define C_DECL_PURE_FUNCTION
#endif
#ifndef C_DECL_COLD_FUNCTION
#define C_DECL_COLD_FUNCTION
#endif
#endif

#ifndef C_ALLOC_SIZE
#if defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 403) && !(((__clang_major__ * 100) + __clang_minor__))
#define C_ALLOC_SIZE(x)         __attribute__((alloc_size(x)))
#else
#define C_ALLOC_SIZE(x)
#endif
#endif


// 计算结构体中成员在结构体中的偏移位置
#ifndef C_STRUCT_OFFSET_OF
#define C_STRUCT_OFFSET_OF(structType, member)                                  offsetof(structType, member)
#endif

// 检查结构体大小是否符合预期
#ifndef C_STRUCT_SIZE_CHECK
#if C_SUPPORTED_C11
#define C_STRUCT_SIZE_CHECK(structType, expectedSize)                           _Static_assert((sizeof(structType) == (expectedSize)), "struct size '#structType' is wrong");
#else
#define C_STRUCT_SIZE_CHECK(structType, expectedSize)                           typedef char _macros_check_size##structType[((sizeof(structType) == (expectedSize)) ? 1 : -1)];
#endif
#endif

#ifndef C_ASSERT
#define C_ASSERT(x)                                                             assert(x)
#endif

// 检查类型大小是否符合预期
#ifndef C_TYPE_SIZE_CHECK
#define C_TYPE_SIZE_CHECK(typeT, sizeT)                                         C_STRUCT_SIZE_CHECK(typeT, (sizeT))
#endif

// 定义 int 类型
#ifndef cint8
typedef signed char                                                             cint8;
#define C_SIZEOF_INT8                                                           1
C_TYPE_SIZE_CHECK(cint8, C_SIZEOF_INT8)
#endif

#ifndef cuint8
typedef unsigned char                                                           cuint8;
#define C_SIZEOF_UINT8                                                          1
C_TYPE_SIZE_CHECK(cuint8, C_SIZEOF_UINT8)
#endif

#ifndef cchar
typedef char                                                                    cchar;
#define C_SIZEOF_CHAR                                                           1
C_TYPE_SIZE_CHECK(cchar, C_SIZEOF_CHAR)
#endif

#ifndef cuchar
#define C_SIZEOF_UCHAR                                                          1
typedef unsigned char                                                           cuchar;
C_TYPE_SIZE_CHECK(cuchar, C_SIZEOF_UCHAR)
#endif

#ifndef cint16
typedef signed short                                                            cint16;
#define C_SIZEOF_SHORT                                                          2
C_TYPE_SIZE_CHECK(cint16, C_SIZEOF_SHORT)
#endif

#ifndef cuint16
typedef unsigned short                                                          cuint16;
#define C_SIZEOF_USHORT                                                         2
C_TYPE_SIZE_CHECK(cuint16, C_SIZEOF_USHORT)
#endif

#ifndef cshort
typedef signed short                                                            cshort;
#define C_SIZEOF_SHORT                                                          2
C_TYPE_SIZE_CHECK(cshort, C_SIZEOF_SHORT)
#endif

#ifndef cushort
typedef unsigned short                                                          cushort;
#define C_SIZEOF_USHORT                                                         2
C_TYPE_SIZE_CHECK(cushort, C_SIZEOF_USHORT)
#endif

#ifndef cint32
typedef signed int                                                              cint32;
#define C_SIZEOF_INT32                                                          4
C_TYPE_SIZE_CHECK(cint32, C_SIZEOF_INT32)
#endif

#ifndef cint
typedef signed int                                                              cint;
#define C_SIZEOF_INT                                                            4
C_TYPE_SIZE_CHECK(cint, C_SIZEOF_INT)
#endif

#ifndef cuint32
typedef unsigned int                                                            cuint32;
#define C_SIZEOF_UINT32                                                         4
C_TYPE_SIZE_CHECK(cuint32, C_SIZEOF_UINT32)
#endif

#ifndef cuint
typedef unsigned int                                                            cuint;
#define C_SIZEOF_UINT                                                           4
C_TYPE_SIZE_CHECK(cuint, C_SIZEOF_UINT)
#endif

#ifndef cint64
typedef signed long long                                                        cint64;
#define C_SIZEOF_LONG                                                           8
C_TYPE_SIZE_CHECK(cint64, C_SIZEOF_LONG)
#endif

#ifndef cuint64
typedef unsigned long long                                                      cuint64;
#define C_SIZEOF_ULONG                                                          8
C_TYPE_SIZE_CHECK(cuint64, C_SIZEOF_ULONG)
#endif

#ifndef cfloat
typedef float                                                                   cfloat;
#define C_SIZEOF_FLOAT                                                          4
C_TYPE_SIZE_CHECK(cfloat, C_SIZEOF_FLOAT)
#endif

#ifndef cdouble
typedef double                                                                  cdouble;
#define C_SIZEOF_DOUBLE                                                         8
C_TYPE_SIZE_CHECK(cdouble, C_SIZEOF_DOUBLE)
#endif

#ifndef cpointer
typedef void*                                                                   cpointer;
#define C_SIZEOF_VOID_PTR                                                       8
C_TYPE_SIZE_CHECK(cpointer, C_SIZEOF_VOID_PTR)
#endif

#ifndef clong
typedef long                                                                    clong;
#define C_SIZEOF_CLONG                                                          8
C_TYPE_SIZE_CHECK(clong, C_SIZEOF_CLONG)
#endif

#ifndef culong
typedef unsigned long                                                           culong;
#define C_SIZEOF_CULONG                                                         8
C_TYPE_SIZE_CHECK(culong, C_SIZEOF_CULONG)
#endif

#ifndef clonglong
typedef long long                                                               clonglong;
#define C_SIZEOF_CLONGLONG                                                      8
C_TYPE_SIZE_CHECK(clonglong, C_SIZEOF_CLONGLONG)
#endif

#ifndef culonglong
typedef unsigned long long                                                      culonglong;
#define C_SIZEOF_CULONGLONG                                                     8
C_TYPE_SIZE_CHECK(culonglong, C_SIZEOF_CULONGLONG)
#endif



#ifndef csize
typedef unsigned long                                                           csize;
#define C_SIZEOF_SIZE                                                           8
C_TYPE_SIZE_CHECK(csize, C_SIZEOF_SIZE)
#endif

#ifndef cssize
typedef signed long                                                             cssize;
#define C_SIZEOF_SSIZE                                                          8
C_TYPE_SIZE_CHECK(cpointer, C_SIZEOF_SSIZE)
#endif

#ifndef cintptr
#define C_SIZEOF_INTPTR                                                         C_SIZEOF_VOID_PTR
typedef signed long                                                             cintptr;
#endif

#ifndef cunicodechar
#define C_SIZEOF_UNICODE_CHAR                                                   4
typedef cuint32                                                                 cunicodechar;
C_TYPE_SIZE_CHECK(cunicodechar, C_SIZEOF_UNICODE_CHAR)
#endif

#ifndef cuintptr
#define C_SIZEOF_UINTPTR                                                        C_SIZEOF_VOID_PTR
typedef unsigned long                                                           cuintptr;
#endif

#ifndef C_CPU_WORDSIZE
#define C_CPU_WORDSIZE                                                          C_SIZEOF_VOID_PTR
#endif

#ifndef C_CPU_BITSIZE
#if C_CPU_WORDSIZE == 8
#define C_CPU_BITSIZE                                                           64
#elif C_CPU_WORDSIZE == 4
#define C_CPU_BITSIZE                                                           32
#endif
#endif

#undef cle16
typedef cuint16                                                                 cle16;

#undef cle32
typedef cuint32                                                                 cle32;

#undef cle64
typedef cuint64                                                                 cle64;

#ifndef C_INT8
#define C_INT8(x)                                                               ((cint8) (x))
#endif

#ifndef C_UINT8
#define C_UINT8(x)                                                              ((cuint8) (x))
#endif

#ifndef C_INT16
#define C_INT16(x)                                                              ((cint16) (x))
#endif

#ifndef C_UINT16
#define C_UINT16(x)                                                             ((cuint16) (x))
#endif

#ifndef C_INT32
#define C_INT32(x)                                                              ((cint32) (x))
#endif

#ifndef C_UINT32
#define C_UINT32(x)                                                             ((cuint32) (x))
#endif

#ifndef C_INT64
#define C_INT64(x)                                                              ((cint64) (x))
#endif

#ifndef C_UINT64
#define C_UINT64(x)                                                             ((cuint64) (x))
#endif

#ifndef C_POINTER
#define C_POINTER(x)                                                            ((C_POINTER) (x))
#endif

#ifndef C_INT64_CONSTANT
#define C_INT64_CONSTANT(val)                                                   (val##L)
#endif

#ifndef C_UINT64_CONSTANT
#define C_UINT64_CONSTANT(val)                                                  (val##L)
#endif

#ifndef C_INT8_MAX
#define C_INT8_MAX                                                              ((cint8) 0x7F)
#endif

#ifndef C_INT8_MIN
#define C_INT8_MIN                                                              ((cint8) (-C_INT8_MAX) - 1)
#endif

#ifndef C_UINT8_MAX
#define C_UINT8_MAX                                                             ((cuint8) 0xFF)
#endif

#ifndef C_UINT8_MIN
#define C_UINT8_MIN                                                             ((cuint8) 0x00)
#endif

#ifndef C_INT16_MAX
#define C_INT16_MAX                                                             ((cint16) 0x7FFF)
#endif

#ifndef C_INT16_MIN
#define C_INT16_MIN                                                             ((cint16) (-C_INT16_MAX) - 1)
#endif

#ifndef C_UINT16_MAX
#define C_UINT16_MAX                                                            ((cuint16) 0xFFFF)
#endif

#ifndef C_UINT16_MIN
#define C_UINT16_MIN                                                            ((cuint16) 0x0000)
#endif

#ifndef C_INT32_MAX
#define C_INT32_MAX                                                             ((cint32) 0x7FFFFFFF)
#endif

#ifndef C_INT32_MIN
#define C_INT32_MIN                                                             ((cint32) (-C_INT32_MAX) - 1)
#endif

#ifndef C_UINT32_MAX
#define C_UINT32_MAX                                                            ((cuint32) 0xFFFFFFFF)
#endif

#ifndef C_UINT32_MIN
#define C_UINT32_MIN                                                            ((cuint32) 0x00000000)
#endif

#ifndef C_INT64_MAX
#define C_INT64_MAX                                                             ((cint64) 0x7FFFFFFFFFFFFFFF)
#endif

#ifndef C_INT64_MIN
#define C_INT64_MIN                                                             ((cint64) (-C_INT64_MAX) - 1)
#endif

#ifndef C_UINT64_MAX
#define C_UINT64_MAX                                                            ((cuint64) 0xFFFFFFFFFFFFFFFF)
#endif

#ifndef C_UINT64_MIN
#define C_UINT64_MIN                                                            ((cuint64) 0x0000000000000000)
#endif

// 自然常数 e
#ifndef C_E
#define C_E                                                                     2.7182818284590452353602874713526624977572470937000
#endif

// log 以 e 为底 2 的对数
#ifndef C_LN_2
#define C_LN_2                                                                  0.69314718055994530941723212145817656807550013436026
#endif

// log 以 e 为底 10 的对数
#ifndef C_LN_10
#define C_LN_10                                                                 2.3025850929940456840179914546843642076011014886288
#endif

// pi
#ifndef C_PI
#define C_PI                                                                    3.1415926535897932384626433832795028841971693993751
#endif

// pi / 2
#ifndef C_PI_DIV_2
#define C_PI_DIV_2                                                              1.5707963267948966192313216916397514420985846996876
#endif

// pi / 4
#ifndef C_PI_DIV_4
#define C_PI_DIV_4                                                              0.78539816339744830961566084581987572104929234984378
#endif

// 根号 2 的结果
#ifndef C_SQRT_2
#define C_SQRT_2                                                                1.4142135623730950488016887242096980785696718753769
#endif

// 定义 bool 类型
#ifdef __cplusplus
#else
#ifndef bool
typedef int                                                                     bool;
#endif
#ifndef false
#define false                                                                   (0)
#endif
#ifndef true
#define true                                                                    (!false)
#endif
#endif

/**
 * @brief C++类相关
 */
#ifdef __cplusplus
C_WARNING_PUSH
C_WARNING_DISABLE_GCC("-Wparentheses")
#define C_DISABLE_COPY(cls) \
    cls (const cls &) = delete; \
    cls &operator=(const cls &) = delete;
#define C_DISABLE_MOVE(cls) \
    cls(cls &&) = delete; \
    cls &operator=(cls &&) = delete;
#define C_DISABLE_COPY_MOVE(cls) \
    C_DISABLE_COPY(cls) \
    C_DISABLE_MOVE(cls)
C_WARNING_POP
#else
#define C_DISABLE_COPY(cls)
#define C_DISABLE_MOVE(cls)
#endif


/**
 * @brief 函数可见性，gcc可见性分为以下几种情况(__attribute__((visibility(""))))：
 *  1. default：默认可见（函数在程序的任何地方可见）
 *  2. hidden：隐藏可见性。函数在链接时候不可见，对于外部链接的符号，将无法从其它目标文件中引用
 *  3. protected：受保护可见。函数在链接时候可见，但只能被其所在的目标文件或具有相同共享库的目标文件引用
 */
#undef C_SYMBOL_EXPORT
#undef C_SYMBOL_IMPORT
#undef C_SYMBOL_HIDDEN
#undef C_SYMBOL_PROTECTED
#if (defined(_WIN32) || defined(__CYGWIN__))
#define C_SYMBOL_EXPORT __declspec(dllexport)
#define C_SYMBOL_IMPORT __declspec(dllimport)
#define C_SYMBOL_HIDDEN
#define C_SYMBOL_PROTECTED
#elif __GNUC__ >= 4
#define C_SYMBOL_IMPORT
#define C_SYMBOL_HIDDEN     __attribute__((visibility("hidden")))
#define C_SYMBOL_EXPORT     __attribute__((visibility("default")))
#define C_SYMBOL_PROTECTED  __attribute__((visibility("protected")))
#else
#define C_SYMBOL_EXPORT
#define C_SYMBOL_IMPORT
#define C_SYMBOL_HIDDEN
#define C_SYMBOL_PROTECTED
#endif

// __func__
#ifndef C_STRFUNC
#if defined (__GNUC__) && defined (__cplusplus)
#define C_STRFUNC                                                               ((const char*) (__PRETTY_FUNCTION__))
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define C_STRFUNC                                                               ((const char*) (__func__))
#elif defined (__GNUC__) || (defined(_MSC_VER) && (_MSC_VER > 1300))
#define C_STRFUNC                                                               ((const char*) (__FUNCTION__))
#else
#define C_STRFUNC                                                               ((const char*) ("???"))
#endif
#endif

// __LINE__
#ifndef C_LINE
#define C_LINE                                                                  (__LINE__)
#endif

// extern "c"
#undef C_BEGIN_EXTERN_C
#undef C_END_EXTERN_C
#ifdef  __cplusplus
#define C_BEGIN_EXTERN_C                                                        extern "C" {
#define C_END_EXTERN_C                                                          }
#else
#define C_BEGIN_EXTERN_C
#define C_END_EXTERN_C
#endif

// do {} while (0);
#undef C_STMT_START
#undef C_STMT_END
#define C_STMT_START  do {
#if defined (_MSC_VER) && (_MSC_VER >= 1500)
#define C_STMT_END \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    } while(0) \
    __pragma(warning(pop))
#else
#define C_STMT_END    } while (0)
#endif

#if C_GNUC_CHECK_VERSION(2, 0) && defined(__OPTIMIZE__)
#define C_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#define C_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
#define C_LIKELY(expr) (expr)
#define C_UNLIKELY(expr) (expr)
#endif


#ifdef __cplusplus
template <typename T> inline T* cGetPtrHelper(T* ptr) C_DECL_NOEXCEPT { return ptr; }

#if C_SUPPORTED_C11
template <typename Ptr> inline auto cGetPtrHelper(Ptr &ptr) C_DECL_NOEXCEPT -> decltype(ptr.get())
{ _Static_assert(noexcept(ptr.get()), "Smart d pointers for C_DECLARE_PRIVATE must have noexcept get()"); return ptr.get(); }
#endif

#define C_CAST_IGNORE_ALIGN(body)   C_WARNING_PUSH C_WARNING_DISABLE_GCC("-Wcast-align") body C_WARNING_POP
#define C_DECLARE_PRIVATE(klass) \
    inline klass##Private* d_func() \
    { C_CAST_IGNORE_ALIGN(return reinterpret_cast<klass##Private*>(cGetPtrHelper(d_ptr));) } \
    inline const klass##Private* d_func() const \
    { C_CAST_IGNORE_ALIGN(return reinterpret_cast<const klass##Private*>(cGetPtrHelper(d_ptr));) } \
    friend class klass##Private;

#define C_DECLARE_PRIVATE_D(Dptr, klass) \
    inline klass##Private* d_func() \
    { C_CAST_IGNORE_ALIGN(return reinterpret_cast<klass##Private*>(cGetPtrHelper(Dptr));) } \
    inline const klass##Private* d_func() const \
    { C_CAST_IGNORE_ALIGN(return reinterpret_cast<const klass##Private *>(cGetPtrHelper(Dptr));) } \
    friend class klass##Private;

#define C_DECLARE_PUBLIC(klass) \
    inline klass* q_func() { return static_cast<klass*>(q_ptr); } \
    inline const klass* q_func() const { return static_cast<const klass*>(q_ptr); } \
    friend class klass;

#define C_D(klass) klass##Private* const d = d_func()
#define C_Q(klass) klass* const q = q_func()
#endif

#undef C_HEX
#define C_HEX(x)                                                                ((x) < 10 ? (x) + '0' : (x) - 10 + 'A')

#undef C_ROUND_UP
#define C_ROUND_UP(num, unit)                                                   (((num) + (unit) - 1) & ~((unit) - 1))

#undef C_ROUND_DOWN
#define C_ROUND_DOWN(num, unit)                                                 ((num) & ~((unit) - 1))

#undef C_MAX
#define C_MAX(a, b)                                                             (((a) > (b)) ? (a) : (b))

#undef C_MIN
#define C_MIN(a, b)                                                             (((a) < (b)) ? (a) : (b))

#undef C_ABS
#define C_ABS(a)                                                                (((a) < 0) ? -(a) : (a))

#undef C_N_ELEMENTS
#define C_N_ELEMENTS(arr)                                                       (sizeof(arr) / sizeof((arr)[0]))

#undef C_ASCII_IS_SPACE
#undef C_ASCII_IS_UPPER
#undef C_ASCII_IS_LOWER
#undef C_ASCII_IS_ALPHA
#undef C_ASCII_TO_UPPER
#undef C_ASCII_TO_LOWER

#define C_ASCII_IS_UPPER(c)                                                     ((c) >= 'A' && (c) <= 'Z')
#define C_ASCII_IS_LOWER(c)                                                     ((c) >= 'a' && (c) <= 'z')
#define C_ASCII_IS_ALPHA(c)                                                     (C_ASCII_IS_UPPER (c) || C_ASCII_IS_LOWER(c))
#define C_ASCII_TO_UPPER(c)                                                     (C_ASCII_IS_LOWER (c) ? (c) - 'a' + 'A' : (c))
#define C_ASCII_TO_LOWER(c)                                                     (C_ASCII_IS_UPPER (c) ? (c) - 'A' + 'a' : (c))
#define C_ASCII_IS_SPACE(c)                                                     ((c) == ' ' || (c) == '\f' || (c) == '\n' || (c) == '\r' || (c) == '\t' || (c) == '\v')

#undef C_FREE
#undef C_FREE_FUNC
#undef C_BREAK_IF_FAIL
#undef C_BREAK_IF_NULL
#undef C_RETURN_IF_FAIL
#undef C_RETURN_VAL_IF_FAIL
#undef C_RETURN_IF_FAIL_SYSLOG_WARN
#undef C_RETURN_VAL_IF_FAIL_SYSLOG_WARN

#define C_BREAK_IF_OK(x)														if (x) { break; }
#define C_BREAK_IF_FAIL(x)                                                      if (!(x)) { break; }
#define C_BREAK_IF_NULL(x)                                                      if ((x) == NULL) { break; }
#define C_BREAK_IF_FAIL_SYSLOG_WARN(x, ...)                                     if (!(x)) { syslog(LOG_WARNING, ##__VA_ARGS__); break; }
#define C_RETURN_IF_OK(ck)														C_STMT_START if ((ck)) { return; } C_STMT_END
#define C_RETURN_IF_FAIL(ck)                                                    C_STMT_START if (!(ck)) { return; } C_STMT_END
#define C_RETURN_IF_FAIL_SYSLOG_WARN(ck, ...)                                   C_STMT_START if (!(ck)) { syslog(LOG_WARNING, ##__VA_ARGS__); return; } C_STMT_END
#define C_RETURN_VAL_IF_OK(ck, val)                                             C_STMT_START if ((ck)) { return (val); } C_STMT_END
#define C_RETURN_VAL_IF_FAIL(ck, val)                                           C_STMT_START if (!(ck)) { return (val); } C_STMT_END
#define C_RETURN_VAL_IF_FAIL_SYSLOG_WARN(ck, val, ...)                          C_STMT_START if (!(ck)) { syslog(LOG_WARNING, ##__VA_ARGS__); return (val); } C_STMT_END
#define C_FREE_FUNC(x, f, ...)                                                  C_STMT_START if ((x)) { f (x, ##__VA_ARGS__); } C_STMT_END
#define C_FREE_FUNC_NULL(x, f, ...)                                             C_STMT_START if ((x)) { f (x, ##__VA_ARGS__); x = NULL; } C_STMT_END
#define C_FREE_FUNC_IF_OK(x, ck, f, ...)                                        C_STMT_START if ((ck)){ f (x, ##__VA_ARGS__); } C_STMT_END
#define C_FREE_FUNC_NULL_IF_OK(x, ck, f, ...)                                   C_STMT_START if ((ck)){ f (x, ##__VA_ARGS__); x = NULL; } C_STMT_END


#undef C_ALIGN_TO
#define C_ALIGN_TO(n, align) ({                         \
    typeof (n) __ret;                                   \
    if ((n) % (align)) {                                \
        __ret = ((n) & (~((align) - 1))) + (align);     \
    }                                                   \
    else {                                              \
        __ret = (n);                                    \
    }                                                   \
})

#define C_SWAP(a, b) C_STMT_START \
    typeof(a) tmp = (a); \
    (a) = (b); \
    (b) = tmp; \
C_STMT_END

#ifndef C_STRLEN
static inline cint64 c_strlen (const char* str)
{
	C_RETURN_VAL_IF_FAIL(str, 0);

	int idx = 0;
	for (idx = 0; '\0' != str[idx]; ++idx);

	return idx;
}
#endif

#ifndef C_STRCMP
static inline int c_strcmp (const char* str1, const char* str2)
{
	C_RETURN_VAL_IF_OK(str1 && !str2, 1);
	C_RETURN_VAL_IF_OK(!str1 && str2, -1);
	C_RETURN_VAL_IF_OK(!str1 && !str2, 0);
	
	int idx = 0;
	for (idx = 0; str1[idx] && str2[idx]; ++idx) {
		int res = str1[idx] - str2[idx];
		if (0 == res) {
			continue;
		}
		return res;
	}

	C_RETURN_VAL_IF_OK(!str1[idx] && !str2[idx], 0);
	C_RETURN_VAL_IF_OK(!str1[idx], -1);

	// !str2[idx]
	return 1;
}
#endif

#ifndef C_STRNCMP
static inline int c_strncmp (const char* str1, const char* str2, cuint64 len)
{
	C_RETURN_VAL_IF_OK(str1 && !str2, 1);
	C_RETURN_VAL_IF_OK(!str1 && str2, -1);
	C_RETURN_VAL_IF_OK(!str1 && !str2, 0);

	int idx = 0;
	for (idx = 0; str1[idx] && str2[idx] && idx < len; ++idx) {
		int res = str1[idx] - str2[idx];
		if (0 == res) {
			continue;
		}
		return res;
	}

	C_RETURN_VAL_IF_OK((idx >= len), (str1[idx - 1] - str2[idx - 1]));

	C_RETURN_VAL_IF_OK(!str1[idx] && !str2[idx], 0);
	C_RETURN_VAL_IF_OK(!str1[idx], -1);

	return 1;
}
#endif

#ifndef C_STR_COPY_LINE_BUFFER
static inline cint64 c_str_copy_line_buffer (const char* str, char* buffer, cint64 bufferSize)
{
    C_RETURN_VAL_IF_FAIL(str, 0);
    C_RETURN_VAL_IF_FAIL(buffer && bufferSize > 0, 0);

    cint64 i = 0;
    cint64 bufferSizeT = bufferSize - 1;

    for (i = 0; i < bufferSizeT && str[i]; ++i) {
        buffer[i] = str[i];
        if ('\n' == str[i]) {
            buffer[i] = '\0';
            ++i;
            break;
        }
    }
    buffer[i] = '\0';

    return i;
}
#endif

#ifndef C_STR_HAS_PREFIX
static inline bool c_str_has_prefix (const char* str, const char* prefix)
{
	C_RETURN_VAL_IF_FAIL(str, false);
	C_RETURN_VAL_IF_FAIL(prefix, false);

	return (0 == c_strncmp(str, prefix, c_strlen(prefix)));
}
#endif

#ifndef C_STR_HAS_SUFFIX
static inline bool c_str_has_suffix (const char* str, const char* suffix)
{
	C_RETURN_VAL_IF_FAIL(str, false);
	C_RETURN_VAL_IF_FAIL(suffix, false);

	cuint64 strLen = c_strlen(str);
	cuint64 sufLen = c_strlen(suffix);

	C_RETURN_VAL_IF_OK(strLen < sufLen, false);

	return (0 == c_strcmp(str + strLen - sufLen, suffix));
}
#endif

#ifndef C_STR_ASCII_TO_LOWER
static inline void c_str_ascii_to_lower  (char* str)
{
    int i = 0;
    char c = '\0';
    for (i = 0; str[i] != '\0'; i++) {
        c = str[i];
        if (isascii(c)) {
            str[i] = (char) tolower(c);
        }
    }
}
#endif

#ifndef C_IS_IPV4_ADDR
// inet_pton
#endif

#endif
