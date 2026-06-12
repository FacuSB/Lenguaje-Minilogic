void main() {
    bool condicion1;
    bool condicion2;
    bool resultado;

    condicion1 = true;
    condicion2 = false;

    // Probar operador 'and' y 'not'
    resultado = condicion1 and not condicion2;
    print(resultado); // Debería imprimir: true

    // Probar operador 'or' con evaluación directa en el If
    if (condicion2 or false) {
        print(1111);
    } else {
        print(resultado); // Debería volver a imprimir: true
    }
}