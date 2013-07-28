/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���������� �����-������ �� ������ ����.
*/

#if !defined( _MBAPI_4__STAGEPOINT_BIND_HPP_ )
#define _MBAPI_4__STAGEPOINT_BIND_HPP_

#include <oess_2/stdsn/h/ent.hpp>

#include <so_5/rt/h/subscription_bind.hpp>
#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/mbox.hpp>
#include <so_5/rt/h/mbox_ref.hpp>

#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/oess_id_wrapper.hpp>

#include <mbapi_4/h/declspec.hpp>

#include <mbapi_4/h/message.hpp>

namespace mbapi_4
{

namespace impl
{

// ���������� ����������� ��������.
class stagepoint_bind_impl_t;

} /* namespace impl */

class mbapi_layer_t;

//! ��������������� �����-��������� ��� ���������� ��������.
class MBAPI_4_TYPE event_subscriber_t
{
		event_subscriber_t(
			const event_subscriber_t & );
		void
		operator = (
			const event_subscriber_t & );

	public:
		event_subscriber_t(
			const so_5::rt::state_t & state );
		virtual ~event_subscriber_t();

		//! ��������� �������� ����������� ������� ������ �� �������� ���������.
		//! ����������, ��������� ������ � ��� ��������� ������ ���������� �������.
		virtual so_5::ret_code_t
		subscribe(
			//! �����.
			so_5::rt::agent_t & agent,
			//! Mbox.
			const so_5::rt::mbox_ref_t & mbox ) = 0;

		//! ��������� ������ �������� ����������� ������� ������ �� �������� ���������.
		//! ����������, ��������� ������ � ��� ��������� ������ ���������� �������.
		virtual so_5::ret_code_t
		unsubscribe(
			//! �����.
			so_5::rt::agent_t & agent,
			//! Mbox.
			const so_5::rt::mbox_ref_t & mbox ) = 0;

		//! ��������� ������ � ������� ���������� ������������ �������.
		inline const so_5::rt::state_t &
		state() const
		{
			return m_state;
		}

	protected:
		const so_5::rt::state_t & m_state;
};

//! ���������� ���������� ��� ���������� ����� ���������.
template< class MBAPI_MESSAGE, class AGENT >
class real_event_subscriber_t
	:
		public event_subscriber_t
{
	public:
		//! ��������� �� ����� ���������� ������� ������.
		typedef void (AGENT::*FN_PTR_T)(
			const so_5::rt::event_data_t< MBAPI_MESSAGE > & );

		real_event_subscriber_t(
			FN_PTR_T pfn,
			const so_5::rt::state_t & state )
			:
				event_subscriber_t( state ),
				m_pfn( pfn )
		{}

		virtual ~real_event_subscriber_t()
		{}

		virtual so_5::ret_code_t
		subscribe(
			//! �����.
			so_5::rt::agent_t & agent,
			//! Mbox.
			const so_5::rt::mbox_ref_t & mbox )
		{
			so_5::rt::subscription_bind_t subscription_bind( agent, mbox );
			return subscription_bind
				.in( m_state )
					.event(
						m_pfn,
						so_5::DO_NOT_THROW_ON_ERROR );
		}

		virtual so_5::ret_code_t
		unsubscribe(
			//! �����.
			so_5::rt::agent_t & agent,
			//! Mbox.
			const so_5::rt::mbox_ref_t & mbox )
		{
			so_5::rt::subscription_unbind_t subscription_bind( agent, mbox );
			return subscription_bind
				.in( m_state )
					.event(
						m_pfn,
						so_5::DO_NOT_THROW_ON_ERROR );
		}
	private:
		FN_PTR_T m_pfn;
};

//! ��������������� �����-��������� ��� ����������� � �������� ���������.
class MBAPI_4_TYPE message_integrator_t
{
	public:
		virtual ~message_integrator_t();

