#include "pch.h"
#include "complex.h"

Complex::Complex()
	: number(0, 0)
{}

Complex::Complex(const float& real)
	: number(real, 0)
{}

Complex::Complex(const float& real, const float& imag)
	: number(real, imag)
{}

Complex::~Complex()
{}


// Accessing the member informations here
inline float Complex::real() const
{
	return number.real();
}

inline float Complex::imag() const
{
	return number.imag();
}

// Other functions for utility
inline float Complex::mod() const
{
	const float& r = number.real();
	const float& i = number.imag();

	return sqrt(r * r + i * i);
}

inline float Complex::arg() const
{
	// TODO : Make sure we get the correct quadrant
	const float& r = number.real();
	const float& i = number.imag();

	return atan(i / r);
}

Complex Complex::operator+(const Complex& c2) const
{
	return Complex(number.real() + c2.real(), number.imag() + c2.imag());
}

Complex Complex::operator-(const Complex& c2) const
{
	return Complex(number.real() - c2.real(), number.imag() - c2.imag());
}

Complex Complex::operator*(const Complex& c2) const
{
	// Simple stuff
	return Complex(number.real() * c2.real() - number.imag() * c2.imag(),
		number.imag() * c2.real() + number.real() * c2.imag());
}

Complex Complex::operator/(const Complex& c2) const
{
	float a = c2.real();
	float b = c2.real();

	// Throw if it's zero
	if ((a == 0) && (b == 0)) {
		throw std::invalid_argument("Cannot divide by zero");
	}

	// We multiply by 1/(a+bi)
	Complex reciprocal(a / (a * a + b * b), -b / (a * a + b * b));
	return (*this) * reciprocal;
}

bool operator==(const Complex& c1, const Complex& c2)
{
	// Component-wise check
	return ((c1.real() == c2.real()) && (c1.imag() == c2.imag()));
}