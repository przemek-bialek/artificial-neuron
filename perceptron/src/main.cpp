#include "pch.h"
#include "neuron\neuron.h"

void insert_value(int& var)
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

bool menu()
{
	int choice;
	std::cout << "Do you want to create a neuron with step function or sigmoidal function?\n";
	std::cout << "1) Step function\n";
	std::cout << "2) Sigmoidal function\n";
	std::cout << "Any other number will be treated as step function as well\n";
	insert_value(choice);
	return choice == 2 ? false : true;
}

int main()
{
	try
	{
		srand(time(0));

		bool is_step = menu();
		Neuron* neuron = is_step ? new Neuron() : new Neuron_sigm();
		neuron->init();
		while (1)
		{
			neuron->fetch_data();
			neuron->training_function();
			neuron->weights_adaptation();
			if (neuron->stop_criterion())
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