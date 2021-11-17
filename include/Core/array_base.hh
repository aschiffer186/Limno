#ifndef ARRAY_BASE_HH
#define ARRAY_BASE_HH

#include <array>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <vector>

#include "concepts.hh"
#include "config.hh"

namespace LIB_NAMESPACE_BASE 
{
    namespace _detail
    {

        //Indicate dimension isn't known at compile-time
        static constexpr int DYNAMIC = -1;
        
        //Convenience variable indicating if dimensions known at 
        //compile-time
        template<int _Nrows, int _Ncols>
        static constexpr bool runtimeDim = _Nrows == DYNAMIC || _Ncols == DYNAMIC;

        template<typename>
        struct _Iterator;

        template<typename>
        struct _ConstIterator;

        //Base implemenation of Matrix, handles memory and some
        //C++ container requirements.
        template<typename _Tp,
            int _Nrows,
            int _Ncols,
            typename _AllocTp = std::allocator<_Tp>>
        class LimnoMatrixBase
        {
            private:
            using storage_type = std::conditional_t<!runtimeDim<_Nrows, _Ncols>,
                std::array<_Tp, static_cast<size_t>(_Nrows*_Ncols)>, 
                std::vector<_Tp, _AllocTp>>;
            public:
            using value_type = _Tp;
            using reference = value_type&;
            using const_reference = const value_type&;
            using iterator = _Iterator<_Tp>;
            using const_iterator = _ConstIterator<_Tp>;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;

            using shape_type = std::pair<int, int>;
            
            //Default constructor
            constexpr LimnoMatrixBase() noexcept 
            {
                if constexpr(runtimeDim<_Nrows, _Ncols>)
                {
                    _numRows = 0;
                    _numCols = 0;
                }
                else 
                {
                    _numRows = static_cast<size_type>(_Nrows);
                    _numCols = static_cast<size_type>(_Ncols);
                }
            }
            //Constructor for num rows and cols, only participates overload resolution
            //for dynamic initialization. Allocates but does not initialize 
            //memory
            CONSTEXPR20 LimnoMatrixBase(size_type numRows, size_type numCols)
                : _numRows(numRows), _numCols(numCols)
            {
                static_assert(runtimeDim<_Nrows, _Ncols>, "Dimensions must not be known at compile-time!");
                _data.reserve(numRows*numCols);
            }

            //Fills matrix with specified value; requires that dimensions be known at compile-time
            constexpr LimnoMatrixBase(_Tp fillValue)
                : _numRows{static_cast<size_type>(_Nrows)}, _numCols{static_cast<size_type>(_Ncols)}
            {
                static_assert(!runtimeDim<_Nrows, _Ncols>, "Dimensions must be known at compile-time!");
                for(size_type i = 0; i < _data.size(); ++i)
                    _data[i] = fillValue;
            }

            //Fills matrix with specified value and specified shape; requires that dimensions be unknknown at compile-time
            constexpr LimnoMatrixBase(_Tp fillValue, size_type numRows, size_type numCols)
                : _numRows{numRows}, _numCols{numCols}, _data{}
            {
                static_assert(runtimeDim<_Nrows, _Ncols>, "Dimensions must be dynamic!");
                _data.resize(numRows*numCols);
            }


            //Constructor from pair of iterators. Only participates in overload 
            //resolution if dimensions known at compile time. Pads elements if 
            //not enough, truncates if too many
            #if __cplusplus > 201703L
            template<typename _IterTp>
                requires std::input_iterator<_IterTp> && (!runtimeDim<_Nrows, _Ncols>)
            #else 
            template<typename _IterTp, 
                std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, 
                typename std::iterator_traits<_IterTp>::iterator_category> &&
                !runtimeDim<_Nrows, _Ncols>, int> = 0>
            #endif 
            LimnoMatrixBase(_IterTp begin, _IterTp end) noexcept
                : _numRows{static_cast<size_type>(_Nrows)}, _numCols{static_cast<size_type>(_Ncols)}
            {
                int count = 0;
                for(; count < _Nrows*_Ncols && begin != end; ++count)
                    _data[count] = *begin++;
                
                for(; count < _Nrows*_Ncols; ++count)
                    _data[count] = 0;
            } 

            //Constructor from pair of iterators + dimensions of matrix. Only participates 
            //in overload resolution if dimensions not known at compiletime. Same deal 
            //as other version 
            #if __cplusplus > 201703L
            template<typename _IterTp> 
                requires std::input_iterator<_IterTp> && runtimeDim<_Nrows, _Ncols>
            #else 
            template<typename _IterTp, 
                std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, 
                typename std::iterator_traits<_IterTp>::iterator_category>&&
                runtimeDim<_Nrows, _Ncols>, int> = 0>
            #endif 
            LimnoMatrixBase(_IterTp begin, _IterTp end, size_type numRows, size_type numCols)
                : _numRows{numRows}, _numCols{numCols}
            {
                size_type size = numRows*numCols;
                size_type count = 0;

                for(; count < size && begin != end; ++count)
                    _data.push_back(*begin++);
                
                for(; count < size; ++count)
                    _data.push_back(0);
            }

