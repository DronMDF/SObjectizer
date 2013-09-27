/*
 * A test for messages handling in difference states.
 */

#include <iostream>
#include <exception>
#include <stdexcept>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

struct test_message : public so_5::rt::signal_t {};

class test_agent_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

		const so_5::rt::state_t m_first_state;
		const so_5::rt::state_t m_second_state;
		const so_5::rt::state_t m_third_state;

	public:

		test_agent_t(
			so_5::rt::so_environment_t & env )
			:
				base_type_t( env ),
				m_first_state( self_ptr() ),
				m_second_state( self_ptr() ),
				m_third_state( self_ptr() ),
				m_test_mbox( so_environment().create_local_mbox() )
		{}

		virtual ~test_agent_t()
		{}

		virtual void
		so_define_agent();

		virtual void
		so_evt_start();

		void
		evt_in_state_default(
			const so_5::rt::event_data_t< test_message > & );

		void
		evt_in_state_1(
			const so_5::rt::event_data_t< test_message > & );

		void
		evt_in_state_2(
			const so_5::rt::event_data_t< test_message > & );

		void
		evt_in_state_3(
			const so_5::rt::event_data_t< test_message > & );

		// Count for event handlers calls.
		static int m_handler_in_state_default_calls;
		static int m_handler_in_state_1_calls;
		static int m_handler_in_state_2_calls;
		static int m_handler_in_state_3_calls;

	private:
		so_5::rt::mbox_ref_t m_test_mbox;
};

int test_agent_t::m_handler_in_state_default_calls = 0;
int test_agent_t::m_handler_in_state_1_calls = 0;
int test_agent_t::m_handler_in_state_2_calls = 0;
int test_agent_t::m_handler_in_state_3_calls = 0;

void
test_agent_t::so_define_agent()
{
	// Subscribe to message in default state...
	so_subscribe( m_test_mbox )
			.event(
				&test_agent_t::evt_in_state_default );

	// ...in first state...
	so_subscribe( m_test_mbox )
		.in( m_first_state )
			.event(
				&test_agent_t::evt_in_state_1 );

	// ...in second state...
	so_subscribe( m_test_mbox )
		.in( m_second_state )
			.event(
				&test_agent_t::evt_in_state_2 );

	// ...in third state.
	so_subscribe( m_test_mbox )
		.in( m_third_state )
			.event(
				&test_agent_t::evt_in_state_3 );
}

void
test_agent_t::so_evt_start()
{
	m_test_mbox->deliver_signal< test_message >();
}

void
test_agent_t::evt_in_state_default(
	const so_5::rt::event_data_t< test_message > & )
{
	++m_handler_in_state_default_calls;
	m_test_mbox->deliver_signal< test_message >();

	// Change state after message has been sent.
	so_change_state( m_first_state );
}

void
test_agent_t::evt_in_state_1(
	const so_5::rt::event_data_t< test_message > & )
{
	++m_handler_in_state_1_calls;

	// Change state before message will be sent.
	so_change_state( m_second_state );

	m_test_mbox->deliver_signal< test_message >();
}

void
test_agent_t::evt_in_state_2(
	const so_5::rt::event_data_t< test_message > & )
{
	++m_handler_in_state_2_calls;

	// Change state before message will be sent.
	so_change_state( m_third_state );

	m_test_mbox->deliver_signal< test_message >();
}

void
test_agent_t::evt_in_state_3(
	const so_5::rt::event_data_t< test_message > & )
{
	++m_handler_in_state_3_calls;

	std::cout << "Stop\n";

	// Shutting down.
	so_environment().stop();
}

void
init( so_5::rt::so_environment_t & env )
{
	so_5::rt::agent_coop_unique_ptr_t coop =
		env.create_coop( "test_coop" );

	coop->add_agent(
		so_5::rt::agent_ref_t( new test_agent_t( env ) ) );

	env.register_coop( std::move( coop ) );
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
				.agent_event_queue_mutex_pool_size( 4 ) );

		if( test_agent_t::m_handler_in_state_default_calls != 1 ||
			test_agent_t::m_handler_in_state_1_calls != 1 ||
			test_agent_t::m_handler_in_state_2_calls != 1 ||
			test_agent_t::m_handler_in_state_3_calls != 1 )
		{
			std::cerr
				<< "test_agent_t::m_handler_in_state_default_calls = "
				<< test_agent_t::m_handler_in_state_default_calls << "\n"
				<< "test_agent_t::m_handler_in_state_1_calls = "
				<< test_agent_t::m_handler_in_state_1_calls << "\n"
				<< "test_agent_t::m_handler_in_state_2_calls = "
				<< test_agent_t::m_handler_in_state_2_calls << "\n"
				<< "test_agent_t::m_handler_in_state_3_calls = "
				<< test_agent_t::m_handler_in_state_3_calls << "\n";

			throw std::runtime_error( "handler calls count error" );
		}

	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
