#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <cmath>
#include <common/utils/print.hpp>
#include <common/utils/str.hpp>

bool isRunning = true;

// サーキット形状クラス（長方形＋上下半円）
class OvalTrackShape : public sf::Drawable {
public:
    OvalTrackShape(float width, float height, sf::Color color) {
        float radius = width / 2.f;

        // 長方形部分（縦棒）
        rect.setSize({width, height - 2 * radius});
        rect.setFillColor(color);
        rect.setOrigin(width / 2.f, (height - 2 * radius) / 2.f);

        // 上の半円
        top.setRadius(radius);
        top.setPointCount(50);
        top.setFillColor(color);
        top.setOrigin(radius, radius);
        top.setPosition(0, -(height / 2.f - radius));

        // 下の半円
        bottom.setRadius(radius);
        bottom.setPointCount(50);
        bottom.setFillColor(color);
        bottom.setOrigin(radius, radius);
        bottom.setPosition(0, (height / 2.f - radius));

        group.setPosition(0, 0); // 初期位置
    }

    void setPosition(sf::Vector2f pos) { group.setPosition(pos); }
    void setScale(float sx, float sy) { group.setScale(sx, sy); }
    sf::Vector2f getPosition() const { return group.getPosition(); }

private:
    sf::RectangleShape rect;
    sf::CircleShape top, bottom;
    sf::Transformable group;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= group.getTransform();
        target.draw(rect, states);
        target.draw(top, states);
        target.draw(bottom, states);
    }
};

using fScalar = float;
using fVector = struct {
    fScalar x, y;
};
fScalar toRad(fScalar deg) {
    return deg * 3.14159265f / 180.f;
}

class EyeObj {
public:
    EyeObj(OvalTrackShape &oval, fVector position, fScalar velocity, fScalar angle): oval(oval), position(position) {
        setVelocity(velocity, angle);
    }

    void setVelocity(fScalar velocity, fScalar deg) {
        this->velocity.x = velocity * std::cos(toRad(deg));
        this->velocity.y = velocity * std::sin(toRad(deg));
        this->angle = deg;
    }

    void update(float dt) {
        fVector deltaPos = {velocity.x * dt, velocity.y * dt};
        position.x += deltaPos.x;
        position.y += deltaPos.y;
        oval.setPosition({position.x, position.y});
    }

    fVector getPosition() {
        return position;
    }

    OvalTrackShape &getOval() {
        return oval;
    }

private:
    OvalTrackShape &oval;
    fVector position;
    fVector velocity;
    fScalar angle;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "OvalTrack with Wobble");
    sf::VertexArray grid(sf::Lines);
    grid.append(sf::Vertex(sf::Vector2f(10, 10), sf::Color::Red));
    grid.append(sf::Vertex(sf::Vector2f(60, 10), sf::Color::Red));
    grid.append(sf::Vertex(sf::Vector2f(60, 10), sf::Color::Red));
    grid.append(sf::Vertex(sf::Vector2f(55, 5), sf::Color::Red));
    grid.append(sf::Vertex(sf::Vector2f(60, 10), sf::Color::Red));
    grid.append(sf::Vertex(sf::Vector2f(55, 15), sf::Color::Red));

    grid.append(sf::Vertex(sf::Vector2f(10, 10), sf::Color::Green));
    grid.append(sf::Vertex(sf::Vector2f(10, 60), sf::Color::Green));
    grid.append(sf::Vertex(sf::Vector2f(10, 60), sf::Color::Green));
    grid.append(sf::Vertex(sf::Vector2f(15, 55), sf::Color::Green));
    grid.append(sf::Vertex(sf::Vector2f(10, 60), sf::Color::Green));
    grid.append(sf::Vertex(sf::Vector2f(5 , 55), sf::Color::Green));


    OvalTrackShape oval(60.f, 100.f, sf::Color::Cyan);
    OvalTrackShape rightEye(60.f, 100.f, sf::Color::Cyan);
    EyeObj rightEyeObj(rightEye, {300.0,100.f}, 0.f, 0.f);
    rightEyeObj.setVelocity(0.f, 90.f);

    std::thread th([&rightEyeObj](){
        while(isRunning) {
            rightEyeObj.update(0.01);

            print("rightEye pos: ", rightEyeObj.getPosition().x, ", ", rightEyeObj.getPosition().y);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    float velocityY = 0.f;
    const float gravity = 980.f;
    const float bounce = -1.0f;
    const float floorY = 380.f;

    sf::Clock clock;

    // ボヨヨン効果用
    bool wobbling = false;
    float wobbleTime = 0.f;
    const float wobbleDuration = 1.2f;
    const float wobbleFreq = 12.f;
    const float wobbleFreqY = 8.f;
    const float wobbleDecay = 3.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                isRunning = false;
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        // 重力運動
        velocityY += gravity * dt;
        sf::Vector2f pos = oval.getPosition();
        pos.y += velocityY * dt;

        if (pos.y >= floorY) {
            pos.y = floorY;
            velocityY *= bounce;

            wobbling = true;
            wobbleTime = 0.f;
        }

        oval.setPosition(pos);

        // ボヨヨン処理
        if (wobbling) {
            wobbleTime += dt;
            if (wobbleTime > wobbleDuration) {
                wobbling = false;
                oval.setScale(1.f, 1.f);
            } else {
                float decay = std::exp(-wobbleDecay * wobbleTime);
                float oscX = std::sin(wobbleFreq * wobbleTime);
                float oscY = std::cos(wobbleFreqY * wobbleTime);

                float scaleX = 1.f + 0.5f * decay * oscX;
                float scaleY = 1.f - 0.4f * decay * oscY;
                oval.setScale(scaleX, scaleY);
            }
        } else {
            oval.setScale(1.f, 1.f);
        }

        window.clear(sf::Color::Black);
        // window.draw(oval);
        window.draw(grid);
        window.draw(rightEyeObj.getOval());

        window.display();
    }

    th.join();
    return 0;
}
