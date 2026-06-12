void main() {
    // Declaramos un array de 5 celdas
    array datos[5];

    // Llenamos las primeras posiciones con operaciones matemáticas usando un for
    for (int i = 0; i < 4; i = i + 1) {
        datos[i] = i * 10;
    }

    // Guardamos un booleano nativo en la última celda libre (¡Heterogéneo!)
    datos[4] = true;

    // Imprimimos todo el contenido para validar
    for (int j = 0; j < 4; j = j + 1) {
        print(datos[j]);
    }
    
    // El print del índice 4 debería reconocer automáticamente el tipo booleano y escribir textualmente "true"
    print(datos[4]);
}