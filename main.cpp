#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_EXECUTORS
#define BOOST_THREAD_FUTURE
#define BOOST_THREAD_USES_MOVE
#include <thread>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/thread/executors/executor.hpp>
#include <boost/thread/executors/inline_executor.hpp>
#include <boost/optional.hpp>
#include <boost/thread/future.hpp>

using namespace boost;
using namespace std;

void main()
{
    promise< int > prom;
    std::thread t{ [ & ]
    {
        int i{};
        cin >> i;
        prom.set_value( i );
    } };

    future< int > f = prom.get_future();
    cout << f.get() << endl;

    t.join();
}