#include <iostream>
#include <random>

struct point;
typedef std::vector<point> path;

struct state {

    void pulse_horizontal(bool right) {
        double rx = dist(mt);
        x += (right ? 1 : -1) * (1.0 + rx);
        double ry = dist(mt) - 0.5;
        y += ry;
    }

    void pulse_vertical(bool up) {
        double ry = dist(mt);
        y += (up ? 1 : -1) * (1.0 + ry);
        double rx = dist(mt) - dist.max() / 2;
        x += rx;
    }

    void right() {
        pulse_horizontal(true);
    }

    void left() {
        pulse_horizontal(false);
    }

    void up() {
        pulse_vertical(true);
    }

    void down() {
        pulse_vertical(false);
    }

    void move_to(const point &p);

    void traverse(const path &path);

    double x;
    double y;

    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> dist;

    state();

};

struct point {
    double x;
    double y;

    point();

    point(std::initializer_list<double> l);
};

std::ostream &operator<<(std::ostream &os, const point &p) {
    os << '(' << p.x << ',' << ' ' << p.y << ')';
}

point::point() :
    x(0),
    y(0) {}

point::point(std::initializer_list<double> l) :
    x(l.begin()[0]),
    y(l.begin()[1]) {}

state::state() :
    x(0),
    y(0),
    rd(),
    mt(rd()),
    dist(0, 5) {}

void state::move_to(const point &p) {
    double err_x = p.x - x;
    double err_y = p.y - y;
    int cycles = 0;
    while (hypot(err_x, err_y) > 1) {
        if (fabs(err_x) > fabs(err_y)) {
            if (p.x > x) {
                right();
            } else {
                left();
            }
        } else {
            if (p.y > y) {
                up();
            } else {
                down();
            }
        }
        err_x = p.x - x;
        err_y = p.y - y;
        ++cycles;
    }
    std::cout << point({x, y}) << " : " << cycles << std::endl;
}

void state::traverse(const path &path) {
    for (const point &p : path) {
        move_to(p);
    }
}

int main() {
    path square = {
        {0,   100},
        {100, 100},
        {100, 0},
        {0,   0}
    };
    for (point &p : square) {
        std::cout << p << std::endl;
    }
    state state;
    state.traverse(square);
}
