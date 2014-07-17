/*
 * A benchmark for sending messages to M mboxes and to N agents.
 */

#include <iostream>
#include <iterator>
#include <numeric>
#include <chrono>
#include <functional>
#include <sstream>
#include <cstdlib>

#include <ace/OS.h>
#include <ace/Get_Opt.h>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>

#include <test/so_5/bench/benchmark_helpers.hpp>

struct cfg_t
	{
		std::size_t m_mboxes;
		std::size_t m_agents;
		std::size_t m_msg_types;
		std::size_t m_iterations;

		cfg_t()
			:	m_mboxes( 1024 )
			,	m_agents( 512 )
			,	m_msg_types( 4 )
			,	m_iterations( 10 )
			{}

		cfg_t(
			std::size_t mboxes,
			std::size_t agents,
			std::size_t msg_types,
			std::size_t iterations )
			:	m_mboxes( mboxes )
			,	m_agents( agents )
			,	m_msg_types( msg_types )
			,	m_iterations( iterations )
			{}
	};

std::size_t
arg_to_value( const char * arg )
	{
		std::stringstream ss;
		ss << arg;
		ss.seekg(0);

		std::size_t r;
		ss >> r;

		if( !ss )
			throw std::runtime_error(
					std::string( "unable to parse value: " ) + arg );

		return r;
	}

cfg_t
try_parse_cmdline(
	int argc,
	char ** argv )
{
	ACE_Get_Opt opt( argc, argv, ":m:a:t:i:h" );
	if( -1 == opt.long_option(
			"mboxes", 'm', ACE_Get_Opt::ARG_REQUIRED ) )
		throw std::runtime_error( "Unable to set long option 'mboxes'"  );
	if( -1 == opt.long_option(
			"agents", 'a', ACE_Get_Opt::ARG_REQUIRED ) )
		throw std::runtime_error( "Unable to set long option 'agents'" );
	if( -1 == opt.long_option(
			"types", 't', ACE_Get_Opt::ARG_REQUIRED ) )
		throw std::runtime_error( "Unable to set long option 'types'"  );
	if( -1 == opt.long_option(
			"iterations", 'i', ACE_Get_Opt::ARG_REQUIRED ) )
		throw std::runtime_error( "Unable to set long option 'iterations'" );
	if( -1 == opt.long_option(
			"help", 'h', ACE_Get_Opt::NO_ARG ) )
		throw std::runtime_error( "Unable to set long option 'iterations'" );

	cfg_t tmp_cfg;

	int o;
	while( EOF != ( o = opt() ) )
		{
			switch( o )
				{
				case 'h' :
					std::cout << "usage:\n"
							"_test.bench.so_5.many_mboxes <options>\n"
							"\noptions:\n"
							"-m, --mboxes      count of mboxes\n"
							"-a, --agents      count of agents\n"
							"-t, --types       count of message types\n"
							"-i, --iterations  count of iterations for every message type\n"
							"-h, --help        show this description\n"
							<< std::endl;
					std::exit(1);
				break;

				case 'm' :
					tmp_cfg.m_mboxes = arg_to_value( opt.opt_arg() );
				break;

				case 'a' :
					tmp_cfg.m_agents = arg_to_value( opt.opt_arg() );
				break;

				case 't' :
					tmp_cfg.m_msg_types = arg_to_value( opt.opt_arg() );
				break;

				case 'i' :
					tmp_cfg.m_iterations = arg_to_value( opt.opt_arg() );
				break;

				case ':' :
					{
						std::ostringstream ss;
						ss << "-" << opt.opt_opt() << " requires an argument";
						throw std::runtime_error( ss.str() );
					}
				}
		}

	if( opt.opt_ind() < argc )
		throw std::runtime_error(
				std::string( "unknown argument: " ) + argv[ opt.opt_ind() ] );

	return tmp_cfg;
}

#define DECLARE_SIGNAL_TYPE(I) \
	struct msg_signal_##I : public so_5::rt::signal_t {}

