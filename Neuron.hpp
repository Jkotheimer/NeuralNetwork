#include "NeuralConnection.hpp"
using namespace std;

template <class Data>
class Neuron {
	protected:
		uint_fast16_t charge;

	public:
		Neuron<Data>() {
			charge = 0;
		}
		virtual void t() = 0;
		
		// Poke this neuron with a force and choose whether to make it propogate
		virtual void poke(uint16_t force, bool prop) = 0;

		// Give this neuron a bias
		void bias(uint16_t force) {
			charge += force;
		}
		void print() {
			printf("(%d) ", charge);
		}
};

/*
 * INTERNEURONS
 * ----------------------------------------------------------------------------
 *  These lil buggers are instantiated with a network of other neurons, represented as a 1 dimentional array of memory addresses (Neuron**)
 *  Their primary goal is to pass along information. That's it. They retrieve a pulse by being poke()'d with a certain amount of force.
 *  They then observe their network, and diffuse the force of that poke to those neurons based on how trained each of them are.
 *  A more trained neuron will take a majority of the force, leaving the rest of the network to recieve much weaker pokes.
 *  This ideally converges on an information neuron (see InformationNeuron)
 */
template <class Data>
class InterNeuron : public Neuron<Data> {
	private:
		// A pointer to the neuron furthest to my left. My network extends <range> neurons to the right from there.
		Neuron<Data>* network;
		int range;
		uint_fast16_t charge;

		void propogate() {
			printf("Here! %d\n", charge);
		}
	public:
		InterNeuron(Neuron<Data>* network, int range) {
			this->network = network;
			this->range = range;
		}

		void poke(uint16_t force, bool prop) {
			printf("HERE\n");
			charge += force;
			if(prop) propogate();
		}
		void t() {
			printf("InterNeuron");
		}
};



template <class Data>
class MotorNeuron : public Neuron<Data> {
	private:
		uint_fast16_t charge;
	public:
		MotorNeuron() {}
		void poke(uint16_t force, bool prop) {
			charge += force;
		}
		void t() {
			printf("MotorNeuron");
		}
};
