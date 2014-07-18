/*
 * A test of trying to bind agent to missing dispatcher.
 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <map>

#include <ace/OS_main.h>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

#include <so_5/disp/one_thread/h/pub.hpp>

class test_agent_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		test_agent_t(
			so_5::rt::so_environment_t & env )
			:
				base_type_t( env )
		{}

		virtual ~test_agent_t()
		{}
};

void
init( so_5::rt::so_environment_t & env )
{
	bool exception_thrown = false;
	try
	{
		env.register_agent_as_coop(
				"test_coop",
				new test_agent_t( env ),
				so_5::disp::one_thread::create_disp_binder( "NODISP" ) );
	}
	catch( const so_5::exception_t & )
	{
		exception_thrown = true;
	}

	if( !exception_thrown )
		throw std::runtime_error( "invalid coop registered" );

	env.stop();
}

int
main( int argc, char * argv[] )
{
	try
	{
		so_5::api::run_so_environment(
			&init,
			std::move(
				so_5::rt::so_environment_params_t()
					.mbox_mutex_pool_size( 2 )
					.agent_event_queue_mutex_pool_size( 2 ) ) );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
