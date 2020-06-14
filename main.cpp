//#include "splash.h"

#include <vector>
#include "math.h"
#include <Eigen/Dense>
#include <iostream>
using namespace std;


#include "neural.h"




int main( int argc, char* args[] ) {

	neural::net test;
	test.fit(100000);

	return 0;
}
