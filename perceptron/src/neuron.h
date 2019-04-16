#pragma once

class Neuron
{
protected:
	std::string training_filename;
	std::string output_filename;

	float learning_coefficient;
	unsigned int inputs_qtty;
	unsigned int samples_qtty;

	float* inputs;
	float* wages;

	unsigned short int output;
	unsigned short int desired_output;
	int file_pos;
	int first_sample_position;
	unsigned int correct_decisions;
public:
	Neuron(const Neuron& other);

	Neuron();
	Neuron(const std::string& in_fname, const std::string& out_fname);
	Neuron(const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty);
	Neuron(const std::string& in_fname, const std::string& out_fname, const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty);

	unsigned short int fetch_data();
	virtual void training_function();
	virtual void wages_adaptation();
	virtual unsigned short int exit_check();

	~Neuron();
};

class Neuron_sigm :public Neuron
{
	float momentum_coefficient;
	unsigned int epoch;

	float* prev_wages;
public:
	Neuron_sigm(const Neuron_sigm& other);

	Neuron_sigm();
	Neuron_sigm(const std::string& in_fname, const std::string& out_fname);
	Neuron_sigm(const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty);
	Neuron_sigm(const std::string& in_fname, const std::string& out_fname, const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty);

	void training_function();
	void wages_adaptation();
	unsigned short int exit_check();

	~Neuron_sigm();
};