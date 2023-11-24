#include "tess/tess.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

#include <concepts>

using int_point = tess::point<int>;

// convert a hex coordinate to an sfml shape
sf::ConvexShape hex_shape(const tess::pointed_fbasis& basis,
                          const tess::Hex<>& hex)
{
    sf::ConvexShape shape{6};

    // calcualte the vertices
    auto verts = basis.vertices<int_point>(hex);

    // add each vertex to the shape
    for (int i = 0; i < verts.size(); i++) {
        shape.setPoint(i, sf::Vector2f(verts[i].x, verts[i].y));
    }
    return shape;
}

int main(int argc, char * argv[])
{
    // local variables for screen width and height and grid unit size
    float const width = 800.f;
    float const height = 600.f;
    float const unit_size = 50.f;

    // Create the window, but make sure it's not resizeable
    sf::RenderWindow window{sf::VideoMode(width, height), "tess simple example",
                            sf::Style::Titlebar | sf::Style::Close};

    // Create the basis for the grid, centered in the middle of the screen
    tess::pointed_fbasis basis{width/2.f, height/2.f, unit_size};

    // initialize the hexes we're working with
    // and set some basic graphical settings
    std::vector<sf::ConvexShape> shapes;
    for (const auto& hex : tess::hex_range(tess::Hex<>::zero, 3)) {
        auto shape = hex_shape(basis, hex);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.0f);
        shapes.push_back(shape);
    }

    while (window.isOpen()) {
        sf::Event event;

        // close if exit button pressed
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // draw the hexes
        window.clear();
        for (const auto& hex : shapes) {
            window.draw(hex);
        }
        window.display();
    }
}
