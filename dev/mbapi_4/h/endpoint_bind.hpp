/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� �������� ����� �� ������ ����.
*/

#if !defined( _MBAPI_4__ENDPOINT_BIND_HPP_ )
#define _MBAPI_4__ENDPOINT_BIND_HPP_

#include <so_5/rt/h/agent.hpp>

#include <mbapi_4/defs/h/endpoint.hpp>
#include <mbapi_4/h/message.hpp>

#include <mbapi_4/h/declspec.hpp>
#include <mbapi_4/h/stagepoint_bind.hpp>

namespace mbapi_4
{

//
// endpoint_bind_t
//

//! �������� �������� �����.
/*!
	������������ ����� ����� �������� ����� �� ������ ����.
	� ��� ������� ����� ������������� �� ���������
	���������� ������ �������� ����� � ���������� ���������
	������ �������� ������.

	�������� �������� ����� �������� mbapi_layer_t,
	������� ������������ �� ������������.

	��� ����������� �������� �������� �����
	������������� ��������� ������������ �� ������ ����.
*/
class MBAPI_4_TYPE endpoint_bind_t
	:
		public stagepoint_bind_t
{
	friend class mbapi_layer_t;

	endpoint_bind_t( const endpoint_bind_t & );
	void
	operator = ( const endpoint_bind_t & );

	//! ��������� ����������� ����� ���������� ������ ���� mbapi.
	endpoint_bind_t(
		std::unique_ptr< impl::stagepoint_bind_impl_t > impl );

	public:
		virtual ~endpoint_bind_t();

		//! �������� ������������ �����-������.
		const endpoint_t &
		endpoint() const;

		//! �������� ���������.
		template< class SERIALIZABLE >
		void
		send(
			//! �������.
			const endpoint_t & to,
			//! ���������.
			std::unique_ptr< SERIALIZABLE > & msg )
		{
			const oess_id_wrapper_t oess_id(
				&SERIALIZABLE::oess_serializer_t::type_name() );

			return send_impl(
				to,
				oess_id,
				std::unique_ptr< oess_2::stdsn::serializable_t >(
					msg.release() ) );
		}

		//! �������� ���������.
		template< class SERIALIZABLE >
		void
		send(
			//! �������.
			const endpoint_t & to,
			//! ���������.
			std::unique_ptr< SERIALIZABLE > && msg )
		{
			const oess_id_wrapper_t oess_id(
				&SERIALIZABLE::oess_serializer_t::type_name() );

			return send_impl(
				to,
				oess_id,
				std::unique_ptr< oess_2::stdsn::serializable_t >(
					msg.release() ) );
		}

		//! �������� �������� ���������.
		void
		send_binary_message(
			//! �������.
			const endpoint_t & to,
			//! ��� ���������.
			const oess_id_wrapper_t & oess_id,
			//! �������� ���������.
			const std::string & binary_message );

	private:
		//! ��������� ��������.
		void
		send_impl(
			//! �������.
			const endpoint_t & to,
			//! ��� ���������.
			const oess_id_wrapper_t & oess_id,
			//! ���������.
			std::unique_ptr< oess_2::stdsn::serializable_t > msg );

		const endpoint_t m_endpoint;
};

//! ��������� unique_ptr ��� endpoint_bind_t
typedef std::unique_ptr< endpoint_bind_t > endpoint_bind_unique_ptr_t;

} /* namespace mbapi_4 */

#endif
