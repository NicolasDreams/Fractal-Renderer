#include <SDL2/SDL.h>
#include <iostream>

inline void ScreenToPt(int p_x, int p_y, double& p_px, double& p_py, int p_windowWidth, int p_windowHeight, double p_cameraX, double p_cameraY, double p_camZoom) {
    p_px = double(p_x - p_windowWidth / 2) / (p_camZoom  * p_windowHeight) - p_cameraX;
    p_py = double(p_y - p_windowHeight / 2) / (p_camZoom  * p_windowHeight) - p_cameraY;
}

inline void Mandelbrot(double& p_x, double& p_y, double p_cx, double p_cy) {
    double zr = p_x * p_x - p_y * p_y + p_cx;
    double zi = 2.0 * p_x * p_y + p_cy;
    p_x = zr;
    p_y = zi;
}
inline void BurningShip(double& p_x, double& p_y, double p_cx, double p_cy) {
    double zr = p_x * p_x - p_y * p_y + p_cx;
    double zi = 2.0 * std::abs(p_x * p_y) + p_cy;
    p_x = zr;
    p_y = zi;
}

inline double ModSquared(double p_x, double p_y)
{
    return p_x * p_x + p_y * p_y;
}

int ComputeIterations(double p_z0X, double p_z0Y, double p_constantX, double p_constantY, int p_fractal, int p_maxIteration)
{
    double znX = p_z0X;
    double znY = p_z0Y;
    int iteration = 0;

    while (ModSquared(znX, znY) < 4.0 && iteration < p_maxIteration)
    {
        switch (p_fractal)
        {
            case 1:
                Mandelbrot(znX, znY, p_constantX, p_constantY);
                break;
            case 2:
                BurningShip(znX, znY, p_constantX, p_constantY);
                break;
        }
        ++iteration;
    }
    return iteration;
}

