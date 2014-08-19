#ifndef MOCOLOR_H
#define MOCOLOR_H

class MoColor
{
public:
    MoColor();
    MoColor(float r, float g, float b);
    MoColor(float r, float g, float b, float a);

    void SetColor(float r, float g, float b);
    void SetColor(float r, float g, float b, float a);
    float GetR() const;
    float GetG() const;
    float GetB() const;
    float GetA() const;

    //Operators
    MoColor& operator=(const MoColor &other);
private:
    float r;
    float g;
    float b;
    float a;
};

#endif // MOCOLOR_H
