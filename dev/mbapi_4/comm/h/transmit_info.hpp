/*
	MBAPI 4.
*/

/*!
	\file
	\brief ��������������� ����������� �����.
*/

#if !defined( _MBAPI_4__COMM__TRANSMIT_INFO_HPP_ )
#define _MBAPI_4__COMM__TRANSMIT_INFO_HPP_

#include <string>

#include <oess_2/defs/h/types.hpp>
#include <oess_2/stdsn/h/serializable.hpp>

#include <so_5/rt/h/message.hpp>

#include <mbapi_4/h/declspec.hpp>

#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/endpoint.hpp>

#include <mbapi_4/defs/h/oess_id_wrapper.hpp>
#include <mbapi_4/defs/h/channel_uid_wrapper.hpp>


namespace mbapi_4
{

namespace comm
{

//! ��� ��� �������� ��������� ������������� ������.
typedef std::string raw_data_t;

//
// transmit_info_t
//

//! ���������� � ������������ ���������.
/*!
	������ ��� ��������� � mbapi �������
	���������, ������� ��������� �� �������.
	� ������ ���������������� ��������� �� ������ ����
	��� ����� �������� �� m_payload. ����
	�� ��������� ������ ���� ������ ���������������
	� ������ �����, �� ��� �������� ����� ���������� ��� ����.
*/
struct MBAPI_4_TYPE transmit_info_t
	:
		public so_5::rt::message_t
{
	transmit_info_t(
		//! �������� ����� � ������� ���� ���������.
		const endpoint_t & from,
		//! �������� ����� �� ������� ���� ���������.
		const endpoint_t & to,
		//! ������� ������.
		const stagepoint_t & current_stage,
		//! ������������� ����.
		const oess_id_wrapper_t & oess_id )
		:
			m_from( from ),
			m_to( to ),
			m_current_stage( current_stage ),
			m_oess_id( oess_id )
	{}

	transmit_info_t(
		//! �������� ����� � ������� ���� ���������.
		const std::string & from,
		//! �������� ����� �� ������� ���� ���������.
		const std::string & to,
		//! ������� ������.
		const std::string & current_stage_name,
		//! ������� ������.
		const std::string & current_stage_endpoint_name,
		//! ������������� ����.
		const std::string & oess_id )
		:
			m_from( from ),
			m_to( to ),
			m_current_stage( current_stage_name, current_stage_endpoint_name ),
			m_oess_id( oess_id )
	{}

	virtual ~transmit_info_t();

	//! ������������� ������ � ������� ���� ��������� ���������.
	/*!
		�������� ����� �����, ������ � ������, ���� ���������
		���������������� mbapi_layer_impl_t � ������ �����.
	*/
	channel_uid_wrapper_t m_channel_id;

	//! �������� �����, � ������� ���������� ���������.
	endpoint_t m_from;
	//! �������� �����, �� ������� ���������� ���������.
	endpoint_t m_to;

	//! ������� ������.
	/*!
		��� ����� ������ �������� ���������
		m_current_stage - ��� m_from.
	*/
	stagepoint_t m_current_stage;

	//! OESS ������������� ���������.
	oess_id_wrapper_t m_oess_id;

	//! �������� ������������� ���������.
	raw_data_t m_payload;
};

//! ��������� unique_ptr ��� transmit_info_t.
typedef std::unique_ptr< transmit_info_t > transmit_info_unique_ptr_t;

//! ���������� � ������������ ���������.
/*!
	������ ��� ��������� � mbapi �������
	���������, ������� ������� � ���������� � �������� �����
	������������� � ������ ����, ��� �������������
	�������� ��������� � ����� ��������� ���������������� � transmit_info_t.
*/
struct MBAPI_4_TYPE local_transmit_info_t
	:
		public so_5::rt::message_t
{
	local_transmit_info_t(
		//! �������� ����� � ������� ���� ���������.
		const endpoint_t & from,
		//! �������� ����� �� ������� ���� ���������.
		const endpoint_t & to,
		//! ������� ������.
		const stagepoint_t & current_stage,
		//! ������������� ����.
		const oess_id_wrapper_t & oess_id,
		//! ���������.
		std::unique_ptr< oess_2::stdsn::serializable_t > message )
		:
			m_from( from ),
			m_to( to ),
			m_current_stage( current_stage ),
			m_oess_id( oess_id ),
			m_message( std::move( message ) )
	{}

	virtual ~local_transmit_info_t();

	//! �������� �����, � ������� ���������� ���������.
	endpoint_t m_from;
	//! �������� �����, �� ������� ���������� ���������.
	endpoint_t m_to;

	//! ������� ������.
	/*!
		��� ����� ������ �������� ���������
		m_current_stage - ��� m_from.
	*/
	stagepoint_t m_current_stage;

	//! OESS ������������� ���������.
	oess_id_wrapper_t m_oess_id;

	//! ��������� �� ������ ���������.
	std::unique_ptr< oess_2::stdsn::serializable_t > m_message;

};

//! ��������� unique_ptr ��� transmit_info_t.
typedef std::unique_ptr< local_transmit_info_t > local_transmit_info_unique_ptr_t;

} /* namespace comm */

} /* namespace mbapi_4 */

#endif
