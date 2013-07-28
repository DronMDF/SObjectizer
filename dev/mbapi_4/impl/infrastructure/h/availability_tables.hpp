/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� ������� ����������� �������� ����� � ������,
	� ����� ������� ������� ���������� � �������� �����
	��� ���, ��� �������� � ������ ������.
*/

#if !defined( _MBAPI_4__IMPL__INFRASTRUCTURE__AVAILABILITY_TABLES_HPP_ )
#define _MBAPI_4__IMPL__INFRASTRUCTURE__AVAILABILITY_TABLES_HPP_

#include <map>

#include <so_5_transport/h/ifaces.hpp>

#include <mbapi_4/defs/h/endpoint.hpp>
#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/mbapi_node_uid.hpp>
#include <mbapi_4/defs/h/channel_uid_wrapper.hpp>

namespace mbapi_4
{

namespace impl
{

namespace infrastructure
{

//
// available_endpoint_t
//

//! ������ � ��������� �������� �����.
struct available_endpoint_t
{
	available_endpoint_t();

	//! ������������� ������ �� �������� ����� ������ �������� �����.
	/*!
		�������� ��������� ������ ��� ������� ������ ����.
		��� �������, ������� �������� ��� �������������,
		���������� m_channel_id �� ����� �������� ��������,
		������ ��� �������������� ������� ������� SO ��������
		�� ������ �������� �������� ��� ������� SO ��������.
	*/
	channel_uid_wrapper_t m_channel_id;

	//! ������������� mbapi-����,
	//! � ������� ���������������� ������ �������� �����.
	mbapi_node_uid_t m_node_uid;

	//! ���������� �� mbapi-����,
	//! � ������� ���������������� ������ �������� �����.
	/*!
		���������� ���� �� ������ ���� ����� 0.
		���������� ����� ������, ������� ������� �������, 1.
		���������� ����� ������, ������� �� ������� ����� �����,
		�� ���� ����, ������� �������� ������ � ����, ����� 2,
		� �.�.

		��� ������������� ������� �������� ����� � ����� �� ��������
		��������� ���� ���� ���������, ��� ���������� ������ ����
		�������� ����� ����� ��� �������� ��� ����.
	*/
	unsigned int m_distance;

	//! �������� ������ �������� �����.
	endpoint_stage_chain_t m_endpoint_stage_chain;
};

//! ��� ������ ��������� ��� available_endpoint_t.
typedef std::shared_ptr< available_endpoint_t >
	available_endpoint_ptr_t;

//! ��� ������� � ����������� � �������� ������.
typedef std::map<
		endpoint_t,
		available_endpoint_ptr_t >
	available_endpoint_table_t;

//
// available_stagepoint_t
//

//! ������ � ������.
struct available_stagepoint_t
{
	available_stagepoint_t();

	//! ������������� ������ �� �������� ����� ������ �������� �����.
	/*!
		�������� ��������� ������ ��� ������� ������ ����.
		��� �������, ������� �������� ��� �������������,
		���������� m_channel_id �� ����� �������� ��������,
		������ ��� �������������� ������� ������� SO ��������
		�� ������ �������� �������� ��� ������� SO ��������.
	*/
	channel_uid_wrapper_t m_channel_id;

	//! ������������� mbapi-����,
	//! � ������� ���������������� ������ �������� �����.
	mbapi_node_uid_t m_node_uid;

	//! ���������� �� mbapi-����,
	//! � ������� ���������������� ������ �������� �����.
	/*!
		���������� ���� �� ������ ���� ����� 0.
		���������� ����� ������, ������� ������� �������, 1.
		���������� ����� ������, ������� �� ������� ����� �����,
		�� ���� ����, ������� �������� ������ � ����, ����� 2,
		� �.�.

		��� ������������� ������� �����-������ � ����� �� ��������
		��������� ���� ���� ���������, ��� ���������� ������ ����
		�������� ����� ����� ��� �������� ��� ����.
	*/
	size_t m_distance;
};

//! ��� ������ ��������� ��� available_stagepoint_t.
typedef std::shared_ptr< available_stagepoint_t >
	available_stagepoint_ptr_t;

//! ��� ������� � ����������� � �������.
typedef std::map<
		stagepoint_t,
		available_stagepoint_ptr_t >
	available_stagepoint_table_t;

} /* namespace infrastructure */

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
