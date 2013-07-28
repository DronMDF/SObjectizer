/*
	MBAPI 4.
*/

#include <algorithm>

#include <ace/Guard_T.h>

#include <oess_2/io/h/bstring_buf.hpp>

#include <oess_2/stdsn/h/ent_std.hpp>

#include <mbapi_4/impl/h/iosession.hpp>
#include <mbapi_4/impl/h/mbapi_layer_impl.hpp>

namespace mbapi_4
{

namespace impl
{

//
// types_repositories_section_t
//

void
types_repositories_section_t::add_repository(
	const oess_2::stdsn::types_repository_t & types_repository )
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock( m_lock );

	types_repositories_list_t::iterator it =
		std::find(
			m_types_repositories.begin(),
			m_types_repositories.end(),
			&types_repository );

	if( m_types_repositories.end() == it )
	{
		m_types_repositories.push_back( &types_repository );
	}
}

void
types_repositories_section_t::remove_repository(
	const oess_2::stdsn::types_repository_t & types_repository )
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock( m_lock );

	types_repositories_list_t::iterator it =
		std::find(
			m_types_repositories.begin(),
			m_types_repositories.end(),
			&types_repository );

	if( m_types_repositories.end() != it )
	{
		m_types_repositories.erase( it );
	}
}

//
// mbapi_layer_impl_t
//

mbapi_layer_impl_t::mbapi_layer_impl_t(
	so_5::rt::so_environment_t & so_environment,
	mbapi_layer_t & mbapi_layer )
	:
		m_so_environment( so_environment ),
		m_mbapi_layer( mbapi_layer )
{
}

mbapi_layer_impl_t::~mbapi_layer_impl_t()
{
}

stagepoint_bind_impl_unique_ptr_t
mbapi_layer_impl_t::create_bind(
	const stagepoint_t & stagepoint,
	so_5::rt::agent_t & agent )
{
	stagepoint_bind_impl_unique_ptr_t result;

	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	if( m_bind_section.m_bind_table.end() ==
		m_bind_section.m_bind_table.find( stagepoint ) )
	{
		result.reset(
			new stagepoint_bind_impl_t(
				stagepoint,
				agent,
				*this ) );

		m_bind_section.m_bind_table[ stagepoint ] = result.get();

		ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );
		m_network_section.m_netowork_environment.add_local_stagepoint( stagepoint );
	}

	return result;
}

stagepoint_bind_impl_unique_ptr_t
mbapi_layer_impl_t::create_bind(
	const endpoint_stage_chain_t & endpoint_stage_chain,
	so_5::rt::agent_t & agent )
{
	stagepoint_bind_impl_unique_ptr_t result;

	const stagepoint_t stagepoint( endpoint_stage_chain.endpoint() );

	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	if( m_bind_section.m_bind_table.end() ==
		m_bind_section.m_bind_table.find( stagepoint ) )
	{
		result.reset(
			new stagepoint_bind_impl_t(
				stagepoint,
				agent,
				*this ) );

		m_bind_section.m_bind_table[ stagepoint ] = result.get();

		ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );
		m_network_section.m_netowork_environment.add_local_stagepoint( stagepoint );
		m_network_section.m_netowork_environment.add_local_endpoint( endpoint_stage_chain );
	}

	return result;
}

void
mbapi_layer_impl_t::remove_bind(
	const stagepoint_t & stagepoint )
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	if( 0 != m_bind_section.m_bind_table.erase( stagepoint ) )
	{
		ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );

		m_network_section.m_netowork_environment.remove_local_stagepoint( stagepoint );

		if( stagepoint.is_endpoint() )
			m_network_section.m_netowork_environment.remove_local_endpoint(
				endpoint_t( stagepoint.endpoint_name() ) );
	}
}

void
mbapi_layer_impl_t::update_channel(
	const channel_uid_wrapper_t & channel_id,
	const mbapi_node_uid_t & node_uid,
	const so_5::rt::mbox_ref_t & mbox,
	const infrastructure::available_endpoint_table_t & endpoints,
	const infrastructure::available_stagepoint_table_t & stagepoints )
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );

	m_network_section
		.m_netowork_environment
			.update_channel(
				channel_id,
				node_uid,
				mbox,
				endpoints,
				stagepoints );
}

void
mbapi_layer_impl_t::copy_tables_data(
	infrastructure::available_endpoint_table_t & endpoints,
	infrastructure::available_stagepoint_table_t & stagepoints ) const
{
	ACE_Read_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );

	m_network_section
		.m_netowork_environment
			.copy_tables_data( endpoints, stagepoints );
}

void
mbapi_layer_impl_t::delete_channel(
	const channel_uid_wrapper_t & channel_id )
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );

	m_network_section
		.m_netowork_environment
			.delete_channel( channel_id );
}

endpoint_list_t
mbapi_layer_impl_t::query_endpoint_list() const
{
	ACE_Write_Guard< ACE_RW_Thread_Mutex > lock_env( m_network_section.m_lock );

	return m_network_section.m_netowork_environment.query_endpoint_list();
}

