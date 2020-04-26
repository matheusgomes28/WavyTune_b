#pragma once
#include <complex>

class Complex
{
public:
	Complex();
	Complex(const float& real);
	Complex(const float& real, const float& imag);
	~Complex();

	// Things which will speed up a lot
	inline float real() const;
	inline float imag() const;
	inline float mod() const;
	inline float arg() const; // TODO : Do this properly


	// For the maths stuff
	Complex operator+(const Complex& c2) const;
	Complex operator-(const Complex& c2) const;
	Complex operator*(const Complex& c2) const;
	Complex operator/(const Complex& c2) const;

	friend bool operator==(const Complex& c1, const Complex& c2);

private:
	std::complex<float> number;
};