            //Constructor from arbitrary container
            #if __cplusplus > 201703L
            template<typename _CTp> 
                requires Container<_CTp>
            #else 
            template<typename _CTp>
            #endif
            LimnoMatrixBase(const _CTp& c) noexcept(!runtimeDim<_Nrows, _Ncols>)
            {
                size_type size = std::begin(c)->size()*c.size();
                size_type contSize = (runtimeDim<_Nrows, _Ncols>) ? size : _Nrows*_Ncols;
                if constexpr(runtimeDim<_Nrows, _Ncols>)
                {
                    _numRows = c.size();
                    _numCols = std::begin(c)->size();
                    _data.resize(size);
                }
                else 
                {
                    _numRows = static_cast<size_type>(_Nrows);
                    _numCols = static_cast<size_type>(_Ncols);
                }
                
                auto rowIt = std::begin(c);
                auto colIt = std::begin(*rowIt);

                size_type count = 0;
                for(; count < contSize && rowIt != std::end(c); ++count)
                {
                    _data[count] = *colIt++;
                    if (colIt == std::end(*rowIt))
                    {
                        ++rowIt;
                        colIt = std::begin(*rowIt);
                    }
                }

                for(; count < contSize; ++count)
                    _data[count] = _Tp{};
            }

            //Constructor from array. Only participates in overload resoluation 
            //if dimensions known at compile-time 
            #if __cplusplus > 201703L
            template<typename _UTp,
                int N> requires std::convertible_to<_UTp, _Tp>
            #else 
                template<typename _UTp, 
                    int N, 
                    typename = std::enable_if_t<std::is_convertible_v<_UTp, _Tp> &&
                    !runtimeDim<_Nrows, _Ncols>, int>>
            #endif
            constexpr LimnoMatrixBase(_UTp (& c)[N]) noexcept 
                : _numRows{static_cast<size_type>(_Nrows)}, _numCols{static_cast<size_type>(_Ncols)}
            {
                static_assert(!runtimeDim<_Nrows, _Ncols>, "Constructor requires dimensions known at compile-time!");
                size_type count = 0; 
                for(; count < _Nrows*_Ncols && count < N; ++count)
                    _data[count] = c[count];
                
                for(; count < _Nrows*_Ncols; ++count)
                    _data[count] = 0;
            }

             //Constructor from array and shape. Only participates in overload resoluation 
            //if dimensions not known at compile-time 
            #if __cplusplus > 201703L
            template<typename _UTp,
                int N> requires std::convertible_to<_UTp, _Tp>
            #else 
                template<typename _UTp, 
                    int N, 
                    std::enable_if_t<std::is_convertible_v<_UTp, _Tp> &&
                    runtimeDim<_Nrows, _Ncols>, int> = 0>
            #endif
            LimnoMatrixBase(_UTp (& c)[N], size_type numRows, size_type numCols) 
            {
                static_assert(runtimeDim<_Nrows, _Ncols>, "Constructor requires dimensions not known at compile-time!");
                size_type size = numRows*numCols;
                _data.resize(size);

                size_type count = 0;
                for(; count < size && count < N; ++count)
                    _data[count] = c[count];
                
                for(; count < size; ++count)
                    _data[count] = 0;
            }

            //Container compliance methods
            
            //Iterator methods

            //Traverse in row-major order
            constexpr iterator begin() noexcept
            {

            }

            constexpr const_iterator begin() const noexcept
            {

            }

            constexpr const_iterator cbegin() const noexcept 
            {

            }

            constexpr iterator end() noexcept
            {

            }

            constexpr const_iterator end() const noexcept
            {

            }

            constexpr const_iterator cend() const noexcept 
            {
                
            }

            //Traverse in column major order 
            constexpr iterator columnBegin() noexcept 
            {

            }


            constexpr const_iterator columnBegin() const noexcept 
            {

            }

            constexpr const_iterator ccolumnBegin() const noexcept 
            {

            }

            constexpr iterator columnEnd() noexcept 
            {

            }


            constexpr const_iterator columnEnd() const noexcept 
            {

            }

            constexpr const_iterator ccolumnEnd() const noexcept 
            {

            }


            constexpr size_type max_size() const noexcept
            {
                return _data.max_size();
            }

            constexpr size_type size() const noexcept 
            {
                return _data.size();
            }

            constexpr bool empty() const noexcept 
            {
                return size() == 0;
            }

            //Misc 
            constexpr size_type numRows() const noexcept 
            {
                return static_cast<size_type>(_numRows);
            }

            constexpr size_type numCols() const noexcept 
            {
                return static_cast<size_type>(_numCols);
            }

            //Operators
            constexpr reference operator()(size_type r, size_type c) 
            {
                return _data[r*_numCols + c];
            }

            constexpr const_reference operator()(size_type r, size_type c) const 
            {
                return _data[r*_numCols + c];
            }
            private:
                template<typename _UTp,
                    int _Nrows1, 
                    int _Ncols1>
                friend std::ostream& operator<<(std::ostream& os, const LimnoMatrixBase<_UTp, _Nrows1, _Ncols1>& mat);
            private:
            size_type _numRows;
            size_type _numCols;
            storage_type _data;
        };

        template<typename _UTp,
            int _Nrows, 
            int _Ncols>
        inline std::ostream& operator<<(std::ostream& os, const LimnoMatrixBase<_UTp, _Nrows, _Ncols>& mat)
        {
            using size_type = typename LimnoMatrixBase<_UTp, _Nrows, _Ncols>::size_type;
            for(size_type i = 0; i < mat._numRows; ++i)
            {
                os << "{";
                for(size_type j = 0; j < mat._numCols; ++j)
                {
                    if (j < mat._numCols - 1)
                        os << mat(i, j) << ", ";
                    else 
                        os << mat(i, j);
                }

                if (i < mat._numRows - 1)
                    os << "},\n";
                else 
                    os << "}";
            }
            return os;
        }
    } // namespace _detail
}

#endif