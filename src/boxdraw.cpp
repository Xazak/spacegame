
// XXX see BoxDraw.draw()
#include <iostream>

// topLeft is the upper left corner of a box
// NB: in BoxDrawArray below, "top left" would be represented with a bottomRight character, bc it means "lines going out to the top and left"
enum BoxDrawChars {
    topLeft = 0x250C,
    topRight = 0x2510,
    bottomLeft = 0x2514,
    bottomRight = 0x2518,
    hLine = 0x2500,
    vLine = 0x2502,
    leftT = 0x251C,
    rightT = 0x2524,
    upperT = 0x252C,
    lowerT = 0x2534,
    cross = 0x253C,
    space = 0x0020,
};

// indexed by bitfield: left bottom right top  (most -> least significant)
// 1 indicates presence of line touching that side of the character
wchar_t BoxDrawArray[] = {
    // 00-11
    space,
    space,
    space,
    bottomLeft,
    // 100-111
    space,
    vLine,
    topLeft,
    leftT,
    // 1000-1011
    space,
    bottomRight,
    hLine,
    lowerT,
    // 1100-1111
    topRight,
    rightT,
    upperT,
    cross,
};

class BoxDraw {

    int width, height;
    char * buffer;  // 2d array, indexed [x + y * width]. only stores a bit: whether there is a line at (x, y)

    char get(int x, int y) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return 0;
        return buffer[x + y * width];
    }

    void set(int x, int y, char value) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return;
        buffer[x + y * width] = value;
    }

public:
    BoxDraw(int w, int h) {
        width = w;
        height = h;
        buffer = new char[width * height];
    }

    ~BoxDraw() {
        delete[] buffer;
    }

    void add(int x, int y, int w, int h) {
        for (int i = x; i < x + w; i++) {
            set(i, y, 1);
            set(i, y + h - 1, 1);
        }
        for (int i = y + 1; i < y + h - 1; i++) {
            set(x, i, 1);
            set(x + w - 1, i, 1);
        }
    }

    void draw() {
        // TODO replace setlocale/wcout with BLT calls
        setlocale(LC_ALL, "en_US.UTF-8");
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int c = 0;
                if (buffer[x + y * width])
                    c = (get(x - 1, y) << 3) | (get(x, y + 1) << 2) | (get(x + 1, y) << 1) | get(x, y - 1);
                std::wcout << BoxDrawArray[c];
            }
            std::wcout << std::endl;
        }
    }
};

// XXX
void testbox() {
    BoxDraw box(80, 25);
    box.add(5, 5, 10, 10);
    box.add(2, 2, 7, 7);
    box.add(14, 10, 8, 4);
    box.draw();
}