void
mbapi_layer_impl_t::transmit_message(
	comm::transmit_info_unique_ptr_t transmit_info )
{
	ACE_Read_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	// ����� ��������� �������� �� ������,
	// �� � m_current_stage ����������� �������.
	bind_section_t::bind_table_t::iterator
		it = m_bind_section
			.m_bind_table.find( transmit_info->m_current_stage );

	// ���� ������ ������ ������������� �� ������ ����.
	while( m_bind_section.m_bind_table.end() != it )
	{
		stagepoint_bind_impl_t & bind = *( it->second );

		{
			oess_2::io::ibstring_t input_stream(
				transmit_info->m_payload );

			// �������� ������ ������.
			isession_t isession(
				input_stream,
				m_types_repositories_section.m_lock,
				m_types_repositories_section.m_types_repositories );

			if( bind.try_to_accept_message(
				isession.ient(),
				*transmit_info ) )
			{
				// ��������� �������, �� ������ � ��� ������
				// ������ ������������ ���������� �����������.
				return;
			}
		}

		// ���� �����-������ �������� �������� ������ ����������,
		// �� �������� ��������� �� ������� ���������.
		if( transmit_info->m_current_stage.is_endpoint() )
			return;

		// ��������� ���� ���������� ����� �� �������.
		{
			ACE_Read_Guard< ACE_RW_Thread_Mutex >
				lock_env( m_network_section.m_lock );

			// ���������� ��������� stagepoint
			if( !m_network_section
				.m_netowork_environment
					.shift_to_next_stage(
						transmit_info->m_from,
						transmit_info->m_to,
						transmit_info->m_current_stage ) )
			{
				// ��������� ������ �� ����������,
				// ������� �������� ��������� ������ ���� ��������.
				return;
			}

		}

		it = m_bind_section
			.m_bind_table.find( transmit_info->m_current_stage );

	}

	// ������ ����������, �� ��� ��������� �� ������ ����.
	// ��������� mbox ������,  ������� ������ � ������ �����.
	so_5::rt::mbox_ref_t channel_mbox;
	channel_uid_wrapper_t channel_id;
	bool channel_info_defined = false;

	// ���������� ��������� �� ���������� � ��������� ����.
	{
		ACE_Read_Guard< ACE_RW_Thread_Mutex >
			lock_env( m_network_section.m_lock );

			channel_info_defined = m_network_section
				.m_netowork_environment
					.define_channel_info(
						transmit_info->m_current_stage,
						channel_mbox,
						channel_id );
	}

	if( channel_info_defined )
	{
		transmit_info->m_channel_id = channel_id;
		channel_mbox->deliver_message( transmit_info );
	}
}

void
mbapi_layer_impl_t::transmit_banary_message(
	comm::transmit_info_unique_ptr_t transmit_info )
{
	ACE_Read_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	// ����� ��������� �������� ,
	// �� � m_current_stage ����������� ����� � ������� ���� ���������,
	// � ��������� �������� ����� ������� ���� ����� � ������� ������.
	// �.�. ��� � ��� local_transmit.
	{
		ACE_Read_Guard< ACE_RW_Thread_Mutex >
			lock_env( m_network_section.m_lock );

		// ���������� ��������� stagepoint
		if( !m_network_section
			.m_netowork_environment
				.shift_to_next_stage(
					transmit_info->m_from,
					transmit_info->m_to,
					transmit_info->m_current_stage ) )
		{
			// ���� ���������� �� �������,
			// �� ��������� ��������.
			return;
		}
	}

	bind_section_t::bind_table_t::iterator it =
		m_bind_section.m_bind_table.find(
			transmit_info->m_current_stage );

	// ���� ������ ������ ������������� �� ������ ����.
	while( m_bind_section.m_bind_table.end() != it )
	{
		stagepoint_bind_impl_t & bind = *( it->second );

		{
			oess_2::io::ibstring_t input_stream(
				transmit_info->m_payload );

			// �������� ������ ������.
			isession_t isession(
				input_stream,
				m_types_repositories_section.m_lock,
				m_types_repositories_section.m_types_repositories );

			if( bind.try_to_accept_message(
				isession.ient(),
				*transmit_info ) )
			{
				// ��������� �������, �� ������ � ��� ������
				// ������ ������������ ���������� �����������.
				return;
			}
		}

		// ���� �����-������ �������� �������� ������ ����������,
		// �� �������� ��������� �� ������� ���������.
		if( transmit_info->m_current_stage.is_endpoint() )
			return;

		// ��������� ���� ���������� ����� �� �������.
		{
			ACE_Read_Guard< ACE_RW_Thread_Mutex >
				lock_env( m_network_section.m_lock );

			// ���������� ��������� stagepoint
			if( !m_network_section
				.m_netowork_environment
					.shift_to_next_stage(
						transmit_info->m_from,
						transmit_info->m_to,
						transmit_info->m_current_stage ) )
			{
				// ��������� ������ �� ����������,
				// ������� �������� ��������� ������ ���� ��������.
				return;
			}

		}

		it = m_bind_section
			.m_bind_table.find( transmit_info->m_current_stage );

	}

	// ������ ����������, �� ��� ��������� �� ������ ����.
	// ��������� mbox ������,  ������� ������ � ������ �����.
	so_5::rt::mbox_ref_t channel_mbox;
	channel_uid_wrapper_t channel_id;
	bool channel_info_defined = false;

	// ���������� ��������� �� ���������� � ��������� ����.
	{
		ACE_Read_Guard< ACE_RW_Thread_Mutex >
			lock_env( m_network_section.m_lock );

			channel_info_defined = m_network_section
				.m_netowork_environment
					.define_channel_info(
						transmit_info->m_current_stage,
						channel_mbox,
						channel_id );
	}

	if( channel_info_defined )
	{
		transmit_info->m_channel_id = channel_id;
		channel_mbox->deliver_message( transmit_info );
	}
}

