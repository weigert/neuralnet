/*

Neural Namespace

This contains my homemade implementation of a neural
network with arbitary layer structure and different
activation / training methods.


ToDo:
	Random Masking + Produce Mean / Variance
	of the result.

	We can thereby have an expected outcome
	with a degree of uncertainty.

*/

namespace neural {

/*
=====================================================
                Activation Functions
=====================================================
*/


/*
//Tanh approximation
double act(double x){
	return 0.5f*(tanh(x)+1.0f);
}

double dact(double x){
	return 0.5f*(1.0f-tanh(x)*tanh(x));
}
*/


//Full Sigmoid
double act(double x){
	return 0.5f*(tanh(x*0.5)+1.0f);
}

double dact(double x){
	return act(x)*act(-x);
}

using afunc = std::function<double(double)>;

/*
====================================================
                Network and Layers
====================================================
*/

class Layer{
public:

	Eigen::MatrixXd wt;
  Eigen::VectorXd val;  //Un-Activated Values
  Eigen::VectorXd err;  //Stored Error Information
	Eigen::VectorXd bias; //Bias (Always One)

	Layer(int in, int out = 0, bool _bias = true){
		val = Eigen::ArrayXd::Zero(in);
		err = Eigen::ArrayXd::Zero(in);
		bias = (_bias)?Eigen::ArrayXd::Ones(1):Eigen::ArrayXd::Ones(0);
		wt 	= Eigen::ArrayXXd::Random(out, in + bias.size());
	}

	Eigen::VectorXd forward(){	//Layer Behind
		Eigen::VectorXd concat(val.size() + bias.size());
		concat << val.unaryExpr(&act), bias;
		return wt*concat; //Activate, Multiply
	}

	void backward(Eigen::VectorXd& ferr){ //Layer in Front
		Eigen::VectorXd fullerr = wt.transpose()*ferr;
		err = fullerr.segment(0,fullerr.size()-bias.size()); //Local Error
	}

	void learn(Layer* l, float rate){
		Eigen::VectorXd concat(val.size() + bias.size());
		concat << val.unaryExpr(&act), bias;
		Eigen::VectorXd fval = l->val.unaryExpr(&dact);
		wt += rate*(l->err.cwiseProduct(fval))*concat.transpose();
	}

};

class net{
public:
	vector<Layer> layers;
	float rate = 1.0; //Learning Rate
	int maxepochs;

	net(){
		Layer in(1, 10);
		Layer hidden(10, 1);
		Layer out(1);

		layers.push_back(in);
		layers.push_back(hidden);
		layers.push_back(out);
	}

	//Function to Fit
	double map(double x){
		return x*x;//sin(3.14159265f*x);
	}

	void fit(int epochs){
		srand(time(NULL));
		maxepochs = epochs;

		while(epochs > 0){
			//First Layer Gets Input Value
			float input = 1.0f*((double)(rand()%1000)/1000.0);
			float mapped = map(input);

			//Choose Input Number...
			layers[0].val = layers[0].val.unaryExpr([&](double x){
				return input;
			});

			//Feed Forward
			for(int i = 1; i < layers.size(); i++)
				layers[i].val = layers[i-1].forward();

			//Compute the Error at the Last Node
			layers.back().err = layers.back().err.unaryExpr([&](double x){
				return mapped - layers.back().val[0];
			});

			//Propagate Backward
			for(unsigned int i = layers.size()-2; i > 0; i--)
				layers[i].backward(layers[i+1].err);

			//Learn
			for(int i = 0; i < layers.size()-1; i++)
				layers[i].learn(&layers[i+1], rate);

			if(epochs < 100000)
				std::cout<<epochs<<" "<<layers.back().err[0]<<endl;

			adapt(epochs);
			epochs--;
		}
	}

	void adapt(int cur){
		//rate = (float)cur/(float)maxepochs;
	}

	void plot(){
		for(float x = 0; x < 1.0; x += 0.001){
			//float mapped = map(input);

			//Set Input
			layers[0].val = layers[0].val.unaryExpr([&](double _x){
				return x;
			});

			for(int i = 1; i < layers.size(); i++)
				layers[i].val = layers[i-1].forward();

			std::cout<<x<<" "<<layers.back().val[0]<<endl;
		}
	}
};

};  //End of Namespace
