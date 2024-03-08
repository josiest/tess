#include "tess/tess.hpp"
#include <SFML/Graphics.hpp>

#include <concepts>
#include <optional>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <cstdint>

struct highlight_settings {
    sf::VideoMode video_mode{ 800, 600 };
    std::string window_name = "Tess Highlight Example";
    std::uint32_t window_style = sf::Style::Titlebar | sf::Style::Close;

    float unit_size = 30.f;
};

// convert a hex coordinate to sfml shape
template<std::integral Integer>
sf::ConvexShape hex_shape(const tess::pointed_fbasis& basis,
                          const tess::hex<Integer>& hex)
{
    std::array<sf::Vector2f, 6> verts;
    basis.vertices<sf::Vector2f>(hex, verts.begin());

    sf::ConvexShape shape{6};
    for (int i = 0; i < verts.size(); i++) {
        shape.setPoint(i, verts[i]);
    }
    return shape;
}

int main()
{
    // Create the window, but make sure it's not resizeable
    const highlight_settings settings{};
    sf::RenderWindow window{ settings.video_mode, settings.window_name,
                             settings.window_style };

    // Create the basis for the grid - centered in the middle of the screen
    tess::pointed_fbasis basis{
        static_cast<float>(settings.video_mode.width)/2.f,
        static_cast<float>(settings.video_mode.height)/2.f,
        settings.unit_size
    };

    // hovered will keep track of which hex the mouse is currently over
    std::optional<tess::hex<int>> hovered = std::nullopt;

    // clicked will keep track of which hex was clicked if mouse button is down
    std::optional<tess::hex<int>> clicked = std::nullopt;

    // clicked_range will keep track of all the hexes from clicked to hovered
    std::unordered_set<tess::hex<int>> clicked_range;

    // initialize the set of hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::hex<int>> hexes;
    tess::hex_range(tess::hex<int>::zero, 30, std::back_inserter(hexes));

    std::unordered_map<tess::hex<int>, sf::ConvexShape> shapes;
    for (const auto& hex : hexes) {
        auto [mapping, _] = shapes.emplace(hex, hex_shape(basis, hex));
        auto& shape = mapping->second;
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.0f);
    }

    while (window.isOpen()) {
        sf::Event event{};

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
                    clicked_range.clear();
                    auto into_clicked = std::inserter(clicked_range,
                                                      clicked_range.begin());
                    tess::line(*clicked, *hovered, into_clicked);
                }
            }
            break;

            case sf::Event::MouseButtonPressed: {
                // keep track of the clicked coordinate
                // when the mouse button gets pressed
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    tess::point clicked_pixel{ event.mouseButton.x,
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

            default: break;
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
