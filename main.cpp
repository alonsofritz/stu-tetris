#include <SFML/Graphics.hpp>

using namespace sf;

int main() {

    RenderWindow window(VideoMode({320,480}), "The Tris!");

    while(window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        window.clear(Color::White);
        window.display();
    }

    return 0;
}