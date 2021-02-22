#include "ImageConverter.hpp"
#include "Neuron.hpp"
#include <vector>
#include <memory>

template <class Data>
class NeuralNetwork {
	public:
		NeuralNetwork<Data>(int num_layers, int neur_per_layer, int range);
		void learn(const char* image_file_name, char value);
		char guess(uint_fast8_t* a_list);
		void print();

	private:
		// how many hidden num_layers and neurons per layer for this neural network
		int num_layers, neur_per_layer, range;

		// A 2d array of neuron layers
		//Neuron<Data>** neuron_matrix;
		vector< vector< unique_ptr< Neuron<Data> > > > neuron_matrix;

		// Easy api for turning image file names into pixel or activation arrays
		ImageConverter img;
};

template <class Data>
NeuralNetwork<Data>::NeuralNetwork(int num_layers, int neur_per_layer, int range) {
	if(num_layers < 2) _err("You must have at least 2 num_layers (you have it set to %d)", num_layers);

	// The range must be an odd number
	if(range%2 == 0) ++range;

	this->range = range;
	this->num_layers = num_layers;
	this->neur_per_layer = neur_per_layer;
	
	// Make space for our 2D vector
	//neuron_matrix = (Neuron<Data>**) malloc(sizeof(Neuron<Data>) * num_layers);
	neuron_matrix = vector< vector< unique_ptr< Neuron<Data> > > >(num_layers);
	
	// We go back to front so new neurons can get connect to the ones in front of them
	for(int i = num_layers - 1; i >= 0; i--) {
		//    ****   <-input layer: size=npl
		//   ******  <-after: size=npl+(i*(range-1))
		//  ********
		//neuron_matrix[i] = (Neuron<Data>*) malloc(sizeof(Neuron<Data>) * (neur_per_layer + (i*(range-1))));
		printf("Size of layer %d: %d\n", i, neuron_matrix.at(i).size());

		
		// Connect each individual neuron to the <range> neurons in the layer in front of it
		// (This is why we create the layers in reverse order)
		for(int j = 0; j < (neur_per_layer + (i*(range-1))); j++) {

			// Fill the last layer with motor neurons (these hold our answers!)
			if(i == num_layers - 1) {
				//neuron_matrix[i][j] = MotorNeuron<Data>();
				neuron_matrix.at(i).push_back(unique_ptr< Neuron<Data> >( new MotorNeuron<Data>));
			}

			// Fill every other layer with interneurons that connect to their neighbor in the next layer
			//else neuron_matrix[i][j] = InterNeuron<Data>( &neuron_matrix[i+1][j], range );
			else neuron_matrix.at(i).push_back(unique_ptr< Neuron<Data> >( new InterNeuron<Data>( &neuron_matrix.at(i+1).at(j), range )));
		}
	}
}

template <class Data>
void NeuralNetwork<Data>::learn(const char* image_file_name, char value) {
	img.open(image_file_name);
	int_fast16_t* activations = img.get_activation_list();
	float scale = neur_per_layer / img.size();
	printf("Npl: %d\nimage size: %d\nscale: %f\n", neur_per_layer, img.size(), scale);

	// The first layer gets poked (charged up with activation values)
	Neuron<Data>* n = &neuron_matrix.at(0).at(0);
	neuron_matrix.at(0).at(0)->t();
	for(int i = 0; i < neur_per_layer; i++) {
		printf("Activation level: %d\n", activations[(int)(i/scale)]);
		n->t();
		n->poke(activations[(int)(i/scale)], true);
		n++;
	}
}

template <class Data>
char NeuralNetwork<Data>::guess(uint_fast8_t* a_list) {
	char g = 'a';
	// TODO implement
	return g;
}

template <class Data>
void NeuralNetwork<Data>::print() {
	for(int i = neuron_matrix.size(); i >= 0; i--) {
		printf("Size of stuff: %d\n", neuron_matrix.at(i).size());
		for(int j = 0; j < neuron_matrix.at(i).size(); j++) {
			printf("*");
		}
		printf("\n");
	}
}
