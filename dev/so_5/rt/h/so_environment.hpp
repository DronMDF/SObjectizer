/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ���������� so_5.
*/

#if !defined( _SO_5__RT__SO_ENVIRONMENT_HPP_ )
#define _SO_5__RT__SO_ENVIRONMENT_HPP_

#include <so_5/h/declspec.hpp>
#include <so_5/h/exception.hpp>

#include <so_5/rt/h/nonempty_name.hpp>
#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/agent_coop.hpp>
#include <so_5/rt/h/disp.hpp>
#include <so_5/rt/h/disp_binder.hpp>
#include <so_5/rt/h/so_layer.hpp>
#include <so_5/rt/h/coop_listener.hpp>
#include <so_5/rt/h/event_exception_logger.hpp>
#include <so_5/rt/h/event_exception_handler.hpp>

#include <so_5/timer_thread/h/timer_thread.hpp>
#include <so_5/timer_thread/h/timer_id.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

class so_environment_impl_t;

} /* namespace impl */

class type_wrapper_t;
class message_ref_t;

//
// so_environment_params_t
//

//! ���������������� ��������� ����� so_5.
/*!
	� ������� ������� ������ �������� ��������� SObjectizer Environment.
	����� ��������� ������� ������� ������� ����������� ���������
	�������� ������ �� ���������.

	\see http://www.parashift.com/c++-faq/named-parameter-idiom.html
*/
class SO_5_TYPE so_environment_params_t
{
		friend class impl::so_environment_impl_t;

	public:
		so_environment_params_t();
		~so_environment_params_t();

		//! ������� ������ ���� �������� ��� ������������� ������ mbox-��.
		/*!
			��� ������������� �������� � mbox-��� ����������
			�������, ��� ���� mbox ���������� ������ (ACE_RW_Thread_Mutex ).
			�� �.�. mbox-�� ����� ���� �����, � ����� ����� �� ����� ����
			���������� ��������, �� ���� �� �� ������������� �
			������ mbox �������� ����������� ������. ������� �
			SObjectizer Environment ��������� ��� �������� ��� �������������
			�������� mbox-���.

			\see so_environment_t::create_local_mbox().
		*/
		so_environment_params_t &
		mbox_mutex_pool_size(
			unsigned int mutex_pool_size );

		//! ������� ������ ���� �������� ��� ���������� �������.
		/*!
			���������� ������� ������� �������������.
			�� ������������ ������������ ��������� ������
			��� ������ ���������� ������������. �������
			SObjectizer Environment ������� ��� ����� ��������,
			������� � ����������� �����������.
		*/
		so_environment_params_t &
		agent_coop_mutex_pool_size(
			unsigned int mutex_pool_size );

		//! ������� ������ ���� �������� ��� �������������
		//! ��������� �������� ������� �������.
		/*!
			��������� ������� ������� ������� ������� �������������.
			���� ��������� ������� ����� �������� �� ����� �������� ��
			SObjectizer Environment, ������� ���������� ��� � ������������.
			��, ��-�� ���� ��� ������� � ������� ����� ���� �����,
			�� �������� ������ ��� ������ ��������� ������� �� �����������,
			������� SObjectizer Environment ������� ��� �������� ���
			����������� ������ ��������� �������� ������� �������.
		*/
		so_environment_params_t &
		agent_event_queue_mutex_pool_size(
			unsigned int mutex_pool_size );

		//! �������� ����������� ���������.
		/*!
			�� ��������� SObjectizer Environment ����� ������ ���� ���������
			� ����� ������� �����. ������������ ����� ���������
			�������������� -� ����������� ���������� ������� �����.

			\note ���� ����� ��� ���������� ��������� � ����� ������,
			�� �� ����� ������� ����� �����������.
		*/
		so_environment_params_t &
		add_named_dispatcher(
			//! ��� ����������.
			const nonempty_name_t & name,
			//! ���������.
			dispatcher_unique_ptr_t && dispatcher );

		//! �������� ��������� ����.
		/*!
			���� \a timer_thread ��������� �� 0, ��
			����� ������������ ��������� ���� �� ���������.
		*/
		so_environment_params_t &
		timer_thread(
			//! ��������� ����.
			so_5::timer_thread::timer_thread_unique_ptr_t && timer_thread );

