/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� ���� SObjectizer.
*/

#if !defined( _SO_5__RT__SO_LAYER_HPP_ )
#define _SO_5__RT__SO_LAYER_HPP_

#include <memory>
#include <map>

#include <ace/Refcounted_Auto_Ptr.h>
#include <ace/Null_Mutex.h>

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>

#include <so_5/rt/h/type_wrapper.hpp>

namespace so_5
{

namespace rt
{

class so_environment_t;

namespace impl
{

class layer_core_t;

} /* namespace impl */


//
// so_layer_t
//

//! ��������� ���� SObjectizer.
/*!
	����� ��� ��������� �������������� �����  SObjectizer.

	\note ���� �������� �� ������� ���� � ��������� � ������ public, ��
	��� �������� ����������� ����� ���������� ��� �������
	� ������  protected ��� private, ����� ���������
	����������� ������ ������� �������� ���� � ���������������� ����.
*/
class SO_5_TYPE so_layer_t
{
		friend class impl::layer_core_t;
	public:
		so_layer_t();
		virtual ~so_layer_t();


		//! \name �������� ������ ����.
		//! \{

		//! ��������� ����.
		/*!
			���������� �� ��������� ������ �� ������.
		*/
		virtual ret_code_t
		start();

		//! ������������ ���������� ���������� ����.
		/*!
			���������� �� ��������� ������ �� ������.
		*/
		virtual void
		shutdown();

		//! �������� ���������� ����.
		/*!
			���������� �� ��������� ������ �� ������.
		*/
		virtual void
		wait();
		//! \}

	protected:
		//! �������� ����� SObjectizer.
		/*!
			���� ���� �� �������� � SObjectizer,
			�� ������������� ����������.
		*/
		so_environment_t &
		so_environment();

	private:
		//! ��������� � ����� SObjectizer.
		void
		bind_to_environment( so_environment_t * env );

		//! C���� SObjectizer �� ������� ������� ������ ����.
		so_environment_t * m_so_environment;
};

//! ��������� unique_ptr ��� so_layer_t.
typedef std::unique_ptr< so_layer_t > so_layer_unique_ptr_t;

//! ��������� ������������ ��������� �� so_layer_t.
typedef std::shared_ptr< so_layer_t >
	so_layer_ref_t;

//! ��������� ��� ����� ����� � �����.
typedef std::map< type_wrapper_t, so_layer_ref_t > so_layer_map_t;

} /* namespace rt */

} /* namespace so_5 */

#endif
