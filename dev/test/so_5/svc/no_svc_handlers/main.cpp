/*
 * A test for the case where there isn't a svc_handler.
 */

#include <iostream>
#include <exception>
#include <sstream>

#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>
#include <so_5/h/types.hpp>

#include <so_5/disp/active_obj/h/pub.hpp>

#include "../a_time_sentinel.hpp"

struct msg_convert : public so_5::rt::message_t
	{
		int m_value;

		msg_convert( int value ) : m_value( value )
			{}
	};

class a_client_t
	:	public so_5::rt::agent_t
	{
	public :
		a_client_t(
			so_5::rt::so_environment_t & env,
			const so_5::rt::mbox_ref_t & svc_mbox )
			:	so_5::rt::agent_t( env )
			,	m_svc_mbox( svc_mbox )
			{}

		virtual so_5::rt::exception_reaction_t
		so_exception_reaction() const
			{
				return so_5::rt::abort_on_exception;
			}

		virtual void
		so_evt_start()
			{
				try
					{
						so_5::rt::service< std::string >( m_svc_mbox )
								.wait_forever().request( new msg_convert( 3 ) );

						std::cerr << "An exception no_svc_handlers excpected"
								<< std::endl;

						std::abort();
					}
				catch( const so_5::exception_t & x )
					{
						if( so_5::rc_no_svc_handlers != x.error_code() )
							{
								std::cerr << "Unexpected error_code: "
										<< x.error_code()
										<< ", expected: "
										<< so_5::rc_no_svc_handlers
										<< std::endl;

								std::abort();
							}
					}

				so_environment().stop();
			}

	private :
		const so_5::rt::mbox_ref_t m_svc_mbox;
	};

void
init(
	so_5::rt::so_environment_t & env )
	{
		auto coop = env.create_coop(
				so_5::rt::nonempty_name_t( "test_coop" ),
				so_5::disp::active_obj::create_disp_binder( "active_obj" ) );

		auto svc_mbox = env.create_local_mbox();

		coop->add_agent( new a_client_t( env, svc_mbox ) );
		coop->add_agent( new a_time_sentinel_t( env ) );

		env.register_coop( std::move( coop ) );
	}

int
main( int, char ** )
	{
		try
			{
				so_5::api::run_so_environment(
					&init,
					std::move(
						so_5::rt::so_environment_params_t()
							.add_named_dispatcher(
								so_5::rt::nonempty_name_t( "active_obj" ),
								so_5::disp::active_obj::create_disp() ) ) );
			}
		catch( const std::exception & ex )
			{
				std::cerr << "Error: " << ex.what() << std::endl;
				return 1;
			}

		return 0;
	}

