#pragma once

//std
#include <complex.h>

//canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Objects/Object.hpp"

class Polynomial
{
public:
	//constructor
	Polynomial(void);

	//destructor
	~Polynomial(void);

	//data
	unsigned order(unsigned);
	unsigned order(void) const;

	double roots_error(void) const;
	const std::complex<double> root(unsigned) const;

	std::complex<double> coefficient(unsigned) const;
	std::complex<double> coefficient(unsigned, std::complex<double>);

	//value
	std::complex<double> value(std::complex<double>) const;
	std::complex<double> derivative(std::complex<double>) const;

	//analysis
	void solve(void);

	//analysis
	void bound(void);
	void predictor(void);
	void corrector(void);
	void draw_roots(void);
	void draw_setup(canvas::Scene*);
	void draw_update(canvas::Scene*);

	//data
	double m_bound;
	unsigned m_order;
	std::complex<double>* m_roots;
	std::complex<double>* m_draw_roots;
	std::complex<double>* m_coefficients;
	std::vector<canvas::objects::Object*> m_draw_objects;
};