/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �����, ������� ������������ ��������� �� ������������.
*/

#if !defined( _SO_5__RT__IMPL__MESSAGE_DISTRIBUTOR_HPP_ )
#define _SO_5__RT__IMPL__MESSAGE_DISTRIBUTOR_HPP_

#include <so_5/rt/impl/h/message_consumer_chain.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

/*!
	�����, ������� ������ ��������������
	<b>��� ��������� => ����������� ���������</b>.
	��������� ����� ������������ � � ������ ����������
	�����-���� ������� ��� ������������� ���� ���������
	������� ��.
*/
class message_distributor_t
{
	public:
		message_distributor_t();
		virtual ~message_distributor_t();

		//! �������� ����������� ��������� � ����� �������.
		/*!
			����� ���� ����� ������������ ����������� ���������,
			������� ����� �� ����� �������� � ����������� �������
			� ������ �������� � �����.
		*/
		void
		push_first(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, �������
			//! ����� ��������.
			std::unique_ptr< impl::message_consumer_link_t > &
				message_consumer_link,
			//! ������ ����������, ������� ����� ��������.
			const event_handler_caller_ref_t &
				event_handler_caller_ref );

		ret_code_t
		push_more(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t * message_consumer_link,
			//! ��������� ���������� ��� ��������.
			const event_handler_caller_ref_t & event_handler_caller_ref,
			//! ������� �� ���������� � ������ ��������.
			throwing_strategy_t throwing_strategy );

		//! �������� ����������� ��������� � ����� �������.
		/*!
			����� ���� ����� ������������ ����������� ���������,
			������� ����� �� ����� �������� � ����������� �������
			� ������ �������� � �����.
		*/
		ret_code_t
		pop(
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

		//! ������� ��� �������� ������� ������ �� ������
		//! ��� ���������.
		void
		pop(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ��������� �� ����������� ���������, ������� ��������
			//! ����� ��������� ��������.
			impl::message_consumer_link_t *
				message_consumer_link );

		//! ��������� ��������� ������������.
		void
		distribute_message(
			const type_wrapper_t & type_wrapper,
			const message_ref_t & message_ref );

	private:
		/*!
			�������� ������ �� ������� ������������
			��������� ���� ���������.
			���� ������� ��� ������� ���� ���������
			��� �� ����������, �� ��� ����� ������� � ���������.
		*/
		message_consumer_chain_t &
		provide_message_consumer_chain(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper );

		//! ����� ��� ��������� => ������� ������������.
		msg_type_to_consumer_chain_map_t
			m_msg_type_to_consumer_chain_map;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
