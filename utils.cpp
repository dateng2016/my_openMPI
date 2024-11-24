#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>

using namespace std;

double estimate_integral_1(int num_samples)
{
    double result = 0.0;
    for (int i = 0; i < num_samples; ++i)
    {
        double x = static_cast<double>(rand()) / RAND_MAX;
        result += x * x; // f(x) = x^2
    }
    return result / num_samples;
}

double estimate_integral_2(int num_samples)
{
    double result = 0.0;
    for (int i = 0; i < num_samples; ++i)
    {
        double x = static_cast<double>(rand()) / RAND_MAX;
        result += exp(-x);
    }
    return result / num_samples;
}