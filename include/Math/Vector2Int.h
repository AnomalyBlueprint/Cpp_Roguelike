#pragma once
#include <cmath> // For sqrt

struct Vector2Int
{
    int x, y;

    Vector2Int() : x(0.0f), y(0.0f) {}
    Vector2Int(float x, float y) : x(x), y(y) {}

    // Addition (pos + velocity)
    Vector2Int operator+(const Vector2Int &other) const
    {
        return Vector2Int(x + other.x, y + other.y);
    }

    // Subtraction (direction = target - pos)
    Vector2Int operator-(const Vector2Int &other) const
    {
        return Vector2Int(x - other.x, y - other.y);
    }

    // Multiplication (speed * 2)
    Vector2Int operator*(float scalar) const
    {
        return Vector2Int(x * scalar, y * scalar);
    }

    // Magnitude (Length of the vector)
    float Magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    // Sign (Get the sign of each component)
    Vector2Int Sign() const
    {
        return Vector2Int(x < 0 ? -1 : 1, y < 0 ? -1 : 1);
    }
};