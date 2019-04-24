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
	float* weights;

	unsigned short int output;
	unsigned short int desired_output;
	int file_pos;
	int first_sample_position;
	unsigned int correct_decisions;

	unsigned int iterator;
	unsigned int sample_number;
public:
	Neuron(const Neuron& other);

	Neuron();

	//init
	virtual void init_arrays();
	virtual void init_with_file();
	virtual void init_with_console();
	virtual void init();

	//learning
	void fetch_data();
	virtual void training_function();
	virtual void weights_adaptation();
	virtual bool stop_criterion();//example of stop condition, you should implement your own though
	void weights_derivation();

	//getters
	std::string GetTrainingFilename() const;
	std::string GetOutputFilename() const;
	float GetLearningCoefficient() const;
	unsigned int GetInputsQtty() const;
	unsigned int GetSamplesQtty() const;
	float* GetInputs() const;
	float* GetWeights() const;
	unsigned short int GetOutput() const;
	unsigned short int GetDesiredOutput() const;
	int GetFilePos() const;
	int GetFirstSamplePosition() const;
	unsigned int GetCorrectDecisions() const;
	unsigned int GetIterator() const;
	unsigned int GetSampleNumber() const;

	//setters
	void SetFilePos(const int& f_pos);
	void SetCorrectDecisions(const unsigned int& corr_decs);
	void SetIterator(const unsigned int& it);
	void SetSampleNumber(const unsigned int& samp_num);

	~Neuron();
};

class Neuron_sigm :public Neuron
{
	float momentum_coefficient;
	unsigned int epoch;

	float* prev_weights;

	float Q;//difference between desired output and output
	unsigned long int max_iterations;
	float Qmax;//max diff between desired output and output
	float learning_tolerance;
	float delta;
public:
	Neuron_sigm(const Neuron_sigm& other);

	Neuron_sigm();

	//init
	void init_arrays();
	void init_with_file();
	void init_with_console();
	void init();

	//learning 
	void training_function();
	void weights_adaptation();
	bool stop_critierion();//example of stop condition, you should implement your own though

	//getters
	float GetMomentumCoefficient() const;
	unsigned int GetEpoch() const;
	float* GetPrevWeights() const;
	float GetQ() const;
	unsigned long int GetMaxIterations() const;
	float GetQmax() const;
	float GetLearningTolerance() const;
	float GetDelta() const;

	//setters
	void SetEpoch(const unsigned int& e);
	void SetQ(const float& q);
	void SetDelta(const float& d);

	~Neuron_sigm();
};