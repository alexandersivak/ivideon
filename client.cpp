// client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream> 
#include <utility>
#include <string>
#include <list>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
using boost::asio::ip::tcp;
enum { max_length = 1024 };
int main(int argc, char * argv[])
{
    if (argc != 3) 
    { 
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n"; 
        return 1; 
    }
    boost::asio::io_context io_context; 
    tcp::socket s(io_context); 
    tcp::resolver resolver(io_context); 
    boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));
    std::cout << "Connection Established!\n";
    std::list<std::pair<std::string, bool>> menu = {
        {"set-led-state", true}, // true means the function, or command, has an argument
        { "get-led-state", false },
        {"set-led-color", true},
        {"get-led-color", false},
        {"set-led-rate", true},
        {"get-led-rate", false},
        {}
    };
    std::list<std::string> allowed = { // each command parameter has a range of allowed values
        {"on|off"},
        {""},
        {"red|green|blue"},
        {""},
        {"1..5"},
        {""}
    };
    auto output_menu = [&menu]()
    {
        auto it = std::begin(menu);
        for (int i = 0; i < menu.size(); ++i, ++it)
        {
            std::cout << boost::lexical_cast<std::string>(i)+" - "+it->first;// e.g. 1 - ...
            std::cout << '\n';
        }
    };
    
    while (true)
    { 
        output_menu();
        std::cout << '>';
        unsigned j;
        std::cin >> j; // index 
        auto it = std::next(std::begin(menu), j);
        auto request = it->first;
        if (it->second)
        {
            std::cout << "Please, enter arg (" + *std::next(std::begin(allowed), j) + ")>";
            std::string arg;
            std::cin >> arg;
            request += ' ' + arg;
        }
        size_t request_length = request.size(); 
        std::cout << "Please, wait... Sending...\n";
        boost::asio::write(s, boost::asio::buffer(request.c_str(), request_length+1));
        //char reply[max_length];
        boost::asio::streambuf b;
        std::cout << "Please, wait... Receiving...\n";
        size_t reply_length = boost::asio::read_until(s, b, "\0"); //N.B.!!! I expect to receive a C-string back from a server
        std::cout << "Reply is: ";

        std::istream is(&b); 
        std::string s; 
        std::getline(is, s);
        std::cout << s;
        std::cout << "\n";
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
