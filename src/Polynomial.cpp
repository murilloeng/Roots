//std
#include <cmath>

//roots
#include "Roots/inc/Polynomial.hpp"

//canvas
#include "Canvas/inc/Objects/1D/Line.hpp"
#include "Canvas/inc/Objects/2D/Circle.hpp"

//constructor
Polynomial::Polynomial(void) : m_order(0), m_roots(nullptr), m_draw_roots(nullptr), m_coefficients(nullptr)
{
	return;
}

//destructor
Polynomial::~Polynomial(void)
{
	delete[] m_roots;
	delete[] m_draw_roots;
	delete[] m_coefficients;
	for(canvas::objects::Object* object : m_draw_objects)
	{
		delete object;
	}
}

//data
unsigned Polynomial::order(void) const
{
	return m_order;
}
unsigned Polynomial::order(unsigned order)
{
	delete[] m_roots;
	delete[] m_draw_roots;
	delete[] m_coefficients;
	m_roots = new std::complex<double>[order];
	m_draw_roots = new std::complex<double>[order];
	m_coefficients = new std::complex<double>[order + 1];
	return m_order = order;
}

double Polynomial::roots_error(void) const
{
	double error = 0;
	for(unsigned i = 0; i < m_order; i++)
	{
		std::complex<double> v = value(m_roots[i]);
		error = fmax(error, sqrt(v.real() * v.real() + v.imag() * v.imag()));
	}
	return error;
}
const std::complex<double> Polynomial::root(unsigned index) const
{
	return m_roots[index];
}

std::complex<double> Polynomial::coefficient(unsigned index) const
{
	return m_coefficients[index];
}
std::complex<double> Polynomial::coefficient(unsigned index, std::complex<double> coefficient)
{
	return m_coefficients[index] = coefficient;
}

//value
std::complex<double> Polynomial::value(std::complex<double> x) const
{
	//data
	std::complex<double> v(0, 0), e(1, 0);
	//value
	for(unsigned i = 0; i <= m_order; i++)
	{
		if(i != 0) e *= x;
		v += m_coefficients[i] * e;
	}
	//return
	return v;
}
std::complex<double> Polynomial::derivative(std::complex<double> x) const
{
	//data
	std::complex<double> v(0, 0), e(1, 0);
	//value
	for(unsigned i = 1; i <= m_order; i++)
	{
		if(i != 1) e *= x;
		v += double(i) * m_coefficients[i] * e;
	}
	//return
	return v;
}

//analysis
void Polynomial::solve(void)
{
	bound();
	predictor();
	unsigned i = 0;
	while(roots_error() > 1e-5 && i < 10000)
	{
		i++;
		corrector();
	}
	printf("Iterations: %04d Max error: %+.2e bound: %+.2e\n", i, roots_error(), m_bound);
}
void Polynomial::bound(void)
{
	double b1 = 0, b2 = 0;
	for(unsigned i = 0; i < m_order; i++)
	{
		b1 += fabs(m_coefficients[i] / m_coefficients[m_order]);
		b2 = fmax(b2, fabs(m_coefficients[i] / m_coefficients[m_order]));
	}
	b2 += 1;
	b1 = fmax(1, b1);
	m_bound = fmin(b1, b2);
}
void Polynomial::predictor(void)
{
	srand(time(nullptr));
	for(unsigned i = 0; i < m_order; i++)
	{
		const double r = m_bound * double(rand()) / RAND_MAX;
		const double t = 2 * M_PI * double(rand()) / RAND_MAX;
		m_roots[i] = std::complex<double>(r * cos(t), r * sin(t));
	}
}
void Polynomial::corrector(void)
{
	printf("Iteration\n");
	for(unsigned i = 0; i < m_order; i++)
	{
		std::complex<double> s(0, 0);
		std::complex<double> v = value(m_roots[i]);
		std::complex<double> d = derivative(m_roots[i]);
		for(unsigned j = 0; j < m_order; j++)
		{
			if(i != j) s += 1.0 / (m_roots[i] - m_roots[j]);
		}
		m_roots[i] = m_roots[i] - v / (d - v * s);
		printf("Index: %02d Root: %+.2e %+.2e I Error: %+.2e %+.2e I\n", i, m_roots[i].real(), m_roots[i].imag(), v.real(), v.imag());
	}
}
void Polynomial::draw_roots(void)
{
	for(unsigned i = 0; i < m_order; i++)
	{
		m_draw_roots[i] = m_roots[i];
	}
	predictor();
}
void Polynomial::draw_setup(canvas::Scene* scene)
{
	//bounds
	for(unsigned i = 0; i < 4; i++)
	{
		m_draw_objects.push_back(new canvas::objects::Line);
		scene->add_object(m_draw_objects[i]);
	}
	((canvas::objects::Line*) m_draw_objects[0])->point(0, {(float) -m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[0])->point(1, {(float) +m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[1])->point(0, {(float) +m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[1])->point(1, {(float) +m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[2])->point(0, {(float) +m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[2])->point(1, {(float) -m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[3])->point(0, {(float) -m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[3])->point(1, {(float) -m_bound, (float) -m_bound, 0});
	//roots
	for(unsigned i = 0; i < m_order; i++)
	{
		canvas::objects::Circle* circle = new canvas::objects::Circle;
		circle->color_fill({0, 0, 1});
		circle->radius(float(m_bound) / 20);
		circle->center({(float) m_roots[i].real(), (float) m_roots[i].imag(), 0});
		scene->add_object(circle);
		m_draw_objects.push_back(circle);
	}
	for(unsigned i = 0; i < m_order; i++)
	{
		canvas::objects::Circle* circle = new canvas::objects::Circle;
		circle->color_fill({1, 0, 0});
		circle->radius(float(m_bound) / 20);
		circle->center({(float) m_draw_roots[i].real(), (float) m_draw_roots[i].imag(), 0});
		scene->add_object(circle);
		m_draw_objects.push_back(circle);
	}
}
void Polynomial::draw_update(canvas::Scene* scene)
{
	//objects
	((canvas::objects::Line*) m_draw_objects[0])->point(0, {(float) -m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[0])->point(1, {(float) +m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[1])->point(0, {(float) +m_bound, (float) -m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[1])->point(1, {(float) +m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[2])->point(0, {(float) +m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[2])->point(1, {(float) -m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[3])->point(0, {(float) -m_bound, (float) +m_bound, 0});
	((canvas::objects::Line*) m_draw_objects[3])->point(1, {(float) -m_bound, (float) -m_bound, 0});
	for(unsigned i = 0; i < m_order; i++)
	{
		((canvas::objects::Circle*) m_draw_objects[i + 0 * m_order + 4])->radius((float) m_bound / 20);
		((canvas::objects::Circle*) m_draw_objects[i + 1 * m_order + 4])->radius((float) m_bound / 20);
		((canvas::objects::Circle*) m_draw_objects[i + 0 * m_order + 4])->center({(float) m_roots[i].real(), (float) m_roots[i].imag(), 0});
		((canvas::objects::Circle*) m_draw_objects[i + 1 * m_order + 4])->center({(float) m_draw_roots[i].real(), (float) m_draw_roots[i].imag(), 0});
	}
	//update
	scene->update(false);
}