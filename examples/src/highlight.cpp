#include "hax.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <optional>
#include <unordered_set>
#include <vector>

// convert a hex coordinate to sfml shape
sf::ConvexShape hex_shape(const hax::Basis<float>& basis, const hax::Hex<>& hex)
{
    sf::ConvexShape shape{6};

    // calcualte the vertices
    auto verts = basis.vertices(hex);

    // add each vertex to the shape
    for (int i = 0; i < verts.size(); i++) {
        shape.setPoint(i, sf::Vector2f(verts[i].x(), verts[i].y()));
    }
    return shape;
}

int main(int argc, char * argv[])
{
    // local variables to use for the screen width and height and the unit
    // size of the grid
    int const width = 800;
    int const height = 600;
    int const unit_size = 30;

    // Create the window, but make sure it's not resizeable
    sf::RenderWindow window{sf::VideoMode(width, height), "hax example",
                            sf::Style::Titlebar | sf::Style::Close};

    // Create the basis for the grid - centered in the middle of the screen
    hax::Basis<float> basis{hax::Point<>(width/2, height/2), unit_size};

    // hovered will keep track of which hex the mouse is currently over
    std::optional<hax::Hex<>> hovered = std::nullopt;

    // clicked will keep track of which hex was clicked if mouse button is down
    std::optional<hax::Hex<>> clicked = std::nullopt;

    // clicked_range will keep track of all the hexes from clicked to hovered
    std::unordered_set<hax::Hex<>> clicked_range;

    // initialize the set of hexes we're working with
    // and set some basic graphical settings
    std::unordered_map<hax::Hex<>, sf::ConvexShape> shapes;
    for (const auto& hex : hax::hex_range(hax::Hex<>::zero, 30)) {
        auto shape = hex_shape(basis, hex);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.0f);
        shapes[hex] = shape;
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseMoved: {
                // convert the sfml point to a hax point
                // and round it to the nearest hex
                hax::Point<> hovered_pixel{event.mouseMove.x,
                                           event.mouseMove.y};
                hovered = hax::hex_round<int>(basis.hex(hovered_pixel));

                // if the mouse button is down, update the line from the
                // clicked hex to the hovered hex
                if (clicked) {
                    auto tiles = hax::line(*clicked, *hovered);
                    clicked_range =
                        std::unordered_set<hax::Hex<>>(tiles.begin(),
                                                       tiles.end());
                }
            }
            break;

            case sf::Event::MouseButtonPressed: {
                // keep track of the clicked coordinate
                // when the mouse button gets pressed
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    hax::Point<> clicked_pixel{event.mouseButton.x,
                                               event.mouseButton.y};
                    clicked = hax::hex_round<int>(basis.hex(clicked_pixel));
                }
            }
            break;

            case sf::Event::MouseButtonReleased:
                // reset clicked data when the mouse button is released
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    clicked = std::nullopt;
                    clicked_range.clear();
                }
                break;
            }
        }
        window.clear();
        for (const auto & [hex, shape] : shapes) {

            // color selected tiles cyan
            if (hex == hovered ||
                    clicked_range.find(hex) != clicked_range.end()) {

                shapes[hex].setFillColor(sf::Color::Cyan);
            }
            // color non selected tiles white
            else {
                shapes[hex].setFillColor(sf::Color::White);
            }
            window.draw(shape);
        }
        window.display();
    }
}
