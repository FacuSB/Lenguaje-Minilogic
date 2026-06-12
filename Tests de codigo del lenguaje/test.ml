Void Main() {
    // Declaración de variables
    int x_tablero;
    int y_tablero;
    int tam;
    int pieza_x;
    int pieza_y;
    int contador_frames;
    int velocidad_caida;
    int i;
    int j;

    // Configuración inicial
    x_tablero = 300;
    y_tablero = 100;
    tam = 20;
    pieza_x = 4;
    pieza_y = 0;
    contador_frames = 0;
    velocidad_caida = 100; // Aumentar este número hará que caiga más lento

    Open_window(800, 600);

    // Dibujar marco estático UNA VEZ (evita parpadeo)
    // Líneas verticales
    i = 0;
    while (i <= 20) {
        Draw(x_tablero - 5, y_tablero + (i * tam), #00A2E8);
        Draw(x_tablero + 200, y_tablero + (i * tam), #00A2E8);
        i = i + 1;
    }
    // Línea inferior
    i = 0;
    while (i <= 10) {
        Draw(x_tablero + (i * tam), y_tablero + 400, #00A2E8);
        i = i + 1;
    }

    // Bucle principal
    while (1 == 1) {
        contador_frames = contador_frames + 1;

        // Lógica de movimiento: solo se mueve cuando contador_frames llega al límite
        if (contador_frames >= velocidad_caida) {
            
            // Borrar posición anterior (pintar de negro)
            Draw(x_tablero + (pieza_x * tam), y_tablero + (pieza_y * tam), #000000);
            
            // Actualizar posición
            pieza_y = pieza_y + 1;
            if (pieza_y >= 20) {
                pieza_y = 0;
            }
            
            // Dibujar nueva posición
            Draw(x_tablero + (pieza_x * tam), y_tablero + (pieza_y * tam), #FF0000);
            
            contador_frames = 0; // Reiniciar contador para el siguiente paso
        }
    }
}