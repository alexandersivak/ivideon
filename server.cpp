// server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <mutex>
#include <algorithm>

#include "LED.h"
#include "Command.h"

LED& get_LED()
{
    static LED l; // create our model
    return l;
}

class session : public std::enable_shared_from_this<session> 
{
    public: 
        
        session(boost::asio::ip::tcp::socket socket) : socket_(std::move(socket))
        { 
            std::cout << "New client!!!\n";
            ++counter;
        } 

        ~session()
        {
            std::cout << "Connection closed!!!\n";
        }

        void start() 
        { 
            do_read(); 
        } 

    private: 
        void do_read() 
        { 
            auto self(shared_from_this()); 
            socket_.async_read_some(
                boost::asio::buffer(data_),
                [this, self](boost::system::error_code ec, std::size_t length)
                {
                    std::lock_guard<std::mutex> g(mtx);
                    if (!ec) 
                    { 
                        // parse
                        // N.B.!!! I expect one command per a call

                        bool noerr = true;
                        std::unique_ptr<Command> ptrCommand;
                        try
                        {
                            std::string c(data_.data());
                            ptrCommand = parse(c);

                            if (false == ptrCommand->apply(get_LED()))
                            {
                                // failed
                                noerr = false;
                            }
                        }
                        catch (std::exception&)
                        {                           
                            // failed
                            noerr = false;
                        }

                        // process result
                        std::string feedback;
                        if (noerr)
                        {
                            feedback = "OK";
                            if (ptrCommand->has_result())
                            {
                                std::string s;
                                ptrCommand->get_result(get_LED(), s);
                                feedback += ' ' + s;
                            }
                            feedback += "\n";
                        }
                        else
                        {
                            feedback = "FAILED\n";
                        }
                        
                        // prepare a C-string
                        std::copy(std::begin(feedback), std::end(feedback), std::begin(self->data_));
                        self->data_[feedback.length()] = '\0';

                        length = feedback.length() + 1;
                        // pass a result
                        do_write(length); 
                    } 
                }); 
        } 
        
        void do_write(std::size_t length) 
        { 
            auto self(shared_from_this()); 
            boost::asio::async_write(
                socket_, 
                boost::asio::buffer(data_, length), 
                [this, self](boost::system::error_code ec, std::size_t /*length*/) 
                { 
                    if (!ec) 
                    { 
                        do_read(); 
                    } 
                }); 
        } 
        // The socket used to communicate with the client. 
        boost::asio::ip::tcp::socket socket_; 
        // Buffer used to store data received from the client. 
        std::array<char, 1024> data_;
        // One mutex for all sessions
        static std::mutex mtx;
        // Number of sessions 
        static unsigned counter;
};
std::mutex session::mtx;
unsigned session::counter;
class server 
{ 
    public: 
        server(boost::asio::io_context& io_context, short port) : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        { 
            do_accept(); 
        } 
    private: 
        void do_accept() 
        { 
            acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
            { 
                if (!ec) 
                { 
                    std::make_shared<session>(std::move(socket))->start(); 
                } 
                do_accept(); 
            }); 
        } 
        boost::asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
    try 
    { 
        if (argc != 2) 
        { 
            std::cerr << "Usage: server <port>\n"; 
            return 1; 
        } 
        boost::asio::io_context io_context; 
        server s(io_context, std::atoi(argv[1])); 
        io_context.run(); 
    }
    catch (std::exception& e) 
    { 
        std::cerr << "Exception: " << e.what() << "\n"; 
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
