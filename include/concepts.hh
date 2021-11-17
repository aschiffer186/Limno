#ifndef CONCEPTS_HH
#define CONCEPTS_HH

#include "config.hh"

#include <concepts>
#include <iterator>

namespace LIB_NAMESPACE_BASE
{
    #if __cplusplus > 201703L
        /*
            Concept to represent a container 
            A container is any type that meets the following requirements
                1) has a member type named value_type representing the type stored in the container
                2) has a member type named size_type representing the size of the array 
                3) std::end can be called on the type and returns a LegacyInputIterator 
                4) std::begin can be called on the type and returns a LegacyInputIterator 
                5) the type has a member function called size that returns size_type
        */ 
        template<typename _Tp>
        concept Container = requires(_Tp c) {
            typename _Tp::value_type;
            typename _Tp::size_type;
            {std::begin(c)} -> std::input_iterator;
            {std::end(c)} -> std::input_iterator;
            {c.size()} -> std::same_as<typename _Tp::size_type>;
        };

        /*
            Concept to represent a number. A number is an integer (signed or unsigned) or a floating point number
        */
        template<typename _Tp>
        concept Number = (std::floating_point<_Tp> || std::integral<_Tp>) && !std::same_as<_Tp, char>;

        /*
            Concept to represent a callable for a set of arguments. 
        */
        template<typename _Tp, typename... _ArgsTp>
        concept Callable = requires(_Tp c, _ArgsTp&&... args) {
            c(args...);
        };
    #endif
}

#endif