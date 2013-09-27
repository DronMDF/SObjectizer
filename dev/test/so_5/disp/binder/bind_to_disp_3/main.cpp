/*
 * A test of binding agents to active group dispatchers.
 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <map>

#include <ace/OS.h>
#include <ace/Time_Value.h>
#include <ace/Thread_Mutex.h>
#include <ace/Thread_Manager.h>
#include <ace/Guard_T.h>

#include <so_5/h/types.hpp>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

#include <so_5/disp/active_group/h/pub.hpp>

typedef std::map<
		ACE_thread_t,
		unsigned int >
	threads_count_map_t;

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

		static int
		agents_cout()
		{
			return 10;
		}

		static bool
		ok()
		{
			threads_count_map_t::const_iterator it =
				m_threads.begin();
			for(; it != m_threads.end(); ++it )
				if( agents_cout() != it->second )
				{
					std::cerr << it->first << " => " << it->second
						<< std::endl;
					return false;
				}

			return true;
		}

	private:
		static ACE_Thread_Mutex m_lock;
		static threads_count_map_t m_threads;
		static bool m_test_ok;
};

ACE_Thread_Mutex test_agent_t::m_lock;
std::map< ACE_thread_t, unsigned int > test_agent_t::m_threads;

void
test_agent_t::so_evt_start()
{
	ACE_thread_t tid = ACE_Thread_Manager::instance()->thr_self();

	ACE_Guard< ACE_Thread_Mutex > lock( m_lock );

	if( m_threads.end() == m_threads.find( tid ) )
		m_threads[ tid ] = 1;
	else
		++m_threads[ tid ];
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
push_group(
	so_5::rt::agent_coop_t & coop,
	const std::string & group_name,
	so_5::rt::so_environment_t & env )
{
	for( int i = 0; i < test_agent_t::agents_cout(); ++i )
	{
		coop.add_agent(
			so_5::rt::agent_ref_t( new test_agent_t( env ) ),
			so_5::disp::active_group::create_disp_binder(
				"active_group",
				group_name ) );
	}
}
void
init( so_5::rt::so_environment_t & env )
{
	so_5::rt::agent_coop_unique_ptr_t coop =
		env.create_coop( "test_coop" );

	push_group( *coop, "grp_1", env );
	push_group( *coop, "grp_2", env );
	push_group( *coop, "grp_3", env );
	push_group( *coop, "grp_4", env );

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
					test_agent_t::agents_cout() )
				.agent_event_queue_mutex_pool_size(
					test_agent_t::agents_cout() )
				.add_named_dispatcher(
					so_5::rt::nonempty_name_t( "active_group" ),
					so_5::disp::active_group::create_disp() ) );

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
