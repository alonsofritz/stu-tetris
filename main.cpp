#include <SFML/Graphics.hpp>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{
    int x, y;
} a[4], b[4];

int figures[7][4] = {
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S 
    3, 5, 4, 7, // T 
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};

bool check() {
    for (int i = 0; i < 4; i++){
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {
            return 0;
        } else if (field[a[i].y][a[i].x] ) {
            return 0;
        }
    }
    return 1;
}

int main() {

    srand(time(0));

    RenderWindow window(VideoMode({480,540}), "Sample Tetris!");

    Texture t;
    bool loaded = t.loadFromFile("assets/tiles.png");
    if (!loaded) {
        return -1; // Error loading texture
    }

    Sprite s(t);
    s.setTextureRect(IntRect({0,0},{24,24}));

    int dir_x = 0;
    bool rotate = 0;
    int typeNum = 1;

    float timer = 0;
    float delay = 0.3;

    Clock clock;


    while(window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (event->is<Event::KeyPressed>()) {
                if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
                    rotate = true;
                } else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
                    dir_x = -1;
                } else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
                    dir_x = 1;
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
            delay = 0.05;
        }

        // MOVE
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dir_x;
        }

        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }

        // Rotate
        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].x - p.x; // Translate to origin
                int y = a[i].y - p.y;
                
                // Rotate 90 degrees clockwise: (x,y) -> (y,-x)
                a[i].x = p.x + y;
                a[i].y = p.y - x;
            }
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i];
                }
            }
        }

        // TICK
        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    field[b[i].y][b[i].x] = typeNum;
                }
                typeNum = 1 + rand()%7;
                int n = rand()%7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }


        // CHECK LINES
        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j]) {
                    count++;
                }
                field[k][j] = field[i][j];
            }
            if (count < N) {
                k--;
            }
        }


        dir_x = 0;
        rotate = false;
        delay = 0.3;

        /// DRAW
        window.clear(Color::White);
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) {
                    continue;
                }
                s.setTextureRect(IntRect({field[i][j]*24, 0},{24,24}));
                s.setPosition(Vector2f(j*24, i*24));
                window.draw(s);
            }
        }
        
        for (int i = 0; i < 4; i ++) {
            s.setTextureRect(IntRect({typeNum*24, 0},{24, 24}));
            s.setPosition(Vector2f(a[i].x*24, a[i].y*24));
            window.draw(s);
        }
    
        window.display();
    }

    return 0;
}