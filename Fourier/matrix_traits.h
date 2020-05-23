#ifndef FOURIER_MATRIX_TRAITS_H
#define FOURIER_MATRIX_TRAITS_H

template <typename T>
class MatrixTraits
{
public:
	static constexpr T default_val = {};
};

template <>
class MatrixTraits<int>
{
public:
	static constexpr int default_val = 0;
};

template <>
class MatrixTraits<float>
{
public:
	static constexpr float default_val = 0;
};

template <>
class MatrixTraits<double>
{
public:
	static constexpr double default_val = 0;
};
#endif // FOURIER_MATRIX_TRAITS_H