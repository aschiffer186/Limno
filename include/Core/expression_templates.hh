#ifndef EXPRESSION_TEMPLATES_HH
#define EXPRESSION_TEMPLATES_HH 1


#include <iterator>
#include <tuple>

#include "config.hh"
#include "concepts.hh"



namespace LIB_NAMESPACE_BASE:: _detail
{

    #if __cplusplus > 201703L
    template<typename _Callable, typename... _ArgsTp> requires Callable<_Callable, _ArgsTp...>
    #else 
    template<typename _Callable, typename... _ArgsTp>
    #endif
    class _Expr 
    {
        public:
            _Expr(_Callable f, _ArgsTp const&... args)
                : _args(args...), _f(f)
            {
                
            }
        private:
            std::tuple<_ArgsTp const&...> _args;
            _Callable _f;
    };
}

#endif