#include "mocolor.h"


MoColor::MoColor()
{
}

MoColor::MoColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1;
}

MoColor::MoColor(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void MoColor::SetColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1;
}

void MoColor::SetColor(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

float MoColor::GetR() const
{
    return this->r;
}

float MoColor::GetG() const
{
    return this->g;
}

float MoColor::GetB() const
{
    return this->b;
}

float MoColor::GetA() const
{
    return this->a;
}

//
//Operators
MoColor& MoColor::operator=(const MoColor &other)
{
    float r = other.GetR();
    float g = other.GetG();
    float b = other.GetB();
    float a = other.GetA();
    this->SetColor(r,g,b,a);
    return *this;
}
