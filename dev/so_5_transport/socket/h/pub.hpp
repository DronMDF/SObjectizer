/*
	SObjectizer 5 Transport.
*/
/*!
	\file
	\brief ��������� ������������� ������ ����������� �������.
*/


#if !defined( _SO_5_TRANSPORT__SOCKET__PUB_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__PUB_HPP_

#include <string>

#include <ace/INET_Addr.h>
#include <ace/Synch_Options.h>
#include <ace/SOCK_Stream.h>
#include <ace/Reactor.h>

#include <so_5_transport/h/declspec.hpp>

#include <so_5/h/throwing_strategy.hpp>
#include <so_5/rt/h/so_environment.hpp>

#include <so_5_transport/h/reactor_layer.hpp>
#include <so_5_transport/h/ifaces.hpp>
#include <so_5_transport/h/channel_params.hpp>

namespace so_5_transport
{

namespace socket
{

//
// modify_old_format_ip
//

/*!
	��������� ������ IP ������ �, ���� �� ���������� ����� � ':',
	�� ��������� � ������ 'localhost'. ��������� ������ ����������������
	������� SObjectizer-� ��������� ������������ ������ ����
	':3000', � ACE_INET_Addr::string_to_addr �� ������������ ���� ������,
	�� ������� ��������������� �������� ������ � 'localhost:3000'.
*/
SO_5_TRANSPORT_EXPORT_FUNC_SPEC( std::string )
modify_old_format_ip( const std::string & ip );

//
// option_setter_t
//

/*!
	\brief ��������� �������, ������� ����� �������� �� ���������
	������������� ����� ��� ������� SOCK_Stream.

	�������� �������� SOCK_Stream ������ �� ������������ � �����������
	connector/acceptor_controller. ��� ����, ����� ������������ ���
	�������� ��������� SOCK_Stream, ������� ������������ �� ���������,
	���������� connector/acceptor_controller ������������� ������������
	����������� ������ ������-������������, ����������� ���������
	option_setter_t. ��� ������ ������������� ��� �������� SOCK_Stream.
*/
class SO_5_TRANSPORT_TYPE option_setter_t
{
	public:
		virtual ~option_setter_t();

		/*!
			���� ����� ���������� connector-��/acceptor-�� ��� ����������
			SOCK_Stream ����������� ������������ �����.
		*/
		virtual so_5::ret_code_t
		setup( ACE_SOCK_Stream & stream ) = 0;

	private:
		// ������ �� �����������.
		option_setter_t(
			const option_setter_t & );
		option_setter_t &
		operator=(
			const option_setter_t & );
};

//
// option_setter_unique_ptr_t
//

//! ��������� unique_ptr ��� option_setter.
typedef std::unique_ptr< option_setter_t > option_setter_unique_ptr_t;

//
// connector_params_t
//

/*!
	\brief ���������, ����������� connector-� ��� ������������
	������ ����������.

	\note ��� ������������ ���������� ��� ACE_Connector::connect.
*/
class SO_5_TRANSPORT_TYPE connector_params_t
{
	public:
		/*!
			����������� ������ ����� �������� �������� \a remote_addr.
		*/
		connector_params_t(
			const ACE_INET_Addr & remote_addr );
		~connector_params_t();

		//! \name Getter-�/Setter-�.
		//! \{
		const ACE_Synch_Options &
		synch_options() const;

		connector_params_t &
		set_synch_options(
			const ACE_Synch_Options & value );

		int
		reuse_addr() const;

		connector_params_t &
		set_reuse_addr(
			int value );

		int
		flags() const;

		connector_params_t &
		set_flags(
			int value );

		const ACE_INET_Addr &
		remote_addr() const;

		connector_params_t &
		set_remote_addr(
			const ACE_INET_Addr & value );

		const ACE_INET_Addr &
		local_addr() const;

		connector_params_t &
		set_local_addr(
			const ACE_INET_Addr & value );
		//! \}

	private:
		//! ��������� �����������/������������ �����������.
		/*!
			�������� �� ���������: ACE_Synch_Options::default.
		*/
		ACE_Synch_Options m_synch_options;

