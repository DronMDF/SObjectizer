/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����� ��� ������ ������ ����������� � ������
	� ������, ������� ������������ ���������� ���������� �������.
*/


#if !defined( _SO_5__RT__EVENT_HANDLER_CALLER_HPP_ )
#define _SO_5__RT__EVENT_HANDLER_CALLER_HPP_

#include <so_5/h/declspec.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>
#include <so_5/rt/h/state.hpp>
#include <so_5/rt/h/event_data.hpp>
#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/message_ref.hpp>

namespace so_5
{

namespace rt
{

class agent_t;
class message_t;


class event_handler_caller_t;

//
// event_handler_caller_t
//

//! ������� ����� ��� ������ �����������.
/*!
	����� ���� ��������� ���������� �������� � ������������
	������������ �������.
*/
class SO_5_TYPE event_handler_caller_t
	:
		private atomic_refcounted_t
{
		friend class event_handler_caller_ref_t;

		event_handler_caller_t(
			const event_handler_caller_t & );

		void
		operator = (
			const event_handler_caller_t & );

	public:
		event_handler_caller_t();
		virtual ~event_handler_caller_t();

		//! ������� � ������ ����� ��������� �������.
		/*!
			\return ���� ������� ���� ���������, ��
			������ true, � ��������� ������ ������ false.

			������� �� ����������� ������ � ��� ������,
			���� ����� ��������� � ��������� �������� ��
			���� � ������� �� ������������ �� ���������.
		*/
		virtual bool
		call(
			//! ���������.
			message_ref_t & message_ref ) const = 0;

		virtual type_wrapper_t
		type_wrapper() const = 0;

		//! �������� ��� ���������
		//! (��������� �� ����� ��������� �������).
		virtual char *
		ordinal() const = 0;

		//! ������ �������� ��� ��������.
		virtual size_t
		ordinal_size() const = 0;

		//! ��������� �� �������.
		virtual const state_t *
		target_state() const;
};

//
// real_event_handler_caller_t
//

//! ��������� ����� ��� ������ ����������� � ����������� ������ �
//! ���������� ����� ���������.
template< class MESSAGE, class AGENT >
class real_event_handler_caller_t
	:
		public event_handler_caller_t
{
	public:
		//! ��������� �� ����� ���������� ������� ������.
		typedef void (AGENT::*FN_PTR_T)(
			const event_data_t< MESSAGE > & );

		real_event_handler_caller_t(
			FN_PTR_T pfn,
			AGENT & agent,
			const state_t * target_state )
			:
				m_pfn( pfn ),
				m_agent( agent ),
				m_target_state( target_state )
		{}

		virtual ~real_event_handler_caller_t()
		{}

		virtual type_wrapper_t
		type_wrapper() const
		{
			return quick_typeid_t< MESSAGE >::m_type_wrapper;
		}

		//! �������� ��� ���������.
		//! ��������� ��������� �� ������� ������ � �������
		//! ��������� ��������� �� ����� ������.
		virtual char *
		ordinal() const
		{
			return (char *) &m_pfn;
		}

		//! ������ �������� ��� ��������.
		virtual size_t
		ordinal_size() const
		{
			return sizeof( FN_PTR_T );
		}

		virtual const state_t *
		target_state() const
		{
			return m_target_state;
		}

		//! ������� � ������ ����� ��������� �������.
		/*!
			\return ���� ������� ���� ���������, ��
			������ true, � ��������� ������ ������ false.
		*/
		virtual bool
		call(
			//! ���������.
			message_ref_t & message_ref ) const
		{
			const bool execute = &m_agent.so_current_state() == m_target_state;

			// ���� ����� ��������� � ��� ���������, � �������
			// �������� �� �������, �� �������� ����������.
			if( execute )
			{
				const event_data_t< MESSAGE > event_data(
					reinterpret_cast< const MESSAGE * >( message_ref.get() ) );

				(m_agent.*m_pfn)( event_data );
			}

			return execute;
		};

	private:
		//! ��������� � ������� ������ �������������� ���������.
		const state_t * const m_target_state;

		//! ��������� �� ����� ���������� ������� ������.
		FN_PTR_T m_pfn;

		//! ������ �� ����� � �������� ���� �������� ����������.
		AGENT & m_agent;
};

//
// not_null_data_real_event_handler_caller_t
//

//! ��������� ����� ��� ������ ����������� � ����������� ������ �
//! ���������� ����� ���������.
template< class MESSAGE, class AGENT >
class not_null_data_real_event_handler_caller_t
	:
		public event_handler_caller_t
{
	public:
		//! ��������� �� ����� ���������� ������� ������.
		typedef void (AGENT::*FN_PTR_T)(
			const not_null_event_data_t< MESSAGE > & );

		not_null_data_real_event_handler_caller_t(
			FN_PTR_T pfn,
			AGENT & agent,
			const state_t * target_state )
			:
				m_pfn( pfn ),
				m_agent( agent ),
				m_target_state( target_state )
		{}

		virtual ~not_null_data_real_event_handler_caller_t()
		{}


		virtual type_wrapper_t
		type_wrapper() const
		{
			return quick_typeid_t< MESSAGE >::m_type_wrapper;
		}

		//! �������� ��� ���������.
		//! ��������� ��������� �� ������� ������, � �������
		//! ��������� ��������� �� ����� ������.
		virtual char *
		ordinal() const
		{
			return (char *) &m_pfn;
		}

		//! ������ �������� ��� ��������.
		virtual size_t
		ordinal_size() const
		{
			return sizeof( FN_PTR_T );
		}

		virtual const state_t *
		target_state() const
		{
			return m_target_state;
		}

		//! ������� � ������ ����� ��������� �������.
		/*!
			\return ���� ������� ���� ���������, ��
			������ true, � ��������� ������ ������ false.
		*/
		virtual bool
		call(
			//! ���������.
			message_ref_t & message_ref ) const
		{
			bool execute = &m_agent.so_current_state() == m_target_state;

			// ���� ����� ��������� � ��� ���������, � �������
			// �������� �� �������, �� �������� ����������.
			if( execute )
			{
				if( message_ref.get() )
				{
					const not_null_event_data_t< MESSAGE > not_null_event_data(
						reinterpret_cast< const MESSAGE & >( *message_ref ) );

					(m_agent.*m_pfn)( not_null_event_data );
				}
			}

			return execute;
		};

	private:
		//! ��������� � ������� ������ �������������� ���������.
		const state_t * const m_target_state;

		//! ��������� �� ����� ���������� ������� ������.
		FN_PTR_T m_pfn;

		//! ������ �� �����, � �������� ���� �������� ����������.
		AGENT & m_agent;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
