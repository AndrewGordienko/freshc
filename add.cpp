#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

void drawBox() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;

    // Create the Box2D world and setup physics simulation
    b2Vec2 gravity(0.0f, 0.1f); // Weaker gravity
    b2World world(gravity);

    // Create a ground body
    b2BodyDef groundBodyDef;
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 10.0f); // Adjust the size to fit the screen width
    groundBodyDef.position.Set(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 10.0f); // Move the floor to the bottom
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create a dynamic box
    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(0.0f, 100.0f);
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
    SDL_Window* window = SDL_CreateWindow("Box2D Physics Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

        // Print the box position
        b2Vec2 boxPosition = boxBody->GetPosition();
        std::cout << "Box Position: (" << boxPosition.x << ", " << boxPosition.y << ")" << std::endl;

        // Perform physics simulation steps
        float32 timeStep = 1.0f / 60.0f; // Use a smaller time step
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        world.Step(timeStep, velocityIterations, positionIterations);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the ground (gray color)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_Rect groundRect;
        groundRect.x = static_cast<int>(groundBody->GetPosition().x - SCREEN_WIDTH / 2.0f);
        groundRect.y = static_cast<int>(groundBody->GetPosition().y - 10.0f);
        groundRect.w = SCREEN_WIDTH;
        groundRect.h = 20;
        SDL_RenderFillRect(renderer, &groundRect);

        // Draw the box (white color)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect boxRect;
        boxRect.x = static_cast<int>(boxBody->GetPosition().x - boxWidth);
        boxRect.y = static_cast<int>(boxBody->GetPosition().y - boxHeight);
        boxRect.w = static_cast<int>(boxWidth * 2);
        boxRect.h = static_cast<int>(boxHeight * 2);
        SDL_RenderFillRect(renderer, &boxRect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
