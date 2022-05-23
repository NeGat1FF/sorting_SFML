#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <unistd.h>

//const int size = 50000;//500000

unsigned int pause = 0;

bool finished = false;

void bubbleSort(int *v, int sizeThis)
{
    for (int i = 0; i < sizeThis; i++)
    {
        for (int j = 0; j < sizeThis - i - 1; j++)
        {
            if (v[j] > v[j + 1])
            {
                std::swap(v[j], v[j + 1]);
                usleep(pause);
            }
        }
    }
    finished = true;
}

void insertionSort(int *v, int sizeThis)
{
    for (int i = 1; i < sizeThis; i++)
    {
        int j = i;
        while (j > 0 && v[j] < v[j - 1])
        {
            std::swap(v[j], v[j - 1]);
            j--;
            usleep(pause);
        }
    }
    finished = true;
}

void selectionSort(int *v, int sizeThis)
{
    for (int i = 0; i < sizeThis; i++)
    {
        int min = i;
        for (int j = i + 1; j < sizeThis; j++)
        {
            if (v[j] < v[min])
            {
                min = j;
            }
        }
        std::swap(v[i], v[min]);
        usleep(pause);
    }
    finished = true;
}

void quickSort(int *v, int sizeThis, int left, int right)
{
    if (left < right)
    {
        int pivot = v[(left + right) / 2];
        int i = left;
        int j = right;
        while (i <= j)
        {
            while (v[i] < pivot)
            {
                i++;
            }
            while (v[j] > pivot)
            {
                j--;
            }
            if (i <= j)
            {
                std::swap(v[i], v[j]);
                i++;
                j--;
                usleep(pause);
            }
        }
        quickSort(v, sizeThis, left, j);
        quickSort(v, sizeThis, i, right);
    }
    finished = true;
}


void merge(int *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            usleep(pause);
            i++;
        }
        else
        {
            arr[k] = R[j];
            usleep(pause);
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        usleep(pause);
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        usleep(pause);
        j++;
        k++;
    }
}

void mergeSort(int *arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
    finished = true;
}


void heapify(int *arr, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
        usleep(pause);
    }
}

void heapSort(int *arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    finished = true;
}


int main(int argc, char *argv[]){
    int size = 10000;
    int width = 800;
    int height = 600;
    sf::Color color = sf::Color::Red;

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("sort,a", po::value<char>(), "sort type \n available: bubble, selection, insertion \n default: bubble")
            ("size,s", po::value<int>(), "size of array default: 1000 max: 5000")
            ("width,w", po::value<int>(), "width of window default: 1600")
            ("height,h", po::value<int>(), "height of window default: 800")
            ("pause,p", po::value<unsigned int>(), "pause in seconds default: 0.005")
            ("color,c", po::value<std::string>(), "color of array default: red");

    po::variables_map vm;

    try{
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch(std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Use --help for more information" << std::endl;
        return 1;
    }
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
        pause = vm["pause"].as<unsigned int>();
    }

    if (vm.count("color"))
    {
        std::string color_str = vm["color"].as<std::string>();
        if (color_str == "red")
            color = sf::Color::Red;
        else if (color_str == "green")
            color = sf::Color::Green;
        else if (color_str == "blue")
            color = sf::Color::Blue;
        else if (color_str == "yellow")
            color = sf::Color::Yellow;
        else if (color_str == "magenta")
            color = sf::Color::Magenta;
        else if (color_str == "cyan")
            color = sf::Color::Cyan;
        else if (color_str == "white")
            color = sf::Color::White;
        else
        {
            std::cout << "color not found, using default: red" << std::endl;
            color = sf::Color::Red;
        }
    }


    sf::RenderWindow window(sf::VideoMode(width, height), "Sorting algorithms!", sf::Style::Close);

    sf::VertexArray vertices(sf::Lines, 2);
    vertices[0].color = color;
    vertices[1].color = color;

    int *data1_X = new int[size];
    int *data1_Y = new int[size];

    std::iota(data1_X, data1_X + size, 0);
    std::iota(data1_Y, data1_Y + size, 0);

    std::shuffle(data1_Y, data1_Y + size, std::mt19937{std::random_device{}()});

    sf::View view(sf::FloatRect(0, 0, float(2 * size), float(2 * size)));

    window.setView(view);

    std::string title;

    std::thread sorting;

    if(vm.count("sort")){
        if(vm["sort"].as<char>() == 'b'){
            sorting = std::thread(bubbleSort, data1_Y, size);
            title = "Bubble sort";
        }
        else if(vm["sort"].as<char>() == 's'){
            sorting = std::thread(selectionSort, data1_Y, size);
            title = "Selection sort";
        }
        else if(vm["sort"].as<char>() == 'i'){
            sorting = std::thread(insertionSort, data1_Y, size);
            title = "Insertion sort";
        }
        else if(vm["sort"].as<char>() == 'm'){
            sorting = std::thread(mergeSort, data1_Y, 0, size - 1);
            title = "Merge sort";
        }
        else if(vm["sort"].as<char>() == 'h'){
            sorting = std::thread(heapSort, data1_Y, size);
            title = "Heap sort";
        }
        else{
            std::cout << "Wrong sorting algorithm name!\n";
            return 1;
        }
    }
    else{
        std::cout << "Please specify sorting algorithm!\n"
                     "Available: bubble, selection, insertion, merge, heap\n";
        return 1;
    }

    sf::Clock clock;
    float time{0};

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

        for(int i = 0; i < size; i++){
            vertices[0].position = sf::Vector2f(float(data1_X[i] * 2), float(2 * size));
            vertices[1].position = sf::Vector2f(float(data1_X[i] * 2), float(2 * size - 2 * data1_Y[i]));
            window.draw(vertices);
        }
        if(!finished) {
            time = clock.getElapsedTime().asSeconds();
        }
        window.setTitle(title + " : " + std::to_string(time));
        window.display();
    }
    sorting.join();
    return 0;
}
