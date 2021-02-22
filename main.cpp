#include "NeuralNetwork.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	if(argc < 2) _err("Missing first parameter: image file name");
	NeuralNetwork<char> brain(4, 64, 3);
	brain.learn(argv[1], 'e');
	brain.print();

	return 0;
}
