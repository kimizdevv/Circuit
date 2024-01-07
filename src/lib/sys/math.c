#include "math.h"

#define LN10 2.3025850929940456840179914546844

double dmod(double x, double y)
{
        return x - (int)(x / y) * y;
}

// https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having
double ln(const double x)
{
        double old_sum = 0.0;
        const double xmlxpl = (x - 1) / (x + 1);
        const double xmlxpl_2 = xmlxpl * xmlxpl;
        double denom = 1.0;
        double frac = xmlxpl;
        double term = frac; // denom start from 1.0
        double sum = term;

        while (sum != old_sum) {
                old_sum = sum;
                denom += 2.0;
                frac *= xmlxpl_2;
                sum += frac / denom;
        }
        return 2.0 * sum;
}

double log10(double x)
{
        return ln(x) / LN10;
}
