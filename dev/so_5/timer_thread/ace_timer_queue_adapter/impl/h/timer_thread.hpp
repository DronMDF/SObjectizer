/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ���������� ��������� ���� �� ������
		ACE_Thread_Timer_Queue_Adapter.
*/

#if !defined( _SO_5__TIMER_THREAD__ACE_TIMER_QUEUE_ADAPTER__IMPL__TIMER_THREAD_HPP_ )
#define _SO_5__TIMER_THREAD__ACE_TIMER_QUEUE_ADAPTER__IMPL__TIMER_THREAD_HPP_

#include <memory>
#include <map>

#include <ace/Timer_Queue_Adapters.h>
#include <ace/Timer_List.h>

#include <so_5/timer_thread/h/timer_thread.hpp>

#include <so_5/timer_thread/ace_timer_queue_adapter/impl/h/event_handler.hpp>

namespace so_5
{

namespace timer_thread
{

namespace ace_timer_queue_adapter
{

namespace impl
{

//
// timer_thread_t
//

/*!
	\brief ���������� ��������� ���� �� ������
	ACE_Thread_Timer_Queue_Adapter.
 */
class timer_thread_t
	:
		public so_5::timer_thread::timer_thread_t,
		public timer_act_utilizator_t
{
		//! ��������� ��� �������� ����.
		typedef so_5::timer_thread::timer_thread_t base_type_t;
	public:

		//! ��������������� ����� ��� ��������
		//! ��������� ��������������� � ACE � � SObjectizer.
		struct timer_keys_t
		{
			timer_keys_t( timer_id_t timer_id, long ace_id )
				:
					m_timer_id( timer_id ),
					m_ace_id( ace_id )
			{}

			timer_id_t	m_timer_id;
			long		m_ace_id;
		};

		//! �����������.
		timer_thread_t();

		virtual ~timer_thread_t();

		/*!
		 * \name ���������� ���������� so_5::timer_thread::timer_thread_t.
		 * \{
		 */

		//! ��������� ���� �������.
		/*!
		 * �������� ACE_Thread_Timer_Queue_Adapter::activate().
		 */
		virtual ret_code_t
		start();

		//! ���� ������ ���� ������� ��������� ������.
		/*!
		 * �������� ACE_Thread_Timer_Queue_Adapter::deactivate().
		 */
		virtual void
		shutdown();

		//! ������� ������� ���������� ������ ���� �������.
		/*!
		 * ������� �� ACE_Thread_Timer_Queue_Adapter::wait().
		 */
		virtual void
		wait();

		//! ��������� ���������� ��� ������������� ��������� � �������.
		/*!
		 * ��������� ������ ������ ����������� ID � ������� Timer ACT.
		 * ������������ ������ � ACE_Thread_Timer_Queue_Adapter � ���������
		 * � m_agent_demands �������� ���� ������.
		 *
		 * \note ��������� ��������� ����.
		 */
		virtual timer_id_t
		schedule_act(
			timer_act_unique_ptr_t & timer_act );

		//! �������� ������������� ���������.
		virtual void
		cancel_act(
			timer_id_t msg_id );

		/*!
		 * \}
		 */

		//! ������� ������ �������, ��� ��������������� ������� �������
		//! � ��� ��������� �� ����.
		//! \note ����������� ������ � ����� ��� ����������
		//! ��������� ����.
		virtual void
		quick_cancel_act(
			const timer_keys_t & timer_keys );

		/*!
		 * \name ���������� ���������� timer_act_utilizator_t.
		 * \{
		 */

		//! ��� ������������� ������������� ��������� ��������.
		virtual void
		utilize_timer_act(
			timer_act_t * timer_act );

		/*!
		 * \}
		 */

	private:
		//! ������� ��� ��������� � ������� �������.
		//! \note ����������� ������ � ����� ��� ����������
		//! ��������� ����.
		void
		cancel_all();

		//! ������ � mutex-�, ������� ����� �������������� ��� ����������
		//! �������������.
		ACE_SYNCH_RECURSIVE_MUTEX &
		mutex();

		//! ��� ��������� ���� �� ACE.
		typedef ACE_Thread_Timer_Queue_Adapter< ACE_Timer_List >
			adapter_t;

		//! ��������� ���� � ������� �� ACE.
		/*!
			������ ������ ������ ���� ���������
			�� ������� m_event_handler, �.�. � ��� �����������
			�������� ��������� � m_event_handler.
		 */
		std::unique_ptr< adapter_t > m_timer_queue;

		//! ������������ ���������� ��������� �������, ������� �����
		//! �������������� ��� ���� �������.
		std::unique_ptr< timer_event_handler_t > m_event_handler;

		//! ��� ����� ��� ��������� ������� � �� ���������������
		//! ������ adapter_t.
		typedef std::map<
				timer_act_t*,
				timer_keys_t >
			scheduled_act_to_id_map_t;

		//! ��������������� ���������
		scheduled_act_to_id_map_t m_scheduled_act_to_id;

		//! ������� ��� ����������� ��������������� ��������� ������.
		/*!
			������������ ���������� �������� ��� �������� ���������������:
			��������������� ������������ �������� 64-� ������� ��������.
			��������������, ��� 64-� ������� �������� ������ ��� ���������
			���������� ��������������� ���� � ������� ������, ���� � �������
			����������� ������������ ������� ������ ���������.
		 */
		timer_id_t m_self_id_counter;

		//! ��� ����� timer_id => ������������� ������ adapter_t.
		typedef std::map<
				timer_id_t,
				long >
			timer_id_to_ace_id_map_t;

		//! ������������� timer_id � ����������������,
		//! ������� ��������� ace.
		timer_id_to_ace_id_map_t m_timer_id_to_ace_id;
	};

} /* namespace impl */

} /* namespace ace_timer_queue_adapter */

} /* namespace timer_thread */

} /* namespace so_5 */

#endif
