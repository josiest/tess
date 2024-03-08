#include "tess/tess.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <array>
#include <cstdint>

struct simple_settings {
    sf::VideoMode video_mode{ 800, 600 };
    std::string window_name = "Simple Tess Example";
    std::uint32_t window_style = sf::Style::Titlebar | sf::Style::Close;

    float unit_size = 50.f;
};

// convert a hex coordinate to an sfml shape
template<std::integral Integer>
sf::ConvexShape hex_shape(const tess::flat_fbasis& basis,
                          const tess::hex<Integer>& hex)
{
    sf::ConvexShape shape{6};
    // if sf::Shape had begin() -> output_iterator<Vector2f>
    // basis.vertices(hex, shape.begin());

    // compute the vertices for the shape
    std::array<sf::Vector2f, 6> verts;
    basis.vertices<sf::Vector2f>(hex, verts.begin());
    for (int i = 0; i < verts.size(); i++) {
        shape.setPoint(i, verts[i]);
    }
    return shape;
}

int main()
{
    const simple_settings settings{};

    // Create the basis for the grid, centered in the middle of the screen
    const tess::flat_fbasis basis{
        static_cast<float>(settings.video_mode.width)/2.f,
        static_cast<float>(settings.video_mode.height)/2.f,
        settings.unit_size
    };

    // initialize the hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::hex<int>> hexes;
    tess::hex_range(tess::hex<int>::zero, 3, std::back_inserter(hexes));

    std::vector<sf::ConvexShape> shapes;
    shapes.reserve(hexes.size());
    for (const auto& hex : hexes) {
        auto& shape = shapes.emplace_back(hex_shape(basis, hex));
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.0f);
    }

    // Create the window, but make sure it's not resizeable
    sf::RenderWindow window{ settings.video_mode, settings.window_name,
                             settings.window_style };

    while (window.isOpen()) {
        sf::Event event{};

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
