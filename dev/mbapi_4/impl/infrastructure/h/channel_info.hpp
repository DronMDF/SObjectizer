/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� ������� ����������� �������� ����� � ������,
	� ����� ������� ������� ���������� � �������� �����
	��� ���, ��� �������� � ������ ������.
*/

#if !defined( _MBAPI_4__IMPL__INFRASTRUCTURE__CHANNEL_INFO_HPP_ )
#define _MBAPI_4__IMPL__INFRASTRUCTURE__CHANNEL_INFO_HPP_

#include <map>

#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/mbox.hpp>

#include <so_5_transport/h/ifaces.hpp>

#include <mbapi_4/defs/h/endpoint.hpp>
#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/channel_uid_wrapper.hpp>
#include <mbapi_4/defs/h/mbapi_node_uid.hpp>

#include <mbapi_4/impl/infrastructure/h/availability_tables.hpp>

namespace mbapi_4
{

namespace impl
{

namespace infrastructure
{

//! ��������� ��� �������� ������ �����-������.
typedef std::vector< stagepoint_t > stagepoint_list_t;

//
// channel_info_t
//

//! ���������� � ������.
/*!
	��������:
		\li uid mbapi-����, ������� ��������� � ������ ������� ������;
		\li ������ �������� ����� ��������� �� ����� ������,
			������� ������ ������ ���� � ����� ������ �������� �����;
		\li ������ �����-������ ��������� �� ����� ������,
			������� ������ ������ ���� � ����� ������ ������.
*/
class channel_info_t
{
	public:
		channel_info_t();
		channel_info_t(
			const mbapi_node_uid_t & mbapi_node_uid,
			const so_5::rt::mbox_ref_t & mbox );

		//! ������������� mbapi-����.
		inline const mbapi_node_uid_t &
		node_uid() const
		{
			return m_node_uid;
		}

		//! �������� mbox ������.
		inline const so_5::rt::mbox_ref_t &
		mbox() const
		{
			return m_mbox;
		}

		//! �������� ������ �������� �����, �������
		//! ��� � �������� ������� �������� �����.
		/*!
			����������� �������� ����� ��������� �� ������.
		*/
		void
		find_dissapered_endpoints(
			//! ������� �������� �����.
			const available_endpoint_table_t & endpoint_table,
			//! �������� ������ ����������� �������� �����.
			endpoint_list_t & dissapered_endpoints );

		//! �������� ������ �����-������, �������
		//! ��� � �������� ������� �����-������.
		/*!
			����������� �����-������ ��������� �� ������.
		*/
		void
		find_dissapered_stagepoints(
			//! ������� �����-������.
			const available_stagepoint_table_t & stagepoint_table,
			//! �������� ������ ����������� �����-������.
			stagepoint_list_t & dissapered_stagepoints );

		//! ���������� � ���������� �������� ����� � ������.
		//! \{
		void
		insert_endpoint(
			const endpoint_t & endpoint );

		void
		remove_endpoint(
			const endpoint_t & endpoint );

		void
		insert_stagepoint(
			const stagepoint_t & stagepoint );

		void
		remove_stagepoint(
			const stagepoint_t & stagepoint );
		//! \}

		//! �������� ������ �����.
		//! \{
		inline const endpoint_list_t &
		endpoints() const
		{
			return m_endpoints;
		}


		inline const stagepoint_list_t &
		stagepoints() const
		{
			return m_stagepoints;
		}
		//! \}

	private:
		//! ������������� mbapi-���� � ������� ���������� �����,
		mbapi_node_uid_t m_node_uid;

		//! Mbox ������.
		so_5::rt::mbox_ref_t m_mbox;

		//! ������� �������� �����.
		endpoint_list_t m_endpoints;

		//! ������� �����-������.
		stagepoint_list_t m_stagepoints;
};

//! ��� ������ ��������� ��� channel_info_t.
typedef std::shared_ptr< channel_info_t >
	channel_info_ptr_t;

//! ������� � ����������� � �������.
typedef std::map<
		channel_uid_wrapper_t,
		channel_info_ptr_t >
	channel_info_table_t;


} /* namespace infrastructure */

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
