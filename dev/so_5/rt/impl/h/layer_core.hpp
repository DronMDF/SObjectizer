/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ��� ����������� ������ �� ������.
*/

#if !defined( _SO_5__RT__IMPL__LAYER_CORE_HPP_ )
#define _SO_5__RT__IMPL__LAYER_CORE_HPP_

#include <vector>

#include <ace/RW_Thread_Mutex.h>

#include <so_5/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/so_layer.hpp>

namespace so_5
{

namespace rt
{

class so_environment_t;

namespace impl
{

//
// typed_layer_ref_t
//

/*! ��������������� ����� ��� �������� so_layer_ref_t
	������ � ���������� ����� ����.
*/
struct typed_layer_ref_t
{
	typed_layer_ref_t();
	typed_layer_ref_t( const so_layer_map_t::value_type & v );
	typed_layer_ref_t(
		const type_wrapper_t & type,
		const so_layer_ref_t & layer );

	bool
	operator < ( const typed_layer_ref_t & tl ) const;

	//! ������� �� ��������� ��� ����.
	type_wrapper_t m_true_type;

	//! ����.
	so_layer_ref_t m_layer;
};

//! ��������� ��� ������ typed_layer_ref_t.
typedef std::vector< typed_layer_ref_t >
	so_layer_list_t;

//
// layer_core_t
//

//! ����� ��� ����������� ������ �� ������.
/*!
	���������� 2 �������� ���������� ����.

	������ ������������� ����������� ����� �� ��������� (����������).
	����� ���� ������ ���� �������� � �����������.
	���� �� ��������� ����� ���� ��������� \see so_layer_t.
	�� ������ Sobjectizer-� � ����� �� ��������� ����������
	����� so_layer_t::start(), � ������� ����� ������ ��������� ��������
	��� ����������� ���� � ������. � ��� ���������� ������
	Sobjectizer-� � ����� ����� ���������� �����
	so_layer_t::shutdown(), ��� ������������� ��������� ������ ����
	� so_layer_t::wait() ��� ���� ����� ���������, ��� ����
	�������� ������.

	������ �������� ��������� ��������� ��������������
	���� � ���� ������ Sobjectizer. ������ so_layer_t::start(),
	� ����� ����� ���������� ��� �� ����� ������ Sobjectizer.

	��������������� �������������� ����� ������ ��������� ��
	��������������� �������� �����.

	\note ���������� ���� ������� � ����� ������ so_layer_t::start()
	���������� �� ��, ��� Sobjectizer ��� �� �������
	����� �������� � ������� � ���������� ���������� ����������.
*/
class layer_core_t
{
	public:
		layer_core_t(
			//! ����.
			const so_layer_map_t & so_layers,
			//! ����� SObjectizer � ������� �����������
			//! ������������� ����.
			so_environment_t * env );
		~layer_core_t();

		//! �������� ����.
		so_layer_t *
		query_layer(
			const type_wrapper_t & type ) const;

		//! ��������� ������ �����.
		ret_code_t
		start();

		//! ���������� ������ �������������� ����� �����.
		void
		shutdown_extra_layers();

		//! ����� ���������� ���� �����.
		/*!
			����� ���������� ������ ����� �� ���������,
			��������� ��� �������������� ����.
		*/
		void
		wait_extra_layers();

		//! ���������� ������ �����.
		void
		shutdown_default_layers();

		//! ����� ���������� ���� �����.
		/*!
			����� ���������� ������ ����� �� ���������,
			��������� ��� �������������� ����.
		*/
		void
		wait_default_layers();

		//! �������� �������������� ����.
		/*!
			���� ������ ���� ��� �����
			����� �� ��������� � ����� �������������� �����,
			�� �� ����������� � ������ �������������� �����.

			����� ����������� ���� ������������� � so_environment
			� � ���� ���������� ����� �������������
			so_layer_t::start().
		*/
		ret_code_t
		add_extra_layer(
			const type_wrapper_t & type,
			const so_layer_ref_t & layer,
			throwing_strategy_t throwing_strategy );

	private:
		//! ����� SObjectizer, � ������� �����������
		//! ������������� ����.
		so_environment_t * m_env;

		//! ������ ����� �� ���������.
		/*!
			���������������� � ������������ � ������ �� ��������.
		*/
		so_layer_list_t m_default_layers;

		//! ����� ��� �������������
		mutable ACE_RW_Thread_Mutex m_extra_layers_lock;

		//! ������ �������������� �����.
		/*!
			���� ����� ����������� � �����������
			� ���� ������ Sobjectizer.
		*/
		so_layer_list_t m_extra_layers;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
