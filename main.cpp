#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <unistd.h>

namespace po = boost::program_options;


float secs = 0.001;
useconds_t pause = useconds_t(secs * 1e6f);

bool finished = false;

//TODO: Add new sorting algorithms

void swapRects(sf::RectangleShape& rect1, sf::RectangleShape& rect2)
{
    sf::Vector2f temp = rect1.getSize();
    rect1.setSize(rect2.getSize());
    rect2.setSize(temp);
}

void shuffleRects(std::vector<sf::RectangleShape> &rects, int size_this)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(0, size_this - 1);
    for (int i = 0; i < size_this; i++)
    {
        int j = dist(g);
        swapRects(rects[i], rects[j]);
    }
}

bool running = true;

void bubbleSort(std::vector<sf::RectangleShape> &rects, int size_this)
{
    for (int i = 0; i < size_this - 1; i++)
    {
        for (int j = 0; j < size_this - i - 1; j++)
        {
            if(!running)return;
            if (rects[j].getSize().y > rects[j + 1].getSize().y)
            {
                swapRects(rects[j], rects[j + 1]);
                usleep(pause);
            }
        }
    }
    finished = true;
}

void selectionSort(std::vector<sf::RectangleShape> &rects, int size_this){
    for (int i = 0; i < size_this - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < size_this; j++)
        {
            if(!running)return;
            if (rects[j].getSize().y < rects[min].getSize().y)min = j;
        }
        swapRects(rects[i], rects[min]);
        usleep(pause);
    }
    finished = true;
}


void insertionSort(std::vector<sf::RectangleShape> &rects, int size_this)
{
    for (int i = 1; i < size_this; i++)
    {
        if(!running)return;
        sf::Vector2f temp = rects[i].getSize();
        int j = i - 1;
        while (j >= 0 && rects[j].getSize().y > temp.y)
        {
            rects[j + 1].setSize(rects[j].getSize());
            j--;
        }
        rects[j + 1].setSize(temp);
        usleep(pause);
    }
    finished = true;
}

int main(int argc, char *argv[]){

    int size = 1000;
    int width = 1800; //1600
    int height = 800; //800

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("sort,a", po::value<char>(), "sort type \n available: bubble, selection, insertion \n default: bubble")
        ("size,s", po::value<int>(), "size of array default: 1000 max: 5000")
        ("width,w", po::value<int>(), "width of window default: 1600")
        ("height,h", po::value<int>(), "height of window default: 800")
        ("pause,p", po::value<float>(), "pause in seconds default: 0.005");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("size"))
    {
        size = vm["size"].as<int>();
    }

    if (vm.count("width"))
    {
        width = vm["width"].as<int>();
    }

    if (vm.count("height"))
    {
        height = vm["height"].as<int>();
    }

    if (vm.count("pause"))
    {
        secs = vm["pause"].as<float>();
        pause = useconds_t(secs * 1e6f);
    }


    sf::RenderWindow window(sf::VideoMode(width, height), "Sorting algorithms!", sf::Style::Close);

    std::vector<sf::RectangleShape> rects;
    for (int i = 0; i < size; i++) {
        rects.emplace_back(sf::RectangleShape());
        rects[i].setSize(sf::Vector2f(5, ((float)i + 1) * (float)height / (float)size));
        rects[i].setPosition(sf::Vector2f((float)i * 5, (float)height));
        rects[i].rotate(180);
        rects[i].setFillColor(sf::Color::Red);
        rects[i].setOutlineColor(sf::Color::Black);
        rects[i].setOutlineThickness(1);
    }

    shuffleRects(rects, size);

    sf::Clock clock;
    float time{0};

    sf::View view(sf::FloatRect(0, 0, (float)size * 5, (float)height));

    view.move(-5, 0);
    view.zoom(1.005);

    window.setView(view);

    std::string title;

    std::thread sorting;


    if(vm.count("sort")){
        if(vm["sort"].as<char>() == 'b'){
            sorting = std::thread(bubbleSort, std::ref(rects), size);
            title = "Bubble sort";
        }
        else if(vm["sort"].as<char>() == 's'){
            sorting = std::thread(selectionSort, std::ref(rects), size);
            title = "Selection sort";
        }
        else if(vm["sort"].as<char>() == 'i'){
            sorting = std::thread(insertionSort, std::ref(rects), size);
            title = "Insertion sort";
        }
        else{
            std::cout << "Wrong sort type!\n";
            return 1;
        }
    }
    else{
        sorting = std::thread(bubbleSort, std::ref(rects), size);
        title = "Bubble sort";
    }


    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }
        window.clear();

        for (int i = 0; i < size; i++) {
            window.draw(rects[i]);
        }
        if(!finished) {
            time = clock.getElapsedTime().asSeconds();
        }
        window.setTitle(title + " : " + std::to_string(time));
        window.display();
    }
    running = false;
    sorting.join();
    return 0;
}
