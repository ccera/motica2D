//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
//
//  This file is part of Motica2D.
//
//  Motica2D is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Motica2D is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Motica2D.  If not, see <http://www.gnu.org/licenses/>.
//

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
    this->SetColor(other.GetR(), other.GetG(), other.GetB(), other.GetA());
    return *this;
}
