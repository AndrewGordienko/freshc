#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

void drawBox() {
    // Create the Box2D world and setup physics simulation
    b2Vec2 gravity(0.0f, -50.0f); // Stronger gravity
    b2World world(gravity);

    // Create a ground body
    b2BodyDef groundBodyDef;
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create a dynamic box
    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(0.0f, 10.0f);
    b2Body* boxBody = world.CreateBody(&boxBodyDef);

    b2PolygonShape boxShape;
    float boxWidth = 10.0f;
    float boxHeight = 10.0f;
    boxShape.SetAsBox(boxWidth, boxHeight);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1.0f;
    boxFixtureDef.friction = 0.3f;
    boxBody->CreateFixture(&boxFixtureDef);

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("Box2D Physics Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);

    // Create SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Main rendering loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Perform physics simulation steps
        world.Step(1.0f / 60.0f, 6, 2);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the box (white color)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect;
        rect.x = static_cast<int>(boxBody->GetPosition().x - boxWidth);
        rect.y = static_cast<int>(boxBody->GetPosition().y - boxHeight);
        rect.w = static_cast<int>(boxWidth * 2);
        rect.h = static_cast<int>(boxHeight * 2);
        SDL_RenderFillRect(renderer, &rect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}