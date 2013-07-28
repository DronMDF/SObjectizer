/*
	������ ����������� ������, ������� ����� ��������� ���������.
	����� ����� ��������� ������ ������������ ������ � ���� �� ���������.
	� ������ ������ ����� ���������� ������������� ��������.
	������ ���������� ��������� ������ � ��������� ���������,
	� ���������� ���������� ��������� �������� ������ SObjectizer Environment.
	������� �� ��������� � ��������� ����������� � ������� ��������� ���������.
*/

#include <iostream>
#include <time.h>

#include <ace/OS.h>
#include <ace/Time_Value.h>

// ��������� �������� ������������ ����� SObjectizer.
#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

// ������������� ���������.
class msg_periodic
	:
		public so_5::rt::message_t
{};

// ��������� ���������, ������� ����� ��������
// ��������� � ���������.
class state_monitor_t
	:
		public so_5::rt::agent_state_listener_t
{
	// ���������, ����� ��� ���������.
	const std::string m_type_hint;

	public:
		state_monitor_t( const std::string & type_hint )
			:
				m_type_hint( type_hint )
		{}

		virtual ~state_monitor_t()
		{}

		// ���������� ������� �� ����� ���������.
		virtual void
		changed(
			// �����, ��� ��������� ����������.
			so_5::rt::agent_t &,
			// ������� ��������� ������.
			const so_5::rt::state_t & state )
		{
			std::cout << m_type_hint << " agent changed state to "
				<< state.query_name()
				<< std::endl;
		}
};

// C++ �������� ������ ������.
class a_state_swither_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

		// ��������� ������:
		so_5::rt::state_t m_state_1;
		so_5::rt::state_t m_state_2;
		so_5::rt::state_t m_state_3;
		so_5::rt::state_t m_state_shutdown;

	public:
		a_state_swither_t( so_5::rt::so_environment_t & env )
			:
				base_type_t( env ),
				m_state_1( self_ptr(), "state_1" ),
				m_state_2( self_ptr(), "state_2" ),
				m_state_3( self_ptr(), "state_3" ),
				m_state_shutdown( self_ptr(), "state_shutdown" ),
				m_self_mbox( so_environment().create_local_mbox() )
		{}

		virtual ~a_state_swither_t()
		{}

		// ����������� ������.
		virtual void
		so_define_agent();

		// ��������� ������ ������ ������ � �������.
		virtual void
		so_evt_start();

		// ���������� ��������� msg_periodic � ��������� �� ���������.
		void
		evt_handler_default(
			const so_5::rt::event_data_t< msg_periodic > & );

		// ���������� ��������� msg_periodic � ��������� m_state_1.
		void
		evt_handler_1(
			const so_5::rt::event_data_t< msg_periodic > & );

		// ���������� ��������� msg_periodic � ��������� m_state_2.
		void
		evt_handler_2(
			const so_5::rt::event_data_t< msg_periodic > & );

		// ���������� ��������� msg_periodic � ��������� m_state_3.
		void
		evt_handler_3(
			const so_5::rt::event_data_t< msg_periodic > & );

		// ���������� ��������� msg_periodic � ��������� m_state_shutdown.
		void
		evt_handler_shutdown(
			const so_5::rt::event_data_t< msg_periodic > & );

	private:
		// Mbox ������� ������.
		so_5::rt::mbox_ref_t m_self_mbox;

		// ��������� ������������� ��� �������������� ���������.
		so_5::timer_thread::timer_id_ref_t m_timer_id;
};

void
a_state_swither_t::so_define_agent()
{
	// ������������� �� ���������.
	so_subscribe( m_self_mbox )
		.event( &a_state_swither_t::evt_handler_default );

	so_subscribe( m_self_mbox )
		.in( m_state_1 )
		.event( &a_state_swither_t::evt_handler_1 );

	so_subscribe( m_self_mbox )
		.in( m_state_2 )
		.event( &a_state_swither_t::evt_handler_2 );

	so_subscribe( m_self_mbox )
		.in( m_state_3 )
		.event( &a_state_swither_t::evt_handler_3 );

	so_subscribe( m_self_mbox )
		.in( m_state_shutdown )
		.event( &a_state_swither_t::evt_handler_shutdown );
}

void
a_state_swither_t::so_evt_start()
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "a_state_swither_t::so_evt_start()" << std::endl;

	// �������� ������������� ���������.
	m_timer_id =
		so_environment()
			.schedule_timer< msg_periodic >(
				m_self_mbox,
				1 * 1000,
				1 * 1000 );
}

void
a_state_swither_t::evt_handler_default(
	const so_5::rt::event_data_t< msg_periodic > & )
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "evt_handler_default" << std::endl;

	// ��������� � ��������� ���������.
	so_change_state( m_state_1 );
}

void
a_state_swither_t::evt_handler_1(
	const so_5::rt::event_data_t< msg_periodic > & )
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "evt_handler_1, state: " << so_current_state().query_name()
		<< std::endl;

	// ��������� � ��������� ���������.
	so_change_state( m_state_2 );
}

void
a_state_swither_t::evt_handler_2(
	const so_5::rt::event_data_t< msg_periodic > & )
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "evt_handler_2, state: " << so_current_state().query_name()
		<< std::endl;

	// ��������� � ��������� ���������.
	so_change_state( m_state_3 );
}

void
a_state_swither_t::evt_handler_3(
	const so_5::rt::event_data_t< msg_periodic > & )
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "evt_handler_3, state: " << so_current_state().query_name()
		<< std::endl;

	// ��������� � ��������� ���������.
	so_change_state( m_state_shutdown );
}

void
a_state_swither_t::evt_handler_shutdown(
	const so_5::rt::event_data_t< msg_periodic > & )
{
	time_t t = time( 0 );
	std::cout << asctime( localtime( &t ) )
		<< "evt_handler_shutdown, state: "
		<< so_current_state().query_name()
		<< std::endl;

	// ��������� � ��������� ���������.
	so_change_state( so_default_state() );

	std::cout << "Stop sobjectizer..." << std::endl;
	// ��������� ������.
	so_environment().stop();
}

// ������ ������������ ���������, ������� �� ������������ �������.
state_monitor_t g_state_monitor( "nondestroyable_listener" );

// ������������� ���������
void
init( so_5::rt::so_environment_t & env )
{
	// �����.
	so_5::rt::agent_ref_t ag( new a_state_swither_t( env ) );

	// ��������� ������������ ���������.
	ag->so_add_nondestroyable_listener( g_state_monitor );

	// ��������� ������������� ���������.
	ag->so_add_destroyable_listener(
		so_5::rt::agent_state_listener_unique_ptr_t(
			new state_monitor_t( "destroyable_listener" ) ) );

	// ������� ����������.
	so_5::rt::agent_coop_unique_ptr_t coop = env.create_coop(
		so_5::rt::nonempty_name_t( "coop" ) );

	// ��������� � ���������� ������.
	coop->add_agent( ag );

	// ������������ ����������.
	env.register_coop( std::move( coop ) );
}

int
main( int, char ** )
{
	try
	{
		so_5::api::run_so_environment( &init );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
