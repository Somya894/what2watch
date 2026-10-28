#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([] {
        return "Crow server is working!";
    });

    app.port(18080).multithreaded().run();
}