void Render(int p_windowWidth, int p_windowHeight, double p_cameraX, double p_cameraY, double p_cameraZoom, SDL_Renderer* p_renderer, int p_fractal, int p_iterations, int p_colorRotationSpeed, int p_colorOffset)
{
    Uint16 color;
    Uint8 r, g, b, temp;

    for (int y = 0; y < p_windowHeight; ++y)
    {
        for (int x = 0; x < p_windowWidth; ++x)
        {
            double znX;
            double znY;
            ScreenToPt(x, y, znX, znY, p_windowWidth, p_windowHeight, p_cameraX, p_cameraY, p_cameraZoom);
            int iterations = ComputeIterations(znX, znY, znX, znY, p_fractal, p_iterations);

            if (iterations < p_iterations)
            {
                color = (iterations * p_colorRotationSpeed + p_colorOffset) % 1536;
                temp = color % 256;
                if (color < 256)
                {
                    r = 255;
                    g = temp;
                    b = 0;
                }
                else if (color < 512)
                {
                    g = 255;
                    r = 255 - temp;
                    b = 0;
                }
                else if (color < 768)
                {
                    g = 255;
                    b = temp;
                    r = 0;
                }
                else if (color < 1024)
                {
                    b = 255;
                    g = 255 - temp;
                    r = 0;
                }
                else if (color < 1280)
                {
                    b = 255;
                    r = temp;
                    g = 0;
                }
                else
                {
                    r = 255;
                    b = 255 - temp;
                    g = 0;
                }

                SDL_SetRenderDrawColor(p_renderer, r, g, b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(p_renderer, x, y);
            }
        }
    }
}

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << "Failed to init SDL. ERROR: " << SDL_GetError() << "\n";

    SDL_Window* window = SDL_CreateWindow("Fractal Renderer v1.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        std::cout << "Failed to create SDL_Window. ERROR: " << SDL_GetError() << "\n";
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        std::cout << "Failed to create SDL_Renderer. ERROR: " << SDL_GetError() << "\n";

    int mouseX = 0;
    int mouseY = 0;

    int windowWidth = 1280;
    int windowHeight = 720;
    double cameraX = 0.5;
    double cameraY = 0.0;
    double cameraZoom = 0.5;
    int fractal = 1;
    int iterations = 128;
    int colorRotationSpeed = 5;
    int colorOffset = 1024;

    bool gameRunning = true;
    SDL_Event event;
    bool updateFractal = true;
    bool fullscreen = false;

    while (gameRunning)
    {
        SDL_GetWindowSize(window, &windowWidth, &windowHeight); // std::cout << windowWidth << ", " << windowHeight << "\n";

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    // std::cout << mouseX << ", " << mouseY << "\n";
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    updateFractal = true;
                    switch (event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            // std::cout << "Zoom in!\n";
                            ScreenToPt(mouseX, mouseY, cameraX, cameraY, windowWidth, windowHeight, cameraX, cameraY, cameraZoom);
                            cameraX = -cameraX;
                            cameraY = -cameraY;
                            cameraZoom *= 2;
                            break;
                        case SDL_BUTTON_RIGHT:
                            // std::cout << "Zoom out!\n";
                            ScreenToPt(windowWidth - mouseX, windowHeight - mouseY, cameraX, cameraY, windowWidth, windowHeight, cameraX, cameraY, cameraZoom);
                            cameraX = -cameraX;
                            cameraY = -cameraY;
                            cameraZoom /= 2;
                            break;
                        default:
                            updateFractal = false;
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                        updateFractal = true;
                    break;
                case SDL_KEYDOWN:
                    updateFractal = true;
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_F11:
                            fullscreen = !fullscreen;
                            SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                            break;
                        case SDLK_m:
                            iterations *= 2;
                            std::cout << "Iterations: " << iterations << "\n";
                            break;
                        case SDLK_n:
                            iterations /= 2;
                            if (iterations < 1)
                            {
                                iterations = 1;
                                updateFractal = false;
                            }
                            std::cout << "Iterations: " << iterations << "\n";
                            break;
                        case SDLK_x:
                            colorRotationSpeed += 2;
                            std::cout << "Color Rotation Speed: " << colorRotationSpeed << "\n";
                            break;
                        case SDLK_z:
                            colorRotationSpeed -= 2;
                            if (colorRotationSpeed < 1)
                            {
                                colorRotationSpeed = 1;
                                updateFractal = false;
                            }
                            std::cout << "Color Rotation Speed: " << colorRotationSpeed << "\n";
                            break;
                        case SDLK_v:
                            colorOffset += 128;
                            if (colorOffset >= 1536)
                                colorOffset -= 1536;
                            std::cout << "Color Offset: " << colorOffset << "\n";
                            break;
                        case SDLK_c:
                            colorOffset -= 128;
                            if (colorOffset < 0)
                                colorOffset += 1536;
                            std::cout << "Color Offset: " << colorOffset << "\n";
                            break;
                        case SDLK_r:
                            cameraX = 0.5;
                            cameraY = 0.0;
                            cameraZoom = 0.5;
                            iterations = 128;
                            colorRotationSpeed = 5;
                            colorOffset = 1024;
                            break;
                        case SDLK_1:
                            fractal = 1;
                            std::cout << "Mandelbrot\n";
                            break;
                        case SDLK_2:
                            fractal = 2;
                            std::cout << "Burning Ship\n";
                            break;
                        case SDLK_ESCAPE:
                            fullscreen ? fullscreen = false : gameRunning = false;
                            SDL_SetWindowFullscreen(window, 0);
                            break;
                        case SDLK_F5:
                            break;
                        default:
                            updateFractal = false;
                            break;
                    }
                    break;
            }
        }

        if (updateFractal && gameRunning)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            Render(windowWidth, windowHeight, cameraX, cameraY, cameraZoom, renderer, fractal, iterations, colorRotationSpeed, colorOffset);
            updateFractal = false;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}