		//! ���������� �������������� �����.
		/*!
			���� ���� ������ ���� ��� ��������,
			�� �� ����� ������� ����� �����������.
		*/
		template< class SO_LAYER >
		so_environment_params_t &
		add_layer(
			//! ���� SObjectizer, ������� ������ ���� �������
			//! � SObjectizer Environment.
			std::unique_ptr< SO_LAYER > && layer_ptr )
		{
			if( layer_ptr.get() )
			{
				so_layer_unique_ptr_t ptr( layer_ptr.release() );

				add_layer(
					type_wrapper_t( typeid( SO_LAYER ) ),
					std::move( ptr ) );
			}

			return *this;
		}

		so_environment_params_t &
		coop_listener(
			coop_listener_unique_ptr_t && coop_listener );

		so_environment_params_t &
		event_exception_logger(
			event_exception_logger_unique_ptr_t && logger );

		so_environment_params_t &
		event_exception_handler(
			event_exception_handler_unique_ptr_t && handler );

		unsigned int
		mbox_mutex_pool_size() const;

		unsigned int
		agent_coop_mutex_pool_size() const;

		unsigned int
		agent_event_queue_mutex_pool_size() const;

		const named_dispatcher_map_t &
		named_dispatcher_map() const;

		const so_layer_map_t &
		so_layers_map() const;

	private:
		//! �������� ����� ����.
		/*!
			���� ���� ������ ���� ��� ��������,
			�� �� ����� ������� ����� �����������.
		*/
		void
		add_layer(
			//! ���������� � ���� ����.
			const type_wrapper_t & type,
			//! ����.
			so_layer_unique_ptr_t layer_ptr );

		//! ������ ���� �������� ��� mbox-��.
		unsigned int m_mbox_mutex_pool_size;

		//! ������ ���� �������� ��� ���������� ������� �������.
		unsigned int m_agent_coop_mutex_pool_size;

		//! ������ ���� �������� ��� ��������� �������� ������� �������.
		unsigned int m_agent_event_queue_mutex_pool_size;

		//! ����������� ����������.
		named_dispatcher_map_t m_named_dispatcher_map;

		//! ��������� ����.
		so_5::timer_thread::timer_thread_unique_ptr_t m_timer_thread;

		//! �������������� ���� SObjectizer.
		so_layer_map_t m_so_layers;

		//! ��������� �������� ��� ������������.
		coop_listener_unique_ptr_t m_coop_listener;

		//! ����� ����������.
		event_exception_logger_unique_ptr_t m_event_exception_logger;

		//! ���������� ����������.
		event_exception_handler_unique_ptr_t m_event_exception_handler;
};

//
// so_environment_t
//