void
mbapi_layer_impl_t::transmit_message(
	comm::local_transmit_info_unique_ptr_t local_transmit_info )
{
	ACE_Read_Guard< ACE_RW_Thread_Mutex > lock( m_bind_section.m_lock );

	// ����� ��������� ������������ ��������,
	// �� � m_current_stage ����������� ����� � ������� ���� ���������,
	// � ��������� �������� ����� ������� ���� ����� � ������� ������.
	{
		ACE_Read_Guard< ACE_RW_Thread_Mutex >
			lock_env( m_network_section.m_lock );

		// ���������� ��������� stagepoint
		if( !m_network_section
			.m_netowork_environment
				.shift_to_next_stage(
					local_transmit_info->m_from,
					local_transmit_info->m_to,
					local_transmit_info->m_current_stage ) )
		{
			// ���� ���������� �� �������,
			// �� ��������� ��������.
			return;
		}
	}

	bind_section_t::bind_table_t::iterator it =
		m_bind_section
			.m_bind_table.find( local_transmit_info->m_current_stage );

	while( m_bind_section.m_bind_table.end() != it )
	{
		stagepoint_bind_impl_t & bind = *( it->second );

		if( bind.try_to_accept_message(
			*local_transmit_info ) )
		{
			// ��������� �������, �� ������ � ��� ������
			// ������ ������������ ���������� �����������.
			return;
		}

		// ���� �����-������ �������� �������� ������ ����������,
		// �� �������� ��������� �� ������� ���������.
		if( local_transmit_info->m_current_stage.is_endpoint() )
			return;

		// ��������� ���� ���������� ����� �� �������.
		{
			ACE_Read_Guard< ACE_RW_Thread_Mutex >
				lock_env( m_network_section.m_lock );

			// ���������� ��������� stagepoint
			if( !m_network_section
				.m_netowork_environment
					.shift_to_next_stage(
						local_transmit_info->m_from,
						local_transmit_info->m_to,
						local_transmit_info->m_current_stage ) )
			{
				// ��������� ������ �� ����������,
				// ������� �������� ��������� ������ ���� ��������.
				return;
			}

		}

		it = m_bind_section
			.m_bind_table.find( local_transmit_info->m_current_stage );
	}

	// ������ ����������, �� ��� ��������� �� ������ ����.
	// ��������� mbox ������,  ������� ������ � ������ �����.
	so_5::rt::mbox_ref_t channel_mbox;
	channel_uid_wrapper_t channel_id;
	bool channel_info_defined = false;

	// ���������� ��������� �� ���������� � ��������� ����.
	{
		ACE_Read_Guard< ACE_RW_Thread_Mutex >
			lock_env( m_network_section.m_lock );

			channel_info_defined = m_network_section
				.m_netowork_environment
					.define_channel_info(
						local_transmit_info->m_current_stage,
						channel_mbox,
						channel_id );
	}

	if( channel_info_defined )
	{
		comm::transmit_info_unique_ptr_t transmit_info =
			create_transmit_info( *local_transmit_info );

		transmit_info->m_channel_id = channel_id;
		channel_mbox->deliver_message( transmit_info );
	}
}

comm::transmit_info_unique_ptr_t
mbapi_layer_impl_t::create_transmit_info(
	const comm::local_transmit_info_t & local_transmit_info ) const
{
	comm::transmit_info_unique_ptr_t res(
		new comm::transmit_info_t(
			local_transmit_info.m_from,
			local_transmit_info.m_to,
			local_transmit_info.m_current_stage,
			local_transmit_info.m_oess_id ) );

	oess_2::io::obstring_t output_stream( res->m_payload );

	osession_t osession( output_stream );

	osession.oent() << *local_transmit_info.m_message;

	return res;
}

} /* namespace impl */

} /* namespace mbapi_4 */
