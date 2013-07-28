/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���������� �������� �����-������ �� ������ ����.
*/

#if !defined( _MBAPI_4__IMPL__STAGEPOINT_BIND_IMPL_HPP_ )
#define _MBAPI_4__IMPL__STAGEPOINT_BIND_IMPL_HPP_

#include <vector>
#include <map>

#include <ace/RW_Thread_Mutex.h>

#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/mbox.hpp>
#include <so_5/rt/h/mbox_ref.hpp>

#include <mbapi_4/defs/h/endpoint.hpp>

#include <mbapi_4/comm/h/transmit_info.hpp>

#include <mbapi_4/h/message.hpp>
#include <mbapi_4/h/stagepoint_bind.hpp>

namespace mbapi_4
{

namespace impl
{

// ���������� ���� ������ � mbapi_4.
class mbapi_layer_impl_t;

//! ��� ��� ������ ���������.
typedef std::vector< const so_5::rt::state_t * > state_list_t;

//! ��� ������ ��������� ��� state_list_t.
typedef std::shared_ptr< state_list_t >
	state_list_ptr_t;

//! ��� ������ ��������� ��� message_integrator_t.
typedef std::shared_ptr< message_integrator_t >
	message_integrator_ptr_t;

//! ���������� � �������� ������ �� �������� ��� ���������.
struct subscription_info_t
{
	subscription_info_t()
		:
			m_states(  new state_list_t )
	{}

	inline bool
	has_state(
		const so_5::rt::state_t & state ) const
	{
		return m_states->end() !=
			std::find( m_states->begin(), m_states->end(), &state );
	}

	void
	add_state( const so_5::rt::state_t & state )
	{
		m_states->push_back( &state );
	}

	bool
	remove_state( const so_5::rt::state_t & state )
	{
		state_list_t::iterator it =
			std::find( m_states->begin(), m_states->end(), &state );

		if( m_states->end() != it )
		{
			m_states->erase( it );
			return true;
		}

		return false;
	}

	bool
	empty() const
	{
		return m_states->empty();
	}

	//! ������ ���������, � ������� ����� �������� �� �������� ��� ���������.
	state_list_ptr_t m_states;

	//! ���������� ���������.
	message_integrator_ptr_t m_message_integrator;
};

//! ��� ��� ������� ����� ��������� �
//! �������������� ���������� �� �������� ������ �� ������ ��� ���������.
struct subscribed_types_table_t
{
	//! ���� �� �������� �� �������������� ���������.
	inline bool
	has_subscribed_types() const
	{
		return !m_subscribed_types_map.empty();
	}

	//! ���� �� �������� �� �������� ���������.
	inline bool
	binary_message_subscribed() const
	{
		return !m_binary_message_subscription_info.empty();
	}

	typedef std::map< oess_id_wrapper_t, subscription_info_t >
		subscribed_types_map_t;
	subscribed_types_map_t m_subscribed_types_map;

	subscription_info_t m_binary_message_subscription_info;
};

//
// stagepoint_bind_impl_t
//

//! �������� ������.
/*!
	���������� �������� ��� �������������
	mbapi_4::stagepoint_bind_t � mbapi_4::endpoint_bind_t.
*/
class stagepoint_bind_impl_t
{
	friend class mbapi_layer_impl_t;

	stagepoint_bind_impl_t( const stagepoint_bind_impl_t & );
	void
	operator = ( const stagepoint_bind_impl_t & );

	//! ��������� �������� ����� ���������� ������ ���� mbapi.
	stagepoint_bind_impl_t(
		//! ������������ �����.
		const stagepoint_t & stagepoint,
		//! �����, �������� �������� ��������.
		so_5::rt::agent_t & agent,
		//! ���� mbapi, ������� ������ ������ ��������.
		impl::mbapi_layer_impl_t & mbapi_layer );

	public:
		virtual ~stagepoint_bind_impl_t();

