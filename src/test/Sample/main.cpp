#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>

bool isRunning = true;

class Eye : public sf::Drawable {
public:
    Eye(float width, float height, sf::Color color, std::string shape) {
        float radius = width / 2.f;
        rect.setSize(sf::Vector2f(width, height - 2 * radius));
        rect.setFillColor(color);
        rect.setOrigin(width / 2, (height - 2 * radius) / 2);

        top.setRadius(radius);
        top.setFillColor(color);
        top.setOrigin(radius, radius);

        bottom.setRadius(radius);
        bottom.setFillColor(color);
        bottom.setOrigin(radius, radius);

        shapeType = shape;
    }

    Eye(float length, sf::Color color, std::string shape) {
        float radius = width_max/ 2.f;
        rect.setSize(sf::Vector2f(width_max, height_max - 2 * radius));
        rect.setFillColor(color);
        rect.setOrigin(width_max / 2, (height_max - 2 * radius) / 2);

        top.setRadius(radius);
        top.setFillColor(color);
        top.setOrigin(radius, radius);

        bottom.setRadius(radius);
        bottom.setFillColor(color);
        bottom.setOrigin(radius, radius);

        shapeType = shape;
    }

    void setPosition(float x, float y) {
        posX = x; posY = y;
        updateParts();
    }

    void setScale(float sx, float sy) {
        scaleX = sx;
        scaleY = sy;
        updateParts();
    }

    void setRotation(float angle) {
        rotation = angle;
        updateParts();
    }

    void setLength(float length) {
        if(length < 0.0){
            length = -length;
            rotation = 90.0f;
        }
        else {
            rotation = 0.0f;
        }
        scaleY  = length / height_max;
        updateParts();
    }

private:
    sf::RectangleShape rect;
    sf::CircleShape top, bottom;
    float posX{}, posY{};
    float scaleX{1.f}, scaleY{1.f};
    float rotation{0.f}; // 回転角度（度）
    float width_max  = 40.0f;
    float height_max = 80.0f;

    std::string shapeType;

    void updateParts() {
        // 高さはスケールに応じて変動
        float rectH = (height_max-width_max) * scaleY;
        float offsetY = rectH / 2;
        print("scaleY: ", scaleY);
        print("rectH:  ", rectH);

        // 長方形
        rect.setSize(sf::Vector2f(width_max, rectH));
        rect.setOrigin(width_max/2.f, rectH/2.f);
        rect.setPosition(posX, posY);
        rect.setRotation(rotation);
        rect.setScale(scaleX, 1.f);

        // 回転角度をラジアンに変換
        float rad = rotation * 3.14159265f / 180.f;

        // 上下丸の回転座標
        float dx = 0;
        float dy = offsetY;

        float cosA = cos(rad);
        float sinA = sin(rad);

        top.setPosition(posX + dx * cosA - (-dy) * sinA,
                        posY + dx * sinA + (-dy) * cosA);
        // top.setRotation(rotation);
        bottom.setPosition(posX + dx * cosA - dy * sinA,
                           posY + dx * sinA + dy * cosA);
        // bottom.setRotation(rotation);
    }

    template <typename... Args>
    void print(Args&&... args) {
        std::cout << shapeType << ": ";
        (std::cout << ... << str(std::forward<Args>(args)));
        std::cout << std::endl;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(rect, states);
        target.draw(top, states);
        target.draw(bottom, states);
    }

    template <typename T>
    std::string str(const T& value) {
        return std::to_string(value);
    }

    // 特殊化: std::string型はそのまま返す
    std::string str(const std::string& value) {
        return value;
    }

    // 特殊化: const char*型もstringに変換
    std::string str(const char* value) {
        return std::string(value);
    }
};


int main() {
    sf::RenderWindow window(sf::VideoMode(300, 200), "Robot Eyes");

    Eye rightEye (80, sf::Color(0, 247, 152), "rightEye");
    rightEye.setPosition(80, 100);
    rightEye.setLength(80);

    Eye leftEye (80, sf::Color(0, 247, 152), "leftEye");
    leftEye.setPosition(220, 100);
    leftEye.setLength(80);

    std::thread thread([&rightEye, &leftEye]() {
        float len   = 80.0f;
        float delta = -5.0f;

        leftEye.setLength(80.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        leftEye.setRotation(90.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        leftEye.setRotation(0.0f);

        while (isRunning) {
            bool loop = true;
            while (loop) {
                if (len < -10.0f) {
                    delta = -delta;
                }
                if (len > 80.0f) {
                    delta = -delta;
                    loop  = false;
                }
                len += delta;
                leftEye.setLength(len);
                rightEye.setLength(len);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                std::cout << "Length: " << len << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                isRunning = false;
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(rightEye);
        window.draw(leftEye);
        window.display();
    }

    thread.join();
    return 0;
}
