/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����� ��� �������.
*/

#if !defined( _SO_5__RT__AGENT_HPP_ )
#define _SO_5__RT__AGENT_HPP_

#include <memory>
#include <map>

#include <so_5/h/declspec.hpp>
#include <so_5/h/types.hpp>

#include <so_5/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>
#include <so_5/rt/h/disp.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/subscription_bind.hpp>
#include <so_5/rt/h/subscription_key.hpp>
#include <so_5/rt/h/event_caller_block_ref.hpp>
#include <so_5/rt/h/agent_state_listener.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

// ���������� ����������� ��� ������ ������.
class local_event_queue_t;
class message_consumer_link_t;
class so_environment_impl_t;
class state_listener_controller_t;

} /* namespace impl */

class state_t;
class so_environment_t;
class agent_coop_t;

//
// agent_t
//

//! ������� ����� �������.
/*!
	����� ����� � SObjectizer-� ������ ���� �����������
	�� agent_t.

	������� ����� ������ ������������� ��������� ������, ������� ����� ��������
	� ��������� �������� ������:
	\li ������ ��� �������������� � SObjectizer;
	\li ���������������� ���-������, ���������� � �������� ����������� ����������,
	������ � ���������� ������ ������;
	\li ������ �������� �� ��������� � ������ ��������;
	\li ������ ������ � ����������� ������.

	<b>������ ��� �������������� � SObjectizer.</b>

	����� so_5::rt::agent_t::so_environment() ������ ��� ��������� ������� �
	SObjectizer Environment, � ������������� � �� ���� �������
	SObjectizer Environment.
	���������� � ������ ����� ����� ����� �������� ������,
	������ ��� ����� ������������� � SObjectizer Environment
	� ������������ �������� ������ so_5::rt::agent_t.

	<b>���-������.</b>

	� ������� ������ ������ ���������� ���-������ ���
	��������� ��������� ��������, ������� �� ��������� ������ �� ������.

	����� ����������� ������ -- agent_t::so_define_agent(),
	������� ���������� �� ���� ��� ����� ������ �������� � SO,
	�.�. �� ����, ��� �������� ��� ������ �� ������� ����.
	����� ����� ������ �������������� � ��������
	����������� ������. ������ ��� ��������� �������� ������ �� ���������.

	�� ����� ���������� ����� ������ ����� �������� ��� �������: ������ ������
	������ (����� ���������� ������� ���������������� � ��� ������ ���������
	� �����������) � ���������� ������ ������, ����� ����������
	���������������� � ����� �������� ��������� ��������� ������
	�� ��������� ���������. ��� ��������� ������ ������ ������ � ����������
	������ ������ ������ ��������� ���-������:
	agent_t::so_evt_start() � agent_t::so_evt_finish().
	� ������ ���� ������� ��������� SObjectizer,
	� ������������ �������� ������ ���������� ������ ������ ���� �������.

	<b>������ �������� �� ��������� � ������ ��������.</b>

	�������� ������������ ��������� �������� �� �������� ���������, mbox-�
	(���� ���������), ����������� ��������� (������ ������ � ������ ����������)
	� �� ������� ��������� ������.

	������������ ��������� ����� ��������� ����� ����� ������ � ����������
	����������� ���������:
	\code
		void
		evt_handler(
			const so_5::rt::event_data_t< MESSAGE > & msg );
	\endcode
	��� \c evt_handler � ��� ����������� �������, � \c MESSAGE � ��� ���������.

	����� so_5::rt::event_data_t �������� �������� ��� ����������
	�� ������ ���� \c MESSAGE, ������� ������������� ������ � �������
	���������� <tt>std::unique_ptr</tt>. ������ ������ �������
	����� � �� ������������, ����� ����������� ��������� �� �����
	��������� ���������� � ��������� ������� �������������� ����������,
	����� ���� ���������, � ����� ������ ��������� ������������ �����
	������ ������������� ���� � ������ �����.

	�������� ������ �� ��������� �������������� � ������� ������
	so_5::rt::agent_t::so_subscribe().
	� ���������� ��������� ������ ������
	so_5::rt::subscription_bind_t, ������� ���
	����� ������ ���������� � mbox, � �������� ���������� ��������,
	� ����� � �������� ��������� �������� �������
	��������� ������ �� ���������.

	<b>������ ������ � ����������� ������</b>

	��� ����� ��������� ������ ������ �����
	so_5::rt::agent_t::so_change_state().
	������, ��� ����� ���������, ����� ���������� ������ �����,
	����� ������������ ������� ��������� ������ � ���������,
	������� �� �� �������. � ��������� ������� ����� ���������� �������
	����� ��������� ������.
	��������, ��� �������� ������� ����������� ����������.
	��� ���� ������� ������������� "���������"��������� ������.
	������ ����� ���� ��������� ����� ���������� "����������".
	��� ���������� ���������, �������� ������� ������������ ����������
	�����������, ������ �����
	so_5::rt::agent_t::so_add_nondestroyable_listener().
	��� ���������� ���������, �������� ������� ���������� ������,
	������ ����� so_5::rt::sgent_t::so_add_destroyable_listener().
*/
class SO_5_TYPE agent_t
	:
		private atomic_refcounted_t
{
		friend class subscription_bind_t;
		friend class subscription_unbind_t;
		friend class agent_ref_t;
		friend class agent_coop_t;

	public:
		//! ����������� ������.
		/*!
			��� �������� �������-������, �� � ������������
			������� ������ ���� �������� � SObjectizer Environment,
			� ��� �������� �� ����� ���� �������� � ����������.
		*/
		explicit agent_t(
			//! ����� SO, ��� ������� ��������� �����.
			so_environment_t & env );

		virtual ~agent_t();

		//! �������� ��������� �� ����.
		/*!
			��� ����, ����� �������� ������������ ��������������
			�� �����������, ����� � ������ �������������
			��������� ������������� this. ��������, ��� �������������
			��������� ������.
			\code
				class a_sample_t
					:
						public so_5::rt::agent_t
				{
						typedef so_5::rt::agent_t base_type_t;

						// ��������� ������.
						const so_5::rt::state_t m_sample_state;
					public:
						a_sample_t( so_5::rt::so_environment_t & env )
							:
								base_type_t( env ),
								m_sample_state( self_ptr() )
						{
							// ...
						}

					// ...

				};
			\endcode
		*/
		const agent_t *
		self_ptr() const;

		//! ��������� ������� ������.
		/*! �.�. �� �������, ������� �������������
			����� ������� ������, � ��� ������ �����,
			����� ����� �������� � ����������.

			����� ������ ���� ������������� ������������,
			� ��������� SObjectizer, ����� ������� ��������������
			��� ������ ������, ����� �� ������������� � ������ ����������
			� �������� ������ �� ���� ���������� � ������ \� so_evt_start().

			\code
				class a_sample_t
					:
						public so_5::rt::agent_t
				{
					// ...
					virtual void
					so_evt_start();
					// ...
				};

				a_sample_t::so_evt_start()
				{
					std::cout << "first agent action on binded"
						" dispatcher" << std::endl;

					// ��������� ������������ ���������.
					m_mbox->deliver_message( ... );
				}
			\endcode
		*/
		virtual void
		so_evt_start();

		//! ����������� ������� ������.
		/*!
			�.�. �� �������, ������� �������������
			����� ������� ���������, �� ���� �������, ���
			����� ����� ������� � ����������.

			����� ������ ���� ������������� �����������,
			� ��������� SObjectizer, ����� ����� ����� ���������
			�����-�� �������� �� ����� ����, ��� �� ����, ���
			����� ������ ���������� ������.

			\code
				class a_sample_t
					:
						public so_5::rt::agent_t
				{
					// ...
					virtual void
					so_evt_finish();
					// ...
				};

				a_sample_t::so_evt_finish()
				{
					std::cout << "last agent activity";

					if( so_current_state() == m_db_error_happened )
					{
						// ���������� ����������� � ��
						// � ��� �� ������ � ������� �������� �
						// � ������� ��������� ������
						m_db.release();
					}
				}
			\endcode
		*/
		virtual void
		so_evt_finish();

		//! �������� ������ �� ������� ���������.
		inline const state_t &
		so_current_state() const
		{
			return *m_current_state_ptr;
		}

		//! ��� ����������, � ������� ����������� �����.
		/*!
			\return ���� ����� ����������� �����-���� ����������,
			�� ������ ��� ����������. ���� �� ����� �� ��������
			������ �����-���� ����������, �� ������������� ����������.
		*/
		const std::string &
		so_coop_name() const;

		//! �������� ������ ���������,
		//! ����� ����� �������� ����� �� ������������.
		void
		so_add_nondestroyable_listener(
			agent_state_listener_t & state_listener );

		//! �������� ������ ����������,
		//! �������� ������� ���������� ������.
		void
		so_add_destroyable_listener(
			agent_state_listener_unique_ptr_t state_listener );

		//! ��������� � ������� ������� ��� ���������� �������.
		/*!
			����� ������� ������������ ��� ������������ ������� ������.
		*/
		static inline void
		call_push_event(
			agent_t & agent,
			//! ���������� ������������.
			const event_caller_block_ref_t & event_handler_caller,
			//! ��������� ���������, ������� ����� ����������
			//! ����������� �������, ������� ������ ����� ��������� �����.
			const message_ref_t & message )
		{
			agent.push_event( event_handler_caller, message );
		}

		//! ������������ ���������� ���������� �������.
		/*!
			����� ������� ������������ ������������ (�������� ������),
			��� ���������� ������� ������.
		*/
		static inline void
		call_next_event(
			//! �����, � �������� � ������� ���� �������,
			//! � ��������� ������� �������� ���� ���������.
			agent_t & agent )
		{
			agent.exec_next_event();
		}

		//! ��������� ������ � ����������.
		static inline void
		call_bind_to_disp(
			//! �����.
			agent_t & agent,
			//! ���������.
			dispatcher_t & disp )
		{
			agent.bind_to_disp( disp );
		}

	protected:
		//! ������ � �����������.
		//! \{

		//! �������� ������ �� ��������� �� ���������.
		const state_t &
		so_default_state() const;

		//! ������� ���������.
		/*!
			\code
				void
				a_sample_t::evt_smth(
					const so_5::rt::event_data_t< message_one_t > & msg )
				{
					// ���� � ��������� ���-�� �� ���, ��
					// ��������� � �������� "������".
					if( error_in_data( *msg ) )
						so_change_state( m_error_state );
				}
			\endcode
		*/
		ret_code_t
		so_change_state(
			//! ����� ��������� ������.
			const state_t & new_state,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy =
				THROW_ON_ERROR );
		//! \}

		//! �������� � ������� �� ���������.
		//! \{

		//! ������� ������ ��� ���������� ��������.
		/*!
			\code
				void
				a_sample_t::so_define_agent()
				{
					// ����������� �� ���������.
					so_subscribe( m_mbox_target )
						.in( m_state_one )
							.event( &a_sample_t::evt_sample_handler );
				}
			\endcode
		*/
		subscription_bind_t
		so_subscribe(
			//! Mbox �� ��������� �������� �����������.
			const mbox_ref_t & mbox_ref );

		//! ������� ������ ��� ������� ��������.
		/*!
			\code
				void
				a_sample_t::evt_smth(
					const so_5::rt::event_data_t< message_one_t > & msg )
				{
					// ���������� �� ���������.
					so_unsubscribe( m_mbox_target )
						.in( m_state_one )
							.event( &a_sample_t::evt_sample_handler );
				}
			\endcode
		*/
		subscription_unbind_t
		so_unsubscribe(
			//! mbox �� ��������� �������� ����������.
			const mbox_ref_t & mbox_ref );
		//! \}

		//! �������������� ������������� ������.
		//! \{

		//! ����� ����������� ������.
		/*!
			�����, ������� ���������� �� ���� ���
			����� ������ �������� � SO, �.�. �� ����
			��� �������� ��� ������ �� ������� ����.
			\code
				class a_sample_t
					:
						public so_5::rt::agent_t
				{
					// ...
					virtual void
					so_define_agent();

					void
					evt_handler_1(
						const so_5::rt::event_data_t< message1_t > & msg );
					// ...

					void
					evt_handler_N(
						const so_5::rt::event_data_t< messageN_t > & msg );

				};

				void
				a_sample_t::so_define_agent()
				{
					// ������������� �� ���������.
					so_subscribe( m_mbox1 )
						.in( m_state_1 )
							.event( &a_sample_t::evt_handler_1 );
					// ...
					so_subscribe( m_mboxN )
						.in( m_state_N )
							.event( &a_sample_t::evt_handler_N );
				}
			\endcode
		*/
		virtual void
		so_define_agent();

		//! \}

		//! ��� �� ����� ���������?
		/*!
			����� ���� ���������� ��������� �� ����� ���.
			\code
				class a_sample_t
					:
						public so_5::rt::agent_t
				{
					// ...

					public:
						void
						set_target_mbox(
							const so_5::rt::mbox_ref_t & mbox )
						{
							// mbox ��� �������� ������ ������
							// ����� ���� ��� ����� �� ���� ����������.

							if( !so_was_defined() && mbox.get() )
							{
								m_target_mbox = mbox;
							}
						}

					private:
						so_5::rt::mbox_ref_t m_target_mbox;
				};
			\endcode
		*/
		bool
		so_was_defined() const;

	public:
		//! �������� ������ �� ����� SO � �������
		//! ����������� �����.
		/*!
			����� ������ ��� ��������� ������� �
			SObjectizer Environment, � ������������� � �� ���� �������
			SObjectizer Environment. ���, �������� ��������� ������
			������� ���������� ������ �������.
			\code
				void
				a_sample_t::evt_on_smth(
					const so_5::rt::event_data_t< some_message_t > & msg )
				{
					so_5::rt::agent_coop_unique_ptr_t coop =
						so_environment().create_coop(
							so_5::rt::nonempty_name_t( "first_coop" ) );

					// ��������� � ���������� �������.
					coop->add_agent( so_5::rt::agent_ref_t(
						new a_another_t( ... ) ) );
					// ...

					// ������������ ����������.
					so_environment().register_coop( coop );
				}
			\endcode

			���, ��������, ������������ ���������� ������.
			\code
				void
				a_sample_t::evt_last_event(
					const so_5::rt::event_data_t< message_one_t > & msg )
				{
					// ������������ ���������.
					process( msg );

					// ���������������.
					so_environment().stop();
				}
			\endcode
		*/
		so_environment_t &
		so_environment();

	private:
		//! �������� ������ �� �����.
		//! ���������� ����� SO.
		//! ���������� ������ ����� - ����� �������������� ����� ���
		//! ����� � ��� ���-�� ������ �� ������.
		agent_ref_t
		create_ref();

		//! ��������� ������ � ����������� SO Runtime.
		//! \{

		//! ��������� ������ � ����������.
		/*!
			�������������� ���������� ��������� �� ����������,
			� ����� ������� ���� ��� ����������, ������� ����������� �����
			�� �������� ���������� �� �������������.
		*/
		void
		bind_to_coop(
			//! ���������� �������.
			agent_coop_t & coop );

		//! ��������� ������ � ����� so_5.
		/*!
			�������������� ���������� ��������� �� ����� SO
			� �������� ���� ��������� �������.
			���������� �� ������������ ������.
		*/
		void
		bind_to_environment(
			//! ����� so_5.
			impl::so_environment_impl_t & env_impl );

		//! ��������� ������ � ����������.
		/*!
			���������� ���������, ������� ����� ��������
			�������� ����������� ������.
			���� � ��������� ������� ���-�� ����������, ��
			�������� ���������� ����� ��������� ������ �� ����������
			����� ���������� ������������ ������� �� �������
			������ ������ ������ �� ������ ����������.
		*/
		void
		bind_to_disp(
			//! ���������.
			dispatcher_t & disp );

		//! ���������� ����� ����������� ������.
		/*!
			�������� so_define_agent(), ����� ����
			������������� ����, ��� ����� ���������.
		*/
		void
		define_agent();

		//! ����� ������� ������ �� �������.
		/*!
			����� ������� ������� ��� �������� ������.
			\note ���������� ����� SO.
		*/
		void
		undefine_agent();

		//! \}

		//! ����������/�������� �������� ��� ��������� ���������.
		//! \{

		//! ������� �������� ����� ������� � mbox-��
		//! ��� ��������� ���������.
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

		//! ���������� �������� ����� ������� � mbox-��
		//! ��� ��������� ���������.
		ret_code_t
		destroy_event_subscription(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! Mbox.
			mbox_ref_t & mbox_ref,
			//! ���������� �����������.
			const event_handler_caller_ref_t & ehc,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy );

		//! ���������� ��� ��������.
		void
		destroy_all_subscriptions();
		//! \}

		//! ���������� ������� � ������� � �� ����������.
		//! \{

		//! ��������� � ������� ��������� �������.
		void
		push_event(
			//! ���������� ������������
			const event_caller_block_ref_t & event_handler_caller,
			//! ��������� ���������.
			const message_ref_t & message );

		//! ��������� ��������� �������.
		/*!
			������ ���������� ������ �� ��������� �������� ����.
			\note ����������, ��� � ������� ������� ���� ��������.
		*/
		void
		exec_next_event();
		//! \}

		//! ��������� �� ���������.
		const state_t m_default_state;

		//! ������� ��������� ������.
		const state_t * m_current_state_ptr;

		//! ��� �� ����� ���������.
		bool m_was_defined;

		//! ���������� ���������� ������.
		std::unique_ptr< impl::state_listener_controller_t >
			m_state_listener_controller;

		//! ��� ����� �������� � ������������ �������.
		typedef std::map<
				subscription_key_t,
				impl::message_consumer_link_t * >
			consumers_map_t;

		//! ����������� ���������, ������� ��������
		//! ������������� �������.
		consumers_map_t m_event_consumers_map;

		//! ��������� ������� �������.
		std::unique_ptr< impl::local_event_queue_t >
			m_local_event_queue;

		//! ����� so_5 ������� ����������� �����.
		impl::so_environment_impl_t * m_so_environment_impl;

		//! ��������� ������� ���������� ������� ������.
		/*! �� ��������� �������� ��������,
			������� ������ �� ������, � ������ ���������
			������ ���������� ������ � ������� �� �����.
			����� �������� ������ � ��������� ����������
			������� ��� ������, � ��� ������� ��� �������
			���������� ������.

			�������� ��� m_dispatcher ����� �� �����.
		*/
		dispatcher_t * m_dispatcher;

		//! ���������� ������� ����������� �����.
		agent_coop_t * m_agent_coop;

		//! ���� �������� �� ����������, ��� ���������?
		bool m_is_coop_deregistered;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