		//! �������� ��������� � ���������
		virtual void
		read_and_send(
			//! ���� mbapi.
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t & current_stage,
			//! �������� �������.
			oess_2::stdsn::ient_t & reader,
			//! Mbox, �� ������� ������� ��������� ���������.
			so_5::rt::mbox_ref_t & mbox ) const = 0;

		//! ������������� ��������� � ��������.
		virtual void
		cast_and_send(
			//! ���� mbapi.
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t & to,
			//! ������� ������.
			const stagepoint_t &  current_stage,
			//! ������ ��� ���������.
			std::unique_ptr< oess_2::stdsn::serializable_t > msg,
			//! Mbox, �� ������� ������� ��������� ���������.
			so_5::rt::mbox_ref_t & mbox ) const = 0;
};

//! ���������� ����������� ��������� ��� ���������� �����.
template < class SERIALIZABLE >
class message_integrator_impl_t
	:
		public message_integrator_t
{
		typedef mbapi_4::message_t< SERIALIZABLE > MBAPI_MESSAGE;

	public:
		virtual ~message_integrator_impl_t()
		{}

		virtual void
		read_and_send(
			//! ���� mbapi.
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t &  from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t &  to,
			//! ������� ������.
			const stagepoint_t & current_stage,
			//! �������� �������.
			oess_2::stdsn::ient_t & reader,
			//! Mbox, �� ������� ������� ��������� ���������.
			so_5::rt::mbox_ref_t & mbox ) const
		{
			std::unique_ptr< MBAPI_MESSAGE >
				mbapi_message(
					new MBAPI_MESSAGE(
						mbapi_layer,
						from,
						to,
						current_stage ) );

			reader >> mbapi_message->msg();

			mbox->deliver_message( mbapi_message );
		}

		virtual void
		cast_and_send(
			//! ���� mbapi.
			mbapi_layer_t & mbapi_layer,
			//! �������� ����� � ������� ���� ���������.
			const endpoint_t & from,
			//! �������� ����� �� ������� ���� ���������.
			const endpoint_t &  to,
			//! ������� ������.
			const stagepoint_t & current_stage,
			//! ������ ���������.
			std::unique_ptr< oess_2::stdsn::serializable_t > msg,
			//! Mbox, �� ������� ������� ��������� ���������.
			so_5::rt::mbox_ref_t & mbox ) const
		{
			std::unique_ptr< SERIALIZABLE >
				casted_message(
					dynamic_cast< SERIALIZABLE * >( msg.get() ) );

			if( 0 != casted_message.get() )
			{
				msg.release();

				std::unique_ptr< MBAPI_MESSAGE > mbapi_message(
					new MBAPI_MESSAGE(
						std::move( casted_message ),
						mbapi_layer,
						from,
						to,
						current_stage ) );

				mbox->deliver_message(
					std::move( mbapi_message ) );
			}
		}
};

//
// stagepoint_bind_t
//

//! �������� � �����-������.
/*!
	������������ ����� ����� ������ �� ������ ���� �����-���� �������� �����.
	� ������� �������� ����� ������������� �� ���������
	���������� ������ �������� ����� ��� ������������ �� ������ �������� �����.

	�������� �������� mbapi_layer_t, ������� ������������ �� ������������.
	��� �������� ���������� ������ ������, � ����������� ��
	���������, � �����������, ����� ������ ������������� ������� ������.

	��� ����������� �������� �����-������
	������������� ��������� ������������ �� ������ ����.

	� ������� �������� �����-������ ����� ������ �������������
	�� ���������, �� �� ���������� ��. ���������� �� ����� ������
	��������� ��������� � ������ ����� �� ������� ������.
*/
class MBAPI_4_TYPE stagepoint_bind_t
{
	friend class mbapi_layer_t;
	friend class endpoint_bind_t;

	stagepoint_bind_t( const stagepoint_bind_t & );
	void
	operator = ( const stagepoint_bind_t & );

	//! ���������� �����������, ������ ��� ��������� ��������
	//! ����� ���������� ������ ���� mbapi.
	stagepoint_bind_t(
		std::unique_ptr< impl::stagepoint_bind_impl_t > impl );

