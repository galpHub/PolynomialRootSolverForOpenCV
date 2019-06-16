#pragma once
#include "PolynomialRootSolver.h"
#include <opencv2/core/core.hpp>
#include <Eigenvalues>
#include <opencv2/core/eigen.hpp>


PolynomialRootSolver::PolynomialRootSolver()
{

}

PolynomialRootSolver::~PolynomialRootSolver()
{

}

Eigen::MatrixXd computeCompanionMatrix(const cv::Mat rowOfCoefficientsInAscendingDegreeOrder)
{
    int numberOfColumns  = rowOfCoefficientsInAscendingDegreeOrder.size().width  -1 ;
    Eigen::MatrixXd eigenCoefficients(1,numberOfColumns+1);
    Eigen::MatrixXd companionMatrix(numberOfColumns,numberOfColumns);
    companionMatrix.setZero();
    Eigen::MatrixXd identitySubmatrix = Eigen::MatrixXd::Identity(numberOfColumns-1,numberOfColumns-1);
    cv::cv2eigen(rowOfCoefficientsInAscendingDegreeOrder, eigenCoefficients);
    eigenCoefficients /= eigenCoefficients(numberOfColumns);

    companionMatrix.block(0,1,numberOfColumns-1,numberOfColumns-1) = identitySubmatrix;
    companionMatrix.block(numberOfColumns-1,0,1,numberOfColumns) -= eigenCoefficients.block(0,0,1,numberOfColumns);
    return companionMatrix;
}

cv::Mat PolynomialRootSolver::computeRoots(const cv::Mat& rowOfCoefficientsInAscendingDegreeOrder)
{
    Eigen::MatrixXd eigenCompanionMatrix = computeCompanionMatrix(rowOfCoefficientsInAscendingDegreeOrder);
    Eigen::EigenSolver<Eigen::MatrixXd> eigenSolver(eigenCompanionMatrix);
    Eigen::MatrixXd realEigenvalues = eigenSolver.eigenvalues().real();
    realEigenvalues.transposeInPlace();
    cv::Mat roots;
    cv::eigen2cv(realEigenvalues, roots);
    return roots;
}
