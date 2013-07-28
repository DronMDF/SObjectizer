/*
	MBAPI 4.
*/

/*!
	\file
	\brief ��������� mbapi-����.
*/

#if !defined( _MBAPI_4__IMPL__INFRASTRUCTURE__NETWORK_ENVIRONMENT_HPP_ )
#define _MBAPI_4__IMPL__INFRASTRUCTURE__NETWORK_ENVIRONMENT_HPP_

#include <ace/RW_Thread_Mutex.h>

#include <so_5/rt/h/mbox_ref.hpp>

#include <mbapi_4/defs/h/mbapi_node_uid.hpp>

#include <mbapi_4/impl/infrastructure/h/availability_tables.hpp>
#include <mbapi_4/impl/infrastructure/h/channel_info.hpp>

namespace mbapi_4
{

namespace impl
{

namespace infrastructure
{

//
// network_environment_t
//

//! ������� ��������� mbapi.
/*!
	������ � ���� ��� ���������� � ��������� �������� ������
	(������� �������� �����)  � ������-������� (������� �����-������)
	������� ����.

	\note ����� �� �������� ������ ����������, � ������������� ������
	���������� ������������.
*/
class network_environment_t
{
	public:
		network_environment_t();

		//! �������� ������������� mbapi-����.
		inline const mbapi_node_uid_t &
		node_uid() const
		{
			return m_node_uid;
		}

		//! �������� ��������� �������� �����.
		/*!
			��� �������� �������� ����� �� ������ mbapi-����
			���������� � ��� ������ ���� ��������� � �������
			�������� ����� ���������� �� ���� ���� �� ��� ��� ��� ���.
		*/
		void
		add_local_endpoint(
			const endpoint_stage_chain_t & endpoint_stage_chain );

		//! ������ ��������� �������� �����.
		void
		remove_local_endpoint(
			const endpoint_t & endpoint );

		//! �������� ��������� �����-������.
		/*!
			��� �������� �����-������ �� ������ mbapi-����
			���������� � ��� ������ ���� ��������� � �������
			�����-������ ���������� �� ���� ���� �� ��� ��� ��� ���.
		*/
		void
		add_local_stagepoint(
			const stagepoint_t & stagepoint );

		//! ������ ��������� �����-������.
		void
		remove_local_stagepoint(
			const stagepoint_t & stagepoint );


		//! �������� ��������� � ������������ � ����������� �� ��������� ������.
		void
		update_channel(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id,
			//! ������������� mbapi-����.
			const mbapi_node_uid_t & node_uid,
			//! Mbox ������������� ������ mbapi.
			const so_5::rt::mbox_ref_t & mbox,
			//! ������� �������� ����� ������.
			const available_endpoint_table_t & endpoints,
			//! ������� �����-������ ������.
			const available_stagepoint_table_t & stagepoints );

		//! ����������� ������ ��� ������ �������� ����� � ������.
		void
		copy_tables_data(
			//! ������� �������� �����.
			infrastructure::available_endpoint_table_t & endpoints,
			//! ������� �����-������.
			infrastructure::available_stagepoint_table_t & stagepoints ) const;

		//! ������ ���������� � ������, ����� ����� ��������.
		void
		delete_channel(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id );

		endpoint_list_t
		query_endpoint_list() const;

		//! ���������� � ��������� ������.
		/*!
			\return ���� ��������� ������ ���������� �������,
			�� ������ true, ����� false.
		*/
		bool
		shift_to_next_stage(
			//! �������� �����, � ������� ���������� ���������.
			const endpoint_t & from,
			//! �������� �����, �� ������� ���������� ���������.
			const endpoint_t & to,
			//! ������� ������, ������� ����� �������� ����
			//! ��������� ������ ����� �������.
			stagepoint_t & current_stage ) const;

		//! ��������� mbox � channel_uid ��� ������,
		//! � ������� ����� �������� stagepoint.
		/*!
			\return true -  ���� ����� stagepoint ������ � ���
			mbox � channel_id ����������.
		*/
		bool
		define_channel_info(
			const stagepoint_t & stagepoint,
			so_5::rt::mbox_ref_t & mbox,
			channel_uid_wrapper_t & channel_id ) const;

		//! ������� ��� ����-������������.
		//! \{
		inline available_endpoint_table_t
		utest_get_endpoint_table() const
		{
			return m_endpoints;
		}

		inline available_stagepoint_table_t
		utest_get_stagepoint_table() const
		{
			return m_stagepoints;
		}

		inline channel_info_table_t
		utest_get_channel_info_table() const
		{
			return m_channels_infos;
		}
		//! \}

	private:
		//! ����� ���������� � ������.
		/*!
			���� ����� �� ������, �� �� �����������
			� ������� ���������� � �������.
		*/
		channel_info_t &
		find_channel(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id,
			//! ������������� mbapi-����.
			const mbapi_node_uid_t & node_uid,
			//! Mbox ������������� ������ mbapi.
			const so_5::rt::mbox_ref_t & mbox );

		//! ����� ���������� � ������.
		channel_info_t &
		find_channel(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id );

		//! ������� ���������� � �������� �������� ������.
		void
		remove_endpoints(
			const endpoint_list_t & endpoints_to_be_removed );

		//! ������� ���������� � �������� ������-�������.
		void
		remove_stagepoints(
			const stagepoint_list_t & stagepoints_to_be_removed );

		//! ���������������� ������ � �������� �������� ����� ������.
		void
		sync_endpoint_table(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id,
			//! ���������� � ������.
			channel_info_t & channel_info,
			//! ������� �������� ����� ������.
			const available_endpoint_table_t & endpoints );

		//! ���������������� ������ � �������� �����-������ ������.
		void
		sync_stagepoint_table(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id,
			//! ���������� � ������.
			channel_info_t & channel_info,
			//! ������� �����-������ ������.
			const available_stagepoint_table_t & stagepoints );

		//! ������� ����� ������� �������� �����.
		void
		copy_endpoints_table(
			//! ������� �������� �����.
			infrastructure::available_endpoint_table_t & endpoints ) const;

		//! ������� ����� ������� �����-������.
		void
		copy_stagepoints_table(
			//! ������� �������� �����.
			infrastructure::available_stagepoint_table_t & stagepoints ) const;

		//! ���������� ���������� ������ � ������� ������ ������ �������� �����.
		//! ������ ������������ � ������ �����������.
		bool
		shift_to_next_stage_direct(
			const endpoint_t & ep,
			stagepoint_t & current_stage ) const;

		//! ���������� � ������ ������ � ������� ������ ������ �������� �����.
		bool
		shift_to_first_stage_direct(
			const endpoint_t & ep,
			stagepoint_t & current_stage ) const;

		//! ������������� mbapi-����.
		const mbapi_node_uid_t m_node_uid;

		//! ������� �������� �����.
		available_endpoint_table_t m_endpoints;

		//! ������� �����-������.
		available_stagepoint_table_t m_stagepoints;

		//! ������� � ����������� � �������.
		channel_info_table_t m_channels_infos;
};

} /* namespace infrastructure */

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
