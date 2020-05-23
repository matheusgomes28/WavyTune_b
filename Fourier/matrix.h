#ifndef FOURIER_MATRIX_H
#define FOURIER_MATRIX_H

#include "matrix_exception.h"
#include "matrix_row.h"
#include "matrix_traits.h"

// includes from the STD
#include <cstdlib>
#include <type_traits>

template <typename T, typename _Matrix_Traits>
class Matrix_T
{
	using non_const_T = typename std::remove_const<T>::type;

public:
	Matrix_T(std::size_t n_rows, std::size_t n_cols)
		: n_rows_{ n_rows },
		  n_cols_{ n_cols },
		  data_ptr_{new T[n_rows * n_cols]}
	{
		for (std::size_t row = 0; row < n_rows_; ++row)
		{
			for (std::size_t col = 0; col < n_cols_; ++col)
			{
				data_ptr_[row * n_cols_ + col] = _Matrix_Traits::default_val;
			}
		}
	}

	MatrixRow<T, _Matrix_Traits> operator[](std::size_t row)
	{
		if (row < n_rows_)
		{
			return MatrixRow<T, _Matrix_Traits>(data_ptr_ + n_cols_ * row, n_cols_);
		}
		throw MatrixException("the row given is out of bounds");
	}

	MatrixRow<const T, _Matrix_Traits> operator[](std::size_t row) const
	{
		if (row < n_rows_)
		{
			return MatrixRow<const T, _Matrix_Traits>(data_ptr_ + n_cols_ * row, n_cols_);
		}
		throw MatrixException("the row given is out of bounds");
	}

	// Scalar operations
	Matrix_T<non_const_T, _Matrix_Traits> operator+(T val) const
	{
		Matrix_T<non_const_T> result{ n_rows_, n_cols_ };

		for (std::size_t row = 0; row < n_rows_; ++row)
		{
			MatrixRow<T, _Matrix_Traits> curr_row = (*this)[row];
			MatrixRow<non_const_T, _Matrix_Traits> result_row = result[row];

			for (std::size_t col = 0; col < n_cols_; ++col)
			{
				result_row[col] = curr_row[col] + val;
			}
		}
		return result;
	}

	Matrix_T<non_const_T, _Matrix_Traits> operator-(T val) const
	{
		Matrix_T<non_const_T> result{ n_rows_, n_cols_ };

		for (std::size_t row = 0; row < n_rows_; ++row)
		{
			MatrixRow<T, _Matrix_Traits> curr_row = (*this)[row];
			MatrixRow<non_const_T, _Matrix_Traits> result_row = result[row];

			for (std::size_t col = 0; col < n_cols_; ++col)
			{
				result_row[col] = curr_row[col] + val;
			}
		}
		return result;
	}

	// Matrix operations
	Matrix_T<non_const_T, _Matrix_Traits> operator+(const Matrix_T<T, _Matrix_Traits>& B) const
	{
		if ((n_cols_ == B.n_cols_) && (n_rows_ == B.n_rows_))
		{
			Matrix_T<non_const_T> result{ n_rows_, B.n_cols_ };

			for (std::size_t row = 0; row < n_rows_; ++row)
			{
				MatrixRow<T, _Matrix_Traits> A_row = (*this)[row];
				MatrixRow<T, _Matrix_Traits> B_row = B[row];
				MatrixRow<non_const_T, _Matrix_Traits> result_row = result[row];

				for (std::size_t col = 0; col < n_cols_; ++col)
				{
					result_row[col] = A_row[col] + B_row[col];
				}
			}

			return result;
		}
		throw MatrixException("cannot add matrices with different dimensions");
	}


	Matrix_T<non_const_T, _Matrix_Traits> operator-(const Matrix_T<T, _Matrix_Traits>& B) const
	{
		if ((n_cols_ == B.n_cols_) && (n_rows_ == B.n_rows_))
		{
			Matrix_T<non_const_T> result{ n_rows_, B.n_cols_ };
			for (std::size_t row = 0; row < n_rows_; ++row)
			{
				MatrixRow<T> A_row = (*this)[row];
				MatrixRow<T> B_row = B[row];
				MatrixRow<non_const_T> result_row = result[row];

				for (std::size_t col = 0; col < n_cols_; ++col)
				{
					result_row[col] = A_row[col] + B_row[col];
				}
			}
			return result;
		}
		throw MatrixException("cannot  subtract matrices with different dimensions");
	}

	Matrix_T<non_const_T, _Matrix_Traits> operator*(const Matrix_T<T, _Matrix_Traits>& B) const
	{
		if (n_rows_ == B.n_rows_)
		{
			// TODO : implement type trait for default value
			Matrix_T<non_const_T, _Matrix_Traits> result{ n_rows_, B.n_cols_, 0};

			for (std::size_t row = 0; row < n_rows_; ++row)
			{
				MatrixRow<const T, _Matrix_Traits> A_row = (*this)[row];
				MatrixRow<non_const_T, _Matrix_Traits> result_row = result[row];
				for (std::size_t col = 0; col < B.n_cols_; ++col)
				{
					for (std::size_t i = 0; i < n_rows_; ++i)
					{
						result_row[col] += A_row[i] * B.data_ptr_[i * B.n_cols_ + col];
					}
				}
			}
			return result;
		}
		throw MatrixException("cannot multiply matrices with different inner dimensions");
	}

	Matrix_T<T, _Matrix_Traits>& operator=(const std::initializer_list<T>& values)
	{
		if (values.size() == n_rows_ * n_cols_)
		{
			for (std::size_t row = 0; row < n_rows_; ++row)
			{
				MatrixRow<T, _Matrix_Traits> curr_row = (*this)[row];
				for (std::size_t col = 0; col < n_cols_; ++col)
				{
					curr_row[col] = *(values.begin() + row * n_cols_ + col);
				}
			}
			return *this;
		}
		throw MatrixException("values must have the same size as the matrix");
	}

private:
	T* data_ptr_;
	std::size_t n_rows_;
	std::size_t n_cols_;
};

template <typename T>
using Matrix = Matrix_T<T, MatrixTraits<T>>;
#endif // FOURIER_MATRIX_H