DECLARE_SIGNAL_TYPE(0);
DECLARE_SIGNAL_TYPE(1);
DECLARE_SIGNAL_TYPE(2);
DECLARE_SIGNAL_TYPE(3);
DECLARE_SIGNAL_TYPE(4);
DECLARE_SIGNAL_TYPE(5);
DECLARE_SIGNAL_TYPE(6);
DECLARE_SIGNAL_TYPE(7);
DECLARE_SIGNAL_TYPE(8);
DECLARE_SIGNAL_TYPE(9);
DECLARE_SIGNAL_TYPE(10);
DECLARE_SIGNAL_TYPE(11);
DECLARE_SIGNAL_TYPE(12);
DECLARE_SIGNAL_TYPE(13);
DECLARE_SIGNAL_TYPE(14);
DECLARE_SIGNAL_TYPE(15);
DECLARE_SIGNAL_TYPE(16);
DECLARE_SIGNAL_TYPE(17);
DECLARE_SIGNAL_TYPE(18);
DECLARE_SIGNAL_TYPE(19);
DECLARE_SIGNAL_TYPE(20);
DECLARE_SIGNAL_TYPE(21);
DECLARE_SIGNAL_TYPE(22);
DECLARE_SIGNAL_TYPE(23);
DECLARE_SIGNAL_TYPE(24);
DECLARE_SIGNAL_TYPE(25);
DECLARE_SIGNAL_TYPE(26);
DECLARE_SIGNAL_TYPE(27);
DECLARE_SIGNAL_TYPE(28);
DECLARE_SIGNAL_TYPE(29);
DECLARE_SIGNAL_TYPE(30);
DECLARE_SIGNAL_TYPE(31);

#undef DECLARE_SIGNAL_TYPE

struct msg_start : public so_5::rt::signal_t {};
struct msg_shutdown : public so_5::rt::signal_t {};
struct msg_next_iteration : public so_5::rt::signal_t {};

class a_worker_t
	:	public so_5::rt::agent_t
	{
	public :
		a_worker_t(
			so_5::rt::so_environment_t & env )
			:	so_5::rt::agent_t( env )
			,	m_signals_received( 0 )
			{
			}

		virtual void
		evt_signal()
			{
				++m_signals_received;
			}

	private :
		unsigned long m_signals_received;
	};

template< class SIGNAL >
class a_sender_t
	:	public so_5::rt::agent_t
	{
	public :
		a_sender_t(
			so_5::rt::so_environment_t & env,
			const so_5::rt::mbox_ref_t & common_mbox,
			std::size_t iterations,
			const std::vector< so_5::rt::mbox_ref_t > & mboxes,
			const std::vector< a_worker_t * > & workers )
			:	so_5::rt::agent_t( env )
			,	m_common_mbox( common_mbox )
			,	m_iterations_left( iterations )
			,	m_mboxes( mboxes )
			{
				for( auto a : workers )
					for( auto & m : m_mboxes )
						a->so_subscribe( m ).event(
								so_5::signal< SIGNAL >,
								&a_worker_t::evt_signal );
			}

		virtual void
		so_define_agent()
			{
				so_subscribe( m_common_mbox ).event(
						so_5::signal< msg_start >,
						&a_sender_t::evt_start );

				so_subscribe( so_direct_mbox() ).event(
						so_5::signal< msg_next_iteration >,
						&a_sender_t::evt_next_iteration );
			}

		void
		evt_start()
			{
				try_start_next_iteration();
			}

		void
		evt_next_iteration()
			{
				try_start_next_iteration();
			}

	private :
		const so_5::rt::mbox_ref_t m_common_mbox;
		std::size_t m_iterations_left;
		const std::vector< so_5::rt::mbox_ref_t > & m_mboxes;

		void
		try_start_next_iteration()
			{
				if( m_iterations_left )
					{
						for( auto & m : m_mboxes )
							m->deliver_signal< SIGNAL >();

						so_direct_mbox()->deliver_signal< msg_next_iteration >();

						--m_iterations_left;
					}
				else
					m_common_mbox->deliver_signal< msg_shutdown >();
			}
	};

