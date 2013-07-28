/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ��������� ��������� ������������� ����.
*/

#if !defined( _SO_5_TRANSPORT__MESSAGES_HPP_ )
#define _SO_5_TRANSPORT__MESSAGES_HPP_

#include <memory>
#include <string>

#include <so_5_transport/h/declspec.hpp>

#include <so_5/rt/h/message.hpp>

#include <so_5_transport/h/ifaces.hpp>
#include <so_5_transport/h/channel_io.hpp>

namespace so_5_transport
{

//
// msg_create_server_socket_result
//

//! ������ �������� ���������� ������.
struct SO_5_TRANSPORT_TYPE msg_create_server_socket_result
	:
		public so_5::rt::message_t
{
	//! ������� �� ����������� �������� ���������� ������.
	bool m_succeed;

	msg_create_server_socket_result();
	msg_create_server_socket_result(
		bool succeed );
};


//
// msg_channel_created
//

/*!
	\brief ��������� �� �������� �������� ������������� ������.

	���������� ��������-connector-�� ��� ��������-acceptor-��
	��� �������� ������������ ���������� � ��������� ��������.

	\note ���������� ��������� ������ ���� ������ 1.
 */
struct SO_5_TRANSPORT_TYPE msg_channel_created
	:
		public so_5::rt::message_t
{
	//! ������������� ������� ������.
	channel_id_t m_channel_id;

	//! ��������� ������� �����-������.
	channel_io_t m_io;

	//! ���������� ������.
	mutable channel_controller_ref_t m_controller;

	msg_channel_created();
	msg_channel_created(
		const channel_id_t & id,
		const channel_io_t & io,
		const channel_controller_ref_t & controller );

	virtual ~msg_channel_created();

};

//
// msg_channel_failed
//

/*!
	\brief ��������� � ��������� �������� ������������� ������.

	���������� ��������-connector-�� ��� ��������-acceptor-��
	����� ��������� ������� ������������ ���������� � ��������� ��������.
*/
struct SO_5_TRANSPORT_TYPE msg_channel_failed
	:
		public so_5::rt::message_t
{
	std::string m_reason;

	msg_channel_failed();
	msg_channel_failed( const std::string & reason );

	virtual ~msg_channel_failed();

};

//
// msg_channel_lost
//

/*!
	\brief ��������� � �������� ������������� ������.

	���������� ��� ����������� ����,
	��� ����� �������� �������� ��� �����������.
 */
struct SO_5_TRANSPORT_TYPE msg_channel_lost
	:
		public so_5::rt::message_t
{
	//! ������������� ����������� ������.
	channel_id_t m_channel_id;

	//! ���������� ������.
	mutable channel_controller_ref_t m_controller;

	/*!
		\brief ��������� �������� ������� ��������.

		\note ����� ���������, ��� �� �� ���� ������� ������� �������
		����� ��������. ������� ������ ��� ���� ����� ���� ������.
	 */
	const std::string m_reason;

	msg_channel_lost();
	msg_channel_lost(
		const channel_id_t & id,
		const std::string & reason,
		const channel_controller_ref_t & controller );

	virtual ~msg_channel_lost();
};

//
// msg_incoming_package
//

//! ���������-����������� � ����������� ������ ������ ������.
struct SO_5_TRANSPORT_TYPE msg_incoming_package
	:
		public so_5::rt::message_t
{
	//! ������������� ������.
	channel_id_t m_channel_id;

	msg_incoming_package();

	msg_incoming_package(
		channel_id_t channel_id,
		const channel_io_t & io );

	virtual ~msg_incoming_package();

	//! \name ��������� ��� �������� ������� ������/������.
	//! \{

	//! ������ ���������� ������.
	/*!
		������ ���������� ������ ����� ������ 1 ���.
	*/
	input_trx_unique_ptr_t
	begin_input_trx() const;

	//! ������� ����� ��� ������.
	output_trx_unique_ptr_t
	begin_output_trx(
		//! ����� ������� ����� ��������� �� ��������
		//! ��������� ������ � ������ �� ���������� ��� ����������
		//! � ���� ������.
		const ACE_Time_Value &
			free_blocks_awaiting_timeout = ACE_Time_Value::zero ) const;
	//! \}

	private:
		//! ��������� ������� �����-������.
		channel_io_t m_io;

		//! ����� �� ����� ���������� ������ ������.
		/*!
			���� ������������ �� ������� ���������� ������ ������,
			����� ��� ���� ����� ������ ����� ������ �������� �
			�������� ����������� � �������� ������.
		*/
		mutable bool m_input_transaction_took_place;
};

} /* namespace so_5_transport */

#endif
