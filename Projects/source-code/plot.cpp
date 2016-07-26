// No rights reserved.
// Author: Amish Naidu (amhndu@gmail.com)
// Simple plotting utility. Define functions to plot in the `curves' array in the main() function using a lambda and compile
// Graph can be panned/moved using arrow-keys
// Requires SFML 2.0+
// Compiling:
// $ g++ -std=c++11 plot.cpp -o plot  -lsfml-graphics -lsfml-window -lsfml-system
// Invoke:
// $ ./plot


#include <iostream>
#include <cmath>
#include <functional>
#include <SFML/Graphics.hpp>
#define TO_RAD(x) ( (x)*M_PI/180.0 )
#define TO_DEG(x) ( (x)*180.0/M_PI )

template<class IntType>
bool inline isInRange(IntType t,IntType a,IntType b)
{
    return (t >= a && t <= b);
}

class Curve
{
    typedef std::function<float(float)> Function;
    public:
        Curve(Function fun, int _start, int _width, sf::Vector2f _scale, const sf::Color &_color) :
            color(_color),
            start(_start),
            width(_width),
            xpan(0),
            dxpan(0),
            scale(_scale),
            f(fun)
        {
            generate();
            hmap.resize(width);
        }

        void draw(sf::RenderTarget& target)
        {
            target.draw(va);
        }

        float getNormAngle(int x)
        {
            int y = hmap[x - xpan];
            float avgX = 0, avgY = 0;
            int sq = 3;
            for(int w = -sq; w <= sq; w++)
            {
                for(int h = -sq; h <= sq; h++)
                {
                    if(isInRange(x + w - xpan, 0, int(hmap.size())) && hmap[x + w - xpan] > y + h)
                    {
                        avgX -= w;
                        avgY -= h;
                    }
                }
            }
            return avgX ? std::atan2(-avgY, avgX) : -M_PI_2;
        }

        void pan(float dx)
        {
            dxpan += dx;
            if (std::abs(dxpan) > 1.0)
            {
                xpan += static_cast<int>(dxpan);
                generate();
                dxpan = dxpan - static_cast<int>(dxpan);
            }
        }

        int getHeight(int x)
        {
            return isInRange(x - xpan, 0, int(hmap.size())) ? -hmap[x - xpan] : 0;
        }
    private:
        void generate()
        {
            va.clear();
            va.setPrimitiveType(sf::LinesStrip);
            hmap.resize(width);
            for (int i = start + xpan; i < width + xpan; ++i)
            {
                hmap[i - xpan] = int(scale.y * f(i / scale.x));
                //y co-ordinate negative because SFML's system has the axis flipped
                va.append({{float(i), -scale.y * f(i / scale.x)}, color});
            }
        }
	const sf::Color& color;
        std::vector<int> hmap;
        int start, width;
        int xpan;
        float dxpan;
        sf::Vector2f scale;
        Function f;
        sf::VertexArray va;
};

int main()
{
    sf::RenderWindow win(sf::VideoMode(800, 600), "Foo");
    sf::View view({400, 0}, {800, 600});
    float view_deltax = 0, view_deltay = 0;

    Curve curves[] = {
        //           <function as a lambda>, start position, width, {scale x, scale y}, color of curve
        //                  |                            |    |     |                   |
       {([](float x){ return x*x*x - x*x + 2*x + 2; }),  0, 800, {50, 50}, sf::Color::Cyan},
       {([](float x){ return std::sin(x); }),  0, 800, {50, 50}, sf::Color::Red},
       {([](float x){ return std::sinh(x); }),  0, 800, {50, 50}, sf::Color::Green},
       {([](float x){ return std::sqrt(x); }),  0, 800, {50, 50}, sf::Color::Blue},
       {([](float x){ return -std::sqrt(x); }),  0, 800, {50, 50}, sf::Color::Blue},
       {([](float x){ return x;}),  0, 800, {50, 50}, sf::Color::Red},
       {([](float x){ return 0; }), 0, 800, {1,1}, sf::Color::Black}
    };

    sf::Event event;
    sf::Clock timer;
    while (win.isOpen())
    {
        float dt = timer.restart().asSeconds();
        while (win.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    win.close();
                    break;
                default:
                    break;
            }
        }
        float velocity = 300;
        //Panning
        auto mouse_x = sf::Mouse::getPosition();
        float dx = 0, dy = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            dx = velocity * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            dx = -velocity * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            dy = -velocity * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            dy = velocity * dt;
        }
        view_deltax += dx;
        view_deltay += dy;
        view.move(dx, dy);
        for (auto &curve: curves)
            curve.pan(dx);

        win.clear(sf::Color::White);
	for (auto &curve: curves)
            curve.draw(win);
        win.setView(view);
        win.display();
    }
    return 0;
}
