/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ������������������ ���������� ACE_Acceptor, ����������� ��
	������������� � ���� � svc_handler.
*/

#if !defined( _SO_5_TRANSPORT__SOCKET__IMPL__ACCEPTOR_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__IMPL__ACCEPTOR_HPP_

#include <cassert>

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include <so_5/rt/h/mbox_ref.hpp>

#include <so_5_transport/h/ifaces.hpp>
#include <so_5_transport/h/channel_params.hpp>

#include <so_5_transport/socket/h/pub.hpp>
#include <so_5_transport/socket/impl/h/svc_handler.hpp>

namespace so_5_transport
{

namespace socket
{

namespace impl
{

//
// acceptor_t
//

/*!
	\brief ������������������ ���������� ACE_Acceptor, ����������� ��
	������������� � ���� � generic_svc_handler.

	�������������� ����� make_svc_handler � ���, ����� �������
	��������� channel_manager_notificator_t � �������� ��� �
	����������� SVC_HANDLER.
*/
class acceptor_t
	:
		public ACE_Acceptor< svc_handler_t, ACE_SOCK_Acceptor >
{
		//! ��������� �������� ����.
		typedef ACE_Acceptor< svc_handler_t, ACE_SOCK_Acceptor > base_type_t;

	public:
		acceptor_t(
			const channel_params_t & params,
			const so_5::rt::mbox_ref_t & ta_mbox,
			//! ������, ������� �������� �� ���������� ����� �����������
			//! ������������� ����������.
			option_setter_t * option_setter )
			:
				m_params( params ),
				m_ta_mbox( ta_mbox ),
				m_channel_id_counter( 1 ),
				m_option_setter( option_setter )
		{}

		virtual ~acceptor_t()
		{}

	protected:
		/*!
			����������� ���������� %make_svc_handler ��� ����, ����� �����
			���������� ������� �������� channel_manager_notificator.
		*/
		virtual int
		make_svc_handler(
			svc_handler_t *& sh )
		{
			assert( !sh );

			std::unique_ptr< svc_handler_t > svc_handler(
				new svc_handler_t(
					this->reactor(),
					m_channel_id_counter++,
					m_ta_mbox,
					m_params ) );

			if( nullptr == svc_handler.get() )
				ACE_ERROR_RETURN( ( LM_ERROR,
					"unable to create SVC_HANDLER object" ), -1 );

			if( setup_options( sh->peer() ) )
				ACE_ERROR_RETURN(( LM_ERROR,
						"unable to setup custom options for "
						"SVC_HANDLER peer" ),
						-1 );

			sh = svc_handler.release();

			return 0;
		}

		//! ��������������� ����� ��� ���������� ������ ����������
		//! ������������� ����������.
		so_5::ret_code_t
		setup_options( stream_type & peer )
		{
			if( nullptr != m_option_setter )
				return m_option_setter->setup( peer );

			return 0;
		}

	private:
		//! ������� ��� �������� channel_id.
		channel_id_t m_channel_id_counter;

		//! ��������� ��� ���������� �����������.
		const channel_params_t m_params;

		//! Mbox ������������� ������.
		so_5::rt::mbox_ref_t m_ta_mbox;

		//! ������, ������� �������� �� ���������� ����� �����������
		//! ������������� ����������.
		option_setter_t * m_option_setter;
};


} /* namespace impl */

} /* namespace socket */

} /* namespace so_5_transport */

#endif