	public:
		virtual ~stagepoint_bind_t();

		//! �������� ������������ �����-������.
		const stagepoint_t &
		stagepoint() const;

		// TODO: ����� �� ������� ��� ����� ��� ��������
		// ���� �� ����������� ��������� ��������� ������,
		// ����� � ���� ������ ������� ��������� �� ���������.
		//! ����������� �� ��������� ���������.
		template< class AGENT, class SERIALIZABLE >
		so_5::ret_code_t
		subscribe_event(
			//! ��������� ������.
			const so_5::rt::state_t & state,
			//! ����� ��������� ���������.
			void (AGENT::*pfn)(
				const so_5::rt::event_data_t< message_t< SERIALIZABLE >  > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy =
				so_5::THROW_ON_ERROR )
		{
			real_event_subscriber_t< message_t< SERIALIZABLE >, AGENT >
				real_event_subscriber(
					pfn,
					state );

			const oess_id_wrapper_t oess_id(
				&SERIALIZABLE::oess_serializer_t::type_name() );

			return subscribe_event_impl(
				oess_id,
				real_event_subscriber,
				std::unique_ptr< message_integrator_t >(
					new message_integrator_impl_t< SERIALIZABLE > ),
				throwing_strategy );
		}

		//! ����������� �� ��������� �������� ���������.
		template< class AGENT >
		so_5::ret_code_t
		subscribe_event(
			//! ��������� ������.
			const so_5::rt::state_t & state,
			//! ����� ��������� ���������.
			void (AGENT::*pfn)(
				const so_5::rt::event_data_t< binary_message_t > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy =
				so_5::THROW_ON_ERROR )
		{
			real_event_subscriber_t< binary_message_t, AGENT >
				real_event_subscriber(
					pfn,
					state );

			return bin_msg_subscribe_event_impl(
				real_event_subscriber,
				throwing_strategy );
		}

		//! �������� �������� �� ��������� ���������.
		template< class AGENT, class SERIALIZABLE >
		so_5::ret_code_t
		unsubscribe_event(
			//! ��������� ������.
			const so_5::rt::state_t & state,
			//! ����� ��������� ���������.
			void (AGENT::*pfn)(
				const so_5::rt::event_data_t< message_t< SERIALIZABLE >  > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy =
				so_5::THROW_ON_ERROR )
		{
			real_event_subscriber_t< message_t< SERIALIZABLE >, AGENT >
				real_event_subscriber(
					pfn,
					state );

			const oess_id_wrapper_t oess_id(
				&SERIALIZABLE::oess_serializer_t::type_name() );

			return unsubscribe_event_impl(
				oess_id,
				real_event_subscriber,
				std::unique_ptr< message_integrator_t >(
					new message_integrator_impl_t< SERIALIZABLE > ),
				throwing_strategy );
		}

		//! �������� �������� �� ��������� �������� ���������.
		template< class AGENT >
		so_5::ret_code_t
		unsubscribe_event(
			//! ��������� ������.
			const so_5::rt::state_t & state,
			//! ����� ��������� ���������.
			void (AGENT::*pfn)(
				const so_5::rt::event_data_t< binary_message_t > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			so_5::throwing_strategy_t throwing_strategy =
				so_5::THROW_ON_ERROR )
		{
			real_event_subscriber_t< binary_message_t, AGENT >
				real_event_subscriber(
					pfn,
					state );

			return bin_msg_unsubscribe_event_impl(
				real_event_subscriber,
				throwing_strategy );
		}

	private:
		//! ��������� ��������.
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

		//! ��������� ������ ��������.
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

	protected:
		//! ���������� ����������.
		std::unique_ptr< impl::stagepoint_bind_impl_t >
			m_impl;
};

//! ��������� unique_ptr ��� stagepoint_bind_t
typedef std::unique_ptr< stagepoint_bind_t > stagepoint_bind_unique_ptr_t;

} /* namespace mbapi_4 */

#endif
