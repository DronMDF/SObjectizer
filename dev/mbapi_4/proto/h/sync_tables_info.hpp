/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� mbapi-������ ��� ������������� ������ �����.
*/

#if !defined( _MBAPI_4__PROTO__SYNC_TABLES_INFO_HPP_ )
#define _MBAPI_4__PROTO__SYNC_TABLES_INFO_HPP_

#include <vector>

#include <oess_2/stdsn/h/serializable.hpp>
#include <mbapi_4/proto/h/req_info.hpp>

namespace mbapi_4
{

namespace proto
{

//
// endpoint_info_t
//

//! ���������� � �������� �����.
class endpoint_info_t
	:
		public oess_2::stdsn::serializable_t
{
	OESS_SERIALIZER( endpoint_info_t )
	public:
		endpoint_info_t();
		virtual ~endpoint_info_t();

		//! ��� �������� �����.
		std::string m_name;

		//! ������������� mbapi-����,
		//! � ������� ���������������� ������ �������� �����.
		std::string m_node_uid;

		//! ���������� �� mbapi-����,
		//! � ������� ���������������� ������ �������� �����.
		oess_2::uint_t m_distance;

		//! �������� ������ �������� �����.
		std::vector< std::string > m_endpoint_stage_chain;
};

//
// endpoint_info_ptr_t
//

/*!
	\breif ����������� ��������� �� %endpoint_info_t.
	����������� oess_2::stdsn::shptr_t ��� endpoint_info_t.
*/
class endpoint_info_ptr_t
	:
		public oess_2::stdsn::shptr_t
{
	OESS_SERIALIZER( endpoint_info_ptr_t )
	OESS_2_SHPTR_IFACE( endpoint_info_ptr_t,
		mbapi_4::proto::endpoint_info_t,
		oess_2::stdsn::shptr_t )
};

//! ������ �������� �����.
typedef std::vector< endpoint_info_ptr_t > endpoint_info_table_t;

//
// stagepoint_info_t
//

//! ���������� � �����-������.
class stagepoint_info_t
	:
		public oess_2::stdsn::serializable_t
{
	OESS_SERIALIZER( stagepoint_info_t )
	public:
		stagepoint_info_t();
		virtual ~stagepoint_info_t();

		//! ��� ������.
		std::string m_name;

		//! ��� �������� �����.
		std::string m_endpoint_name;

		//! ������������� mbapi-����,
		//! � ������� ���������������� ������ �������� �����.
		std::string m_node_uid;

		//! ���������� �� mbapi-����,
		//! � ������� ���������������� ������ �������� �����.
		oess_2::uint_t m_distance;
};

//
// stagepoint_info_ptr_t
//

/*!
	\breif ����������� ��������� �� %stagepoint_info_t.
	����������� oess_2::stdsn::shptr_t ��� stagepoint_info_t.
*/
class stagepoint_info_ptr_t
	:
		public oess_2::stdsn::shptr_t
{
	OESS_SERIALIZER( stagepoint_info_ptr_t )
	OESS_2_SHPTR_IFACE( stagepoint_info_ptr_t,
		mbapi_4::proto::stagepoint_info_t,
		oess_2::stdsn::shptr_t )
};

//! ������ �������� �����.
typedef std::vector< stagepoint_info_ptr_t > stagepoint_info_table_t;

//
// sync_tables_info_t
//

/*!
	\brief ����� ������� ������������� ������.
*/
class sync_tables_info_t
	:
		public req_info_t
{
		OESS_SERIALIZER( sync_tables_info_t )

	public:
		//! ����������� �� ���������.
		sync_tables_info_t();
		virtual ~sync_tables_info_t();

		endpoint_info_table_t m_endpoints;
		stagepoint_info_table_t m_stagepoints;
};

//
// ping_resp_info_t
//

/*!
	\brief ����� ������ �� ������ ������������� ������.
*/
class sync_tables_resp_info_t
	:
		public resp_info_t
{
		OESS_SERIALIZER( sync_tables_resp_info_t )

	public:
		//! ����������� �� ���������.
		sync_tables_resp_info_t();
		virtual ~sync_tables_resp_info_t();
};

} /* namespace proto */

} /* namespace mbapi_4 */

#endif