//! ����� ���������� SObjectizer.
/*!
	����� ���������� SObjectizer ������������� ����������� ��� �������
	��������������. ��� �������� �����������,
	�� ������� �������� SObjectizer ������.

	��� �������� � ������� SObjectizer Environment ��������������,
	��� ���������� ����������� ������� ����������� ����� ����������
	�� so_environment_t � �������������� �����
	so_environment_t::init().
	������ ����� ������ ����������� ���������� ������ ������ ������
	SObjectizer Environment, ��������, ����������� �������������� ����������
	� �������� ����������������� ���������.
	� ������ ������ init() SObjectizer Environment ��� ������� ���������������
	� �������� ����������� ��������������, ����� �� ������ init() ���� �� ������.

	��� ����������������� ������� SObjectizer Environment ������ �����
	so_environment_t::run(), ������� ��������� ���������� ����
	�� ���������� ����� ������� ������ SObjectizer.
	���� ������ �� ���������, �� ����� so_environment_t::run() ���������� 0.
	���� �� ����� ������ SObjectizer Environment ��������� ������,
	�� ������ �� ��� ��� �������� ����������. ��� ������� �� ���������
	\a throwing_strategy (so_5::throwing_strategy_t).

	��� ��������� ������ SObjectizer Environment ������ �����
	so_environment_t::stop().
	����� so_environment_t::stop() ������ ���������� ���������� ������
	SObjectizer Environment. ���� ����� ���������� ������������� ����
	����������. � ����� ��� ���������� ����� ����������������,
	����� ��������� ������ ���������� ��������� SObjectizer Environment.
	����� ����� ������ SObjectizer ����� ���������.

	�� ����� ������ SObjectizer Environment �������������
	��������� ��� ������ ���������� �������:
		\li ������ ������ � mbox-���;
		\li ������ ������ � ������������;
		\li ������ �������� ���������� � ������������� ���������.

	<b>������ ������ � mbox-���.</b>

	SObjectizer Environment ��������� ���������
	���������� mbox-� � ��������� ������������� �� ���� ���
	��������� �������������, ��� ����� ������� mbox, ������ �������
	����� SObjectizer Environment.

	��� �������� mbox-�� ������ ������ so_environment_t::create_local_mbox().
	��� ��� ���������� ������ ������ mbox_ref_t, ������� �������� ����������
	� ��������� ������ �� ������ ������ mbox_t.
	����������� ���������� mbox-�� ���������� ���� �����, ����� ������ �� ����
	�� ��������.
	����������� mbox-� �� �������� � SObjectizer Environment, ��� ����
	���������� ������ �� mbox. ��� ���� ����� ������ ����������� mbox ��
	����� ������, ������ ����� so_environment_t::destroy_mbox().
	���� ����� ��������� ����������� mbox �� ������ �, ���� �� ���� ������ ����� ��
	���������, �� �� ����� ���������.

	\note ���� ����� ���������� ������������ mbox ��������, ��������,
	� ������-���� ������, � ����� ��������� ����������� mbox
	� ����� �� ������ �� ��� ��� ����� ������ mbox-�. � ���������,
	���������� ����� ���� mbox, �� ����� ������������ ����������� �������.

	<b>������ ������ � ������������.</b>

	���������� ����� ��������� � ������� ������� so_environment_t::create_coop(),
	��� ����������� ���������� ������ ����� so_environment_t::register_coop(),
	� ��� ������������� ���������� ������ �����
	so_environment_t::deregister_coop().

	<b>������ �������� ���������� � ������������� ���������</b>

	�������� ����������� � �������������� ��������� ���������� ��������� ��������.
	�������� ��������� ������� ��������� �� ������ ��������,
	�� � �������� ��������� �������. � ������ ����������� ��������� ���
	�� ����� ����������, ����, �������, ��� ��� �� ���� ����������, � � ������
	�������������� ��������� � �������� ��� ����� ����������.
	��� �������� ��������� ������� ������ �����
	so_environment_t::schedule_timer(), ������� ����� ��� ������,
	������ �������������, ��� ���������� �������� ��������� ���������,
	� ������ ��������������� ������ ��� ��������� ��� ���������
	���������� ���������.
	so_environment_t::schedule_timer() ���������� ���������
	������������� � ������ ���� so_5::timer_thread::timer_id_ref_t,
	������� ������ ������, ����������� ��� ������ ���������� �������.
	� �������� ��������� ������� ����� ���� � ������� ������
	so_5::timer_thread::timer_id_ref_t::release(), ����
	��� ����� ���������� �������������, ���� ��� ������ �� ���������
	������� ����������.

	� ��������� ������� ���� ������������� ��������� ���������� ���������,
	������� �� � ����� ������� �� ������� ������, ��
	so_environment_t::schedule_timer() ������� ���������� ���������
	�������������, � ��������� ������, �������� ����������� ��������� �����
	��� �� ��������. ����� �������� �������� ����������
	�������������� � ����� ������� � SObjectizer Environment ������������
	����� so_environment_t::single_timer(), ������� ��������� ����������
	�������� ���������, ��� ������� �� ���� ������� ��������� �������������.
*/
class SO_5_TYPE so_environment_t
{
		//! �������� ������ �� ����.
		/*!
			������ ��� ������������� ���������� ����������
			� ������������ so_environment_t, ����������� �������
			������� ������� ������ �� so_environment_t.
			��� ��������� �������� �������������� �����������
			�� ������������� \c this.
		*/
		so_environment_t &
		self_ref();

	public:
		explicit so_environment_t(
			//! ��������� ����� SObjectizer.
			const so_environment_params_t & so_environment_params );

		virtual ~so_environment_t();

		//! \name ������ � mbox-���.
		//! \{

