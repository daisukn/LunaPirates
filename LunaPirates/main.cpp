#include "Application.h"
// エントリポイント
int main(int, char**)
{
    class Application* app = new Application();
    
    if (app->Initialize())
    {
        app->RunLoop();
    }
    app->Shutdown();
    delete app;
    return 0;
}



