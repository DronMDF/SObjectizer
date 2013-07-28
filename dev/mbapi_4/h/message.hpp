/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������ ��������� ��� �������� �� mbapi.
*/

#if !defined( _MBAPI_4__MESSAGE_HPP_ )
#define _MBAPI_4__MESSAGE_HPP_

#include <memory>

#include <oess_2/stdsn/h/serializable.hpp>

#include <so_5/rt/h/message.hpp>

#include <mbapi_4/h/declspec.hpp>

#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/endpoint.hpp>
#include <mbapi_4/defs/h/oess_id_wrapper.hpp>

namespace mbapi_4
{

class mbapi_layer_t;

//
// message_base_t
//

//! ������� ����� mbapi ���������.
/*!
	������ ��� ������� �������� ����������� �� �����������
	mbapi_layer_t, ������� � ���� �������, ������� �����������
	message_t. ������� mbapi_layer_t-��������� ��� ������� �
	���� ��������������. ��������� ����������� ����� �������� �
	message_t, �� ����������, ������ ��� message_t �������� ��������� �������.

*/
class MBAPI_4_TYPE message_base_t
	:
		public so_5::rt::message_t
{
	public:
		message_base_t(
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t & current_stage );

		virtual ~message_base_t();

		//! ���������� � �������� ������ ����� �������� ���� ���������.
		//! \{
		const endpoint_t &
		from() const
		{
			return m_from;
		}

		const endpoint_t &
		to() const
		{
			return m_to;
		}
		//! \}

		//! ��� ������� ������.
		const stagepoint_t &
		current_stage() const
		{
			return m_current_stage;
		}

	protected:
		//! ���������� ��������.
		void
		move_next_impl(
			const oess_id_wrapper_t & oess_id,
			std::unique_ptr< oess_2::stdsn::serializable_t > msg ) const;

	private:
		//! ���� mbapi.
		mbapi_layer_t & m_mbapi_layer;

		//! �������� ����� � ������� ���� ���������.
		const endpoint_t m_from;
		//! �������� ����� �� ������� ���� ���������.
		const endpoint_t m_to;

		//! ������� ������.
		const stagepoint_t m_current_stage;

};

//
// message_t
//

//! ����� ������� ��� �������� ��������� �� mbapi.
template < class SERIALIZABLE >
class message_t
	:
		public message_base_t
{
		typedef message_base_t base_type_t;

	public:
		message_t(
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t & current_stage )
			:
				base_type_t( mbapi_layer, from, to, current_stage ),
				m_msg( new SERIALIZABLE )
		{}

		message_t(
			std::unique_ptr< SERIALIZABLE > msg,
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t & current_stage )
			:
				message_base_t( mbapi_layer, from, to, current_stage ),
				m_msg( std::move( msg ) )
		{}

		virtual ~message_t()
		{}

		//! �������� ������ � ���������.
		//! \{
		SERIALIZABLE &
		msg()
		{
			return *m_msg;
		}

		inline const SERIALIZABLE &
		msg() const
		{
			return *m_msg;
		}
		//! \}

		//! ��������� ��������� ������ �� �������.
		//! ����� �������� ��������� ������������ �� ������
		//! \{

		//! ��������� ��������� ��� ���������.
		//! ��������� ���������� ���������.
		void
		move_next(
			std::unique_ptr< SERIALIZABLE > msg ) const
		{
			if( msg.get() && !current_stage().is_endpoint() )
			{
				std::unique_ptr< oess_2::stdsn::serializable_t >
					message(
						static_cast< oess_2::stdsn::serializable_t * >(
							msg.release() ) );

				oess_id_wrapper_t oess_id(
					&SERIALIZABLE::oess_serializer_t::type_name() );

				move_next_impl(
					oess_id,
					std::move( message ) );
			}
		}

		void
		move_next() const
		{
			move_next( std::move( m_msg ) );
		}
		//! \}

	private:
		//! ���������.
		mutable std::unique_ptr< SERIALIZABLE > m_msg;
};


//
// binary_message_t
//

//! ����� ��������� ���������, � �������
//! ������ �������� � ������� oess.
/*!
	������ ��� �������, ����� ���������� �������� ��� ���������
	���������� �� ����.
	�������� ����������� ������ � ������������ ������,
	�.�. ����� �� ������ ���� �������� �� ��������� ����
	mbapi_4 ��������� ����� ������ endpoint ��� stagepoint.
*/
class MBAPI_4_TYPE binary_message_t
	:
		public message_base_t
{
		typedef message_base_t base_type_t;

	public:
		binary_message_t(
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t & current_stage,
			//! ������������� oess ����.
			const oess_id_wrapper_t & oess_id_wrapper,
			//! ����������� � ������� oess ���������.
			const std::string & bin_message );

		virtual ~binary_message_t();

		//! ������������� oess ����.
		oess_id_wrapper_t m_oess_id_wrapper;

		//! ����������� � ������� oess ���������.
		std::string m_bin_message;
};

} /* namespace mbapi_4 */

#endif
