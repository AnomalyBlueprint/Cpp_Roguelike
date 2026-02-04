#pragma once
#include <cmath> // For sqrt

struct Vector2
{
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Addition (pos + velocity)
    Vector2 operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    // Subtraction (direction = target - pos)
    Vector2 operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    // Multiplication (speed * 2)
    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    // Magnitude (Length of the vector)
    float Magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    // Normalize (Make length 1, keep direction)
    Vector2 Normalized() const
    {
        float mag = Magnitude();
        if (mag > 0)
            return Vector2(x / mag, y / mag);
        return Vector2(0, 0);
    }
};