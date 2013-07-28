/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���������� ���� ������ � mbapi_4.
*/

#if !defined( _MBAPI_4__IMPL__MBAPI_LAYER_IMPL_HPP_ )
#define _MBAPI_4__IMPL__MBAPI_LAYER_IMPL_HPP_

#include <vector>

#include <ace/RW_Thread_Mutex.h>

#include <oess_2/stdsn/h/serializable.hpp>

#include <so_5/rt/h/so_environment.hpp>

#include <so_5/util/h/mutex_pool.hpp>

#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/mbox.hpp>

#include <mbapi_4/defs/h/mbapi_node_uid.hpp>
#include <mbapi_4/defs/h/channel_uid_wrapper.hpp>

#include <mbapi_4/impl/h/types.hpp>

#include <mbapi_4/impl/h/stagepoint_bind_impl.hpp>
#include <mbapi_4/impl/infrastructure/h/network_environment.hpp>

namespace mbapi_4
{

class mbapi_layer_t;

namespace impl
{

//
// bind_section_t
//

//! ����������� ������ ��� ������ ������������.
struct bind_section_t
{
	bind_section_t()
		:
			m_bind_mutex_pool( 32 )
	{}

	//! ����� ��� �������� � �������������.
	ACE_RW_Thread_Mutex m_lock;

	//! ��� ��� ������� �������� ������������.
	typedef std::map< stagepoint_t, stagepoint_bind_impl_t * >
		bind_table_t;

	//! ������� �������� ������������.
	bind_table_t m_bind_table;

	//! ��� �������� ��� ������ ������������.
	so_5::util::mutex_pool_t< ACE_RW_Thread_Mutex > m_bind_mutex_pool;
};

//
// network_section_t
//

//! ������ ��� ������ � mbapi-�����.
struct network_section_t
{
	//! ����� ��� ���������� �������� ��������� ��������� mbapi-����.
	mutable ACE_RW_Thread_Mutex m_lock;

	//! ��������� mbapi-����.
	infrastructure::network_environment_t m_netowork_environment;
};

//
// types_repositories_section_t
//

//! ������ ��� ������ � ������������� �����.
struct types_repositories_section_t
{
	void
	add_repository(
		const oess_2::stdsn::types_repository_t & types_repository );

	void
	remove_repository(
		const oess_2::stdsn::types_repository_t & types_repository );

	//! ��������� �����.
	types_repositories_list_t m_types_repositories;

	//! ����� ��� ������ ������������.
	ACE_RW_Thread_Mutex m_lock;
};

//
// mbapi_layer_impl_t
//

//! ���������� ���� ������ � mbapi_4.
class mbapi_layer_impl_t
{
	public:
		mbapi_layer_impl_t(
			//! ����� SO.
			so_5::rt::so_environment_t & so_environment,
			//! ���� mbapi.
			mbapi_layer_t & mbapi_layer );

		~mbapi_layer_impl_t();

		inline so_5::rt::so_environment_t &
		so_environment()
		{
			return m_so_environment;
		}

		inline mbapi_layer_t &
		mbapi_layer()
		{
			return m_mbapi_layer;
		}

		//! �������� ������������� mbapi-����.
		inline const mbapi_node_uid_t &
		node_uid() const
		{
			return m_network_section
				.m_netowork_environment
					.node_uid();
		}

		//! ���������� � �������� ������������.
		//! \{
		inline void
		add_repository(
			const oess_2::stdsn::types_repository_t & types_repository )
		{
			m_types_repositories_section.add_repository( types_repository );
		}

		void
		remove_repository(
			const oess_2::stdsn::types_repository_t & types_repository )
		{
			m_types_repositories_section.remove_repository( types_repository );
		}
		//! \}

		//! ������ � �������������.
		//! \{
		inline ACE_RW_Thread_Mutex &
		allocate_mutex()
		{
			return m_bind_section
				.m_bind_mutex_pool
					.allocate_mutex();
		}