		//! ������� ������������� ����������������� ���������� ������.
		/*!
			�� ��������: 0 (�� ����������������).
		*/
		int m_reuse_addr;

		//! ����� ��� ������ �����������.
		/*!
			�� ���������: O_RDWR.
		*/
		int m_flags;

		//! ����� ���������� �����.
		/*!
			�������� �� ��������� �� �����, ������ ������������ � ������������.
		*/
		ACE_INET_Addr m_remote_addr;

		//! ����� ���������� �����.
		/*!
			�������� �� ���������: ��������, ����������� �������������
			ACE_INET_Addr �� ���������.
		*/
		ACE_INET_Addr m_local_addr;
};

/*!
	\brief ��������������� ����� ��� ��������� �������� connector_params.

	������ �������������:
	\code
	connector_params( remote_ip ).
		set_synch_options( ACE_Synch_Options::asynch ).
		set_reuse_addr( 1 ).
		set_local_addr( local_ip );
	\endcode
*/
SO_5_TRANSPORT_EXPORT_FUNC_SPEC( connector_params_t )
create_connector_params( const std::string & remote_addr );

/*!
	\brief ��������������� ����� ��� ��������� �������� connector_params.

	������ �������������:
	\code
	connector_params( remote_ip_str ).
		set_synch_options( ACE_Synch_Options::asynch ).
		set_reuse_addr( 1 ).
		set_local_addr( local_ip );
	\endcode
*/
inline connector_params_t
create_connector_params( const ACE_INET_Addr & remote_addr )
{
	return connector_params_t( remote_addr );
}

//
// acceptor_params_t
//

/*!
	\brief ���������, ����������� acceptor-� ��� ��������
	���������� ������.

	\note ��� ������������ ���������� ��� ACE_Acceptor::open.
*/
class SO_5_TRANSPORT_TYPE acceptor_params_t
{
	public :
		/*!
			����������� ������ ����� �������� �������� \a local_addr.
		 */
		acceptor_params_t(
			const ACE_INET_Addr & local_addr );
		~acceptor_params_t();

		//! \name Getter-�/Setter-�.
		//! \{
		const ACE_INET_Addr &
		local_addr() const;

		acceptor_params_t &
		set_local_addr(
			const ACE_INET_Addr & value );

		int
		reuse_addr() const;

		acceptor_params_t &
		set_reuse_addr(
			int value );

		int
		flags() const;

		acceptor_params_t &
		set_flags(
			int value );

		int
		use_select() const;

		acceptor_params_t &
		set_use_select(
			int value );
		//! \}

	private :
		//! ��������� ����� ���������� ������.
		ACE_INET_Addr m_local_addr;

		//! ������� ������������� ����������������� ���������� ������.
		/*!
			�� ��������: 1 (����������������).
		 */
		int m_reuse_addr;

		//! ����� ��� Acceptor-�.
		/*!
			�� ���������: ACE_NONBLOCK.
		 */
		int m_flags;

		//! ����� �� ������������ select() ��� �������� �������
		//! ������� ���������� �����������.
		/*!
			�� ���������: 1.
		 */
		int m_use_select;
};

/*!
	\brief ��������������� ����� ��� ��������� �������� acceptor_params.

	������ �������������:
	\code
	acceptor_params( local_ip ).
		set_reuse_addr( 0 ).
		set_flags( ACE_NONBLOCK );
	\endcode
*/
inline acceptor_params_t
create_acceptor_params( const ACE_INET_Addr & local_addr )
{
	return acceptor_params_t( local_addr );
}

/*!
	\brief ��������������� ����� ��� ��������� �������� acceptor_params.

	������ �������������:
	\code
	acceptor_params( local_ip_str ).
		set_reuse_addr( 0 ).
		set_use_select( 0 );
	\endcode
*/
SO_5_TRANSPORT_EXPORT_FUNC_SPEC( acceptor_params_t )
create_acceptor_params( const std::string & local_addr );

/*!
	\name �������� ��� �������� acceptor_controller-��.
	\{
*/

//
// acceptor_controller_creator_t
//

//! ��������� acceptor_controller-��.
class SO_5_TRANSPORT_TYPE acceptor_controller_creator_t
{
	public:
		acceptor_controller_creator_t(
			//! ����� SO � ������� ������������� acceptor_controller-�.
			so_5::rt::so_environment_t & env );

