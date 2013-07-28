/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������������ ����� ��� ����� ���� ������������ ���������.

*/

#if !defined( _SO_5__RT__IMPL__MESSAGE_CONSUMER_LINK_HPP_ )
#define _SO_5__RT__IMPL__MESSAGE_CONSUMER_LINK_HPP_

#include <memory>

#include <so_5/h/types.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/event_handler_caller.hpp>
#include <so_5/rt/h/event_handler_caller_ref.hpp>
#include <so_5/rt/h/event_caller_block_ref.hpp>

namespace so_5
{

namespace rt
{

// ��������, ��� ���� ����� �����.
class message_ref_t;

namespace impl
{

class message_consumer_link_t;

//! ����� unique_ptr ��� message_consumer_link_t.
typedef std::unique_ptr< message_consumer_link_t >
	message_consumer_link_unique_ptr_t;

//! ����� ��������� � ��������� ������ �� message_consumer_link_t.
typedef std::shared_ptr< message_consumer_link_t >
	message_consumer_link_ref_t;

//
// message_consumer_link_t
//

//! ������������ ����� ��� ����� ������� ������������ ���������.
class message_consumer_link_t
{
	public:
		message_consumer_link_t(
			//! ������ �� ������.
			const agent_ref_t & agent_ref );

		~message_consumer_link_t();

		//! ������������������ ���������.
		void
		dispatch( const message_ref_t & message );

		//! ���������� ������ ������.
		void
		set_left(
			const message_consumer_link_ref_t & left );

		//! ���������� ������� ������.
		void
		set_right(
			const message_consumer_link_ref_t & right );

		//! �������� ������ ������.
		inline const message_consumer_link_ref_t &
		query_left() const
		{
			return m_left;
		}

		//! �������� ������� ������.
		inline const message_consumer_link_ref_t
		query_right() const
		{
			return m_right;
		}

		//! �������� �� ������� ������ � �������.
		inline bool
		is_first()
		{
			return 0 == m_left.get();
		}

		//! �������� �� ������� ��������� � �������.
		inline bool
		is_last()
		{
			return 0 == m_right.get();
		}

		inline const event_caller_block_ref_t &
		event_caller_block() const
		{
			return m_event_handler_caller;
		}

		//! �������� �����������.
		inline event_caller_block_ref_t &
		event_caller_block()
		{
			return m_event_handler_caller;
		}

	protected:
		//! ����� �����.
		message_consumer_link_ref_t m_left;

		//! ������ �����.
		message_consumer_link_ref_t m_right;

		//! ���� ���������� �����������.
		event_caller_block_ref_t m_event_handler_caller;

		//! ������ �� ������, � ����������� �������� ������ ��������
		//! � ����������.
		agent_ref_t m_agent_ref;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
