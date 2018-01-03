# Métodos Numéricos - TP2

## Cómo compilar

```
mkdir build && cd build
cmake ..
make
```

Esto compila todos los targets bajo la carpeta build (que está ignorada). Los targets son:

- tp2
- test/correrTests
- data/crossval

## Ejecutable principal

### Cómo ejecutar

El mismo contiene instrucciones de uso. El ejecutable toma los parámetros como argumentos de Unix.

Se recomienda descargar [los datasets de dígitos de Kaggle][1] primero.

Ejemplo de ejecución:

```
./tp2 -m 1 -i data/train.csv -o out.csv -q data/test.csv
```

## Unit tests

### Cómo ejecutar

El target `test/correrTests` contiene todos los tests. Ejecutarlo efectivamente corre todos los tests.

## Cross-validation

El target `data/crossval` realiza cross-validation estilo kFold

### Cómo ejecutar

Primero tienen que descargar 2 cosas: por un lado, [el dataset de Kaggle][1], y por el otro [los tests de la cátedra][2]. Se recomienda guardar todos los archivos en la carpeta `data`.

`crossval` toma los parámetros por stdin. El formato de estos parámetros está definido en el comprimido con los tests.

Ejemplo de ejecución:

```
cd build
cat ../data/test1.in | ./data/crossval
```


[1]: http://www-2.dc.uba.ar/materias/metnum/dnload/2017_C2/tp2/data.zip
[2]: http://www-2.dc.uba.ar/materias/metnum/dnload/2017_C2/tp2/tests.zip