		inline void
		deallocate_mutex( ACE_RW_Thread_Mutex & m )
		{
			m_bind_section
				.m_bind_mutex_pool
					.deallocate_mutex( m );
		}

		//! �������� ���������� ��� �������� �����-������.
		//! ���� ���������� ��� ������ �����-������ �����,
		//! �� ���������� ������� ���������.
		stagepoint_bind_impl_unique_ptr_t
		create_bind(
			//! ������������ �����-������.
			const stagepoint_t & stagepoint,
			//! �����, �������� �������� ����������.
			so_5::rt::agent_t & agent );

		//! �������� ���������� ��� �������� �������� �����.
		//! ���� ���������� ��� ������ �������� ����� �����,
		//! �� ���������� ������� ���������.
		stagepoint_bind_impl_unique_ptr_t
		create_bind(
			//! ������������ �������� ����� � �� ������.
			const endpoint_stage_chain_t & endpoint_stage_chain,
			//! �����, �������� �������� ����������.
			so_5::rt::agent_t & agent );

		//! ��������� ���������� ��� ������ �����-������ �� ������� ��������
		//! ������������.
		void
		remove_bind(
			const stagepoint_t & stagepoint );
		//! \}

		//! ������ � ������� ���������� mbapi.
		//! \{

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
			const infrastructure::available_endpoint_table_t & endpoints,
			//! ������� �����-������ ������.
			const infrastructure::available_stagepoint_table_t & stagepoints );

		//! ��������� ������ ��� ������������� � �������� �������.
		/*!
			������ ����� ����� ������ �������� ����� � �����-������.
		*/
		void
		copy_tables_data(
			//! ������� �������� �����.
			infrastructure::available_endpoint_table_t & endpoints,
			//! ������� �����-������.
			infrastructure::available_stagepoint_table_t & stagepoints ) const;

		//! �������� ������.
		void
		delete_channel(
			//! ������������� ������.
			const channel_uid_wrapper_t & channel_id );

		//! �������� ������ ��������� endpoint-��.
		endpoint_list_t
		query_endpoint_list() const;

		//! \}

		//! �������� ���������.
		//! \{

		//! �������� ��������� �� ������.
		void
		transmit_message(
			comm::transmit_info_unique_ptr_t transmit_info );

		//! �������� �������� ���������.
		/*!
			���������� �� transmit_message(), ��� ��� ��� � � ������
			local_transmit_info, � m_current_stage ��������
			�������� �� �������� ���������, � �����������.
		*/
		void
		transmit_banary_message(
			comm::transmit_info_unique_ptr_t transmit_info );

		//! �������� ��������� �� ������� mbapi-����.
		void
		transmit_message(
			comm::local_transmit_info_unique_ptr_t local_transmit_info );
		//! \}

	private:
		//! ������� transmit_info_t �� local_transmit_info_t.
		comm::transmit_info_unique_ptr_t
		create_transmit_info(
			const comm::local_transmit_info_t & local_transmit_info ) const;

		//! ����� SObjectizer.
		so_5::rt::so_environment_t & m_so_environment;

		//! ���� mbapi.
		mbapi_layer_t & m_mbapi_layer;

		//! ������ ��� ������ ����.
		/*!
			������ ������ ����� �����, ��� ������������ ������
			��� ������ �������. ��� �������������� ������
			� ��������� deadlock-��, ������� ������� ������,
			� ������ ������������� ����������� ����� ���������
			������ ���� ���������:
				\li m_bind_section.m_lock;
				\li m_network_section.m_lock;
				\li m_types_repositories_section.m_lock.
		*/
		//! \{
		//! ������ ��� ������ � �������������.
		bind_section_t m_bind_section;

		//! ������ ��� ������ � ���������� mbapi-����.
		network_section_t m_network_section;

		//! ������ ��� ������ � ������������� �����.
		types_repositories_section_t m_types_repositories_section;
		//! \}
};

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
