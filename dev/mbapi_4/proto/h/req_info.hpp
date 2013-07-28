/*
	SObjectizer 5 SOP4
*/

/*!
	\file
	\brief ������� ������ � ����������� ��� ��������� binSOP.
*/

#if !defined( _MBAPI_4__PROTO__REQ_INFO_HPP_ )
#define _MBAPI_4__PROTO__REQ_INFO_HPP_

#include <memory>
#include <string>
#include <deque>

#include <oess_2/stdsn/h/serializable.hpp>
#include <oess_2/stdsn/h/shptr.hpp>

namespace mbapi_4
{

namespace proto
{

//
// req_info_t
//

/*!
	\brief ������� ����� ��� ��������� mbapi-�������.

	�� ����� ������ ������������ ��� ������-�������,
	��� � ������-������. ������-������� ������������
	��������������� �� %req_info_t, � ������-������
	������������ �� resp_info_t.
*/
class req_info_t
	:
		public oess_2::stdsn::serializable_t
{
	OESS_SERIALIZER( req_info_t )
	public:
		//! ����������� �� ���������.
		/*!
			�������� ������������� �������.
		*/
		req_info_t();
		//! ��������� ���������������� �����������.
		req_info_t(
			//! ������������� �������/������
			oess_2::int_t req_id );
		virtual ~req_info_t();

		//! ��������� �������������� �������� SOP.
		/*!
			\return ������� ������������ req_id_t.
		*/
		oess_2::int_t
		query_req_id() const;

	private:
		//! ������������� �������� SOP.
		/*! ������� ������������ req_id_t. */
		oess_2::int_t	m_req_id;
};

//
// req_info_ptr_t
//

/*!
	\breif ����������� ��������� �� %req_info_t.
	����������� oess_2::stdsn::shptr_t ��� req_info_t.
*/
class req_info_ptr_t
	:
		public oess_2::stdsn::shptr_t
{
	OESS_SERIALIZER( req_info_ptr_t )
	OESS_2_SHPTR_IFACE( req_info_ptr_t,
		mbapi_4::proto::req_info_t,
		oess_2::stdsn::shptr_t )
};

//
// resp_info_t
//

/*!
	\brief ������� ����� ��� �������-�������.

	� ������ �� ������ ������ ���������� ��� �������� �,
	��������, ������ � ��������� ������. �������� ����
	����� �������� � ������� ��� ���� ������� �����.
*/
class resp_info_t
	:
		public req_info_t
{
		OESS_SERIALIZER( resp_info_t )

	public:
		//! ����������� �� ���������.
		/*! �������� req_id � ret_code */
		resp_info_t();

		//! ���������������� �����������.
		/*! ��� ������, ����� ��� �������� ������. */
		resp_info_t(
			//! ������������� ��������.
			oess_2::int_t req_id,
			//! ��� ��������.
			oess_2::int_t ret_code );

		//! ���������������� �����������.
		resp_info_t(
			//! ������������� ��������.
			oess_2::int_t req_id,
			//! ��� ��������.
			oess_2::int_t ret_code,
			//! ��������� �������� ������.
			const std::string & error_msg );

		virtual ~resp_info_t();

		//! ���������� ��� �������� ��������� �������.
		oess_2::int_t
		query_ret_code() const;

		//! ���������� �������� ������.
		/*! ���������� ������ ������, ���� ��������
			������ �� ���� �������� � ���������� ����. */
		const std::string &
		query_error_msg() const;

	private:
		//! ��� �������� ��������� �������.
		oess_2::int_t m_ret_code;
		//! �������� ������.
		std::string m_error_msg;
};

//
// unknown_resp_info_t
//

/*!
	\brief ����� ������ �� ����������� �����.

	���� �� SOP �������� ����������� �����, ���� ����� ��
	������� ���������, �� � ����� ���������� ������
	����� ����.
*/
class unknown_resp_info_t
	:
		public resp_info_t
{
		OESS_SERIALIZER( unknown_resp_info_t )

	public:
		//! ����������� �� ���������.
		unknown_resp_info_t();
		//! ��������� ���������������� �����������.
		unknown_resp_info_t(
			//! ��� ��������.
			oess_2::int_t ret_code,
			//! ��������� �������� ������.
			const std::string & error_msg );
		virtual ~unknown_resp_info_t();
};

//
// req_info_storage_t
//

/*!
	\brief ��������� ��������� SOP.
*/
typedef std::deque< req_info_ptr_t > req_info_storage_t;

namespace req_id
{

//
// req_id_t
//

/*!
	\brief �������������� ��������� SOP.

	������: aarrvv, ���:
	- aa - ������ (0x00) ��� ����� (0x01)
	- rr - ��� ��������
	- vv - ������ SOP (������� - 0x07)
*/
enum req_id_t
{
	//! ������ �� ������� ���������.
	REQ_SEND_MSG = 0x000105,

	//! ����� �� ������� ������� ���������.
	RESP_SEND_MSG = 0x010105,

	//! ��������� �������� ������ unknown_resp_info_t.
	RESP_UNKNOWN = 0x010005,

	//! ������ ping.
	REQ_PING = 0x000406,

	//! ����� �� ping.
	RESP_PING = 0x010406,

	//! ������ ��������� handshake.
	REQ_HANDSHAKE = 0x000507,

	//! ����� �� ��������� handshake.
	RESP_HANDSHAKE = 0x010507,

	//! ������ �� ������������� ���������� ������.
	REQ_SYNC_TABLES = 0x000607,

	//! ����� �� ������������� ���������� ������.
	RESP_SYNC_TABLES = 0x010607
};

} /* namespace req_id */

} /* namespace proto */

} /* namespace mbapi_4 */

#endif
