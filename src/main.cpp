#include "render/window.hpp"
#include <raylib.h>

int main() {
    astranav::render::Window window(1280, 720, "astranav-2D");

    while(!window.should_close()) {
        // --- ETAPA DE ATUALIZAÇÃO (Update) ---
        // TODO: instâncias de Probe e Station (Issue #2)

        window.begin_drawing();
        window.clear_background(Color{ 10, 15, 25, 255 });
        DrawText("astranav-2D : Motor de Renderização Ativo", 400, 320, 20, LIGHTGRAY);
        DrawText("Pressione ESC para fechar.", 395, 360, 18, DARKGRAY);

        window.end_drawing();
    }

    return 0;
}