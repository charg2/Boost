#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_EXECUTORS
#define BOOST_THREAD_FUTURE
#define BOOST_THREAD_USES_MOVE
#define BOOST_THREAD_PROVIDES_FUTURE_UNWRAP
#include <thread>
#include <vector>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/executors/executor.hpp>
#include <boost/thread/executors/inline_executor.hpp>
#include <boost/optional.hpp>
#include <boost/thread/future.hpp>
#include <concepts>
#include <queue>
#include <shared_mutex>

using namespace boost;
using namespace std;

template< typename T >
class LockQueue : public std::queue< T >
{
    using queue_type = std::queue< T >;

    std::shared_mutex _lock;

public:
    void push( const T& rhs )
    {
        std::unique_lock{ _lock };
        return queue_type::push( rhs );
    }
    void push( T&& rhs )
    {
        std::unique_lock{ _lock };
        return queue_type::push( rhs );
    }
    bool empty()
    {
        std::shared_lock{ _lock };
        return queue_type::empty();
    }
    size_t size()
    {
        std::shared_lock{ _lock };
        return queue_type::size();
    }
    queue_type pop_all()
    {
        std::unique_lock{ _lock };

        return std::move( *(queue_type*)this );
    }
    void clear()
    {
        auto z{ pop_all() };
    }

};

//struct IExecutor : public boost::executor
//{
//    ~IExecutor() = default;
//};

//template< typename T >
//struct Future : boost::future< T >
//{
    //template< std::invocable< T > >
    //Future operator|( Future&& f )
    //{
    //    // then 메서드를 사용하여 작업을 연결
    //    return this->then( [ f = std::forward<F>( f ) ]( boost::future< T > fut )
    //    {
    //        return boost::make_ready_future( f( fut.get() ) );
    //    } ).unwrap();

    //    //return then( std::move( f ) ).unwrap();
    //}
//};


template< typename T, typename F >
auto operator|( boost::future< T >& f, F&& func ) -> boost::future< decltype( func( f.get() ) ) >
{
    return f.then( [ func = std::forward< F >( func ) ]( boost::future< T > fut )
    {
        return boost::make_ready_future( func( fut.get() ) );
    } ).unwrap();
}

void main()
{
    LockQueue< int > q;
    q.push( 1 );
    q.push( 2 );
    q.push( 3 );

    q.clear();

    q.push( 4 );

    while ( !q.empty() )
    {
        std::cout << q.front() << std::endl;;
        q.pop();
    }


    //int lhs{};
    //int rhs{};

    //promise< int > p1;
    //promise< int > p2;
    //std::thread t{ [ & ]
    //{
    //    cin >> lhs;
    //    p1.set_value( lhs );
    //    p2.set_value( lhs );
    //} };

    //future< int > f1 = p1.get_future();
    //future< int > f2 = p2.get_future();

    ///// UNWRAP
    //auto ff = f1 | [ & ]( auto _ ){ return lhs * 3; };
    //cout << ff.get() << std::endl;


    ///// Default
    //auto ff2 = f2.then( [ & ]( auto _ )
    //{
    //    cin >> rhs;
    //    return boost::make_ready_future< int >( rhs * 3 );
    //} );
    //cout << ff2.get().get() << endl;

    //t.join();

    //auto fff{ ff.then( [ & ]( auto _ )
    //{
    //    return lhs * rhs;
    //} ) };

    //cout << fff.get() << endl;

}