/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief �������� ���������� ������������� ���� SObjectizer.
*/

#if !defined( _SO_5_TRANSPORT__IFACES_HPP_ )
#define _SO_5_TRANSPORT__IFACES_HPP_

#include <string>
#include <memory>

#include <ace/Addr.h>

#include <cpp_util_2/h/nocopy.hpp>

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/ret_code.hpp>
#include <so_5_transport/h/channel_controller.hpp>

namespace so_5_transport
{

/*!
	\brief ��� �������������� ������������� ������.
 */
typedef unsigned long channel_id_t;

//
// connector_controller_t
//

//! ��������� ���������� ��������-connector-��.
class SO_5_TRANSPORT_TYPE connector_controller_t
	:
		private cpp_util_2::nocopy_t
{
	public :
		connector_controller_t();
		virtual ~connector_controller_t();

		//! ������������� �������� ������������ ���������� �
		//! ��������� ��������.
		/*!
			��������� �������� �� ����������� � ��������� ����� �����
			�� ��������� �������. ���� �������� ������ �������,
			�� � �� ���������� ��������� ������ ����� ��������
			� ������� ��������� msg_channel_created ������������� �
			\a ta_mbox.

			���� ����� ����� ������� ������, �� ��� ����������� �����������
			� ��������� ������ ����� ������������ ��������� ������ � �������
			\a ta_mbox.
		 */
		virtual so_5::ret_code_t
		connect(
			//! Mbox ������������� ������.
			const so_5::rt::mbox_ref_t & ta_mbox ) = 0;
};

//
// connector_controller_unique_ptr_t
//

//! ��������� unique_ptr ��� connector_controller_t.
typedef std::unique_ptr< connector_controller_t >
	connector_controller_unique_ptr_t;

//
// acceptor_controller_t
//

//! ��������� ���������� ��������-acceptor-��.
class SO_5_TRANSPORT_TYPE acceptor_controller_t
	:
		private cpp_util_2::nocopy_t
{
	public :
		acceptor_controller_t();
		virtual ~acceptor_controller_t();

		//! �������� ���������� ������.
		/*!
			��������������, ��� ��� ����������� ��������� �������� ����������
			���������� ����� ���������� � ������������ �����������
			���������� ������ acceptor_controller_t.

			\retval 0 ���� �������� ������ �������.
		 */
		virtual so_5::ret_code_t
		create(
			//! Mbox ������������� ������.
			const so_5::rt::mbox_ref_t & ta_mbox ) = 0;
};

//
// acceptor_controller_unique_ptr_t
//

//! ��������� unique_ptr ��� acceptor_controller_t.
typedef std::unique_ptr< acceptor_controller_t >
	acceptor_controller_unique_ptr_t;

} /* namespace so_5_transport */

#endif
