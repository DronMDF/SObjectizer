/*
	������ ����������� ������.
*/

#include <iostream>

// ��������� �������� ������������ ����� SObjectizer.
#include <so_5/rt/h/rt.hpp>
#include <so_5/api/h/api.hpp>
#include <so_5/rt/h/coop_listener.hpp>

// C++ �������� ������ ������.
class a_hello_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		a_hello_t( so_5::rt::so_environment_t & env )
			:
				 // �������� ����� SObjectizer, � ������� ����������� �����.
				base_type_t( env )
		{}

		virtual ~a_hello_t()
		{}
};

// ������������� ���������
void
init( so_5::rt::so_environment_t & env )
{
	// ������� ����������.
	so_5::rt::nonempty_name_t coop_name( "coop" );
	so_5::rt::agent_coop_unique_ptr_t coop = env.create_coop(
		coop_name );

	// ��������� � ���������� ������.
	coop->add_agent(
		so_5::rt::agent_ref_t(
			new a_hello_t( env ) ) );

	// ������������ ����������.
	env.register_coop( std::move( coop ) );

	env.stop();
}

class coop_listener_impl_t
	:
		public so_5::rt::coop_listener_t
{
	public:
		virtual ~coop_listener_impl_t()
		{}

		// ����� ��������� ����������� ����������.
		virtual void
		on_registered(
			so_5::rt::so_environment_t & so_env,
			const std::string & coop_name ) throw()
		{
			std::cout << "coop_listener: register coop '"
				<< coop_name << "'\n";
		}

		// ����� ��������� ������������� ����������.
		virtual void
		on_deregistered(
			so_5::rt::so_environment_t & so_env,
			const std::string & coop_name ) throw()
		{
			std::cout << "coop_listener: deregister coop '"
				<< coop_name << "'\n";
		}
};

int
main( int, char ** )
{
	try
	{
		so_5::api::run_so_environment(
			&init,
			so_5::rt::so_environment_params_t()
				.coop_listener(
					so_5::rt::coop_listener_unique_ptr_t(
						new coop_listener_impl_t ) )  );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
