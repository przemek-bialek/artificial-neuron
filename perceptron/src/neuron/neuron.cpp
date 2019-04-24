#include "pch.h"
#include "neuron.h"

static const float e = std::exp(1.0f);
static std::string training_dir = "training/";
static std::string results_dir = "results/";
static std::string def_train_file = "training_data.txt";
static std::string def_res_file = "weights.txt";

template <typename T>
void insert_value(T& var)
{
	std::cin >> var;
	while (!std::cin)
	{
		std::cout << "Input correct number\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> var;
	}
}

void insert_fname(std::string& name)
{
	std::cin >> name;
	int size = name.size();
	if (size < 4)
	{
		name += ".txt";
		return;
	}
	std::string tmp;
	for (int i = size - 4; i < size; i++)
		tmp += name[i];
	if (tmp != ".txt" && tmp != ".TXT")
		name += ".txt";
}


Neuron::Neuron(const Neuron& other)
	: training_filename(other.training_filename), output_filename(other.output_filename), learning_coefficient(other.learning_coefficient),
	inputs_qtty(other.inputs_qtty), samples_qtty(other.samples_qtty), inputs(other.inputs), weights(other.weights), output(other.output),
	desired_output(other.desired_output), file_pos(other.file_pos), first_sample_position(other.first_sample_position), correct_decisions(other.correct_decisions),
	iterator(other.iterator), sample_number(other.sample_number)
{
	std::cout << "Neuron copied\n";
}

Neuron::Neuron()
	: training_filename(training_dir + def_train_file), output_filename(results_dir + def_res_file), learning_coefficient(0.0f), inputs_qtty(0), samples_qtty(0), 
	output(0), desired_output(0), file_pos(0), first_sample_position(0), correct_decisions(0), iterator(0), sample_number(1)
{
}

void Neuron::init_with_file()
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		file >> samples_qtty;
		file >> learning_coefficient;
		file_pos = (int)file.tellg();
		first_sample_position = file_pos;
	}
}

void Neuron::init_with_console()
{
	std::cout << "Learning coefficient: ";
	insert_value(learning_coefficient);
	std::cout << "Inputs quantity: ";
	insert_value(inputs_qtty);
	std::cout << "Amount of samples: ";
	insert_value(samples_qtty);
}

void Neuron::init_arrays()
{
	inputs_qtty += 1;

	inputs = new float[inputs_qtty];
	weights = new float[inputs_qtty];

	inputs[0] = 1.0f;

	for (unsigned int i = 0; i < inputs_qtty; i++)
		weights[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
}

void Neuron::init()
{
	int choice;
	char optional;
	std::string tmp;

	std::cout << "Please do specify number of inputs without bias (appliciation will add it itself)\n";
	std::cout << "Default filenames (respectively trainig file, output file) : training_data.txt, weights.txt\n";
	std::cout << "Training data in a file should be in given order:\n";
	std::cout << "\namount_of_inputs samples_qtty learning_coefficient momentum_coefficient(this one only for sigmoidal function)\n";
	std::cout << "input[1] input[2] ... input[amount_of_inputs] desired_output\n";
	std::cout << "input[1] input[2] ... input[amount_of_inputs] desired_output\n\n";

	std::cout << "\nHow to init neuron?\n";
	std::cout << "1) File (.txt)\n";
	std::cout << "2) Console\n";
	std::cout << "Other - file with default fnames \n";
	insert_value(choice);
	switch (choice)
	{
	case 1:
	{
		training_filename = training_dir;
		output_filename = results_dir;
		std::cout << "Insert training filename and output filename\n";
		std::cout << "Training filename: ";
		insert_fname(tmp);
		training_filename += tmp;
		std::cout << "Output filename: ";
		insert_fname(tmp);
		output_filename += tmp;
		init_with_file();
		break;
	}
	case 2:
	{
		std::cout << "Insert learning coefficient, momentum_coefficient (only for sigm), inputs quantity and amount of samples (training filename and output filename are optional)\n";
		std::cout << "First line of training file is not needed in this case\n";
		init_with_console();
		std::cout << "Do you want to insert optional params?(y)\n";
		std::cin >> optional;
		if (optional == 'y')
		{
			training_filename = training_dir;
			output_filename = results_dir;
			std::cout << "Training filename: ";
			insert_fname(tmp);
			training_filename += tmp;
			std::cout << "Output filename: ";
			insert_fname(tmp);
			output_filename += tmp;
		}
		break;
	}
	default:
		init_with_file();
	}
	init_arrays();
}

void Neuron::fetch_data()
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(file_pos,std::ios_base::beg);
		for (unsigned int i = 1; i < inputs_qtty; i++)
			file >> inputs[i];
		file >> desired_output;
		file_pos = (int)file.tellg();
		file.close();
	}
	else
		throw "[FETCH] Could not open \"" + training_filename + "\"";
}

