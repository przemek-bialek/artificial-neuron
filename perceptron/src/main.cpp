#include "pch.h"
#include "neuron.h"

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

void check_filename(std::string& name)
{
	int size = name.size();
	if (size < 4)
	{
		name += ".txt";
		return;
	}
	std::string tmp;
	for (int i = size - 4; i < size; i++)
		tmp += name[i];
	if (tmp != ".txt"&&tmp != ".TXT")
		name += ".txt";
}

void insert_fnames(std::string& name1, std::string& name2)
{
	std::cout << "Insert training filename and output filename\n";
	std::cout << "Training filename: ";
	std::cin >> name1;
	check_filename(name1);
	std::cout << "Output filename: ";
	std::cin >> name2;
	check_filename(name2);
}

bool sub_menu()
{
	int choice;
	std::cout << "Do you want to create a neuron with step function or sigmoidal function?\n";
	std::cout << "1) Step function\n";
	std::cout << "2) Sigmoidal function\n";
	std::cout << "Other number will be treated as step fucntion as well\n";
	insert_value(choice);
	switch (choice)
	{
	case 2:
		return false;
	default:
		return true;
	}
}

int menu()
{
	int choice;

	std::cout << "Default filenames (respectively trainig file, output file) : training_data.txt, wages.txt\n";
	std::cout << "Training data in a file should be in given order\n";
	std::cout << "\namount_of_inputs samples_qtty learning_coefficient momentum_coefficient(this one only for sigmoidal function)\n";
	std::cout << "input[1] input[2] ... input[amount_of_inputs] desired_output\n";
	std::cout << "input[1] input[2] ... input[amount_of_inputs] desired_output\n\n";
	
	std::cout << "How to init neuron?\n";
	std::cout << "1) File (.txt)\n";
	std::cout << "2) Console\n";
	std::cout << "Other - default (no params)\n";
	insert_value(choice);
	return choice;
}

int main()
{
	try
	{
		srand(time(0));

		std::string t_fname;
		std::string o_fname;

		float l_coeff;
		unsigned int i_qtty;
		unsigned int s_qtty;

		char optional;

		Neuron* neuron = nullptr;

		bool step = sub_menu();
		int choice = menu();

		switch (choice)
		{
		case 1:
		{
			insert_fnames(t_fname, o_fname);
			neuron = step ? new Neuron(t_fname, o_fname) : new Neuron_sigm(t_fname, o_fname);
			break;
		}
		case 2:
		{
			std::cout << "Insert learning coefficient, inputs quantity and amount of samples (training filename and output filename are optional)\n";
			std::cout << "Learning coefficient: ";
			insert_value(l_coeff);
			std::cout << "Inputs quantity: ";
			insert_value(i_qtty);
			std::cout << "Amount of samples: ";
			insert_value(s_qtty);
			std::cout << "Do you want to insert optional params?(y)\n";
			std::cin >> optional;
			if (optional == 'y')
			{
				std::cout << "First line of training file is not needed in this case\n";
				insert_fnames(t_fname, o_fname);
				neuron = step ? new Neuron(t_fname, o_fname, l_coeff, i_qtty, s_qtty) : new Neuron_sigm(t_fname, o_fname, l_coeff, i_qtty, s_qtty);
			}
			neuron =  step ? new Neuron(l_coeff, i_qtty, s_qtty) : new Neuron_sigm(t_fname, o_fname, l_coeff, i_qtty, s_qtty);
			break;
		}
		default:
			neuron = step ? new Neuron() : new Neuron_sigm();
		}

		while (1)
		{
			neuron->fetch_data();
			neuron->training_function();
			neuron->wages_adaptation();
			if (neuron->exit_check())
				break;
		}
		delete neuron;
	}
	catch (const std::string& exception)
	{
		std::cout << exception << "\n";
	}
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}