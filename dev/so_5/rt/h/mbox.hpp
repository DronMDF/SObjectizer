/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� mbox_t.

	������� ����� ��� mbox-��.
*/


#if !defined( _SO_5__RT__MBOX_HPP_ )
#define _SO_5__RT__MBOX_HPP_

#include <string>
#include <memory>

#include <ace/RW_Thread_Mutex.h>

#include <so_5/h/declspec.hpp>

#include <so_5/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>
#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/message_ref.hpp>
#include <so_5/rt/h/event_data.hpp>
#include <so_5/rt/h/event_handler_caller_ref.hpp>

namespace so_5
{

namespace timer_thread
{
	class timer_act_t;
}

namespace rt
{

namespace impl
{

class message_consumer_link_t;
class named_local_mbox_t;

} /* namespace impl */

class agent_t;
class agent_ref_t;
class mbox_ref_t;
class message_ref_t;

//
// mbox_t
//

//! ������� ����� ��������� �����.
/*!
	������ ����������� ��� �������� � ��������� ��������� ��������.

	Mbox-� ��������� ����� SObjectizer Environment �
	������������� � so_5::rt::mbox_ref_t.
	������� ����� mbox-� � ��� ����� �������� ���������
	mbox_t::deliver_message(), ������� ����� ��� ������:
	���� ������������� ��� ��������� ��������� ���������� ���������,
	� ������ ��� �������� ��������� ��� ��������� �������,
	����� �������� ������ ��� ���������.
	���� ������ �������� � ����������� ������ ���� �������
	����������� �� ������ ��� ��������� � � ������������ ����������
	������ �� ��������� ��������� � ��������� �������
	������� ������� ������ (agent_t) �� ����� ������.

	�������� ���������� � ������������� ������� ���������� ����
	����� mbox_t. SObjectizer Environment ���������� mbox,
	����� ������� �������������� �������� ���������, � ��������� ����
	� ����������� ����� ���������� �������� ��������� ����� �������� mbox.
	\see so_environment_t::schedule_timer(), so_environment_t::single_timer().
*/
class SO_5_TYPE mbox_t
	:
		private atomic_refcounted_t
{
		friend class agent_t;
		friend class mbox_ref_t;
		friend class impl::named_local_mbox_t;
		friend class so_5::timer_thread::timer_act_t;

		mbox_t( const mbox_t & );
		void
		operator = ( const mbox_t & );

	public:
		mbox_t();
		virtual ~mbox_t();

		//! ��������� ��������� �� ������ mbox.
		/*!
			��������� ��������� ������������ ������� ��
			\a msg_unique_ptr, ����� �� �������� ���������
			�������� SO.
		*/
		template< class MESSAGE >
		inline void
		deliver_message(
			//! ����� ��������� �� ��������� ���������.
			std::unique_ptr< MESSAGE > & msg_unique_ptr );


		//! ��������� ��������� �� ������ mbox.
		/*!
			��������� ��������� ������������ ������� ��
			\a msg_unique_ptr, ����� �� �������� ���������
			�������� SO.
		*/
		template< class MESSAGE >
		inline void
		deliver_message(
			//! ����� ��������� �� ��������� ���������.
			std::unique_ptr< MESSAGE > && msg_unique_ptr );


		//! ��������� ��������� �� ������ mbox.
		/*!
			��� ��� ������� ����� ��������� �������� ��������
			� ��������� ���������� ��������� ��������� �� �����.
		*/
		template< class MESSAGE >
		inline void
		deliver_message();

		//! �������� ���.
		virtual const std::string &
		query_name() const = 0;

	protected:
		//! �������� ����������� ���������,
		//! ������� �������� ������������ �������.
		/*!
			����� ����������, ����� ����� ������ ��� �������������
			�� ��������� ��������� ����.
		*/
		virtual void
		subscribe_first_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, �������
			//! ����� ��������.
			std::unique_ptr< impl::message_consumer_link_t > &
				message_consumer_link,
			//! ������ ����������, ������� ����� ��������.
			const event_handler_caller_ref_t &
				event_handler_caller_ref ) = 0;

		//! �������� ����������� ���������,
		//! ������� �������� ������������ �������.
		/*!
			����� ����������, ����� ����� ������������� ��
			��� ���������, �� ������� �� ����� ��� ������������.
		*/
		virtual ret_code_t
		subscribe_more_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ��������� ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t * message_consumer_link,
			//! ��������� ���������� ��� ��������.
			const event_handler_caller_ref_t & event_handler_caller_ref,
			//! ������� �� ���������� � ������ ��������.
			throwing_strategy_t throwing_strategy ) = 0;

		//! ������� ����������� ���������,
		//! ������� �������� ������������ �������.
		virtual ret_code_t
		unsubscribe_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ��������� ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t *
				message_consumer_link,
			//! ����������, ������� ���� �������.
			const event_handler_caller_ref_t &
				event_handler_caller_ref,
			//! �������� ��� �����, �������� �� ���������
			//! ���������� ��������� � �������� ������ �� ��������
			//! ��� ���������.
			bool & is_last_subscription,
			//! ������� �� ���������� � ������ ��������.
			throwing_strategy_t throwing_strategy ) = 0;

		//! ������� ���� ������������ ���������.
		virtual void
		unsubscribe_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ��������� ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t *
				message_consumer_link ) = 0;

		//! ��������� ���� ����������� ���������.
		virtual void
		deliver_message(
			const type_wrapper_t & type_wrapper,
			const message_ref_t & message_ref ) = 0;

		//! ����� ��� ���������� �������� ���������.
		/*!
			���������� �� ��������� ���������� this.
		*/
		virtual const mbox_t *
		cmp_ordinal() const;
};


template< class MESSAGE >
void
mbox_t::deliver_message(
	std::unique_ptr< MESSAGE > & msg_unique_ptr )
{
	deliver_message(
		type_wrapper_t( typeid( MESSAGE ) ),
		message_ref_t( msg_unique_ptr.release() ) );
}

template< class MESSAGE >
void
mbox_t::deliver_message(
	std::unique_ptr< MESSAGE > && msg_unique_ptr )
{
	deliver_message(
		type_wrapper_t( typeid( MESSAGE ) ),
		message_ref_t( msg_unique_ptr.release() ) );
}

template< class MESSAGE >
void
mbox_t::deliver_message()
{
	deliver_message(
		type_wrapper_t( typeid( MESSAGE ) ),
		message_ref_t() );
}

} /* namespace rt */

} /* namespace so_5 */

#endif
