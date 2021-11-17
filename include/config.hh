#ifndef CONFIG_HH
#define CONFIH_HH

//Namespace 
#define LIB_NAMESPACE_BASE Limno

static_assert(__cplusplus >= 201703L, "C++17 Required");

#if __cplusplus > 201703L
    #define CONSTEXPR20 constexpr 
    #define CONSTEVAL consteval
    #define CONSTINIT constinit
#else
    #define CONSTEXPR20
    #define CONSTEVAL constexpr
    #define CONSTINIT constexpr
#endif

#if __cplusplus >= 201703L
    #define CONSTEXPR17 constexpr
    #define NOEXCEPT17 noexcept
    #define NOEXCEPT17_cond(o) noexcept(o)
#else
    #define CONSTEXPR17
    #define NOEXCEPT17
    #define NOEXCEPT17_cond(o  
#endif

#define TYPE_CHECK(a, b, message) static_assert(std::is_convertible_v<a, b>, #message)

#endif