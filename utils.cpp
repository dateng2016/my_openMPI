/*
Author: Da Teng
Class: ECE6122
Last Date Modified: 11/24/2024
Description: This file gives the two integral estimation function for
            the assignment.
*/

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>

using namespace std;

double estimateIntegral1(int numSamples)
{
    /**
     * @brief Estimates the integral of the function f(x) = x^2 over the
     * interval [0, 1] using Monte Carlo integration.
     *
     * This function generates `num_samples` random samples uniformly
     * distributed over the interval [0, 1], evaluates the function f(x) = x^2
     * at each sample, and computes the average of the function values. This
     * average serves as an estimate for the integral of f(x) over the interval
     * [0, 1].
     *
     * @param numSamples The number of random samples to generate for the
     * estimation. A larger number of samples will result in a more accurate
     * estimate.
     *
     * @return A double representing the estimated value of the integral of f(x)
     * = x^2 over the interval [0, 1]. The expected result is 1/3 (approximately
     * 0.3333).
     */

    double result = 0.0;
    for (int i = 0; i < numSamples; ++i)
    {
        double x = static_cast<double>(rand()) / RAND_MAX;
        result += x * x; // f(x) = x^2
    }
    return result / numSamples;
}

double estimateIntegral2(int numSamples)
{
    /**
     * @brief Estimates the integral of the function f(x) = e^(-x^2) over the
     * interval [0, 1] using Monte Carlo integration.
     *
     * This function generates `num_samples` random samples uniformly
     * distributed over the interval [0, 1], evaluates the function f(x) =
     * e^(-x^2) at each sample, and computes the average of the function values.
     * This average serves as an estimate for the integral of f(x) over the
     * interval [0, 1].
     *
     * @param numSamples The number of random samples to generate for the
     * estimation. A larger number of samples will result in a more accurate
     * estimate.
     *
     * @return A double representing the estimated value of the integral of f(x)
     * = e^(-x^2) over the interval [0, 1]. The expected result is approximately
     * 0.7468.
     */

    double result = 0.0;
    for (int i = 0; i < numSamples; ++i)
    {
        double x = static_cast<double>(rand()) / RAND_MAX;
        result += exp(-x * x); // f(x) = e^(-x^2)
    }
    return result / numSamples;
}