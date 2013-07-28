/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ���������� acceptor_controller ��� �������.
*/

#if !defined( _SO_5_TRANSPORT__SOCKET__IMPL__ACCEPTOR_CONTROLLER_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__IMPL__ACCEPTOR_CONTROLLER_HPP_

#include <so_5_transport/h/ret_code.hpp>

#include <so_5_transport/socket/h/pub.hpp>

#include <so_5_transport/socket/impl/h/acceptor.hpp>

namespace so_5_transport
{

namespace socket
{

namespace impl
{

//
// socket_acceptor_controller_t
//

/*!
	\brief ���������� acceptor_controller ��� �������.
*/
class SO_5_TRANSPORT_TYPE socket_acceptor_controller_t
	:
		public acceptor_controller_t
{
	private:
		//! ��������� ��� ���������� ������.
		const acceptor_params_t m_params;

		//! ��������� ��� ���������� �������.
		const channel_params_t m_channel_params;

		//! ������-ACCEPTOR, ������� ��������� ����� ����� �����������.
		std::unique_ptr< acceptor_t > m_acceptor;

		//! ������, ������� �������� �� ���������� ����� �����������
		//! ������������� ����������.

		option_setter_unique_ptr_t m_option_setter;
		//! �������, � ������� ����� ����� ��������.
		/*!
		 * ������������ ��� ����������� ������ m_acceptor.open().
		 */
		ACE_Reactor * m_reactor;

	public :
		//! ���������������� �����������.
		socket_acceptor_controller_t(
			//! ��������� ��� ���������� ������.
			const acceptor_params_t & params,
			//! ��������� ��� ���������� �������.
			const channel_params_t & channel_params,
			//! ������, ������� �������� �� ���������� ����� �����������
			//! ������������� ����������.
			option_setter_unique_ptr_t option_setter,
			//! �������, � ������� ����� ����� ��������.
			ACE_Reactor * reactor );
		virtual ~socket_acceptor_controller_t();

		//! \name ���������� �������������� �������.
		//! \{
		virtual so_5::ret_code_t
		create(
			const so_5::rt::mbox_ref_t & ta_mbox );
		//! \}
};

} /* namespace impl */

} /* namespace socket */

} /* namespace so_5_transport */

#endif