void Neuron::training_function()
{
	float sum = 0;

	for (unsigned int i = 0; i < inputs_qtty; i++)
		sum += inputs[i] * weights[i];

	output = (sum > 0 ? 1 : 0);
}

void Neuron::weights_adaptation()
{
	if (desired_output != output)
	{
		float tmp = learning_coefficient * (desired_output - output);

		correct_decisions = 0;
		for (unsigned int i = 0; i < inputs_qtty; i++)
			weights[i] = weights[i] + tmp * inputs[i];
	}
	else
		correct_decisions += 1;
}

bool Neuron::stop_criterion()
{
	if (correct_decisions == samples_qtty)
	{
		weights_derivation();
		return true;
	}
	else
	{
		iterator += 1;
		sample_number = (sample_number + 1) > samples_qtty ? 1 : (sample_number + 1);
		if (file_pos < 0)
		{
			file_pos = first_sample_position;
		}

	}
	return false;
}

void Neuron::weights_derivation()
{
	std::ofstream file;

	file.open(output_filename);
	if (file.is_open())
	{
		std::cout << "Done! Check \"" << output_filename << "\" for results\n";
		for (unsigned int i = 0; i < inputs_qtty; i++)
			file << "weights[" << i << "] = " << weights[i] << "\n";
		file.close();
		return;
	}
	else
		throw "[DERIVATION] Could not open \"" + output_filename + "\"";
}

std::string Neuron::GetTrainingFilename() const
{
	return training_filename;
}
std::string Neuron::GetOutputFilename() const
{
	return output_filename;
}
float Neuron::GetLearningCoefficient() const
{
	return learning_coefficient;
}
unsigned int Neuron::GetInputsQtty() const
{
	return inputs_qtty;
}
unsigned int Neuron::GetSamplesQtty() const
{
	return samples_qtty;
}
float* Neuron::GetInputs() const
{
	return inputs;
}
float* Neuron::GetWeights() const
{
	return weights;
}
unsigned short int Neuron::GetOutput() const
{
	return output;
}
unsigned short int Neuron::GetDesiredOutput() const
{
	return desired_output;
}
int Neuron::GetFilePos() const
{
	return file_pos;
}
int Neuron::GetFirstSamplePosition() const
{
	return first_sample_position;
}
unsigned int Neuron::GetCorrectDecisions() const
{
	return correct_decisions;
}
unsigned int Neuron::GetIterator() const
{
	return iterator;
}
unsigned int Neuron::GetSampleNumber() const
{
	return sample_number;
}

void Neuron::SetFilePos(const int& f_pos)
{
	file_pos = f_pos;
}
void Neuron::SetCorrectDecisions(const unsigned int& corr_decs)
{
	correct_decisions = corr_decs;
}
void Neuron::SetIterator(const unsigned int& it)
{
	iterator = it;
}
void Neuron::SetSampleNumber(const unsigned int& samp_num)
{
	sample_number = samp_num;
}

Neuron::~Neuron()
{
	delete[] inputs;
	delete[] weights;
}


Neuron_sigm::Neuron_sigm(const Neuron_sigm& other)
	: momentum_coefficient(other.momentum_coefficient), epoch(other.epoch), prev_weights(other.prev_weights)
{
	std::cout << "Neuron_sigm copied\n";
}

Neuron_sigm::Neuron_sigm()
	: Neuron::Neuron(), momentum_coefficient(0.0f), epoch(0), Q(0), max_iterations(100), Qmax(2.2f), learning_tolerance(0.1f), delta(0.0f)
{
}

