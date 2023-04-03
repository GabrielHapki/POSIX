#pragma once
#include "Turtle.hpp"

class Painter
{
        Turtle* turtle;
public:
        Painter( Turtle* turtle )
                :       turtle(turtle){}

        bool DrawCircle(int, int, int){
                turtle->PenDown();
                return true;
        }
};