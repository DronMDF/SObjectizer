/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ���������� ����� ���������� so_5.
*/

#if !defined( _SO_5__RT__IMPL__SO_ENVIRONMENT_IMPL_HPP_ )
#define _SO_5__RT__IMPL__SO_ENVIRONMENT_IMPL_HPP_

#include <so_5/rt/impl/h/layer_core.hpp>
#include <so_5/rt/h/so_environment.hpp>

#include <so_5/rt/impl/h/mbox_core.hpp>
#include <so_5/rt/impl/h/agent_core.hpp>
#include <so_5/rt/impl/h/disp_core.hpp>
#include <so_5/rt/impl/h/layer_core.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// so_environment_impl_t
//

//! ���������� ����� so_5.
class so_environment_impl_t
{
	public:
		explicit so_environment_impl_t(
			const so_environment_params_t & so_environment_params,
			so_environment_t & public_so_environment );

		~so_environment_impl_t();


		//! \name ������ � mbox-���.
		//! \{
		inline mbox_ref_t
		create_local_mbox()
		{
			return m_mbox_core->create_local_mbox();
		}

		inline mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & nonempty_name )
		{
			return m_mbox_core->create_local_mbox( nonempty_name );
		}

		inline mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & nonempty_name,
			//! ����� ��������� �������������
			std::unique_ptr< ACE_RW_Thread_Mutex >
				lock_ptr )
		{
			return m_mbox_core->create_local_mbox(
				nonempty_name,
				std::move( lock_ptr ) );
		}

		inline mbox_ref_t
		create_local_mbox(
			//! ����� ��������� �������������
			std::unique_ptr< ACE_RW_Thread_Mutex >
				lock_ptr )
		{
			return m_mbox_core->create_local_mbox( std::move( lock_ptr ) );
		}
		//! \}

		//! ������� ������ ��� ���������� �������.
		inline ACE_Thread_Mutex &
		create_agent_coop_mutex()
		{
			return m_agent_core.allocate_agent_coop_mutex();
		}

		//! ���������� ������ ��� ���������� �������.
		inline void
		destroy_agent_coop_mutex( ACE_Thread_Mutex & m )
		{
			return m_agent_core.deallocate_agent_coop_mutex( m );
		}

		//! ������� ��������� ������� ��� ������.
		inline local_event_queue_unique_ptr_t
		create_local_queue()
		{
			return m_agent_core.create_local_queue();
		}

		//! \name ����������.
		//! \{

		//! �������� ��������� �� ���������.
		inline dispatcher_t &
		query_default_dispatcher()
		{
			return m_disp_core.query_default_dispatcher();
		}

		//! �������� ��������� �� �����.
		inline dispatcher_ref_t
		query_named_dispatcher(
			const std::string & disp_name )
		{
			return m_disp_core.query_named_dispatcher( disp_name );
		}

		//! ���������� ����� ����������.
		inline void
		install_exception_logger(
			event_exception_logger_unique_ptr_t && logger )
		{
			m_disp_core.install_exception_logger( std::move( logger ) );
		}

		//! ���������� ���������� ����������.
		inline void
		install_exception_handler(
			event_exception_handler_unique_ptr_t && handler )
		{
			m_disp_core.install_exception_handler( std::move( handler ) );
		}
		//! \}

		//! \name ������ � ������������.
		//! \{

		//! ���������������� ����������.
		ret_code_t
		register_coop(
			//! ����������, ������� ������� ���� ����������������.
			agent_coop_unique_ptr_t agent_coop,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy )
		{
			return m_agent_core.register_coop(
				std::move( agent_coop ),
				throwing_strategy );
		}

		//! ���������������� ����������.
		ret_code_t
		deregister_coop(
			//! ��� ���������������� ����������.
			const nonempty_name_t & name,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy )
		{
			return m_agent_core.deregister_coop(
				name, throwing_strategy );
		}

		//! ��������� � ���������� ����������
		//! � ������������� �������������.
		inline void
		ready_to_deregister_notify(
			agent_coop_t * coop )
		{
			m_agent_core.ready_to_deregister_notify( coop );
		}

		//! ������������ ���������������� ����������.
		inline void
		final_deregister_coop(
			//! ��� ���������������� ����������.
			const std::string & coop_name )
		{
			m_agent_core.final_deregister_coop( coop_name );
		}
		//! \}

		//! \name ������ � ���������� ���������.
		//! \{

		//! ������������� ��������� �������.
		so_5::timer_thread::timer_id_ref_t
		schedule_timer(
			//! ��� ���������
			const rt::type_wrapper_t & type_wrapper,
			//! ���������
			const message_ref_t & msg,
			//! mbox �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ������ ��������� ���������.
			unsigned int delay_msec,
			//! ������ �������� ���������, ��� ���������� ���������
			//! period_msec == 0.
			unsigned int period_msec );

		//! ������������� ��������� ��������� �������,
		//! ������� ������ ��������.
		void
		single_timer(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ���������.
			const message_ref_t & msg,
			//! mbox �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ��������� ���������.
			unsigned int delay_msec );

		//! \}

		//! \name ������ ������.
		//! \{
		so_layer_t *
		query_layer(
			const type_wrapper_t & type ) const;

		//! �������� �������������� ����.
		/*!
			\see layer_core_t::add_extra_layer().
		*/
		ret_code_t
		add_extra_layer(
			const type_wrapper_t & type,
			const so_layer_ref_t & layer,
			throwing_strategy_t throwing_strategy );
		//! \}

		//! \name ������, ������������� � ���������.
		//! \{
		ret_code_t
		run(
			so_environment_t & env,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );

		void
		stop();
		//! \}

		//! �������� ������ �� ��������� ��������� so_environment.
		so_environment_t &
		query_public_so_environment();

	private:
		//! ���� ���������� mbox-��.
		mbox_core_ref_t m_mbox_core;

		//! ���� ���������� �������.
		agent_core_t m_agent_core;

		//! ���� ���������� �����������.
		disp_core_t m_disp_core;

		//! ���� ���������� �����.
		layer_core_t m_layer_core;

		//! ������ �� ��������� ��������� ����� SO.
		so_environment_t & m_public_so_environment;

		//! ��������� ����.
		so_5::timer_thread::timer_thread_unique_ptr_t
			m_timer_thread;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