		//! ������� ���������� mbox.
		/*!
			������ ��� ������������� ������� �� ������ ����.
		*/
		mbox_ref_t
		create_local_mbox();

		//! ������� ����������� mbox.
		/*!
			���� mbox-� � ����� ������ ���, �� �� ���������.
			���� �� ����� mbox ��� ��� ������, �� ������
			������������ ����� �� ����.
			������ ��� ������������� ������� �� ������ ����.
		*/
		mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & mbox_name );

		//! ������� ���������� mbox,
		//! ������ ��� ������������� ��������� �������������.
		/*!
			���� � ������ ������, ����� ���� ����������,
			����� mbox ����������������� ��� ������������� �������������
			� ��� �� ����� ������� mbox-���. � ����� �������, �������� mbox,
			����� ������� ����������� ����������� ��������� ������
			(ACE_RW_Thread_Mutex).
		*/
		mbox_ref_t
		create_local_mbox(
			//! ������ ��������� �������������.
			std::unique_ptr< ACE_RW_Thread_Mutex > lock_ptr );

		//! ������� ����������� mbox.
		/*!
			���� mbox-� � ����� ������ ���, �� �� ���������.
			���� �� ����� mbox ��� ��� ������, �� ������
			������������ ����� �� ����.
			������ ��� ������������� ��������� �������������.
		*/
		mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & mbox_name,
			//! ������ ��������� �������������
			std::unique_ptr< ACE_RW_Thread_Mutex > lock_ptr );
		//! \}

		//! \name ����������.
		//! \{

		//! �������� ��������� �� ���������.
		dispatcher_t &
		query_default_dispatcher();

		//! �������� ����������� ���������.
		/*!
			\return ���� ��������� � ����� ������ ����,
			�� �������� dispatcher_ref_t, ������� ��������� �� ����,
			� ��������� ������ �������� dispatcher_ref_t �� 0.
		*/
		dispatcher_ref_t
		query_named_dispatcher(
			//! ��� ����������.
			const std::string & disp_name );

		//! ���������� ����� ����������.
		void
		install_exception_logger(
			event_exception_logger_unique_ptr_t && logger );

		//! ���������� ���������� ����������.
		void
		install_exception_handler(
			event_exception_handler_unique_ptr_t && handler );
		//! \}

		//! \name ������ � ������������.
		//! \{

		//! ������� ����������.
		/*!
			��������� ���������� � �������� ������,
			� ������� �������� ������� � ���������� SObjectizer �� ���������.
		*/
		agent_coop_unique_ptr_t
		create_coop(
			//! ��� ����������.
			const nonempty_name_t & name );

		//! ������� ����������.
		/*!
			�� ��������� ������ ���������� �������������
			� ���������� � ������� \a disp_binder.

			������������ ��� ��� �������, ������� ����������� �
			���������� ��� ������ �������� �������� � ����������.
			\code
				so_5::rt::agent_coop_unique_ptr_t coop = so_env.create_coop(
					so_5::rt::nonempty_name_t( "some_coop" ),
					so_5::disp::active_group::create_disp_binder(
						"active_group",
						"some_active_group" ) );

				// ����� ����� �������� � ���������� �������� ����� "active_group"
				// � ����� �������� �� ���� �������� ������ "some_active_group"
				coop->add_agent(
					so_5::rt::agent_ref_t( new a_some_agent_t( env ) ) );
			\endcode
			*/
		agent_coop_unique_ptr_t
		create_coop(
			//! ��� ����������.
			const nonempty_name_t & name,
			//! ��������  ������� � ����������, ������������
			//! �� ��������� ��� ������� ����������.
			disp_binder_unique_ptr_t disp_binder );

		//! ���������������� ����������.
		/*!
			��� ����������� ������ ������� ������������� � ����������,
			����� ����� ������ ���������� ����� � ����� ���������� ��� ������.
			����� SObjectizer Environment ���������: �������� �� ���
			���������� ���������� �, ���� ��� �� ����������,
			�� ��� �������� � ������. ���� ��� ����������, �� ������
			�������������� ����������� �������, � ��� ���������� �����
			agent_t::so_define_agent(), ��� ��������� ������ ����������� ��
			���������, ��� �� ���� ��� ����� �������� � ������������ ������� ����.
			� � ��������� ������� ���������� �������� ������� � �� �����������.
			���� ��� ������ ������� ����������� � ����� �����������,
			�� ���������� ���������� ������� ������������������
			� ����������� � ������� ������������������ ����������.
			� ������ �������� ����������� register_coop ������ 0.
		*/
		ret_code_t
		register_coop(
			//! ����������, ������� ������� ���� ����������������.
			agent_coop_unique_ptr_t agent_coop,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy =
				THROW_ON_ERROR );

		//! ���������������� ����������.
		/*!
			��� ������������� SObjectizer Environment ��������� �������
			���������� � ������� ������������������ ���������� �,
			���� ��� ���������, �� ���������� �� �������������.

			����� ��������, ��� ������������� ���������� �� ������,
			��� �� ������ ���������� ��������� ��������,
			� ���� ������ ������ ������������, ��� ��� ����������������.
			������� ����� �����������, ������ ��������� ��������� ����� ������
			�� ��������� ���������. � ������ ����� ������ ����������
			��������� ������� ������� �� �����, ��� �������� �� ���� ����������.
			����������, � ���� �������, ������������ ������� �� ���� �����
			�������, ���������� �� ���� SObjectizer,
			������� ����� �� ��������� ��������� ���� ������������
			�������������� ����������.

			����� ������������� ������������� ����������, ������ ������������
			�� �����������. ����� ����� ���������� ��������� ��
			������� ������������������ ����������.
		*/
		ret_code_t
		deregister_coop(
			//! ��� ���������������� ����������.
			const nonempty_name_t & name,
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy =
				DO_NOT_THROW_ON_ERROR );

		//! \}

		//! \name ������ � ���������� ���������.
		//! \{

		//! ������������� ��������� �������.
		template< class MESSAGE >
		so_5::timer_thread::timer_id_ref_t
		schedule_timer(
			//! ���������.
			std::unique_ptr< MESSAGE > & msg,
			//! Mbox, �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ������ ��������� ���������.
			unsigned int delay_msec,
			//! ������ �������� ���������. ��� ���������� ���������
			//! \a period_msec ����� 0.
			unsigned int period_msec )
		{
			return schedule_timer(
				type_wrapper_t( typeid( MESSAGE ) ),
				message_ref_t( msg.release() ),
				mbox,
				delay_msec,
				period_msec );
		}

		//! ������������� ��������� �������.
		template< class MESSAGE >
		so_5::timer_thread::timer_id_ref_t
		schedule_timer(
			//! ���������.
			std::unique_ptr< MESSAGE > && msg,
			//! Mbox, �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ������ ��������� ���������.
			unsigned int delay_msec,
			//! ������ �������� ���������. ��� ���������� ���������
			//! \a period_msec ����� 0.
			unsigned int period_msec )
		{
			return schedule_timer(
				type_wrapper_t( typeid( MESSAGE ) ),
				message_ref_t( msg.release() ),
				mbox,
				delay_msec,
				period_msec );
		}

		//! ������������� ��������� ������� � ������ ����������.
		template< class MESSAGE >
		so_5::timer_thread::timer_id_ref_t
		schedule_timer(
			//! Mbox, �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ������ ��������� ���������.
			unsigned int delay_msec,
			//! ������ �������� ���������. ��� ���������� ���������
			//! \a period_msec ����� 0.
			unsigned int period_msec )
		{
			return schedule_timer(
				type_wrapper_t( typeid( MESSAGE ) ),
				message_ref_t(),
				mbox,
				delay_msec,
				period_msec );
		}

		//! ������������� ��������� ��������� �������,
		//! ������� ������ ��������.
		template< class MESSAGE >
		void
		single_timer(
			//! ���������.
			std::unique_ptr< MESSAGE > msg,
			//! Mbox, �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ��������� ���������.
			unsigned int delay_msec )
		{
			single_timer(
				type_wrapper_t( typeid( MESSAGE ) ),
				message_ref_t( msg.release() ),
				mbox,
				delay_msec );
		}

		//! ������������� ��������� ��������� �������,
		//! ������� ������ ��������, � ������ ����������.
		template< class MESSAGE >
		void
		single_timer(
			//! Mbox, �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ��������� ���������.
			unsigned int delay_msec )
		{
			single_timer(
				type_wrapper_t( typeid( MESSAGE ) ),
				message_ref_t(),
				mbox,
				delay_msec );
		}
		//! \}

		//! \name ������ �� ������.
		//! \{

		//! �������� ��������� �� �������� ����.
		template< class SO_LAYER >
		SO_LAYER *
		query_layer(
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR ) const
		{
			// �������� �� ������������� SO_LAYER ���� so_layer_t
			// ������� ���������� �� ����� ����������.
			so_layer_t * layer = static_cast< so_layer_t* >( (SO_LAYER *)0 );

			layer = query_layer(
				type_wrapper_t( typeid( SO_LAYER ) ) );

			if( layer )
				return dynamic_cast< SO_LAYER * >( layer );

			if( THROW_ON_ERROR == throwing_strategy )
				throw exception_t(
					"layer does not exist",
					rc_layer_does_not_exist );

			return nullptr;
		}

		//! �������� �������������� ����.
		template< class SO_LAYER >
		ret_code_t
		add_extra_layer(
			std::unique_ptr< SO_LAYER > layer_ptr,
			throwing_strategy_t throwing_strategy = THROW_ON_ERROR )
		{
			return add_extra_layer(
				type_wrapper_t( typeid( SO_LAYER ) ),
				so_layer_ref_t( layer_ptr.release() ),
				throwing_strategy );
		}
		//! \}

		//! \name ������, ������������� � ���������.
		//! \{
		//! ��������� ����� SO_5.
		ret_code_t
		run(
			//! ���� - ������� �� ���������� � ������ ������.
			throwing_strategy_t throwing_strategy =
				THROW_ON_ERROR );

		//! ���������������� ����� SO.
		//! ����� ���������� �����, ����� ����� ��� ��������.
		/*!
			\attention ��� ���� ����� ��������
			�� ��������� ����� Sobjectizer (����� stop())
			�������� �������� ����������
			����� ������ init() �����������. � ��������� ������
			����������� ���� ����� Sobjectizer, �������� ������
			init(), � �� ����� ���������� � ��������
			���������� ������, � �������������� ��������� ��� �
			������ ���������� ������ Sobjectizer. ��������, ����
			� init() ������� �������������� ����������,
			�� ������ �������� �������� �� ����� ����������� ������� �����,
			� init() ����� �������� ����������� ������ � �������������.
			����� ���� ���� ����-���� ����� ������� so_environment_t::stop(), ��
			����� Sobjectizer ��������� �������� � ��� �� ������,
			��� � �� �����, ��������� ��������� �������.
		*/
		virtual void
		init() = 0;

		//! ��������� ���������� ����� SO_5.
		void
		stop();
		//! \}

		//! �������� ���������� so_environment_impl.
		impl::so_environment_impl_t &
		so_environment_impl();

	private:
		//! ������������� ��������� �������.
		so_5::timer_thread::timer_id_ref_t
		schedule_timer(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ���������.
			const message_ref_t & msg,
			//! mbox �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ������ ��������� ���������.
			unsigned int delay_msec,
			//! ������ �������� ���������, ��� ���������� ���������
			//! \a period_msec == 0.
			unsigned int period_msec = 0 );

		//! ������������� ��������� ��������� �������,
		//! ������� ������ ��������.
		void
		single_timer(
			//! ��� ���������.
			const type_wrapper_t & type_wrapper,
			//! ���������.
			const message_ref_t & msg,
			//! mbox �� ������� ���� �������� ���������.
			const mbox_ref_t & mbox,
			//! �������� ����� ��������� ���������.
			unsigned int delay_msec );

		//! �������� ����.
		so_layer_t *
		query_layer(
			const type_wrapper_t & type ) const;

		//! �������� �������������� ����.
		ret_code_t
		add_extra_layer(
			const type_wrapper_t & type,
			const so_layer_ref_t & layer,
			throwing_strategy_t throwing_strategy );

		//! ������ �������������� ����.
		ret_code_t
		remove_extra_layer(
			//! ��� ����.
			const type_wrapper_t & type,
			throwing_strategy_t throwing_strategy );

		//! ���������� ����� so_5.
		impl::so_environment_impl_t * m_so_environment_impl;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
