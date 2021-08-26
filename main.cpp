#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>

bool calcz(sf::Vector2f crd, std::complex<float>& z, std::complex<float>& c, float p = 2)
{
    c.real(crd.x); c.imag(crd.y);
    z = std::pow(z, 2) + c;
    return std::norm(z) < p;
}

sf::Vector2f coords(sf::Vector2f in, sf::Vector2f size, float zoom, sf::Vector2f offset = {-2, -1})
{
    return sf::Vector2f(((in.x * (1 + 2) / zoom) / size.x) + offset.x / zoom, ((in.y * (1 + 1) / zoom) / size.y) + offset.y / zoom);
}

void gen(sf::Image& output, int maxiters, float zoom, sf::Vector2f offset = {-2, -1})
{
    std::complex<float> c, z;

    for (float y = 0; y < output.getSize().y; y++)
    {
        for (float x = 0; x < output.getSize().x; x++)
        {
            for (int i = 1; i <= maxiters; i++)
            {
                if(!calcz(coords({x, y}, {(float)output.getSize().x, (float)output.getSize().y}, zoom, offset), z, c, 4)) break;
            }
            if(calcz(coords({x, y}, {(float)output.getSize().x, (float)output.getSize().y}, zoom, offset), z, c, 4)) output.setPixel(x, y, sf::Color::White);
            z = 0;
        }
    }
}

int main()
{
    sf::Vector2f offset = { -2, -1 };
    float zoom = 1;

    sf::RenderWindow w(sf::VideoMode(900, 600), "fractal");

    sf::Image output;
    output.create(900, 600);
    
    gen(output, 50, 1);

    sf::Texture tex;
    tex.loadFromImage(output);

    sf::Sprite sprite(tex);
    sf::Clock clock;
    while(w.isOpen())
    {
        sf::Event event;
        while(w.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) w.close();
        }

        float time = clock.restart().asSeconds() / 40;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) offset.x -= 0.6 * time;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) offset.x += 0.6 * time;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) offset.y -= 0.6 * time;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) offset.y += 0.6 * time;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) zoom -= 1 * time;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) zoom += 1 * time;

        output.create(900, 600);
        gen(output, 50, zoom, offset);
        tex.update(output);
               
        w.clear();
        w.draw(sprite);
        w.display();
    }

    output.saveToFile("output.jpg");
}
