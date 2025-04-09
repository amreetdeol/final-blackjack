#pragma once
#include "request.hpp"
#include "response.hpp"
#include "game_manager.hpp"

class Router {
public:
    static Response route(const Request& request);
};