		//! �������� ������������ �������� �����.
		inline const stagepoint_t &
		stagepoint() const
		{
			return m_stagepoint;
		}

		//! ��������� �������� �� ���������.
		so_5::ret_code_t
		subscribe_event_impl(
			//! ��� ���������.
			const oess_id_wrapper_t & oess_id,
			//! ���������� �������� ������.
			event_subscriber_t & event_subscriber,
			//! ���������� ���������.
			std::unique_ptr< message_integrator_t > message_integrator,
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy );

		so_5::ret_code_t
		unsubscribe_event_impl(
			//! ��� ���������.
			const oess_id_wrapper_t & oess_id,
			//! ���������� �������� ������.
			event_subscriber_t & event_subscriber,
			//! ���������� ���������.
			std::unique_ptr< message_integrator_t > message_integrator,
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy );

		//! ��������� ��������.
		so_5::ret_code_t
		bin_msg_subscribe_event_impl(
			//! ���������� �������� ������.
			event_subscriber_t & event_subscriber,
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy );

		//! ��������� ������ ��������.
		so_5::ret_code_t
		bin_msg_unsubscribe_event_impl(
			//! ���������� �������� ������.
			event_subscriber_t & event_subscriber,
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy );

		//! ��������� �������� ���������.
		/*!
			������������ ������ ��� ������������ endpoint-��.
		*/
		void
		send_impl(
			const endpoint_t & to,
			const oess_id_wrapper_t & oess_id,
			std::unique_ptr< oess_2::stdsn::serializable_t > msg );

		//! ��������� �������� ��������� ���������.
		/*!
			������������ ������ ��� ������������ endpoint-��.
		*/
		void
		send_binary_message_impl(
			const endpoint_t & to,
			const oess_id_wrapper_t & oess_id,
			const std::string & binary_message );

		//! ���������� ������� ���������.
		/*!
			���� ����� ��������� �� �������� ��� ���������,
			�� ��� ���������� ������������ � ���������� ��������.

			���� �� �� ��� ��������� ��� ������� ��������,
			�� ��� ������������. � ���� ������ ��� ����
			���������� ����� �� �������.

			\return ���� ��������� �������, �� ������������ true,
			� ��������� ������ false.
		*/
		bool
		try_to_accept_message(
			oess_2::stdsn::ient_t & reader,
			const comm::transmit_info_t & transmit_info ) const;

		//! ���������� ������� ���������.
		/*!
			���� ����� ��������� �� �������� ��� ���������,
			�� �������� ���������� ����� � �������������
			�������� ���������.

			���� �� �� ��� ��������� ��� ������� ��������,
			�� ��� ������������. � ���� ������ ��� ����
			���������� ����� �� �������.

			\return ���� ��������� �������, �� ������������ true,
			� ��������� ������ false.
		*/
		bool
		try_to_accept_message(
			comm::local_transmit_info_t & local_transmit_info ) const;

	private:
		so_5::ret_code_t
		try_subscribe_event(
			subscription_info_t & subscription_info,
			event_subscriber_t & event_subscriber,
			so_5::throwing_strategy_t throwing_strategy );

		//! ������������ ������.
		const stagepoint_t m_stagepoint;

		//! �����, �������� ������ ��������.
		so_5::rt::agent_t & m_agent;

		//! ���� mbapi, ������� ������ ������ ��������.
		mbapi_layer_impl_t & m_mbapi_layer;

		//! ����� ��� �������� ������� � ��������.
		ACE_RW_Thread_Mutex & m_lock;

		//! ������� ����������� �����.
		subscribed_types_table_t m_subscribed_types_table;

		//! Mbox, ��� ����������� ��������.
		mutable so_5::rt::mbox_ref_t m_mbox;
};

//! ��� unique_ptr ��� stagepoint_bind_impl_t;
typedef std::unique_ptr< stagepoint_bind_impl_t >
	stagepoint_bind_impl_unique_ptr_t;

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
