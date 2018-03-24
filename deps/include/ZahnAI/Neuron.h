#pragma once

#include <vector>
#include <cmath>

namespace ZahnAI{

	class Neuron;

	typedef std::vector<Neuron> Layer;

	struct Connection{
		double weight;
		double deltaWeight;
	};

	class Neuron
	{
	public:
		Neuron(unsigned, unsigned);
		~Neuron();

		static void setTraining(bool b){ isTraining = b; }
		static void setAlpha(double a){ alpha = a; } //Set the momentum
		static void setETA(double e){ eta = e; }	//Set the learning rate


		void setChanceToActivate(double d){ m_chanceToActivate = d; }
		void activate(bool randomize){
			if (randomize){
				double d = rand() / double(RAND_MAX);
				m_isActive = d <= m_chanceToActivate;
			}
			else{
				m_isActive = true;
			}
		}

		bool getActive(){ return m_isActive; }

		void setOutputVal(double val){ m_outputVal = val; }
		double getOutputVal(){
			if (isTraining)
				return m_outputVal;

			return m_outputVal * m_chanceToActivate;
		}

		void feedForward(Layer &prevLayer);

		void calcOutputGradients(double target);
		void calcHiddenGradients(Layer& nextLayer);
		void updateInputWeights(Layer& prevLayer, double eta, double alpha);

		std::vector<Connection>& getOutputWeights(){ return m_outputWeights; }
		void setOutputWeights(const std::vector<Connection>& weights){ m_outputWeights = weights; }

	private:
		bool m_isActive;
		double m_chanceToActivate;
		static bool isTraining;

		unsigned m_index;

		static double eta;
		static double alpha;

		double m_gradient;
		double m_outputVal;
		std::vector<Connection> m_outputWeights;

		static double randomWeight(){ return rand() / double(RAND_MAX) - 0.5; }
		static double transferFunction(double);
		static double transferFunctionDerivative(double);
		double sumDOW(Layer& nextLayer);
	};
}
