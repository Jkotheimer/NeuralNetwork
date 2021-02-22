class NeuralConnection {
	public:
		NeuralConnection();

		float get();
		float update(int_fast16_t value);
	private:
		// -2147483648 to 2147483647 how much the neurons are touching (how fluid a pulse from one neuron will affect the next)
		int_fast16_t strength;

		// How many times has this connection been overwritten. If This connection has been updated a bunch of times, it'll be very stable
		unsigned int stability;
};

NeuralConnection::NeuralConnection() {
	strength = 0;
	stability = 0;
}

// Return a number between 0 and 1 representing the 
float NeuralConnection::get() {
	return (float) strength / 255.0;
}

float NeuralConnection::update(int_fast16_t value) {
	// A weighted average
	strength = ((strength * stability) + value) / ++stability;
	return get();
}
