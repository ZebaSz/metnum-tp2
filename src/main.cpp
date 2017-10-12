#include <iostream>
#include "matrix/Matrix.h"
#include "io/IO.h"
#include "machines/MachineFactory.h"

void print_usage() {
	std::cerr <<
			"Uso: \"./tp2 -m <method> -i <train_set> -q <test_set> -o <classif>\"\n"
			"\n"
			"donde:\n"
			"- <method>: método a ejecutar (0: kNN, 1: PCA + kNN)\n"
			"- <train_set>: archivo con datos de entrenamiento\n"
			"- <test_set>: archivo con datos de test a clasificar\n"
			"- <classif>: archivo de salida con la clasificación de"
			" los datos de test de <test_set>" << std::endl;
}

void save_results(const vector<unsigned char> &results, const char* out_fn) {
    std::ofstream out_file(out_fn, std::ofstream::out | std::ofstream::trunc);
    out_file << "ImageId,Label" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        out_file << (i+1) << ',' << +results[i] << std::endl;
    }
    out_file.close();
}

void run_machine(const parameters &p) {
    MachineRef machine = MachineFactory::create(p);

    std::cout << "Loading train set...            \r" << std::flush;
    std::ifstream train_file(p.train_file);
    TrainSet<Pixel> trainSet = IO::loadTrainSet<Pixel>(train_file);
    train_file.close();
    machine->train(trainSet);

    std::cout << "Loading test set...             \r" << std::flush;
    std::ifstream test_file(p.train_file);
    TestSet<Pixel> testSet = IO::loadTestSet<Pixel>(test_file);
    test_file.close();

    std::cout << "Guessing...                     \r" << std::flush;
    vector<unsigned char> results = machine->guess(testSet);

    std::cout << "Saving results...               \r" << std::flush;
    save_results(results, p.out_file);

    std::cout << "All done!                        " << std::endl;
}

int main(int argc, char const *argv[]) {
    const char* unknown = NULL;
	parameters p = {255, NULL, NULL, NULL};
    int res = parse(argc, argv, p, &unknown);
    switch (res) {
        case MISSING_ARG:
            std::cerr << "ERROR: parametros faltantes\n" << std::endl;
            print_usage();
            break;
        case UNKNOWN_METHOD:
            std::cerr << "ERROR: metodo no reconocido \"" << unknown << "\"\n" << std::endl;
            print_usage();
            break;
        case UNKNOWN_ARG:
            std::cerr << "ERROR: parametro no reconocido \"" << unknown << "\"\n" << std::endl;
            print_usage();
            break;
        default:
            break;
    }
    if(res != OK) {
        return res;
    }

    run_machine(p);

	return res;
}
