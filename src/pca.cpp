#include <limits>
#include "power.h"
#include "benchmark/benchmark.h"
#include "pca.h"

#define BENCH_FILE_EIGENVECTORS "bench-eigenvectors.csv"

vector<double> get_mean(const Matrix<double> &X) {
    vector<double> mean;
    for (size_t j = 0; j < X.width(); j++) {
        double sum = 0;
        for (size_t i = 0; i < X.height(); i++)
            sum += X[i][j];
        mean.push_back(sum / X.height());
    }
    return mean;
}

MatrixRef<double> substract_mean(const Matrix<double>& X, vector<double> &mean) {
    MatrixRef<double> result = X.makeNew();
    for (size_t i = 0; i < X.height(); i++) {
        for (size_t j = 0; j < X.width(); j++) {
            (*result)[i][j] = X[i][j] - mean[j];
        }
    }
    return result;
}

vector<vector<double>> get_eigenvectors(ConstMatrixRef<double> M_x, unsigned int alfa) {
    vector<vector<double>> eigenvectors;
    INIT_BENCH(BENCH_FILE_EIGENVECTORS);
    for (unsigned int i = 0; i < alfa; i++) {
        vector<double> eigenvector;
        START_BENCH;
        double eigenvalue = power_method(M_x, eigenvector);
        END_BENCH(BENCH_FILE_EIGENVECTORS);
        eigenvectors.push_back(eigenvector);
        M_x = deflate(M_x, eigenvector, eigenvalue);
    }
    return eigenvectors;
}

/*
 * 1 <= alfa <= filas(X)
 */
MatrixRef<double> pca(const ConstMatrixRef<double> &X, unsigned int alfa) {
    // Calcular X - mean
    vector<double> mean = get_mean(*X);
    MatrixRef<double> X_sub_mean = substract_mean(*X, mean);

    // Calcular M_x
    X_sub_mean->inplaceTranspose();
    MatrixRef<double> M_x = X_sub_mean->transposedProduct()->dotProduct(1.0/ (X_sub_mean->width() - 1));

    // Calcular autovalores y autovectores
    vector<vector<double>> eigenvectors = get_eigenvectors(M_x, std::min(alfa, (unsigned int) X_sub_mean->width()));

    // Construir V ortogonal con los autovectores de M_x dispuestos como
    // columnas. Alfa es la cantidad de autovectores que se toman; se
    // descartan el resto de los autovectores, tomando de esta manera
    // solo los principales.
    // Como solo usamos V^t, la creamos con los autovectores como filas
    return FullMatrix<double>::create(eigenvectors);
}
