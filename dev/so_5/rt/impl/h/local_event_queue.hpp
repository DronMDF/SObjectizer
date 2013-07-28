/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ��������� ������� ������� ������.
*/

#if !defined( _SO_5__RT__IMPL__LOCAL_EVENT_QUEUE_HPP_ )
#define _SO_5__RT__IMPL__LOCAL_EVENT_QUEUE_HPP_

#include <deque>
#include <memory>

#include <ace/Thread_Mutex.h>

#include <so_5/util/h/mutex_pool.hpp>

#include <so_5/rt/h/event_caller_block_ref.hpp>
#include <so_5/rt/h/message_ref.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// event_item_t
//

//! ������� ������� �������.
struct event_item_t
{
	event_item_t()
	{}

	event_item_t(
		//! ���������� �������.
		const event_caller_block_ref_t & event_caller_block,
		//! ���������.
		const message_ref_t & message_ref )
		:
			m_event_caller_block( event_caller_block ),
			m_message_ref( message_ref )
	{}

	//! ���������� �������.
	event_caller_block_ref_t m_event_caller_block;

	//! ���������.
	message_ref_t m_message_ref;
};

//
// local_event_queue_t
//

//! ��������� ������� ������� ������.
/*!
	\note ��� ������ � �������� ���������� ������ ������.
	��� �������� ��� ������� �� �������� ��������.
	����� ��������� ��������, ������ ���������� � ������
	����� ������� ���.
*/
class local_event_queue_t
{
	public:
		explicit local_event_queue_t(
			util::mutex_pool_t< ACE_Thread_Mutex > & mutex_pool );
		~local_event_queue_t();

		//! �������� ������ ������.
		inline void
		pop( event_item_t & event_item );

		//! �������� ������ � �����.
		inline void
		push(
			const event_item_t & evt );

		//! �������� �����.
		inline ACE_Thread_Mutex &
		lock();

		//! ���������� ��������� �������.
		//! \note �� �������� thread-safe.
		inline size_t
		size() const;

		//! ��������� �������.
		inline void
		clear();

	private:
		//! ��� ��������, �� �������� ������� m_lock.
		/*!
			��� �������� ������� ���� ��������� ���,
			�� �������� �������� ������, ������ ���
			��� ����������� ������� ���� ������� ������ � ���.
		*/
		util::mutex_pool_t< ACE_Thread_Mutex > & m_mutex_pool;

		//! ����� �� ���������� �������� � ��������.
		ACE_Thread_Mutex & m_lock;

		//! ��� ��� ���������� ������� �������.
		typedef std::deque< event_item_t > events_queue_t;

		//! ��������� ��� ������� �������.
		events_queue_t m_events_queue;
};

inline void
local_event_queue_t::pop(
	event_item_t & event_item )
{
	event_item = m_events_queue.front();
	m_events_queue.pop_front();
}

inline void
local_event_queue_t::push(
	const event_item_t & evt )
{
	m_events_queue.push_back( evt );
}

inline ACE_Thread_Mutex &
local_event_queue_t::lock()
{
	return m_lock;
}

inline size_t
local_event_queue_t::size() const
{
	return m_events_queue.size();
}

inline void
local_event_queue_t::clear()
{
	m_events_queue.clear();
}

//! ��������� unique_ptr ��� local_event_queue_t.
typedef std::unique_ptr< local_event_queue_t >
	local_event_queue_unique_ptr_t;

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