void Neuron_sigm::init_with_file()
{
	std::ifstream file;
	file.open(training_filename);
	if (file.is_open())
	{
		file.seekg(0, std::ios_base::beg);
		file >> inputs_qtty;
		file >> samples_qtty;
		file >> learning_coefficient;
		file >> momentum_coefficient;
		file_pos = (int)file.tellg();
		first_sample_position = file_pos;
	}
}

void Neuron_sigm::init_with_console()
{
	std::cout << "Learning coefficient: ";
	insert_value(learning_coefficient);
	std::cout << "Momentum coefficient: ";
	insert_value(momentum_coefficient);
	std::cout << "Inputs quantity: ";
	insert_value(inputs_qtty);
	std::cout << "Amount of samples: ";
	insert_value(samples_qtty);
}

void Neuron_sigm::init_arrays()
{
	inputs_qtty += 1;

	inputs = new float[inputs_qtty];
	weights = new float[inputs_qtty];
	prev_weights = new float[inputs_qtty];

	inputs[0] = 1.0f;

	for (unsigned int i = 0; i < inputs_qtty; i++)
	{
		weights[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		prev_weights[i] = 0.0f;
	}
}

void Neuron_sigm::init()
{
	char choice;
	Neuron::init();

	std::cout << "Following parameters are optional\n";
	std::cout << "Do you want to specify?(y-yes other-no):\n";
	std::cout << "max_iterations\n";
	std::cin >> choice;
	if (choice == 'y')
	{
		std::cout << "Insert a value for max_iterations: ";
		insert_value(max_iterations);
	}

	std::cout << "Do you want to specify?(y-yes other-no):\n";
	std::cout << "Qmax\n";
	std::cin >> choice;
	if (choice == 'y')
	{
		std::cout << "Insert a value for Qmax: ";
		insert_value(Qmax);
	}

	std::cout << "Do you want to specify?(y-yes other-no):\n";
	std::cout << "learning_tolerance\n";
	std::cin >> choice;
	if (choice == 'y')
	{
		std::cout << "Insert a value for learning_tolerance: ";
		insert_value(learning_tolerance);
	}
}

void Neuron_sigm::training_function()
{
	float sum = 0;

	for (unsigned int i = 0; i < inputs_qtty; i++)
		sum += inputs[i] * weights[i];

	output = (1.0f / (1.0f + pow(e, -sum)) > 0.5f ? 1 : 0);
}

void Neuron_sigm::weights_adaptation()
{
	if (desired_output != output)
	{
		correct_decisions = 0;

		float tmp;

		for (unsigned int i = 0; i < inputs_qtty; i++)
		{
			tmp = weights[i];
			weights[i] = weights[i] + learning_coefficient * inputs[i] + momentum_coefficient * (prev_weights[i] - weights[i]);
			prev_weights[i] = tmp;
		}
	}
	else
		correct_decisions += 1;
}

bool Neuron_sigm::stop_critierion()
{
	if (correct_decisions == samples_qtty || epoch == 10)
	{
		weights_derivation();
		return true;
	}
	else
	{
		iterator += 1;
		sample_number = (sample_number + 1) > samples_qtty ? 1 : (sample_number + 1);
		if (file_pos < 0)
		{
			file_pos = first_sample_position;
			epoch += 1;
		}
	}
	return false;
}

float Neuron_sigm::GetMomentumCoefficient() const
{
	return momentum_coefficient;
}
unsigned int Neuron_sigm::GetEpoch() const
{
	return epoch;
}
float* Neuron_sigm::GetPrevWeights() const
{
	return prev_weights;
}
float Neuron_sigm::GetQ() const
{
	return Q;
}
unsigned long int Neuron_sigm::GetMaxIterations() const
{
	return max_iterations;
}
float Neuron_sigm::GetQmax() const
{
	return Qmax;
}
float Neuron_sigm::GetLearningTolerance() const
{
	return learning_tolerance;
}
float Neuron_sigm::GetDelta() const
{
	return delta;
}

void Neuron_sigm::SetEpoch(const unsigned int& e)
{
	epoch = e;
}
void Neuron_sigm::SetQ(const float& q)
{
	Q = q;
}
void Neuron_sigm::SetDelta(const float& d)
{
	delta = d;
}

Neuron_sigm::~Neuron_sigm()
{
	delete[] prev_weights;
}