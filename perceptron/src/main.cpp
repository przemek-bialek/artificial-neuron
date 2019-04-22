#include "pch.h"
#include "neuron\neuron.h"

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
		if (is_step)
		{
			Neuron neuron;
			neuron.init();
			while (1)
			{
				neuron.fetch_data();
				neuron.training_function();
				neuron.weights_adaptation();
				if (neuron.stop_criterion())
					break;
			}
		}
		else
		{
			Neuron_sigm neuron;
			neuron.init();
			while (1)
			{
				neuron.fetch_data();
				neuron.training_function();
				neuron.weights_adaptation();
				if (neuron.stop_criterion())
					break;
			}
		}
	}
	catch (const std::string& exception)
	{
		std::cout << exception << "\n";
	}
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}