/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����.
*/

#if !defined( _SO_5__DISP__REUSE__WORK_THREAD__WORK_THREAD_HPP_ )
#define _SO_5__DISP__REUSE__WORK_THREAD__WORK_THREAD_HPP_

#include <deque>

#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Atomic_Op.h>

#include <so_5/h/declspec.hpp>
#include <so_5/rt/h/agent_ref.hpp>

namespace so_5
{

namespace disp
{

namespace reuse
{

namespace work_thread
{

//
// demand_t
//

//! ��� �������� ������ ������.
struct demand_t
{
	//! ����� ������� �������� ���� ���������.
	so_5::rt::agent_ref_t m_agent_ref;

	//! ���������� ������� ������� ���� ���������.
	unsigned int m_event_cnt;

	demand_t()
		:
			m_event_cnt( 0 )
	{}

	demand_t(
		//! ����� � �������� ���� ��������� �������.
		const so_5::rt::agent_ref_t & agent_ref,
		//! ���������� ����������� ��� ������ �������.
		unsigned int event_cnt )
		:
			m_agent_ref( agent_ref ),
			m_event_cnt( event_cnt )
	{}
};

//! ��� ��� ���������� ������� ������ �� ���������� �������.
typedef std::deque< demand_t > demand_container_t;

//
// demand_queue_t
//

//! ������� ������.
/*!
	��� �� ������ ������� ������������� ����������
	������.

	������������� ��� ������������� �����������
	������ ������������.
*/
class demand_queue_t
{
	public:
		demand_queue_t();
		~demand_queue_t();

		//! ��������� ������ �� ���������� ������� � �������.
		void
		push(
			//! ����� � �������� ���� ��������� �������.
			const so_5::rt::agent_ref_t & agent_ref,
			//! ���������� ����������� ��� ������ �������.
			unsigned int event_cnt );

		enum
		{
			//! ���� ��������� ������.
			demand_extracted = 1,
			//! ������ �� ���� ���������, �.�.
			//! ��������� ������� ���������� ������.
			shutting_down = 2,
			//! ������ �� ���� ���������, �.�. � ������� ��� ������.
			no_demands = 3
		};

		//! ����� ��������� ������.
		/*!
			���� ������ � ������� ���, �� ������� ����
			�������� �� ��������� ������ � �������, ����
			�� ����������� �������� ���������� ������.
		*/
		int
		pop(
			/*! �������� �a����. */
			demand_container_t & queue_item );

		//! ������ ������������ ������.
		void
		start_service();

		//! ���������� ������������ ������.
		void
		stop_service();

		//! ��������� �������.
		void
		clear();

	private:
		//! ��������� �������.
		demand_container_t m_demands;

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

//
// work_thread_t
//

//!	����� ������� ����.
/*!
	������� ���� ������ �������������� � ������� ������-����
	����������. ��� ���� ����� ����� �������-���������� ������
	��������� ����� ����� �������-����.
*/
class work_thread_t
{
	public:
		work_thread_t(
			rt::dispatcher_t & disp );

		~work_thread_t();

		//! ��������� ������ �� ���������� ������� �������.
		//! �.�. ������������� ����� ������� ������.
		void
		put_event_execution_request(
			//! ����� ������� �������� ���� �������������.
			const so_5::rt::agent_ref_t & agent_ref,
			//! ���������� �������,
			//! ������� ������ ��������� � ����� ������.
			unsigned int event_count );

		//! ��������� ����.
		void
		start();

		//! ���� ������ � �������� ������.
		void
		shutdown();

		//! ������� ���������� ������.
		/*!
			����� �������� ���� ��������� ���,
			���������� �� �������������, ������.
		*/
		void
		wait();

	protected:
		//! �������� ���� ����������� ������.
		void
		body();

		//! ��������� ����������.
		void
		handle_exception(
			//! ����������� ���������� ����������� �� ����������
			//! ������-���� �������.
			const std::exception & ex,
			//! ����� ��� ���������� �������� ����������.
			const so_5::rt::agent_ref_t & a_exception_producer );

		//! ��������� ���� ������.
		void
		serve_demands_block(
			//! ������ ������ ��� ������������.
			demand_container_t & executed_demands );

		//! ����� ����� � ���� ��� ACE_Thread_Manager.
		static ACE_THR_FUNC_RETURN
		entry_point( void * self_object );

	private:
		//! ������� ������ ��� ������ ������� ����.
		demand_queue_t m_queue;

		//! ��� ����� - ���������� ��������� ������?
		enum
		{
			//! 0 - ���, �� ����������.
			WORK_THREAD_STOP = 0,
			//! 1 - ��, ����������.
			WORK_THREAD_CONTINUE = 1
		};

		//! ���� - ���������� ��������� ������?
		/*! ����� ��������� ��������:
			WORK_THREAD_STOP, WORK_THREAD_CONTINUE
		*/
		ACE_Atomic_Op< ACE_Thread_Mutex, long > m_continue_work;

		//! ������������� ����, ��������� ��� ������� �������.
		/*!
			\note �������� ��������� ������ ����� ������ start().
		*/
		ACE_thread_t m_tid;

		//! ��������� ����������� ������� ����.
		/*!
			��� ��������� ������������ ����������, �� ����������
			������� ������, �������� ������������ ���������.
		*/
		rt::dispatcher_t & m_disp;
};

} /* namespace work_thread */

} /* namespace reuse */

} /* namespace disp */

} /* namespace so_5 */

#endif