		~acceptor_controller_creator_t();

		/*!
			����� ������� �������, ���������� � �������� ��������� ������
			� IP-�������.

			��� ��������� ��������� ����� �������� �� ���������.

			\note ������������ ������ ":<PORT>", ��������, ":3000".
		*/
		acceptor_controller_unique_ptr_t
		create(
			//! ������ c IP-������� �������.
			const std::string & ip_address,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ������ ��������� acceptor_controller-�.

			��������� ��������� ����� �������� �� ��������.
		*/
		acceptor_controller_unique_ptr_t
		create(
			const acceptor_params_t & acceptor_params,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ��������� acceptor_controller-� �
			����������������� ������.

			������ ���������� option_setter � ������� �������.
		*/
		acceptor_controller_unique_ptr_t
		create(
			const acceptor_params_t & acceptor_params,
			const channel_params_t & channel_params,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� �� ��������� �������� �������.
		*/
		acceptor_controller_unique_ptr_t
		create(
			const acceptor_params_t & acceptor_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ����������� �������.

			���� �������� � ����� ������ �� ���������, �� ��������
			acceptor_controller_unique_ptr_t( 0 ).
		*/
		acceptor_controller_unique_ptr_t
		create(
			const acceptor_params_t & acceptor_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			const std::string & reactor_name,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

	private:
		//! ���������� ����� ��� �������� acceptor_controller.
		acceptor_controller_unique_ptr_t
		create(
			const acceptor_params_t & acceptor_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			ACE_Reactor * reactor,
			so_5::throwing_strategy_t throwing_strategy );

		//! ���� ���������.
		const reactor_layer_t * const m_reactor_layer;
};

/*!
	\}
*/

/*!
	\name �������� ��� �������� connector_controller-��.
	\{
*/

//
// connector_controller_creator_t
//

//! ���������� connector_controller-��.
class SO_5_TRANSPORT_TYPE connector_controller_creator_t
{
	public:
		connector_controller_creator_t(
			//! ����� SO � ������� ������������� connector_controller-�.
			so_5::rt::so_environment_t & env );
		~connector_controller_creator_t();

		/*!
			����� ������� �������, ���������� � �������� ��������� ������
			� IP-�������.

			��� ��������� ��������� ����� �������� �� ���������.

			\note ������������ ������ ":<PORT>", ��������, ":3000".
		*/
		connector_controller_unique_ptr_t
		create(
			//! ������ c IP-������� �������.
			const std::string & ip_address,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ������ ��������� connector_controller-�.

			��������� ��������� ����� �������� �� ��������.
		*/
		connector_controller_unique_ptr_t
		create(
			const connector_params_t & connector_params,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ��������� connector_controller-� �
			����������������� ������.

			������ ���������� option_setter � ������� �������.
		*/
		connector_controller_unique_ptr_t
		create(
			const connector_params_t & connector_params,
			const channel_params_t & channel_params,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� �� ��������� �������� �������.
		*/
		connector_controller_unique_ptr_t
		create(
			const connector_params_t & connector_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

		/*!
			�������, ������� ��������� ������ ���.
		*/
		connector_controller_unique_ptr_t
		create(
			const connector_params_t & connector_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			const std::string & reactor_name,
			so_5::throwing_strategy_t throwing_strategy = so_5::THROW_ON_ERROR );

	private:
		//! ���������� ����� ��� �������� connector_controller.
		connector_controller_unique_ptr_t
		create(
			const connector_params_t & connector_params,
			const channel_params_t & channel_params,
			option_setter_unique_ptr_t option_setter,
			ACE_Reactor * reactor,
			so_5::throwing_strategy_t throwing_strategy );

		//! ���� ���������.
		const reactor_layer_t * const m_reactor_layer;
};

/*!
	\}
*/

} /* namespace socket */

} /* namespace so_5_transport */

#endif
