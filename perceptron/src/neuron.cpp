#include "pch.h"
#include "neuron.h"

static const float e = std::exp(1.0f);
static std::string training = "training/";
static std::string results = "results/";

Neuron::Neuron(const Neuron& other)
	: training_filename(other.training_filename), output_filename(other.output_filename),
	learning_coefficient(other.learning_coefficient), inputs_qtty(other.inputs_qtty), samples_qtty(other.samples_qtty),
	inputs(other.inputs), wages(other.wages), output(other.output), desired_output(other.desired_output), file_pos(other.file_pos),
	first_sample_position(other.first_sample_position), correct_decisions(other.correct_decisions)
{
	std::cout << "Neuron copied\n";
}

Neuron::Neuron()
	: training_filename(training + "training_data.txt"), output_filename(results + "wages.txt"), output(0),
	desired_output(0), correct_decisions(0)
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		inputs_qtty += 1;
		file >> samples_qtty;
		file >> learning_coefficient;
		file_pos = file.tellg();
		first_sample_position = file_pos;

		inputs = new float[inputs_qtty];
		wages = new float[inputs_qtty];

		inputs[0] = 1.0f;
		for (int i = 0; i < inputs_qtty; i++)
			wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		file.close();
	}
	else
		throw "[INIT ERROR] Could not open \"" + training_filename + "\"\n";
}

Neuron::Neuron(const std::string& in_fname, const std::string& out_fname)
	: training_filename(training + in_fname), output_filename(results + out_fname), output(0), desired_output(0), correct_decisions(0)
{
	std::ifstream file;
	file.open(in_fname);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		inputs_qtty += 1;
		file >> samples_qtty;
		file >> learning_coefficient;
		file_pos = file.tellg();
		first_sample_position = file_pos;

		inputs = new float[inputs_qtty];
		wages = new float[inputs_qtty];

		inputs[0] = 1.0f;
		for (int i = 0; i < inputs_qtty; i++)
			wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		file.close();
	}
	else
		throw "[INIT] Could not open \"" + in_fname + "\"";
}

Neuron::Neuron(const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty)
	: training_filename(training + "training_data.txt"), output_filename(results + "wages.txt"), learning_coefficient(learn_coeff), inputs_qtty(in_qtty),
	output(0), desired_output(0), file_pos(0), first_sample_position(0), samples_qtty(samp_qtty), correct_decisions(0)
{
	inputs_qtty += 1;
	inputs = new float[inputs_qtty];
	wages = new float[inputs_qtty];

	inputs[0] = 1.0f;
	for (int i = 0; i < inputs_qtty; i++)
		wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
}

Neuron::Neuron(const std::string& in_fname, const std::string& out_fname, const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty)
	: training_filename(training + in_fname), output_filename(results + out_fname), learning_coefficient(learn_coeff), inputs_qtty(in_qtty), output(0),
	  desired_output(0), file_pos(0), first_sample_position(0), samples_qtty(samp_qtty), correct_decisions(0)
{
	inputs_qtty += 1;
	inputs = new float[inputs_qtty];
	wages = new float[inputs_qtty];

	inputs[0] = 1.0f;
	for (int i = 0; i < inputs_qtty; i++)
		wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
}

unsigned short int Neuron::fetch_data()
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(file_pos,std::ios_base::beg);
		for (int i = 1; i < inputs_qtty; i++)
			file >> inputs[i];
		file >> desired_output;
		file_pos = file.tellg();
		file.close();
		return 1;
	}
	else
		throw "[FETCH] Could not open \"" + training_filename + "\"";
}

void Neuron::training_function()
{
	float sum = 0;

	for (int i = 0; i < inputs_qtty; i++)
		sum += inputs[i] * wages[i];

	output = (sum > 0 ? 1 : 0);
}

void Neuron::wages_adaptation()
{
	if (desired_output != output)
	{
		float tmp = learning_coefficient * (desired_output - output);

		correct_decisions = 0;
		for (int i = 0; i < inputs_qtty; i++)
			wages[i] = wages[i] + tmp * inputs[i];
	}
	else
		correct_decisions += 1;
}

unsigned short int Neuron::exit_check()
{
	std::ofstream file;

	if (correct_decisions == samples_qtty)
	{
		file.open(output_filename);
		if (file.is_open())
		{
			std::cout << "Done! Check \"" << output_filename << "\" for results\n";
			for (int i = 0; i < inputs_qtty; i++)
				file << "wages[" << i << "] = " << wages[i] << "\n";
			return 1;
		}
		else
			throw "[DERIVATION] Could not open \"" + output_filename + "\"";
	}
	else
		if (file_pos < 0)
			file_pos = first_sample_position;
	return 0;
}

