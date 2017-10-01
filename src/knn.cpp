#include "knn.h"

int kNN(int k, const BaseMatrix<double> &unknownNumber, row< BaseMatrix<double> > &knownNumbers) {
    // Invariante: el valor en la pos i de minNorms corresponde a la norma de ||unkownNumber - knownNumber||2
    // donde knownNumber es nn[i]
    // Además, validKnownNumbers indica si en la pos i de nn hay una matriz valida.
    row< BaseMatrix<double>* > nn(k);
    row< double > minNorms(k, std::numeric_limits< double >::max());
    row< int > validKnownNumbers(k, 0);

    for (int counter = 0; counter < knownNumbers.size(); ++counter) {

        BaseMatrix<double> *knownNumber = &knownNumbers[counter];
        double result = 0;
        for (int i = 0; i < knownNumber->width(); ++i) {
            double c = 0.0;
            for (int j = 0; j < knownNumber->height(); ++j) {
                // Super Kahan!!
                double distance = *(*knownNumber)[i][j] - *unknownNumber[i][j];
                double y = pow(distance, 2) - c;
                double t = result + y;
                c = (t - result) - y;
                result = t;
            }
        }
        double twoNorm = sqrt(result);

        for (int nnIndex = 0; nnIndex < k; ++nnIndex) {
            if (twoNorm < minNorms[nnIndex] || validKnownNumbers[nnIndex] == 0) {
                validKnownNumbers[nnIndex] = 1;
                minNorms[nnIndex] = twoNorm;
                nn[nnIndex] = knownNumber;
                nnIndex = k+1;
            }
        }

    }

    return 1;


}