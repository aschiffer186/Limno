#ifndef DEBUG_ALLOCATOR
#define DEBUG_ALLOCATOR

#include <memory>
#include <map>
#include <string>

#include "config.hh"

namespace LIB_NAMESPACE_BASE::_detail
{

    std::map<std::string, int> allocator_alloc_counts;
    std::map<std::string, int> allocator_dealloc_counts;

    static const std::string nameBase = "allocator";

    template<typename _Tp>
    struct debug_allocator 
    {
        static const int count = 1;
        //Allocator requirements
        using value_type = _Tp;
        using is_always_equal = std::false_type;

        using allocator_base_type = std::allocator<_Tp>;

        debug_allocator()
            : _base{}
        {
            _name = nameBase + std::to_string(count);
            ++count;
        }

        template<typename _UTp>
        debug_allocator(const debug_allocator<_UTp>& other)
            : _base{other._base}
        {
            _name = other._name;
            _name.append("_copy");
        }

        debug_allocator(const debug_allocator<_Tp>& other)
            : _base{other._base}
        {
            _name = other._name;
            _name.append("_copy");
        }

        _Tp* allocate(size_t n)
        {
            void* retVal = _base.allocate(n);
            ++allocator_alloc_counts[_name];
        }

        void deallocate(_Tp* p, size_t n) noexcept 
        {
            _base.deallocate(p, n);
            try 
            {
                ++allocator_dealloc_counts[_name];
            }
            catch (...)
            {

            }
        }

    private:
        template<typename _Tp1, typename _Tp2>
        friend bool operator==(const debug_allocator<_Tp1>& lhs, const debug_allocator<_Tp2>& rhs) noexcept;
    private:
        allocator_base_type _base;
        std::string _name;
    };

    template<typename _Tp1, typename _Tp2>
    bool operator==(const debug_allocator<_Tp1>& lhs, const debug_allocator<_Tp2>& rhs) noexcept 
    {
        return lhs._name == rhs._name;
    }
} // namespace L


#endif