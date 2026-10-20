// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
public:
    BasicTransform(sf::Vector2f s, sf::Vector2f t): scale(s), translation(t) {}

    // Scale, then translate and return
    sf::Vector2f applyPoint(const sf::Vector2f& p) {
        sf::Vector2f out =  sf::Vector2f(
            this->scale.x * p.x + this->translation.x,
            this->scale.y * p.y + this->translation.y
        );
    }

    // Scale and return (vector does not care about origin)
    sf::Vector2f applyVector(const sf::Vector2f& v) {
        sf::Vector2f out =  sf::Vector2f(
            this->scale.x * v.x,
            this->scale.y * v.y
        );
    }

    // TODO: Verify that no other cases are true
    bool operator==(const BasicTransform& other) {
        if( other.scale != this->scale ) return false;
        if( other.translation != this->translation ) return false;
        return true;
    }
    sf::Vector2f scale;
    sf::Vector2f translation;
};

struct BasicRotation {
public:
    BasicRotation(float t): theta(t) {}

    // Rotate, then return
    sf::Vector2f rotate(const sf::Vector2f& p) {
        sf::Vector2f out =  sf::Vector2f(
            p.x * cos(theta) - p.x * sin(theta),
            p.x * sin(theta) + p.x * cos(theta)
        );
    }
    float theta;
};