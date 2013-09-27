/*
 * A test of binding of agent to active object dispatcher.
 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <set>

#include <ace/OS.h>
#include <ace/Time_Value.h>
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>
#include <ace/Guard_T.h>

#include <so_5/h/types.hpp>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

#include <so_5/disp/active_obj/h/pub.hpp>

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

		virtual void
		so_evt_start();

		static int agents_cout()
		{
			return 20;
		}

		static bool
		ok()
		{
			return m_test_ok && agents_cout() == m_threads.size();
		}

	private:
		static ACE_Thread_Mutex m_lock;
		static std::set< ACE_thread_t > m_threads;
		static bool m_test_ok;
};

ACE_Thread_Mutex test_agent_t::m_lock;
std::set< ACE_thread_t > test_agent_t::m_threads;
bool test_agent_t::m_test_ok = true;

void
test_agent_t::so_evt_start()
{
	ACE_thread_t tid = ACE_Thread_Manager::instance()->thr_self();

	ACE_Guard< ACE_Thread_Mutex > lock( m_lock );

	if( m_threads.end() != m_threads.find( tid ) )
		m_test_ok = false;

	m_threads.insert( tid );
}


class test_agent_finisher_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		test_agent_finisher_t(
			so_5::rt::so_environment_t & env )
			:
				base_type_t( env )
		{}
		virtual ~test_agent_finisher_t()
		{}

		virtual void
		so_evt_start()
		{
			ACE_OS::sleep( ACE_Time_Value( 0, 200*1000 ) );
			so_environment().stop();
		}
};

void
init( so_5::rt::so_environment_t & env )
{
	so_5::rt::agent_coop_unique_ptr_t coop =
		env.create_coop( "test_coop" );

	for( int i = 0; i < test_agent_t::agents_cout(); ++i )
	{
		coop->add_agent(
			so_5::rt::agent_ref_t( new test_agent_t( env ) ),
			so_5::disp::active_obj::create_disp_binder(
				"active_obj" ) );
	}

	coop->add_agent(
		so_5::rt::agent_ref_t(
			new test_agent_finisher_t( env ) ) );

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
				.mbox_mutex_pool_size(
					test_agent_t::agents_cout()/2 )
				.agent_event_queue_mutex_pool_size(
					test_agent_t::agents_cout()/2 )
				.add_named_dispatcher(
					so_5::rt::nonempty_name_t( "active_obj" ),
					so_5::disp::active_obj::create_disp() ) );

		if( !test_agent_t::ok() )
			throw std::runtime_error( "!test_agent_t::ok()" );

		// Wait all threads to finish.
		ACE_Thread_Manager::instance()->wait();
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

