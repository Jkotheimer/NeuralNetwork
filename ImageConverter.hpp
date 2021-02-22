#include <png.h>
#include <string>
#include <algorithm>
#include "ErrHandler.hpp"
#include "Pixel.hpp"

class ImageConverter {
	public:
		ImageConverter(const char* file_name);
		ImageConverter();

		// Umbrella function to read any file into the pixel list
		void open(const char* file_name);

		// Specifics in case the umbrella doesn't work
		void open_png(const char* file_name);
		void open_jpg(const char* file_name);
		void open_gif(const char* file_name);

		int_fast16_t* get_activation_list();
		Pixel* get_pixel_list();

		int width();
		int height();
		int size();

		// Just for visual debugging
		void print_pixels();
		void print_activations();

	private:
		Pixel* p_list;
		int w, h;
};

ImageConverter::ImageConverter() {}
ImageConverter::ImageConverter(const char* file_name) {
	open(file_name);
}

// Very sloppy way of getting a file type by file extension
// TODO: find a mime library for this
string get_file_type(string file_name) {
	size_t i = file_name.rfind('.', file_name.length());
	if(i != string::npos) return file_name.substr(i + 1, file_name.length() - i);
	return "";
}

void ImageConverter::open(const char* file_name) {
	string file_type = get_file_type((string) file_name);
	if(file_type == "png") open_png(file_name);
	else _err("Could not determine the file type of %s", file_name);
}

void ImageConverter::open_png(const char* file_name) {
	// libpng boilerplate code, found anywhere on the internet
	// --------------------------------------------------------
	png_infop info;
	png_structp p_struct;
	png_byte color_type, bit_depth;
	png_bytep* rows;
	
	FILE* f_object = fopen(file_name, "rb");
	if(!f_object) _err("Could not open file: %s\n Ensure it exists and you have proper permissions.", file_name);

	char header[8];
	fread(header, 1, 8, f_object);
	if(png_sig_cmp((png_const_bytep) header, 0, 8)) _err("%s is not recognized as a png file", file_name);
	
	p_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!p_struct) _err("Failed to create png object");
	
	info = png_create_info_struct(p_struct);
	if(!info) _err("Failed to create png info object");

	if(setjmp(png_jmpbuf(p_struct))) _err("Failed to initialize io");
	
	png_init_io(p_struct, f_object);
	png_set_sig_bytes(p_struct, 8);
	
	png_read_info(p_struct, info);
	
	w = png_get_image_width(p_struct, info);
	h = png_get_image_height(p_struct, info);
	color_type = png_get_color_type(p_struct, info);
	bit_depth = png_get_bit_depth(p_struct, info);
	
	png_read_update_info(p_struct, info);
	
	if(setjmp(png_jmpbuf(p_struct))) _err("Failed to read the image");
	
	rows = (png_bytep*) malloc(sizeof(png_bytep) * h);
	for(int i = 0; i < h; i++) rows[i] = (png_byte*) malloc(png_get_rowbytes(p_struct,info));
	
	png_read_image(p_struct, rows);

	if(png_get_color_type(p_struct, info) != PNG_COLOR_TYPE_RGBA)
		_err("Color type of %s must be RGBA (%d), but it is %d",
			file_name,
			PNG_COLOR_TYPE_RGBA,
			png_get_color_type(p_struct, info)
		);
	// ---------------------------------------------------
	// End libpng setup mumbo jumbo
	
	// Fill our global pixel list with pixel objects
	p_list = (Pixel*) malloc(sizeof(Pixel) * h * w);
	for(int y = 0; y < h; y++) {
		png_byte* row = rows[y];
		for(int x = 0; x < w; x++) {
			png_byte* p_next = &(row[x*4]);
			p_list[y*w + x] = Pixel(p_next[0], p_next[1], p_next[2], p_next[3]);
		}
	}

	fclose(f_object);
}

void ImageConverter::open_jpg(const char* file_name) {
	_err("Not implemented");
}
void ImageConverter::open_gif(const char* file_name) {
	_err("Not implemented");
}

int ImageConverter::width() {
	return w;
}
int ImageConverter::height() {
	return h;
}
int ImageConverter::size() {
	return w * h;
}

// A list of values 0 - 255 representing the activation level of each pixel in the image
int_fast16_t* ImageConverter::get_activation_list() {
	int_fast16_t* a_list = (int_fast16_t*) malloc(size());
	for(int i = 0; i < size(); i++) a_list[i] = p_list[i].get_activation();

	return a_list;
}

Pixel* ImageConverter::get_pixel_list() {
	return p_list;
}

void ImageConverter::print_pixels() {
	for(int i = 0; i < size(); i++) {
		if(i % w == 0) cout << endl;
		char p_notation[20];
		sprintf(p_notation, 
			"(%d, %d, %d, %d)         ",
			p_list[i].red,
			p_list[i].green,
			p_list[i].blue,
			p_list[i].alpha
		);
		printf("%.21s", p_notation);
	}
	cout << endl;
}
void ImageConverter::print_activations() {
	for(int i = 0; i < size(); i++) {
		if(i % w == 0) cout << endl;
		char p_notation[20];
		sprintf(p_notation, 
			"%d    ",
			p_list[i].get_activation()
		);
		printf("%.5s", p_notation);
	}
	cout << endl;
}
