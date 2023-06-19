#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <random>
#include <cmath>

class Floor {
public:
    Floor(b2World& world) {
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, 400.0f);
        groundBody = world.CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(650.0f, 10.0f);
        
        b2FixtureDef groundFixtureDef;
        groundFixtureDef.shape = &groundBox;
        groundBody->CreateFixture(&groundFixtureDef);
    }

    b2Body* GetGroundBody() {
        return groundBody;
    }

private:
    b2Body* groundBody;
};

class Shape {
public:
    Shape(b2World& world) {
        const float shapeX = 100.0f; // X position relative to the center of the screen
        const float shapeY = 100.0f; // Y position relative to the center of the screen

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(shapeX, shapeY);
        box = world.CreateBody(&bodyDef);

        int numSides = std::rand() % 5 + 4; // Random number of sides between 4 and 8
        float distance = 30.0f * std::sin(b2_pi / numSides);
        vertices.reserve(numSides);
        for (int i = 0; i < numSides; i++) {
            float x = distance * std::cos(2 * b2_pi / numSides * i) + getRandom(-10.0f, 10.0f);
            float y = distance * std::sin(2 * b2_pi / numSides * i) + getRandom(-10.0f, 10.0f);
            vertices.emplace_back(x, y);
        }

        b2PolygonShape boxShape;
        boxShape.Set(vertices.data(), numSides);
        boxFixture.shape = &boxShape;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.3f;
        box->CreateFixture(&boxFixture);
    }

    const b2Vec2* GetVertices() const {
        return vertices.data();
    }

    int GetNumVertices() const {
        return vertices.size();
    }

private:
    b2Body* box;
    b2FixtureDef boxFixture;
    std::vector<b2Vec2> vertices;

    float getRandom(float min, float max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};



void drawBox() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;

    // Create the Box2D world and setup physics simulation
    b2Vec2 gravity(0.0f, 0.1f); // Weaker gravity
    b2World world(gravity);

    // Create the Floor object
    Floor floor(world);

    // Create the Shape object
    Shape shape(world);

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
        groundRect.x = static_cast<int>(floor.GetGroundBody()->GetPosition().x - SCREEN_WIDTH / 2.0f);
        groundRect.y = static_cast<int>(floor.GetGroundBody()->GetPosition().y - 10.0f);
        groundRect.w = SCREEN_WIDTH;
        groundRect.h = 20;
        SDL_RenderFillRect(renderer, &groundRect);

        // Draw the shape (white color)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        const b2Vec2* vertices = shape.GetVertices();
        int numVertices = shape.GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            int nextIndex = (i + 1) % numVertices;
            SDL_RenderDrawLine(renderer,
                static_cast<int>(vertices[i].x + SCREEN_WIDTH / 2.0f),
                static_cast<int>(vertices[i].y + SCREEN_HEIGHT / 2.0f),
                static_cast<int>(vertices[nextIndex].x + SCREEN_WIDTH / 2.0f),
                static_cast<int>(vertices[nextIndex].y + SCREEN_HEIGHT / 2.0f));
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

