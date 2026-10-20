#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture): mTexture(&texture), mTransform({1.f, 1.f}, {0.f, 0.f}) {
        mOrigin = sf::Vector2f(0.f,0.f);
        mMinPointTex = sf::Vector2f(0.f,0.f);
        mMaxPointTex = static_cast<sf::Vector2f>(mTexture->getSize());

        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
        const float width = mMaxPointTex.x - mMinPointTex.x;
        const float height = mMaxPointTex.y - mMinPointTex.y;
        mVertexArray[0] = {{0.f,0.f},       sf::Color::White, mMinPointTex};                    //(0,0)   
        mVertexArray[1] = {{0.f,height},    sf::Color::White, {mMinPointTex.x,mMaxPointTex.y}}; //(0,1)
        mVertexArray[2] = {{width,0.f},     sf::Color::White, {mMaxPointTex.x,mMinPointTex.y}}; //(1,0)
        mVertexArray[3] = {{width,0.f},     sf::Color::White, {mMaxPointTex.x,mMinPointTex.y}}; //(1,0)
        mVertexArray[4] = {{0.f,height},    sf::Color::White, {mMinPointTex.x,mMaxPointTex.y}}; //(0,1)
        mVertexArray[5] = {{width,height},  sf::Color::White, mMaxPointTex};                    //(1,1)
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint): mTexture(&texture), mTransform({1.f, 1.f}, {0.f, 0.f}),
           mMinPointTex(texMinPoint), mMaxPointTex(texMaxPoint) {
        mOrigin = sf::Vector2f(0.f,0.f);

        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
        const float width = mMaxPointTex.x - mMinPointTex.x;
        const float height = mMaxPointTex.y - mMinPointTex.y;
        mVertexArray[0] = {{0.f,0.f},       sf::Color::White, mMinPointTex};                    //(0,0)   
        mVertexArray[1] = {{0.f,height},    sf::Color::White, {mMinPointTex.x,mMaxPointTex.y}}; //(0,1)
        mVertexArray[2] = {{width,0.f},     sf::Color::White, {mMaxPointTex.x,mMinPointTex.y}}; //(1,0)
        mVertexArray[3] = {{width,0.f},     sf::Color::White, {mMaxPointTex.x,mMinPointTex.y}}; //(1,0)
        mVertexArray[4] = {{0.f,height},    sf::Color::White, {mMinPointTex.x,mMaxPointTex.y}}; //(0,1)
        mVertexArray[5] = {{width,height},  sf::Color::White, mMaxPointTex};                    //(1,1)
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Bind texture
        states.texture = mTexture;
        target.draw(recomputeVertexArray(), states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {};  // TODO

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    sf::VertexArray recomputeVertexArray() const {
        BasicTransform transform = mTransform;
        sf::VertexArray output(sf::PrimitiveType::Triangles, 0);

        for(auto it = mVertexArray.begin(); it != mVertexArray.end(); it++) {
            output.append({transform.applyPoint(it->position - mOrigin), it->color, it->texCoords});
        }

        return output;
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
