/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ��� ����������� ������� � �� ���������� ����������.
*/

#if !defined( _SO_5__RT__IMPL__AGENT_CORE_HPP_ )
#define _SO_5__RT__IMPL__AGENT_CORE_HPP_

#include <memory>
#include <string>
#include <map>

#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>

#include <so_5/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/util/h/mutex_pool.hpp>

#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/agent_coop.hpp>
#include <so_5/rt/h/coop_listener.hpp>

#include <so_5/rt/impl/h/local_event_queue.hpp>

#include <so_5/rt/impl/coop_dereg/h/coop_dereg_executor_thread.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// agent_core_t
//

//! �����, ��� ����������� ������� ����������.
class agent_core_t
{
		agent_core_t( const agent_core_t & );
		void
		operator = ( const agent_core_t & );

	public:
		explicit agent_core_t(
			//! ����� SObjectizer.
			so_environment_t & so_environment_impl,
			//! ������ ���� �������� ��� ����������.
			unsigned int agent_coop_mutex_pool,
			//! ������ ���� �������� ��� ��������� �������� �������.
			unsigned int agent_queue_mutex_pool_size,
			//! ��������� �������� ��� ������������.
			coop_listener_unique_ptr_t coop_listener );

		~agent_core_t();

		//! ���������������� ����.
		void
		start();

		//! ������������ ���������.
		void
		shutdown();

		//! ������� ���������.
		void
		wait();

		//! �������� � �������� �����������.
		//! \{
		//! ����� ������ � �����������.
		ACE_Thread_Mutex &
		allocate_agent_coop_mutex();

		//! ���������� �� ������������� �������.
		void
		deallocate_agent_coop_mutex(
			ACE_Thread_Mutex & m );
		//! \}

		//! ������� ��������� ������� ��� ������.
		local_event_queue_unique_ptr_t
		create_local_queue();

		//! ���������������� ����������.
		ret_code_t
		register_coop(
			//! ����������, ������� ������� ���� ����������������.
			agent_coop_unique_ptr_t agent_coop,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );

		//! ���������������� ����������.
		ret_code_t
		deregister_coop(
			//! ��� ���������������� ����������.
			const nonempty_name_t & name,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );


		//! ��������� � ���������� ����������
		//! � ������������� �������������.
		void
		ready_to_deregister_notify(
			agent_coop_t * coop );

		//! ������������ ���������������� ����������.
		void
		final_deregister_coop(
			//! ��� ���������������� ����������.
			/*!
				\note �������� ������ ���� ������� ������,
				�.�. � ������ ���������� ��� ������ ����������,
				� � ���������� ������ �� ������������. �������,
				��������� ������ �� �������� �������, ������� �����
				��������� - �� ���������.
			*/
			const std::string coop_name );

		//! ������������ ������ �������������.
		void
		start_deregistration();

		//! ������� ������� � ������ �������������.
		void
		wait_for_start_deregistration();

		//! ���������������� ��� ����������.
		/*!
			����� ����������� ������ SO, �� ��� ����������
			����������������.
		*/
		void
		deregister_all_coop();

		//! ������� ���� ��� ���������� �� ���� �����������������.
		void
		wait_all_coop_to_deregister();

	private:
		//! ��� ����� ��� ������������������ ����������.
		typedef std::map<
				std::string,
				agent_coop_ref_t >
			coop_map_t;

		//! ����� ��� ������������� ���������� � std::for_each.
		static void
		coop_undefine_all_agents( agent_core_t::coop_map_t::value_type & coop );

		so_environment_t & m_so_environment;

		//! ��� �������� ��� ���������� �������.
		util::mutex_pool_t< ACE_Thread_Mutex > m_agent_coop_mutex_pool;

		//! ��� �������� ��� �������� ������� �������.
		util::mutex_pool_t< ACE_Thread_Mutex > m_agent_queue_mutex_pool;

		//! ����� �� �������� � ������������.
		ACE_Thread_Mutex m_coop_operations_lock;

		//! �������� ����������, ��� ������������� ������.
		ACE_Condition_Thread_Mutex m_deregistration_started_cond;

		//! �������� ����������, ��� ������������� ���������.
		ACE_Condition_Thread_Mutex m_deregistration_finished_cond;

		//! ���� � ������ ������������� ���� ����������.
		bool m_deregistration_started;

		//! ������������������ ����������.
		coop_map_t m_registered_coop;

		//! ����������, ������� �������� ��� �������������.
		coop_map_t m_deregistered_coop;

		//! ���� ������������� ������������� ����������.
		coop_dereg::coop_dereg_executor_thread_t m_coop_dereg_executor;

		//! ��������� �������� ��� ������������.
		coop_listener_unique_ptr_t m_coop_listener;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
