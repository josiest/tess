#include "tess/tess.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <optional>
#include <unordered_set>
#include <vector>
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

// convert a hex coordinate to sfml shape
sf::ConvexShape hex_shape(const tess::Basis<float>& basis,
                          const tess::hex& h)
{
    sf::ConvexShape shape{6};
    const auto verts = basis.vertices(h);
    // add each vertex to the shape
    for (const int i : views::iota(0, 6)) {
        shape.setPoint(i, sf::Vector2f(verts[i].x, verts[i].y));
    }
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.f);
    return shape;
}

int main(int argc, char * argv[])
{
    // local variables to use for the screen width and height and the unit
    // size of the grid
    constexpr int width = 800;
    constexpr int height = 600;
    constexpr int unit_size = 30;

    // Create the window, but make sure it's not resizeable
    sf::RenderWindow window{sf::VideoMode(width, height), "tess example",
                            sf::Style::Titlebar | sf::Style::Close};

    // Create the basis for the grid - centered in the middle of the screen
    tess::Basis<float> basis{ tess::point{ width/2, height/2 },
                              unit_size };

    // hovered will keep track of which hex the mouse is currently over
    std::optional<tess::hex> hovered = std::nullopt;

    // clicked will keep track of which hex was clicked if mouse button is down
    std::optional<tess::hex> clicked = std::nullopt;

    // clicked_range will keep track of all the hexes from clicked to hovered
    std::unordered_set<tess::hex> clicked_range;
    clicked_range.reserve(20);

    // initialize the set of hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::hex> hexes;
    tess::hex_range(tess::hex::zero, 30, std::back_inserter(hexes));

    std::unordered_map<tess::hex, sf::ConvexShape> shapes;
    shapes.reserve(hexes.size());
    for (const auto& h : hexes) {
        shapes.emplace(h, hex_shape(basis, h));
    }

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseMoved: {
                // convert the sfml point to a tess point
                // and round it to the nearest hex
                tess::point hovered_pixel{ event.mouseMove.x,
                                               event.mouseMove.y };
                hovered = tess::hex_round<int>(basis.hex(hovered_pixel));

                // if the mouse button is down, update the line from the
                // clicked hex to the hovered hex
                if (clicked) {
                    std::vector<tess::hex> tiles;
                    tiles.reserve(tess::hex_norm(*clicked-*hovered));
                    tess::line(*clicked, *hovered, std::back_inserter(tiles));

                    clicked_range.clear();
                    auto into_clicked =
                        std::inserter(clicked_range,
                                      std::begin(clicked_range));
                    ranges::copy(tiles, into_clicked);

                    // clicked_range.clear();
                    // ranges::copy(tess::line_view(*clicked, *hovered),
                    //              std::inserter(clicked_range));
                }
            }
            break;

            case sf::Event::MouseButtonPressed: {
                // keep track of the clicked coordinate
                // when the mouse button gets pressed
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    const tess::point clicked_pixel{ event.mouseButton.x,
                                                     event.mouseButton.y };
                    clicked = tess::hex_round<int>(basis.hex(clicked_pixel));
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
        for (auto& shape_pair : shapes) {
            const auto& h = shape_pair.first;
            auto& shape = shape_pair.second;

            // color selected tiles cyan
            if (h == hovered || clicked_range.contains(h)) {
                shape.setFillColor(sf::Color::Cyan);
            }
            // color non selected tiles white
            else {
                shape.setFillColor(sf::Color::White);
            }
            window.draw(shape);
        }
        window.display();
    }
}
