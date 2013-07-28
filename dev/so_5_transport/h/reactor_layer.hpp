/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ���� ��� ������ � ����������.
*/

#if !defined( _SO_5_TRANSPORT__REACTOR_LAYER_HPP_ )
#define _SO_5_TRANSPORT__REACTOR_LAYER_HPP_

#include <ace/Guard_T.h>
#include <ace/Synch.h>
#include <ace/Synch_Traits.h>
#include <ace/Thread_Mutex.h>
#include <ace/Hash_Map_Manager_T.h>

#include <so_5/rt/h/so_layer.hpp>
#include <so_5/rt/h/nonempty_name.hpp>

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/ret_code.hpp>

#include <so_5_transport/ace/h/reactors.hpp>
#include <so_5_transport/ace/h/ace_hash_adaptation.hpp>

namespace so_5_transport
{

//
// reactor_instance_hash_map_t
//

//! ��� Hash_Map-� ��� �������� reactor_instance.
typedef ACE_Hash_Map_Manager_Ex<
		std::string,
		so_5_transport::ace::reactor_instance_ref_t,
		std_string_djb_hash_t,
		ACE_Equal_To< std::string >,
		ACE_Null_Mutex >
	reactor_instance_hash_map_t;

//
// reactor_layer_params_t
//

//! ����� ��� ���������� ���������� ���� ���������
class SO_5_TRANSPORT_TYPE reactor_layer_params_t
{
	public:
		//! �������� ����������� �������.
		/*!
			���� ������� � ����� ������ ��� ��� ��������,
			�� �� ����� ������� ������� ���������.

			���� ������� reactor = 0, �� ���������� ������������.
		*/
		reactor_layer_params_t &
		add_named_reactor(
			//! ��� ��������.
			const so_5::rt::nonempty_name_t & name,
			//! �������.
			so_5_transport::ace::reactor_instance_unique_ptr_t reactor );

		const so_5_transport::ace::reactor_instance_map_t &
		query_named_reactor_map() const;

	private:
		so_5_transport::ace::reactor_instance_map_t
			m_named_reactor_map;
};

//
// reactor_layer_t
//

//! ����� ��� ����������� ������ � ������������.
class SO_5_TRANSPORT_TYPE reactor_layer_t
	:
		public so_5::rt::so_layer_t
{
	public:
		reactor_layer_t();
		reactor_layer_t(
			const reactor_layer_params_t & params );
		virtual ~reactor_layer_t();

		//! �������� ������� �� ���������.
		ACE_Reactor *
		query_default_reactor() const;

		//! �������� ����������� �������.
		/*!
			\return ���� ������� � ����� ������ ����,
			�� �� � ��������, � ��������� ������ �������� 0.
		*/
		ACE_Reactor *
		query_named_reactor(
			const std::string & reactor_name ) const;

	protected:
		//! \name ���������� �������������� �������.
		//! \{
		//! ��������� ������ ���������.
		so_5::ret_code_t
		start();

		//! ���������� ������ ���������.
		void
		shutdown();

		//! ����� ���������� ���� ���������.
		void
		wait();
		//! \}

	private:
		//! ������� ������� �� ���������.
		void
		create_default_reactor();

		//! ��������� ����������� ��������.
		void
		create_named_reactors(
			const so_5_transport::ace::reactor_instance_map_t &
				named_reactors );

		//! ������� �� ���������.
		so_5_transport::ace::reactor_instance_unique_ptr_t m_default_reactor;

		//! ����� ���� ������������������ ���������.
		std::unique_ptr< reactor_instance_hash_map_t > m_reactors_map;
};

} /* namespace so_5_transport */

#endif
