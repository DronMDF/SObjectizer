/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ������ �� ������������� ����������.
*/

#if !defined( _SO_5__RT__IMPL__COOP_DEREG__DEREG_DEMAND_QUEUE_HPP_ )
#define _SO_5__RT__IMPL__COOP_DEREG__DEREG_DEMAND_QUEUE_HPP_

#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>

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
// dereg_demand_queue_t
//

//! ������� ������.
/*!
	��� �� ������ ������� ������������� ����������
	������.

	������������� ��� ������������� �����������
	������ ������������.
*/
class dereg_demand_queue_t
{
	public:
		typedef std::vector< agent_coop_t* > dereg_demand_container_t;

		dereg_demand_queue_t();
		~dereg_demand_queue_t();

		//! ��������� ������ �� ���������� ������� � �������.
		void
		push( agent_coop_t * coop );

		//! ����� ������ ������.
		/*!
			���� ������ � ������� ���, �� ������� ����
			�������� �� ��������� ������ � �������, ����
			�� ����������� �������� ���������� ������.

			����� ������ ���������, �� �������� �� ����� ��������� ������.
		*/
		void
		pop(
			/*! �������� �a����. */
			dereg_demand_container_t & demands );

		//! ������ ������������ ������.
		void
		start_service();

		//! ���������� ������������ ������.
		void
		stop_service();

	private:
		//! ��������� �������.
		dereg_demand_container_t m_demands;

		//! �������������.
		//! \{
		ACE_Thread_Mutex m_lock;
		ACE_Condition_Thread_Mutex m_not_empty;
		//! \}

		//! ���� - ����������� �� �������� �������?.
		/*! ��������� ��������:
			true - ���� ���������� ������ - ����������� ������ push/pop.
			false - ���������� ������������.
		*/
		bool m_in_service;
};

} /* namespace coop_dereg */

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
