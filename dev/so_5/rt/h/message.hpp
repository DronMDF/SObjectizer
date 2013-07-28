/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����� ��� ���������.
*/

#if !defined( _SO_5__RT__MESSAGE_HPP_ )
#define _SO_5__RT__MESSAGE_HPP_

#include <so_5/h/declspec.hpp>
#include <so_5/rt/h/atomic_refcounted.hpp>

namespace so_5
{

namespace rt
{

//
// message_t
//

//! ������� ����� ��� ���������.
/*!
	��� ����������, ���������� ����� mbox-� ������
	���� ������������ ������� ������.
*/
class SO_5_TYPE message_t
	:
		private atomic_refcounted_t
{
		friend class message_ref_t;

	public:
		message_t();
		message_t( const message_t & );
		void
		operator = ( const message_t & );

		virtual ~message_t();

		//! ��������� ������ � ���������.
		/*!
			\return true ���� ��������� ������ ��������.
			false ���� ��������� �� �������� ��������.

			\note ���������� �� ��������� ������ ����������
			true.
		*/
		virtual bool
		check() const;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
