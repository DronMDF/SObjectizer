/*
 * A test for handling of exception during so_define_agent() calling.
 */

#include <iostream>
#include <map>
#include <exception>
#include <stdexcept>
#include <cstdlib>

#include <ace/OS.h>
#include <ace/Time_Value.h>

#include <so_5/h/types.hpp>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>
#include <so_5/disp/active_obj/h/pub.hpp>

so_5::rt::nonempty_name_t g_test_mbox_name( "test_mbox" );

struct some_message : public so_5::rt::signal_t {};

class a_ordinary_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:

		a_ordinary_t( so_5::rt::so_environment_t & env )
			:
				base_type_t( env )
		{}

		virtual ~a_ordinary_t()
		{}

		virtual void
		so_define_agent()
		{
			so_5::rt::mbox_ref_t mbox = so_environment()
				.create_local_mbox( g_test_mbox_name );

			so_subscribe( mbox )
				.in( so_default_state() )
					.event( &a_ordinary_t::some_handler );

			// Give some time to agent which sends messages.
			ACE_OS::sleep( ACE_Time_Value( 0, 10*1000 ) );
		}

		virtual void
		so_evt_start();

		void
		some_handler(
			const so_5::rt::event_data_t< some_message > & );
};

void
a_ordinary_t::so_evt_start()
{
	// This method should not be called.
	std::cerr << "error: a_ordinary_t::so_evt_start called.";
	std::abort();
}



void
a_ordinary_t::some_handler(
	const so_5::rt::event_data_t< some_message > & )
{
	// This method should not be called.
	std::cerr << "error: a_ordinary_t::some_handler called.";
	std::abort();
}

class a_throwing_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		a_throwing_t( so_5::rt::so_environment_t & env )
			:
				base_type_t( env )
		{}

		virtual ~a_throwing_t()
		{}

		virtual void
		so_define_agent()
		{
			throw std::runtime_error(
				"test throwing while defining agent" );
		}

		virtual void
		so_evt_start();
};

void
a_throwing_t::so_evt_start()
{
	// This method should not be called.
	std::cerr << "error: a_throwing_t::so_evt_start called.";
	std::abort();
}

// An agent which sends messages.
class a_message_sender_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:

		a_message_sender_t( so_5::rt::so_environment_t & env )
			:
				base_type_t( env )
		{}

		virtual ~a_message_sender_t()
		{}

		virtual void
		so_define_agent()
		{}

		virtual void
		so_evt_start()
		{
			so_5::rt::mbox_ref_t mbox =
				so_environment().create_local_mbox( g_test_mbox_name );

			ACE_Time_Value time_limit = ACE_OS::gettimeofday();
			time_limit += ACE_Time_Value( 0, 150*1000 );

			int msg_cnt = 0;
			while( time_limit > ACE_OS::gettimeofday() )
			{
				mbox->deliver_signal< some_message >();
				++msg_cnt;
			}
		}
};

void
reg_message_sender(
	so_5::rt::so_environment_t & env )
{
	so_5::rt::agent_coop_unique_ptr_t coop =
		env.create_coop(
			"message_sender_coop",
			so_5::disp::active_obj::create_disp_binder(
				"active_obj" ) );

	coop->add_agent(
		so_5::rt::agent_ref_t( new a_message_sender_t( env ) ) );
}

void
reg_coop(
	so_5::rt::so_environment_t & env )
{
	so_5::rt::agent_coop_unique_ptr_t coop =
		env.create_coop( "test_coop" );

	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );

	// An agent which will throw an exception.
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_throwing_t( env ) ) );

	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );
	coop->add_agent( so_5::rt::agent_ref_t(
		new a_ordinary_t( env ) ) );

	try {
		env.register_coop( std::move( coop ) );
	}
	catch(...) {}
}

void
init( so_5::rt::so_environment_t & env )
{
	reg_message_sender( env );
	reg_coop( env );
	env.stop();
}

int
main( int argc, char * argv[] )
{
	try
	{
		so_5::api::run_so_environment(
			&init,
			so_5::rt::so_environment_params_t()
				.mbox_mutex_pool_size( 4 )
				.agent_event_queue_mutex_pool_size( 4 )
				.add_named_dispatcher(
					"active_obj",
					so_5::disp::active_obj::create_disp() ) );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
