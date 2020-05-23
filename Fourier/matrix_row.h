#ifndef FOURIER_MATRIX_ROW_H
#define FOURIER_MATRIX_ROW_H
#include "pch.h"
#include "matrix_exception.h"

#include <cstdlib>
#include <type_traits>

template <typename T>
class Matrix;

template <typename T>
class MatrixRow
{
	using T_non_cost = typename std::remove_const<T>::type;
public:
	T& operator[](std::size_t i)
	{
		if (i < data_size_)
		{
			return data_ptr_[i];
		}
		throw MatrixException("row given is out of bounds");
	}
	
	// Maths operators
	MatrixRow<T>& operator*(T val)
	{
		for (std::size_t i = 0; i < data_size_; ++i)
		{
			data_ptr_[i] *= val;
		}
		return *this;
	}

	MatrixRow<T>& operator+(const MatrixRow<T>& B)
	{
		if (B.data_size_ != data_size_)
		{
			throw MatrixException("cannot add row with different sizes");
		}

		for (std::size_t i = 0; i < data_size_; ++i)
		{
			data_ptr_[i] += B.data_ptr_[i];
		}
		return *this;
	}

	MatrixRow<T>& operator-(const MatrixRow<T>& B)
	{
		if (B.data_size_ != data_size_)
		{
			throw MatrixException("cannod subtract rows with different sizes");
		}

		for (std::size_t i = 0; i < data_size_; ++i)
		{
			data_ptr_[i] -= B.data_ptr_[i];
		}
		return *this;
	}

	MatrixRow<T>& operator=(const std::initializer_list<T>& values)
	{
		if (values.size() == data_size_)
		{
			for (std::size_t i = 0; i < data_size_; ++i)
			{
				data_ptr_[i] = *(values.begin() + i);
			}
			return *this;
		}
		throw MatrixException("value sizes do not match row size")
	}


private:
	MatrixRow(T* data_ptr, std::size_t data_size)
		: data_ptr_{ data_ptr }, data_size_{ data_size }
	{
	}

	T* data_ptr_;
	std::size_t data_size_;

	friend class Matrix<T>;
	friend class Matrix<T_non_cost>;
};
#endif // FOURIER_MATRIX_ROW_H
