/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� ����� ��� �������� ������� �� ���������.
*/

#if !defined( _SO_5__RT__SUBSCRIPTION_BIND_HPP_ )
#define _SO_5__RT__SUBSCRIPTION_BIND_HPP_

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>
#include <so_5/h/types.hpp>
#include <so_5/h/throwing_strategy.hpp>
#include <so_5/h/exception.hpp>

#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/state.hpp>
#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/event_data.hpp>
#include <so_5/rt/h/event_handler_caller.hpp>
#include <so_5/rt/h/event_handler_caller_ref.hpp>

namespace so_5
{

namespace rt
{

//
// agent_owns_state()
//

//! ��������� �������� �� ����� ���������� ���������.
SO_5_EXPORT_FUNC_SPEC( ret_code_t )
agent_owns_state(
	agent_t & agent,
	const state_t * state,
	//! ���� - ������� �� ���������� � ������ ������.
	throwing_strategy_t throwing_strategy );

//! ��������� �������� �� ����� ���������� � ������� ����.
template< class AGENT >
ret_code_t
agent_convertable_to(
	//! �����, ������������ �������� � ���� AGENT �����������.
	agent_t * agent,
	AGENT * & casted_agent,
	//! ���� - ������� �� ���������� � ������ ������.
	throwing_strategy_t throwing_strategy )
{
	ret_code_t res = 0;
	casted_agent = dynamic_cast< AGENT * >( agent );

	// ���� ����� �� ���������� � ��������� ����,
	// �� ��� ����� �������.
	if( nullptr == casted_agent )
	{
		// ����� �� �������� �������� �����.
		res = rc_agent_incompatible_type_conversion;

		// ���� ���� ������� ����������, �� ������� ���.
		if( THROW_ON_ERROR == throwing_strategy )
		{
			std::string error_msg = "Unable convert agent to type ";
			const std::type_info & ti = typeid( AGENT );
			error_msg += ti.name();
			throw exception_t( error_msg, res );
		}
	}

	return res;
}

//
// subscription_bind_t
//

/*!
	����� ��� �������� �������� ������ �� ��������� �� mbox.
*/
class SO_5_TYPE subscription_bind_t
{
	public:
		subscription_bind_t(
			agent_t & agent,
			const mbox_ref_t & mbox_ref );

		~subscription_bind_t();

		//! ���������� ��������� � �������
		//! ������ ��������� �������.
		subscription_bind_t &
		in(
			//! ��������� ������ � ������� �������� ��������� ���������.
			const state_t & state );

