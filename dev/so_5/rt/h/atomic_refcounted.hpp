/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����� ������� � ��������� ������.
*/

#if !defined( _SO_5__RT__ATOMIC_REFCOUNTED_HPP_ )
#define _SO_5__RT__ATOMIC_REFCOUNTED_HPP_

#include <so_5/h/declspec.hpp>
#include <so_5/h/types.hpp>

namespace so_5
{

namespace rt
{

//! ������� ����� � ���������� �������� ������ ����.
/*!
	������ ��� ����������� ����� ������ �� �����������.
*/
class SO_5_TYPE atomic_refcounted_t
{
		//! ��������� ����������� �����������.
		atomic_refcounted_t(
			const atomic_refcounted_t & );

		//! ��������� �������� ������������.
		atomic_refcounted_t &
		operator = (
			const atomic_refcounted_t & );

	public:
		//! ����������� �� ���������.
		//! ������������� ���������� ������ � 0.
		atomic_refcounted_t();

		virtual ~atomic_refcounted_t();

		//! ��������� ���������� ������.
		inline void
		inc_ref_count()
		{
			++m_ref_counter;
		}

		//! ��������� ���������� ������.
		/*!
			���������� ������ �� ������ ����� ����������.
		*/
		inline unsigned long
		dec_ref_count()
		{
			return --m_ref_counter;
		}

	private:
		//! ������� ������ �� ������.
		atomic_counter_t m_ref_counter;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
