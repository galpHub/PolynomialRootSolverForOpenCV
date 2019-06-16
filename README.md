# PolynomialRootSolverForOpenCV
Simple real-root polynomial solver for use with OpenCV

Dependencies:
	- OpenCV 4.0.1: Or any older version of OpenCV that has access to the
	cv2eigen function for converting between the required cv and eigen types
	- Eigen 3.3.7: Strictly speaking most versions of the Eigen library
	having access to EigenSolvers that are compatible the MatrixXd template
	argument will work
	- UnitTest++ (optional): Only really required for running the tests.

Caveats/Warnings:
	- At this time the solver merely returns the real part of the roots it
	computes. This means that in the case that your root is a complex number
	it will merely drop that information.

Usage:
	- Use a CV_64F type
	- The program will yell at you if you pass in a column matrix for the coeffs
	- The coefficients should be in order of ascending degree: y = 1 + 2x^3 - 3x
	would be passed in as [1,-3,0,2] 