		//! ����������� �� ���������.
		template< class MESSAGE, class AGENT >
		ret_code_t
		event(
			//! ����� ��������� ���������.
			void (AGENT::*pfn)( const event_data_t< MESSAGE > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR )
		{
			// ��������� ����� ����� ������� ���������� ���������.
			ret_code_t res = agent_owns_state(
				m_agent,
				m_state,
				throwing_strategy );

			if( res )
				return res;

			AGENT * casted_agent = nullptr;
			// ��������� ����� ����� ��� ������� ��������� ����.
			res = agent_convertable_to< AGENT >(
				&m_agent,
				casted_agent,
				throwing_strategy );

			if( res )
				return res;

			event_handler_caller_ref_t event_handler_caller_ref(
				new real_event_handler_caller_t< MESSAGE, AGENT >(
					pfn,
					*casted_agent,
					m_state ) );

			return create_event_subscription(
				quick_typeid_t< MESSAGE >::m_type_wrapper,
				m_mbox_ref,
				event_handler_caller_ref,
				throwing_strategy );
		}

		//! ����������� �� ���������.
		template< class MESSAGE, class AGENT >
		ret_code_t
		event(
			//! ����� ��������� ���������.
			void (AGENT::*pfn)( const not_null_event_data_t< MESSAGE > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR )
		{
			// ��������� ����� ����� ������� ���������� ���������.
			ret_code_t res = agent_owns_state(
				m_agent,
				m_state,
				throwing_strategy );

			if( res )
				return res;

			AGENT * casted_agent = 0;
			// ��������� ����� ����� ��� ������� ��������� ����.
			res = agent_convertable_to< AGENT >(
				&m_agent,
				casted_agent,
				throwing_strategy );

			if( res )
				return res;

			event_handler_caller_ref_t event_handler_caller_ref(
				new not_null_data_real_event_handler_caller_t< MESSAGE, AGENT >(
					pfn,
					*casted_agent,
					m_state ) );

			return create_event_subscription(
				quick_typeid_t< MESSAGE >::m_type_wrapper,
				m_mbox_ref,
				event_handler_caller_ref,
				throwing_strategy );
		}

	private:
		//! ���������� ����� �������� ����������.
		ret_code_t
		create_event_subscription(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ������ �� mbox.
			mbox_ref_t & mbox_ref,
			//! ���������� �����������.
			const event_handler_caller_ref_t & ehc,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );

		//! �����, ������� �������������.
		agent_t & m_agent;
		//! ������ �� mbox � �������� �������� ��������.
		mbox_ref_t m_mbox_ref;
		//! ��������� � ������� ������� �������� ���������.
		const state_t * m_state;
};

//
// subscription_unbind_t
//

/*!
	����� ��� ������� �������� ������ �� ��������� �� mbox.
*/
class SO_5_TYPE subscription_unbind_t
{
	public:
		subscription_unbind_t(
			agent_t & agent,
			const mbox_ref_t & mbox_ref );

		~subscription_unbind_t();

		//! ���������� ��������� � �������
		//! ������ ��������� �������.
		subscription_unbind_t &
		in(
			//! ��������� ������ � ������� ���� �������� ��������� ���������.
			const state_t & state );

		template< class MESSAGE, class AGENT >
		ret_code_t
		event(
			//! ���������� �������.
			void (AGENT::*pfn)( const event_data_t< MESSAGE > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR )
		{
			// ��������� ����� ����� ������� ���������� ���������.
			ret_code_t res = agent_owns_state(
				m_agent,
				m_state,
				throwing_strategy );

			if( res )
				return res;

			AGENT * casted_agent = nullptr;
			// ��������� ����� ����� ��� ������� ��������� ����.
			res = agent_convertable_to< AGENT >(
				&m_agent,
				casted_agent,
				throwing_strategy );

			if( res )
				return res;

			type_wrapper_t type_wrapper( typeid( MESSAGE ) );

			event_handler_caller_ref_t event_handler_caller_ref(
				new real_event_handler_caller_t< MESSAGE, AGENT >(
					pfn,
					*casted_agent,
					m_state ) );

			return destroy_event_subscription(
				quick_typeid_t< MESSAGE >::m_type_wrapper,
				m_mbox_ref,
				event_handler_caller_ref,
				throwing_strategy );
		}

		template< class MESSAGE, class AGENT >
		ret_code_t
		event(
			//! ���������� �������.
			void (AGENT::*pfn)( const not_null_event_data_t< MESSAGE > & ),
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR )
		{
			// ��������� ����� ����� ������� ���������� ���������.
			ret_code_t res = agent_owns_state(
				m_agent,
				m_state,
				throwing_strategy );

			if( res )
				return res;

			AGENT * casted_agent = 0;
			// ��������� ����� ����� ��� ������� ��������� ����.
			res = agent_convertable_to< AGENT >(
				&m_agent,
				casted_agent,
				throwing_strategy );

			if( res )
				return res;

			type_wrapper_t type_wrapper( typeid( MESSAGE ) );

			event_handler_caller_ref_t event_handler_caller_ref(
				new not_null_data_real_event_handler_caller_t< MESSAGE, AGENT >(
					pfn,
					*casted_agent,
					m_state ) );

			return destroy_event_subscription(
				quick_typeid_t< MESSAGE >::m_type_wrapper,
				m_mbox_ref,
				event_handler_caller_ref,
				throwing_strategy );
		}

	private:
		//! ���������� ����� �������� ���������.
		ret_code_t
		destroy_event_subscription(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! mbox.
			mbox_ref_t & mbox_ref,
			//! ���������� �����������.
			const event_handler_caller_ref_t & ehc,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );

		//! �����, ������� �������������.
		agent_t & m_agent;
		//! ������ �� mbox � �������� �������� ��������.
		mbox_ref_t m_mbox_ref;
		//! ��������� � ������� ������� �������� ���������.
		const state_t * m_state;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
