#include <vector>
using namespace std;

class Pixel {
	public:
		Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		uint8_t red, green, blue, alpha;
		void set_all(uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
		uint8_t* get_array();
		int_fast16_t get_activation();
	
	private:
		uint8_t activation; // i.e. darkness
};

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

void Pixel::set_all(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

uint8_t* Pixel::get_array() {
	uint8_t* px;
	px[0] = red;
	px[1] = green;
	px[2] =	blue;
	px[3] = alpha;
	return px;
}

int_fast16_t Pixel::get_activation() {
	return (alpha * 3) - (red + green + blue);
}
