#ifndef FOURIER_SLOW_FFT_H
#define FOURIER_SLOW_FFT_H

#include "matrix.h"
#include <vector>

#define MATH_PI 3.14159265358979323846

using signal = Matrix<std::complex<double>>;


Matrix<std::complex<double>> get_multiplier(const std::size_t N)
{
	using namespace std::complex_literals;

	const std::complex<double> W = std::exp(-(2.0 * MATH_PI / static_cast<double>(N)) * 1i);
	
	// Generating the multiplier matrix
	Matrix<std::complex<double>> m1{ N,N };
	for (std::size_t i = 0; i < N; ++i)
	{
		auto row = m1[i];
		
		for (std::size_t c = 0; c < N; c++)
		{
			// figure out the row here
			row[c] = std::pow(W, (i* c) % N);
		}
	}

	return m1;
}

inline const std::complex<double> get_from_basis(const std::vector<std::complex<double>>& basis, const std::size_t& N)
{
	if (N < basis.size())
	{
		return basis[N];
	}
	else
	{
		return - basis[N - basis.size()];
	}
}


Matrix<std::complex<double>> get_multiplier_f(const std::size_t N)
{
	using namespace std::complex_literals;

	const std::complex<double> W = std::exp(-(2.0 * MATH_PI / static_cast<double>(N)) * 1i);

	std::vector<std::complex<double>> basis;
	basis.reserve(N / 2);
	for (std::size_t i = 0; i < N / 2; ++i)
	{
		basis.push_back(std::pow(W, i));
	}
	
	// Generating the multiplier matrix
	Matrix<std::complex<double>> m1{ N,N };
	for (std::size_t i = 0; i < N; ++i)
	{
		auto row = m1[i];
		for (std::size_t c = 0; c < N; c++)
		{
			// figure out the row here
			row[c] = get_from_basis(basis, (i * c) % N);
		}
	}
	return m1;
}

Matrix<std::complex<double>> slow_fft(const signal& input)
{
	Matrix<std::complex<double>> multiplier = get_multiplier_f(input.n_rows());
	return multiplier * input;
}

enum class FFT_PARTITION : char
{
	ODD,
	EVEN
};

#include <cmath>
std::vector<std::size_t> fft_partition_indices(const signal& input, const std::vector<FFT_PARTITION>& order)
{
	std::vector<std::size_t> g_index; // odd

	std::size_t g_start = 0;
	std::size_t skip = 0;
	
	// Calculate the offset for the starting value
	for (std::size_t i = 0; i < order.size(); ++i)
	{
		const auto& o = order[i];
		skip = static_cast<std::size_t>(1) << i;

		if (o == FFT_PARTITION::ODD)
		{
			g_start += skip;
		}
	}

	// Get the starting value 
	std::size_t i = g_start;
	skip = static_cast<std::size_t>(1) << order.size();
	while (i < input.n_rows())
	{
		g_index.push_back(i);
		i += skip;
	}
	
	// TEST CODE: (Add these to unit tests)
	// { n0, n1, n2, n3, n4, n5, n6, n7, .., nN } Normal                     SKIP = 2^0, offset = 0
	// { n0, n2, n5, n6, n8, n10, n12, ..., nN } Even                        SKIP = 2^1, offset = 0
	// { n2, n6, n10, n14, n18, n22, n26, ... nN} Even -> Odd                SKIP = 2^2, offset = 2
	// { n2, n10, n18, n26, n34, n42, n50, ..., nN} Even -> Odd -> Even      SKIP = 2^3, offset = 2
	// { n10, n26, n42, n58, n76, n92, ..., nN}  Even -> Odd -> Even -> Odd  SKIP = 2^4, offset = 10

	return g_index;
}

Matrix<std::complex<double>> fast_fft_helper(const signal& input, const std::vector<FFT_PARTITION>& partition_order)
{
	using namespace std::complex_literals;

	auto half_divisor = static_cast<std::size_t>(1) << partition_order.size();
	if ((input.n_rows() / half_divisor) > 8)
	{
		std::vector<FFT_PARTITION> even_partition_order{ begin(partition_order), end(partition_order) };
		even_partition_order.push_back(FFT_PARTITION::EVEN);
		std::vector<FFT_PARTITION> odd_partition_order{ begin(partition_order), end(partition_order) };
		odd_partition_order.push_back(FFT_PARTITION::ODD);

		Matrix<std::complex<double>> even = fast_fft_helper(input, even_partition_order);
		Matrix<std::complex<double>> odd = fast_fft_helper(input, odd_partition_order);

		// Combine the results
		const std::size_t N = (input.n_rows() / half_divisor);
		Matrix<std::complex<double>> ret{ N , 1 };
		std::vector<std::complex<double>> basis;
		basis.reserve(N / 2);
		const std::complex<double> W = std::exp(-(2.0 * MATH_PI / static_cast<double>(N)) * 1i);
		for (std::size_t i = 0; i < N / 2; ++i)
		{
			basis.push_back(std::pow(W, i));
		}

		for (std::size_t i = 0; i < ret.n_rows(); ++i)
		{
			auto index = i % (N >> 1);
			ret[i][0] = even[index][0] + get_from_basis(basis, i) * odd[index][0];
		}
		return ret;
	}
	else
	{
		// Partition the vector
		auto indices = fft_partition_indices(input, partition_order);
		Matrix<std::complex<double>> partition{ input.n_rows() /  half_divisor, 1 };
		for (std::size_t i = 0; i < partition.n_rows(); ++i)
		{
			partition[i][0] = input[indices[i]][0];
		}

		return slow_fft(partition);
	}
}

Matrix<std::complex<double>> fast_fft(const signal& input)
{
	auto h = fft_partition_indices(input, { FFT_PARTITION::EVEN });
	auto g = fft_partition_indices(input, { FFT_PARTITION::ODD }); // Odd
	return Matrix<std::complex<double>>{2, 2};
}


#include <functional>
template <typename T, typename U>
Matrix<T> apply(std::function<T(const U&)> f, const Matrix<U>& m)
{
	Matrix<T> result{m.n_rows(), m.n_cols()};
	for (std::size_t i = 0; i < m.n_rows(); ++i)
	{
		auto m_row = m[i];
		auto result_row = result[i];
		for (std::size_t j = 0; j < m.n_cols(); ++j)
		{
			result_row[j] = f(m_row[j]);
		}
	}
	return result;
}



#endif // FOURIER_SLOW_FFT_H
