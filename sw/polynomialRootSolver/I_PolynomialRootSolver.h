#include <opencv2/core/core.hpp>

class I_PolynomialRootSolver
{
public:
	virtual ~I_PolynomialRootSolver(){};
	virtual cv::Mat computeRoots(const cv::Mat& rowOfCoefficientsInAscendingDegreeOrder) = 0;
};
