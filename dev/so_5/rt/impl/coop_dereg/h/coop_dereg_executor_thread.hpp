/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ������ �� ������������� ����������.
*/

#if !defined( _SO_5__RT__IMPL__COOP_DEREG__COOP_DEREG_EXECUTOR_THREAD_HPP_ )
#define _SO_5__RT__IMPL__COOP_DEREG__COOP_DEREG_EXECUTOR_THREAD_HPP_

#include <so_5/rt/impl/coop_dereg/h/dereg_demand_queue.hpp>

#include <so_5/rt/h/agent_coop.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

namespace coop_dereg
{

//
// coop_dereg_executor_thread_t
//

//! ����� ���� �������������� ����������.
/*!
	��� ���� ����� ������� ������� �� ����������� ����������� ��
	�� ���� ������ �� ������� ����������, � �� ����������� ��
	���������� ����, ��� ����������� �������, ����� ��� ������
	���������� ��������� ���������� ����� ��������, ����������
	������ ���� � ������� �� �������������, ������� ��������������
	�� ��������� ���� agent_core.
*/
class coop_dereg_executor_thread_t
{
	public:
		coop_dereg_executor_thread_t();
		~coop_dereg_executor_thread_t();

		//! ��������� ����.
		void
		start();

		//! ���� ������ � �������� ������.
		void
		shutdown();

		//! ������� ���������� ������.
		void
		wait();

		//! ��������� ���������� � ������� �� �������������.
		void
		push_dereg_demand(
			agent_coop_t * coop );

	protected:
		//! �������� ���� ����������� ������.
		void
		body();

		//! ����� ����� � ���� ��� ACE_Thread_Manager.
		static ACE_THR_FUNC_RETURN
		entry_point( void * self_object );

	private:
		//! ������� ������ �� ������������� ����������.
		dereg_demand_queue_t m_dereg_demand_queue;

		//! ������������� ����, ��������� ��� ������� �������.
		/*!
			\note �������� ��������� ������ ����� ������ start().
		*/
		ACE_thread_t m_tid;
};

} /* namespace coop_dereg */

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