class a_starter_stopper_t
	:	public so_5::rt::agent_t
	{
	public :
		a_starter_stopper_t(
			so_5::rt::so_environment_t & env,
			const cfg_t & cfg )
			:	so_5::rt::agent_t( env )
			,	m_common_mbox( env.create_local_mbox() )
			,	m_cfg( cfg )
			,	m_agents_finished( 0 )
			{
				create_sender_factories();
			}

		static const std::size_t max_msg_types = 32;

		virtual void
		so_define_agent()
			{
				so_subscribe( m_common_mbox )
						.event( so_5::signal< msg_shutdown >,
								&a_starter_stopper_t::evt_shutdown );
			}

		virtual void
		so_evt_start()
			{
				std::cout << "* mboxes: " << m_cfg.m_mboxes << "\n"
						<< "* agents: " << m_cfg.m_agents << "\n"
						<< "* msg_types: " << m_cfg.m_msg_types << "\n"
						<< "* iterations: " << m_cfg.m_iterations << std::endl;

				create_child_coop();

				m_benchmark.start();

				m_common_mbox->deliver_signal< msg_start >();
			}

		virtual void
		evt_shutdown()
			{
				++m_agents_finished;
				if( m_agents_finished == m_cfg.m_msg_types )
					{
						auto messages =
								static_cast< unsigned long long >( m_cfg.m_agents ) *
								m_cfg.m_mboxes *
								m_cfg.m_msg_types *
								m_cfg.m_iterations;

						m_benchmark.finish_and_show_stats( messages, "messages" );

						so_environment().stop();
					}
			}

	private :
		const so_5::rt::mbox_ref_t m_common_mbox;

		const cfg_t m_cfg;

		std::size_t m_agents_finished;

		benchmarker_t m_benchmark;

		std::vector< so_5::rt::mbox_ref_t > m_mboxes;
		std::vector< a_worker_t * > m_workers;

		typedef std::function< so_5::rt::agent_t *() >
			sender_factory_t;

		std::vector< sender_factory_t > m_sender_factories;

		void
		create_sender_factories()
			{
				m_sender_factories.reserve( m_cfg.m_msg_types );

#define MAKE_SENDER_FACTORY(I)\
				m_sender_factories.emplace_back( \
						[this]() \
						{ \
							return new a_sender_t< msg_signal_##I >( so_environment(),  \
									m_common_mbox, \
									m_cfg.m_iterations, \
									m_mboxes, \
									m_workers ); \
						} )

				MAKE_SENDER_FACTORY(0);
				MAKE_SENDER_FACTORY(1);
				MAKE_SENDER_FACTORY(2);
				MAKE_SENDER_FACTORY(3);
				MAKE_SENDER_FACTORY(4);
				MAKE_SENDER_FACTORY(5);
				MAKE_SENDER_FACTORY(6);
				MAKE_SENDER_FACTORY(7);
				MAKE_SENDER_FACTORY(8);
				MAKE_SENDER_FACTORY(9);
				MAKE_SENDER_FACTORY(10);
				MAKE_SENDER_FACTORY(11);
				MAKE_SENDER_FACTORY(12);
				MAKE_SENDER_FACTORY(13);
				MAKE_SENDER_FACTORY(14);
				MAKE_SENDER_FACTORY(15);
				MAKE_SENDER_FACTORY(16);
				MAKE_SENDER_FACTORY(17);
				MAKE_SENDER_FACTORY(18);
				MAKE_SENDER_FACTORY(19);
				MAKE_SENDER_FACTORY(20);
				MAKE_SENDER_FACTORY(21);
				MAKE_SENDER_FACTORY(22);
				MAKE_SENDER_FACTORY(23);
				MAKE_SENDER_FACTORY(24);
				MAKE_SENDER_FACTORY(25);
				MAKE_SENDER_FACTORY(26);
				MAKE_SENDER_FACTORY(27);
				MAKE_SENDER_FACTORY(28);
				MAKE_SENDER_FACTORY(29);
				MAKE_SENDER_FACTORY(30);
				MAKE_SENDER_FACTORY(31);

#undef MAKE_SENDER_FACTORY
			}

		void
		create_child_coop()
			{
				std::cout << "creating child coop..." << std::endl;

				m_mboxes.reserve( m_cfg.m_mboxes );
				for( std::size_t i = 0; i != m_cfg.m_mboxes; ++i )
					m_mboxes.emplace_back( so_environment().create_local_mbox() );

				auto coop = so_environment().create_coop( "child" );
				coop->set_parent_coop_name( so_coop_name() );
				
				m_workers.reserve( m_cfg.m_agents );
				for( std::size_t i = 0; i != m_cfg.m_agents; ++i )
					{
						m_workers.push_back( new a_worker_t( so_environment() ) );
						coop->add_agent( m_workers.back() );
					}

				for( std::size_t i = 0; i != m_cfg.m_msg_types; ++i )
					{
						std::unique_ptr< so_5::rt::agent_t > sender(
								m_sender_factories[i]() );
						coop->add_agent( std::move( sender ) );
					}

				so_environment().register_coop( std::move( coop ) );

				std::cout << "child coop created..." << std::endl;
			}
	};

int
main( int argc, char ** argv )
{
	try
	{
		cfg_t cfg = try_parse_cmdline( argc, argv );
		if( cfg.m_msg_types > a_starter_stopper_t::max_msg_types )
			{
				std::ostringstream ss;
				ss << "too many msg_types specified: "
					<< cfg.m_msg_types
					<< ", max avaliable msg_types: "
					<< a_starter_stopper_t::max_msg_types;

				throw std::logic_error( ss.str() );
			}

		so_5::api::run_so_environment(
			[cfg]( so_5::rt::so_environment_t & env )
			{
				env.register_agent_as_coop( "test",
						new a_starter_stopper_t( env, cfg ) );
			} );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

