/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ���������� ������������ mbox-�.
*/

#if !defined( _SO_5__RT__IMPL__NAMED_LOCAL_MBOX_HPP_ )
#define _SO_5__RT__IMPL__NAMED_LOCAL_MBOX_HPP_

#include <so_5/h/types.hpp>
#include <so_5/h/exception.hpp>

#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/mbox.hpp>
#include <so_5/rt/h/mbox_ref.hpp>

#include <so_5/rt/impl/h/mbox_core.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// named_local_mbox_t
//

//! ��������� ����������� mbox.
/*!
	� ������������ �������� �������� mbox, �������� ����������
	��� ����������� mbox-�.
	��� ����� ����� ��� ���������� �������������� ��������� �����
	�� ����������� mbox-�. ������� �������� ������ �� ����������� mbox
	�� ���������� ����������� � ���, ��� ����
	����� ������ ����������� mbox � ������ Sobjectizer Environment.
*/
class named_local_mbox_t
	:
		public mbox_t
{
		friend class impl::mbox_core_t;

		named_local_mbox_t(
			const std::string & name,
			const mbox_ref_t & mbox,
			impl::mbox_core_t & mbox_core );

	public:
		virtual ~named_local_mbox_t();

		//! ��������������� ������� �������� ������.
		//! \{

		//! �������� ���.
		virtual const std::string &
		query_name() const;

	protected:
		//! �������� ����������� ���������,
		//! ������� �������� ������������ �������.
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
				event_handler_caller_ref );

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
			//! ��������� �� ����������� ���������, ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t * message_consumer_link,
			//! ��������� ���������� ��� ��������.
			const event_handler_caller_ref_t & event_handler_caller_ref,
			//! ������� �� ���������� � ������ ��������.
			throwing_strategy_t throwing_strategy );

		//! ������� ����������� ���������,
		//! ������� �������� ������������ �������.
		virtual ret_code_t
		unsubscribe_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, ������� ��������
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
			throwing_strategy_t throwing_strategy );

		//! ������� ��� �������� ������ �� �������� ��� ���������.
		//! \note ���������� ��� ������������� ������,
		//! ����� ���� ������� ��� ��� ��������,
		//! � �� ��������� ����������.
		virtual void
		unsubscribe_event_handler(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t *
				message_consumer_link );
		//! \}

		//! ��������� ���� ����������� ���������.
		void
		deliver_message(
			const type_wrapper_t & type_wrapper,
			const message_ref_t & message_ref );

		//! ����� ��� ���������� �������� ���������.
		/*!
			���������� ����� ��������������� mbox-�.
		*/
		virtual const mbox_t *
		cmp_ordinal() const;

	private:
		//! �������� mbox-�, ���� �� �����������.
		const std::string m_name;

		//! ��������� ������� mbox-�.
		impl::mbox_core_ref_t m_mbox_core;

		//! �������� mbox.
		mbox_ref_t m_mbox;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
