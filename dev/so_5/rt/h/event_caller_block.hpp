/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �����, ������� ������������ ������ ����������� ���������.
*/

#if !defined( _SO_5__RT__EVENT_CALLER_BLOCK_HPP_ )
#define _SO_5__RT__EVENT_CALLER_BLOCK_HPP_

#include <vector>

#include <so_5/h/declspec.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>
#include <so_5/rt/h/message.hpp>
#include <so_5/rt/h/message_ref.hpp>
#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/event_handler_caller_ref.hpp>

namespace so_5
{

namespace rt
{

//
// event_caller_block_t
//

//! ��� ��� ������� �����������.
typedef std::vector< event_handler_caller_ref_t > evt_caller_array_t;

//! ����� - ����� �����������.
class SO_5_TYPE event_caller_block_t
	:
		private atomic_refcounted_t
{
		friend class event_caller_block_ref_t;

	public:
		event_caller_block_t();
		event_caller_block_t(
			const event_caller_block_t & event_caller_block );

		~event_caller_block_t();

		//! ������� ����������.
		inline void
		call(
			//! ���������.
			message_ref_t & message_ref ) const;

		//! ������� ����������
		void
		erase( const event_handler_caller_ref_t & ehc_ref );

		//! ������� ����������
		void
		insert( const event_handler_caller_ref_t & ehc_ref );

		//! ��������� ���� �� ����� ����������?
		bool
		has( const event_handler_caller_ref_t & ehc_ref ) const;

		//! ����� �������� ����������, ������� � ������ ����������.
		//! ���� ���� ���������� ��� ��������� ���������
		//! � �������� ���������.
		const event_handler_caller_t *
		find( const event_handler_caller_ref_t & ehc_ref ) const;

		//! ���� �� ���������� ������.
		bool
		is_empty() const;

	private:
		//! ������ �����������.
		evt_caller_array_t m_callers_array;
};

inline void
event_caller_block_t::call(
	message_ref_t & message_ref ) const
{
	evt_caller_array_t::const_iterator it = m_callers_array.begin();
	evt_caller_array_t::const_iterator it_end = m_callers_array.end();

	for(; it != it_end; ++it )
		if( (*it)->call( message_ref ) )
			break;
}

} /* namespace rt */

} /* namespace so_5 */

#endif
