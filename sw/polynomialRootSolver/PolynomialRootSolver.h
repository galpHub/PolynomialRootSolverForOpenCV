#include "I_PolynomialRootSolver.h"
class PolynomialRootSolver: public I_PolynomialRootSolver
{
public:
	PolynomialRootSolver();
	virtual ~PolynomialRootSolver();
	virtual cv::Mat computeRoots(const cv::Mat& rowOfCoefficientsInAscendingDegreeOrder);
};