Neuron::~Neuron()
{
	delete[] inputs;
	delete[] wages;
}


Neuron_sigm::Neuron_sigm(const Neuron_sigm& other)
	: momentum_coefficient(other.momentum_coefficient), epoch(other.epoch), prev_wages(other.prev_wages)
{
	std::cout << "Neuron_sigm copied\n";
}

Neuron_sigm::Neuron_sigm()
	: Neuron::Neuron(), epoch(0)
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		inputs_qtty += 1;
		file >> samples_qtty;
		file >> learning_coefficient;
		file >> momentum_coefficient;
		file_pos = file.tellg();
		first_sample_position = file_pos;

		inputs = new float[inputs_qtty];
		wages = new float[inputs_qtty];
		prev_wages = new float[inputs_qtty];

		inputs[0] = 1.0f;
		for (int i = 0; i < inputs_qtty; i++)
		{
			wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
			prev_wages[i] = 0.0f;
		}
		file.close();
	}
	else
		throw "[INIT ERROR] Could not open \"training_data.txt\"";
}

Neuron_sigm::Neuron_sigm(const std::string& in_fname, const std::string& out_fname)
	: Neuron::Neuron(in_fname, out_fname), epoch(0)
{
	std::ifstream file;
	file.open(in_fname);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		inputs_qtty += 1;
		file >> samples_qtty;
		file >> learning_coefficient;
		file >> momentum_coefficient;
		file_pos = file.tellg();
		first_sample_position = file_pos;

		inputs = new float[inputs_qtty];
		wages = new float[inputs_qtty];
		prev_wages = new float[inputs_qtty];

		inputs[0] = 1.0f;
		for (int i = 0; i < inputs_qtty; i++)
		{
			wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
			prev_wages[i] = 0.0f;
		}
		file.close();
	}
	else
		throw "[INIT] Could not open \"" + in_fname + "\"";
}

Neuron_sigm::Neuron_sigm(const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty)
	: Neuron::Neuron(learn_coeff, in_qtty, samp_qtty), momentum_coefficient(0.2f), epoch(0)
{
	inputs_qtty += 1;
	inputs = new float[inputs_qtty];
	wages = new float[inputs_qtty];
	prev_wages = new float[inputs_qtty];

	inputs[0] = 1.0f;
	for (int i = 0; i < inputs_qtty; i++)
	{
		wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		prev_wages[i] = 0.0f;
	}
}

Neuron_sigm::Neuron_sigm(const std::string& in_fname, const std::string& out_fname, const float& learn_coeff, const unsigned int& in_qtty, const unsigned int& samp_qtty)
	: Neuron::Neuron(in_fname, out_fname, learn_coeff, in_qtty, samples_qtty), momentum_coefficient(0.2f), epoch(0)
{
	inputs_qtty += 1;
	inputs = new float[inputs_qtty];
	wages = new float[inputs_qtty];
	prev_wages = new float[inputs_qtty];

	inputs[0] = 1.0f;
	for (int i = 0; i < inputs_qtty; i++)
	{
		wages[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		prev_wages[i] = 0.0f;
	}
}

void Neuron_sigm::training_function()
{
	float sum = 0;

	for (int i = 0; i < inputs_qtty; i++)
		sum += inputs[i] * wages[i];

	output = (1.0f / (1.0f + pow(e, -sum)) > 0.5f ? 1 : 0);
}

void Neuron_sigm::wages_adaptation()
{
	if (desired_output != output)
	{
		correct_decisions = 0;

		float* tmp = new float[inputs_qtty];
		for (int i = 0; i < inputs_qtty; i++)
			tmp[i] = wages[i] - prev_wages[i];

		for (int i = 0; i < inputs_qtty; i++)
		{
			prev_wages[i] = wages[i];
			wages[i] = wages[i] + learning_coefficient * inputs[i] + momentum_coefficient * tmp[i];
		}
		delete[] tmp;
	}
	else
		correct_decisions += 1;
}

unsigned short int Neuron_sigm::exit_check()
{
	std::ofstream file;

	if (correct_decisions == samples_qtty || epoch == 10)
	{
		file.open(output_filename);
		if (file.is_open())
		{
			std::cout << "Done! Check \"" << output_filename << "\" for results\n";
			for (int i = 0; i < inputs_qtty; i++)
				file << "wages[" << i << "] = " << wages[i] << "\n";
			file << "epochs = " << epoch << "\n";
			return 1;
		}
		else
			throw "[DERIVATION] Could not open \"" + output_filename + "\"";
	}
	else
		if (file_pos < 0)
		{
			file_pos = first_sample_position;
			epoch += 1;
		}
	return 0;
}

Neuron_sigm::~Neuron_sigm()
{
	delete[] prev_wages;
}