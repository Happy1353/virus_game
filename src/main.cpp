#include <memory>

#include "engine.h"

int main()
{
    std::shared_ptr<Engine> engine = std::make_shared<Engine>();
    engine->LoadResources();
    engine->CreateWindow();
    engine->InitializeLogic();
    engine->InitializeGraphics();
    while (engine->GetWindow().isOpen())
    {
        engine->UserInput();
        engine->Render();
    }
}
