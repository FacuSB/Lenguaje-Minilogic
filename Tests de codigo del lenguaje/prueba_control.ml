Void Main() {
    int contador;
    int limite;

    contador = 1;
    limite = 5;

    // 1. Probar el bucle While (Debería imprimir del 1 al 4)
    while (contador < limite) {
        print(contador);
        contador = contador + 1;
    }

    // 2. Probar la estructura If-Else (Debería entrar al Else porque contador ahora vale 5)
    if (contador < limite) {
        print(1111); 
    } else {
        print(9999); 
    }
}