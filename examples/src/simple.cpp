#include "tess/tess.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

// convert a hex coordinate to an sfml shape
sf::ConvexShape hex_shape(const tess::Basis<float>& basis,
                          const tess::ihex& h)
{
    sf::ConvexShape shape{6};
    const auto verts = basis.vertices(h);

    // add each vertex to the shape
    for (const int i : views::iota(0, 6)) {
        shape.setPoint(i, sf::Vector2f(verts[i].x, verts[i].y));
    }
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.0f);
    return shape;
}

int main()
{
    // local variabeles for screen width and height and grid unit size
    constexpr char window_name[] = "tess simple example";
    constexpr int width = 800;
    constexpr int height = 600;
    constexpr int unit_size = 50;

    // Create the window, but make sure it's not resizeable
    sf::RenderWindow window{sf::VideoMode(width, height), window_name,
                            sf::Style::Titlebar | sf::Style::Close};

    // Create the basis for the grid, centered in the middle of the screen
    tess::Basis<float> basis{ tess::point{ width/2, height/2 },
                              unit_size };

    // initialize the hexes we're working with
    // and set some basic graphical settings
    constexpr std::size_t num_hexes = 37;
    std::vector<tess::ihex> hexes;
    hexes.reserve(num_hexes);
    tess::hex_range(tess::ihex::zero, 3, std::back_inserter(hexes));

    std::vector<sf::ConvexShape> shapes;
    shapes.reserve(num_hexes);
    for (const auto& h : hexes) {
        shapes.push_back(hex_shape(basis, h));
    }

    // constexpr std::size_t num_hexes = 37;
    // std::vector<sf::ConvexShape> shapes;
    // shapes.reserve(num_hexes);

    // ranges::transform(tess::hex_range(tess::ihex::zero, 3),
    //                   hex_shape_fn(basis), std::back_inserter(shapes));

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
        for (const auto& h : shapes) {
            window.draw(h);
        }
        window.display();
    }
}
