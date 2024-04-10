//std
#include <cmath>
#include <ctime>

//roots
#include "Roots/inc/Polynomial.hpp"

//canvas
#include "Canvas/inc/Windows/Glut.hpp"

//data
static const unsigned n = 10;

int main(int argc, char** argv)
{
	//data
	Polynomial polynomial;
	canvas::windows::Glut app(argc, argv, "../Canvas/shd/");
	//coefficients
	polynomial.order(n);
	srand(time(nullptr));
	for(unsigned i = 0; i <= n; i++)
	{
		polynomial.coefficient(i, 2 * double(rand()) / RAND_MAX - 1);
	}
	polynomial.solve();
	polynomial.draw_roots();
	//canvas
	polynomial.draw_setup(app.scene());
	app.scene()->update(true);
	app.callback_special([&polynomial, &app] (int key, int, int){
		if(key == 0x0066)
		{
			polynomial.corrector();
			polynomial.draw_update(app.scene());
		}
	});
	app.callback_keyboard([&polynomial, &app] (unsigned char key, int, int){
		if(key == 'p')
		{
			polynomial.predictor();
			polynomial.draw_update(app.scene());
		}
		if(key == 'n')
		{
			for(unsigned i = 0; i <= n; i++)
			{
				polynomial.coefficient(i, 2 * double(rand()) / RAND_MAX - 1);
			}
			polynomial.solve();
			polynomial.draw_roots();
			polynomial.draw_update(app.scene());
			app.scene()->camera().bound();
			app.scene()->camera().apply();
			app.scene()->camera().update();
		}
	});
	app.start();
	//return
	return 0;
}