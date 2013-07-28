/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� mbapi-������ ��� �������� ���������.
*/

#if !defined( _MBAPI_4__PROTO__SEND_MSG_INFO_HPP_ )
#define _MBAPI_4__PROTO__SEND_MSG_INFO_HPP_

#include <mbapi_4/comm/h/transmit_info.hpp>

#include <mbapi_4/proto/h/req_info.hpp>

namespace mbapi_4
{

namespace proto
{

//
// send_msg_info_t
//

/*!
	\brief ����� ������� �� �������� ���������.
*/
class send_msg_info_t
	:
		public req_info_t
{
		OESS_SERIALIZER( send_msg_info_t )

	public:
		//! ����������� �� ���������.
		send_msg_info_t();

		//! ���������������� �����������.
		send_msg_info_t(
			comm::transmit_info_t & transmit_info );

		virtual ~send_msg_info_t();

		//! ��� �������� ����� �����������.
		std::string m_from;

		//! ��� �������� ����� ����������.
		std::string m_to;

		//! ��� ������.
		std::string m_current_stage;

		//! ���� ����������� �������������� ������
		//! �������� ����� m_from ��� m_to.
		/*!
			���� m_stagpoint_owner ����� �������� 0, ��
			�������� ������ m_from, ����� m_to
		*/
		oess_2::uchar_t m_stagpoint_owner;

		//! ��������� ���� ������������� ������.
		//! \{
		inline void
		set_stage_owner_is_from()
		{
			m_stagpoint_owner = 0;
		}

		inline void
		set_stage_owner_is_to()
		{
			m_stagpoint_owner = 1;
		}
		//! \}

		//! �������� ��� �������� ����� ������� ������� ������ ������.
		inline const std::string &
		stage_owner_endpoint() const
		{
			if( 0 == m_stagpoint_owner )
				return m_from;

			return m_to;
		}

		//! ������������� ���� ���������.
		std::string m_oess_id;

		//! �������� ����� ���������.
		std::string m_payload;
};

//
// send_msg_resp_info_t
//

/*!
	\brief ����� ������ �� �������� ���������.
*/
class send_msg_resp_info_t
	:
		public resp_info_t
{
		OESS_SERIALIZER( send_msg_resp_info_t )

	public:
		//! ����������� �� ���������.
		send_msg_resp_info_t();

		//! ���������������� �����������.
		/*! ��� ������ ���������� ��������� �� ������. */
		send_msg_resp_info_t(
			//! ��� �������� �������� ���������.
			int ret_code );

		//! ���������������� �����������.
		send_msg_resp_info_t(
			//! ��� �������� �������� ���������.
			int ret_code,
			//! ��������� �������� ������.
			const std::string & error_msg );

		virtual ~send_msg_resp_info_t();
};

} /* namespace proto */

} /* namespace mbapi_4 */

#endif
