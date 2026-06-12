void main() {
    string jugador1;
    string jugador2;

    jugador1 = "Puntos Jugador 1:";
    jugador2 = "Puntos Jugador 2:";
    
    int left_x;
    int left_y;
    int left_y_prev;

    int right_x;
    int right_y;
    int right_y_prev;

    int ball_x;
    int ball_y;
    int ball_x_prev;
    int ball_y_prev;

    int ball_dx;
    int ball_dy;

    int score_left;
    int score_right;

    int tick;
    int frame_delay;

    // Configuración inicial de posiciones
    left_x = 20;
    left_y = 260;
    left_y_prev = 260;

    right_x = 760;
    right_y = 260;
    right_y_prev = 260;

    ball_x = 400;
    ball_y = 300;
    ball_x_prev = 400;
    ball_y_prev = 300;

    // Velocidad inicial de la bola (20 pixeles por frame para alineación perfecta)
    ball_dx = 20;
    ball_dy = -20;

    score_left = 0;
    score_right = 0;

    tick = 0;
    frame_delay = 150000; // Ajusta este valor para aumentar o disminuir la velocidad del juego

    Open_window(800, 600);

    // Dibujar paletas iniciales
    Draw(left_x, left_y, #FFFFFF);
    Draw(left_x, left_y + 20, #FFFFFF);
    Draw(left_x, left_y + 40, #FFFFFF);
    Draw(left_x, left_y + 60, #FFFFFF);

    Draw(right_x, right_y, #FFFFFF);
    Draw(right_x, right_y + 20, #FFFFFF);
    Draw(right_x, right_y + 40, #FFFFFF);
    Draw(right_x, right_y + 60, #FFFFFF);

    Draw(ball_x, ball_y, #FF0000);

    // Bucle principal del juego
    while (1 == 1) {
        tick = tick + 1;

        if (tick >= frame_delay) {
            // Guardar posiciones anteriores
            left_y_prev = left_y;
            right_y_prev = right_y;
            ball_x_prev = ball_x;
            ball_y_prev = ball_y;

            // --- Movimiento del Jugador 1 (Paleta Izquierda) ---
            // 'w' = Código de tecla 87, 's' = Código de tecla 83
            if (IsKeyPressed(87)) {
                if (left_y > 0) {
                    left_y = left_y - 20;
                }
            }
            if (IsKeyPressed(83)) {
                if (left_y < 520) {
                    left_y = left_y + 20;
                }
            }

            // --- Movimiento del Jugador 2 (Paleta Derecha) ---
            // Flecha Arriba = Código de tecla 38, Flecha Abajo = Código de tecla 40
            if (IsKeyPressed(38)) {
                if (right_y > 0) {
                    right_y = right_y - 20;
                }
            }
            if (IsKeyPressed(40)) {
                if (right_y < 520) {
                    right_y = right_y + 20;
                }
            }

            // --- Movimiento y Lógica de la Bola ---
            ball_x = ball_x + ball_dx;
            ball_y = ball_y + ball_dy;

            // Colisión con bordes superior e inferior
            if (ball_y <= 0) {
                ball_y = 0;
                ball_dy = 20;
            }
            if (ball_y >= 580) {
                ball_y = 580;
                ball_dy = -20;
            }

            // Colisión con Paleta Izquierda (en la columna x = 40)
            if (ball_x == 40) {
                if (ball_dx < 0) {
                    if (ball_y >= left_y) {
                        if (ball_y <= left_y + 60) {
                            ball_dx = 20;
                        }
                    }
                }
            }

            // Colisión con Paleta Derecha (en la columna x = 740)
            if (ball_x == 740) {
                if (ball_dx > 0) {
                    if (ball_y >= right_y) {
                        if (ball_y <= right_y + 60) {
                            ball_dx = -20;
                        }
                    }
                }
            }

            // Punto para Jugador Derecho (si la bola supera la paleta izquierda)
            if (ball_x < 20) {
                score_right = score_right + 1;
                print(jugador2);
                print(score_right); // Imprime el marcador del Jugador Derecho en consola
                ball_x = 400;
                ball_y = 300;
                ball_dx = 20;
            }

            // Punto para Jugador Izquierdo (si la bola supera la paleta derecha)
            if (ball_x > 760) {
                score_left = score_left + 1;
                print(jugador1);
                print(score_left); // Imprime el marcador del Jugador Izquierdo en consola
                ball_x = 400;
                ball_y = 300;
                ball_dx = -20;
            }

            // --- RENDERIZADO GRÁFICO (Doble Buffer en interprete.c evita el parpadeo) ---
            // 1. Borrar dibujos viejos (sobrepintando en negro)
            Draw(left_x, left_y_prev, #000000);
            Draw(left_x, left_y_prev + 20, #000000);
            Draw(left_x, left_y_prev + 40, #000000);
            Draw(left_x, left_y_prev + 60, #000000);

            Draw(right_x, right_y_prev, #000000);
            Draw(right_x, right_y_prev + 20, #000000);
            Draw(right_x, right_y_prev + 40, #000000);
            Draw(right_x, right_y_prev + 60, #000000);

            Draw(ball_x_prev, ball_y_prev, #000000);

            // 2. Dibujar nuevas posiciones (paletas blancas y bola roja)
            Draw(left_x, left_y, #FFFFFF);
            Draw(left_x, left_y + 20, #FFFFFF);
            Draw(left_x, left_y + 40, #FFFFFF);
            Draw(left_x, left_y + 60, #FFFFFF);

            Draw(right_x, right_y, #FFFFFF);
            Draw(right_x, right_y + 20, #FFFFFF);
            Draw(right_x, right_y + 40, #FFFFFF);
            Draw(right_x, right_y + 60, #FFFFFF);

            Draw(ball_x, ball_y, #FF0000);

            tick = 0; // Reiniciar contador de frames
        }
    }
}
