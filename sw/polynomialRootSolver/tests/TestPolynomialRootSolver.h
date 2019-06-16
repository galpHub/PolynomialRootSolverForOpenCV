#include <UnitTest++.h>
#include "../PolynomialRootSolver.h"
#include <random>


void veryfyThatActualAndExpectedRootsAreWithinTolerance(
    cv::Mat actual, cv::Mat expected)
{
    cv::sort(actual,actual, cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING);
    cv::sort(expected,expected, cv::SortFlags::SORT_EVERY_ROW + cv::SortFlags::SORT_ASCENDING);
    for(int rootIndex = 0; rootIndex < actual.cols; rootIndex++)
    {
        CHECK_CLOSE(expected.at<double>(rootIndex),actual.at<double>(rootIndex),1e-6);
    }
}

namespace TestPolynomialRootSolver
{
    cv::Mat shiftDegreeUpByOne(const cv::Mat& polynomialCoefficients)
    {
        cv::Mat shiftedPolynomialCoefficients(polynomialCoefficients.rows,polynomialCoefficients.cols,polynomialCoefficients.type(),cv::Scalar(0));
        if(polynomialCoefficients.cols < 2)
        {
            // Degree is 0
            return shiftedPolynomialCoefficients;
        }

        for(unsigned degreeIndex = 1; degreeIndex < static_cast<unsigned>(polynomialCoefficients.cols); degreeIndex++)
        {
            shiftedPolynomialCoefficients.at<double>(0,degreeIndex) = polynomialCoefficients.at<double>(0,degreeIndex-1);
        }
        return shiftedPolynomialCoefficients;
    }
    std::pair<cv::Mat,cv::Mat> polynomialAndRootsGenerator(unsigned int degree, double minCoefficients = 0.0, double maxCoefficients = 1.0)
    {
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(minCoefficients,maxCoefficients);
        cv::Mat polynomial(1,degree+1,CV_64F,cv::Scalar(0));
        polynomial.at<double>(0,0) = 1.0; // Starts as the constant polynomial p = 1
        cv::Mat roots(1,degree,CV_64F,cv::Scalar(0));
        for(unsigned coefficientIndex = 0u; coefficientIndex < degree; coefficientIndex++)
        {
            double currentRoot = distribution(generator);
            roots.at<double>(coefficientIndex) = currentRoot;
            cv::Mat polynomialCopy = polynomial.clone();
            cv::Mat shiftedPoly = shiftDegreeUpByOne(polynomialCopy);
            polynomial = -currentRoot*polynomialCopy + shiftedPoly;
        }
        return std::make_pair(polynomial,roots);
    }
    TEST(WillReturnValuesOfLengthOneSmallerThanTheInput)
    {
        PolynomialRootSolver solver;
        cv::Mat simpleQuadratic = cv::Mat::ones(1,3,CV_64F);
        simpleQuadratic.at<double>(0,1) += 1.0;

        cv::Mat expectedRoots =  cv::Mat::ones(1,2,CV_64F);
        cv::Mat actualRoots = solver.computeRoots(simpleQuadratic);

        CHECK_EQUAL(expectedRoots.cols,actualRoots.cols);
        CHECK_EQUAL(expectedRoots.rows,1);
    }

    TEST(WillRecoverTheCorrectRootsForASimpleQuadratic)
    {
        PolynomialRootSolver solver;
        cv::Mat simpleQuadratic = cv::Mat::ones(1,3,CV_64F);
        simpleQuadratic.at<double>(0,1) += 1.0;

        cv::Mat expectedRoots = cv::Mat(1,2,CV_64F,cv::Scalar(-1));
        cv::Mat actualRoots = solver.computeRoots(simpleQuadratic);

        veryfyThatActualAndExpectedRootsAreWithinTolerance(actualRoots,expectedRoots);
    }

    TEST(WillRecoverTheCorrectRootsForASimpleCubic)
    {
        PolynomialRootSolver solver;
        cv::Mat simpleQuadratic = cv::Mat::ones(1,4,CV_64F);
        simpleQuadratic.at<double>(0,0) = 6.0;
        simpleQuadratic.at<double>(0,1) = 1.0;
        simpleQuadratic.at<double>(0,2) = -4.0;
        simpleQuadratic.at<double>(0,3) = 1.0;

        cv::Mat expectedRoots = cv::Mat(1,3,CV_64F,cv::Scalar(-1));
        expectedRoots.at<double>(0,0) = -1.0;
        expectedRoots.at<double>(0,1) = 2.0;
        expectedRoots.at<double>(0,2) = 3.0;

        cv::Mat actualRoots = solver.computeRoots(simpleQuadratic);
        veryfyThatActualAndExpectedRootsAreWithinTolerance(actualRoots,expectedRoots);
    }

    TEST(WillRecoverCorrectRootsForArbitraryPositiveDegree)
    {
        PolynomialRootSolver solver;
        // The test case generator isn't particularly robust at generating high accuracy coefficients for high degrees
        for(unsigned currentDegreeTested = 1u; currentDegreeTested < 13u; currentDegreeTested++)
        {
            std::pair<cv::Mat,cv::Mat> polynomialCoefficientsAndRoots = polynomialAndRootsGenerator(currentDegreeTested,-1.0,1.0);
            cv::Mat actualRoots = solver.computeRoots(polynomialCoefficientsAndRoots.first);

            veryfyThatActualAndExpectedRootsAreWithinTolerance(actualRoots,polynomialCoefficientsAndRoots.second);
        }
    }
}
