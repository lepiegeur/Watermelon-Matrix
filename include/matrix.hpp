#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <iostream>
#include <array>

namespace Core::Maths
{
    template<size_t SIZE, typename ELEM_TYPE>
    class SquareMatrix;


    // REAL_CLASS is the type of the class currently used; 
    // if we do something like SquareMatrix<4, float>::zero(), 
    // then REAL_CLASS is SquareMatrix<4, float>,
    // and since zero() would return REAL_CLASS, 
    // we wouldn't have to reinterpret_cast, and so, it can be a constexpr function.
    // If the class currently used is a Matrix, then REAL_CLASS is std::nullptr_t to prevent recursion.
    template<size_t ROWS, size_t COLUMNS, typename ELEM_TYPE, class REAL_CLASS = std::nullptr_t>
    class Matrix 
    {
    protected:
        using SelfType  = Matrix<ROWS, COLUMNS, ELEM_TYPE>;

    public:
        using ElemType = ELEM_TYPE;

        // ========================================== //
        // ================= MATRIX ================= //
        // ========================================== //

        inline static constexpr size_t getNbRows()     noexcept;
        inline static constexpr size_t getNbColumns()  noexcept;
        inline static constexpr size_t getNbElements() noexcept;

        ElemType elements[ROWS * COLUMNS] = {};

    public:
        inline constexpr Matrix() noexcept = default;
        inline constexpr Matrix(const SelfType& rhs) noexcept = default;
        inline constexpr Matrix(SelfType&& rhs) noexcept = default;
        inline ~Matrix() noexcept = default;
    public:
        constexpr SelfType& operator=(const SelfType& rhs) noexcept = default;
        constexpr SelfType& operator=(SelfType&& rhs)      noexcept = default;

        constexpr SelfType operator+(const SelfType& rhs) noexcept;
        constexpr SelfType operator-(const SelfType& rhs) noexcept;
        constexpr SelfType operator*(const SelfType& rhs) const noexcept;
        constexpr SelfType operator*(ElemType f) const noexcept;
        constexpr SelfType operator/(ElemType f) const noexcept;
        constexpr SelfType& operator*=(ElemType f) noexcept;
        constexpr SelfType& operator/=(ElemType f) noexcept;
        constexpr ElemType const * operator[](size_t id) const noexcept;
        constexpr ElemType* operator[](size_t id) noexcept;

        inline constexpr       Matrix<1, COLUMNS, ELEM_TYPE>& getRow(size_t index)       noexcept;
        inline constexpr const Matrix<1, COLUMNS, ELEM_TYPE>& getRow(size_t index) const noexcept;

        constexpr bool operator==(const SelfType& rhs) const noexcept;
        constexpr bool operator!=(const SelfType& rhs) const noexcept;

        template<typename OTHER_MATRIX>
        inline static constexpr bool isMatrix();

        template<typename OTHER_MATRIX>
        using EnableIfMatrix = std::enable_if<std::is_base_of<Matrix, OTHER_MATRIX>::value>;

        template<typename OTHER_MATRIX, typename = EnableIfMatrix<OTHER_MATRIX>>
        inline static constexpr bool isMultiplicationPossible();

        template<typename OTHER_MATRIX>
        using EnableIfMultiplicationPossible = std::enable_if<isMultiplicationPossible<OTHER_MATRIX>()>;

        template<typename OTHER_MATRIX, // template args
            typename = EnableIfMultiplicationPossible<OTHER_MATRIX>> // sfinae conditions
        constexpr Core::Maths::Matrix<ROWS, OTHER_MATRIX::getNbColumns(), ElemType> // return type
            operator*(const OTHER_MATRIX& rhs) const noexcept;

        template<size_t SELF_ROWS    = getNbRows(), 
                 size_t SELF_COLUMNS = getNbColumns(), 
                 typename = std::enable_if_t<(SELF_ROWS > 0) && (SELF_COLUMNS > 0)>>
        constexpr Matrix<ROWS - 1, COLUMNS - 1, ElemType> getSubMatrix(size_t excludedRowIndex, size_t excludedColumnIndex) const noexcept;

        static constexpr SelfType zero() noexcept;
        
        template<size_t SIZE>
        inline constexpr operator Core::Maths::SquareMatrix<SIZE, ELEM_TYPE>() noexcept
        {
            return * reinterpret_cast<Core::Maths::SquareMatrix<SIZE, ELEM_TYPE>*> (this);
        }
    };

    template<size_t RHS_ROWS, size_t RHS_COLUMNS, typename RHS_ELEM_TYPE>
    std::ostream& operator<<(std::ostream& stream, const Core::Maths::Matrix<RHS_ROWS, RHS_COLUMNS, RHS_ELEM_TYPE>& rhs);
}

#include "matrix.inl"

#endif