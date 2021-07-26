// Simple Hello World program
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <unistd.h>
using boost::asio::ip::tcp;
//#pragma pack(push,r1,1)

struct PlatformData {
	unsigned char securityId = 0;
	unsigned char position = 0;
	unsigned char velocity = 0;
	unsigned char acceleration = 0;

};
//#pragma pack(pop,r1)

class tcp_client
{
public:
	tcp_client(boost::asio::io_context& io_context_,
			boost::asio::ip::tcp::endpoint endpoint
			)
	:socket(io_context_),endpoint_(endpoint),io_context(io_context_)
	{
		connect();
//		tcp::endpoint endpoint = *endpoint_iterator;

				      }
	void Close()
	    {
	        io_context.post(
	            boost::bind(&DoClose, this));
	    }
	    void DoClose()
	    {
	        socket.close();
	    }
		void connect()
		{
			socket.async_connect(endpoint_,
							            boost::bind(&tcp_client::handle_connect, this,
							            		boost::asio::placeholders::error));
		}
		  void handle_connect(const boost::system::error_code& err)
		  {
			  if (!err)
			      {
			        // The connection was successful. Send the request.
			        socket.async_read( boost::asio::buffer(&platformData,sizeof(platformData)),
			            boost::bind(&tcp_client::handle_read, this,
			              boost::asio::placeholders::error));
			        std::cout<<"server:"<<(int)platformData.velocity<<std::endl;
			      }
			      else
			      {
			        // The connection failed. Try the next endpoint in the list.
			        socket.close();
//			        tcp::endpoint endpoint = *endpoint_iterator;
			        socket.async_connect(endpoint_,
			            boost::bind(&tcp_client::handle_connect, this,
			              boost::asio::placeholders::error));
			      }
		  }
		  void handle_read(const boost::system::error_code& err)
		  {
			  platformData.position = 25;
			  socket.async_write( boost::asio::buffer(&platformData,sizeof(platformData)),
			  			            boost::bind(&tcp_client::handle_connect, this,
			  			              boost::asio::placeholders::error));
			  std::cout<<"handle_read"<<std::endl;
		  }


private:

	  tcp::socket socket;
	  tcp::endpoint endpoint_;
	  PlatformData platformData;
	  boost::asio::io_context &io_context;


};


int main()
{
    try
    {

        boost::asio::io_context io_context;

//        tcp::resolver Resolver(io_context);


//        tcp::resolver::query Query("127.0.0.1", port);
//
//        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);

        boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), 8080);

        tcp_client Client(io_context, endpoint);

//        std::thread thread1 = std::thread([](){boost::bind(io_context::run();});

        std::thread thread1 = std::thread([&](){io_context.run();});
//
//        std::this_thread::sleep_for(boost::asio::chrono::seconds(5));
        Client.Close();
        thread1.join();
//        io_context.run();

//        thread1.join();

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}
