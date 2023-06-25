#include "tess/tess.hpp"
#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>
#include <optional>

#include <ranges>
#include <algorithm>

namespace views = std::views;
namespace ranges = std::ranges;

// convert a hex coordinate to sfml shape
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
    shape.setOutlineThickness(1.f);
    return shape;
}

struct input_manager{
    static input_manager with_basis(int x, int y, int unit_size);

    constexpr bool is_hovered(const tess::ihex& h) const;
    constexpr bool is_clicked(const tess::ihex& h) const;

    void on_mouse_moved(int x, int y);
    void on_mouse_clicked(sf::Mouse::Button button, int x, int y);
    void on_mouse_released(sf::Mouse::Button button);

    tess::Basis<float> basis;
    std::optional<tess::ihex> hovered = std::nullopt;
    std::vector<tess::ihex> clicked;
};

input_manager
input_manager::with_basis(int x, int y, int unit_size)
{
    const tess::point origin{ x, y };
    input_manager input{ tess::Basis<float>{ origin, unit_size } };
    input.clicked.reserve(20);
    return input;
}

constexpr bool input_manager::is_hovered(const tess::ihex& h) const
{
    return hovered == h;
}

constexpr bool input_manager::is_clicked(const tess::ihex& h) const
{
    return ranges::find(clicked, h) != ranges::end(clicked);
}

void input_manager::on_mouse_moved(int x, int y)
{
    // convert the sfml point to a tess point
    // and round it to the nearest hex
    hovered = tess::hex_round<int>(basis.nearest_hex(tess::point{ x, y }));
    
    // if the mouse button is down, update the line from the
    // clicked hex to the hovered hex
    if (clicked.empty()) { return; }

    const auto first = clicked.front();
    clicked.clear();
    tess::line(first, *hovered, std::back_inserter(clicked));
}

void input_manager::on_mouse_clicked(sf::Mouse::Button button, int x, int y)
{
    if (button != sf::Mouse::Button::Left) { return; }

    const tess::point clicked_pixel{ x, y };
    clicked.push_back(tess::hex_round<int>(basis.nearest_hex(clicked_pixel)));
}

void input_manager::on_mouse_released(sf::Mouse::Button button)
{
    if (button != sf::Mouse::Button::Left) { return; }
    clicked.clear();
}

struct ui_manager{
    static ui_manager
    with_board_radius(const tess::Basis<float>& basis, int r);

    void draw_shapes(sf::RenderWindow& window, const input_manager& input);

    std::unordered_map<tess::ihex, sf::ConvexShape> shapes;
};

ui_manager
ui_manager::with_board_radius(const tess::Basis<float>& basis, int r)
{
    ui_manager ui;

    // initialize the set of hexes we're working with
    // and set some basic graphical settings
    std::vector<tess::ihex> hexes;
    tess::hex_range(tess::ihex::zero, r, std::back_inserter(hexes));

    ui.shapes.reserve(hexes.size());
    for (const auto& h : hexes) {
        ui.shapes.emplace(h, hex_shape(basis, h));
    }
    return ui;
}

void ui_manager::draw_shapes(sf::RenderWindow& window,
                             const input_manager& input)
{
    for (auto& shape_pair : shapes) {
        const auto& h = shape_pair.first;
        auto& shape = shape_pair.second;

        // color selected tiles cyan
        if (input.is_hovered(h) || input.is_clicked(h)) {
            shape.setFillColor(sf::Color::Cyan);
        }
        // color non selected tiles white
        else {
            shape.setFillColor(sf::Color::White);
        }
        window.draw(shape);
    }
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

    // handles input, keeping track of hovered and clicked tiles
    auto input = input_manager::with_basis(width/2, height/2, unit_size);
    // handles displaying the tiles
    auto ui = ui_manager::with_board_radius(input.basis, 30);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseMoved:
                input.on_mouse_moved(event.mouseMove.x, event.mouseMove.y);
                break;

            case sf::Event::MouseButtonPressed:
                input.on_mouse_clicked(event.mouseButton.button,
                                       event.mouseButton.x,
                                       event.mouseButton.y);
                break;

            case sf::Event::MouseButtonReleased:
                input.on_mouse_released(event.mouseButton.button);
                break;
            }
        }
        window.clear();
        ui.draw_shapes(window, input);
        window.display();
    }
}
