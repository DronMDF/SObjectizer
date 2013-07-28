/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ��� ��������� ����������
	�� ������ ����������� �������.
*/

#if !defined( _SO_5__RT__IMPL__CMP_METHOD_PTR_HPP_ )
#define _SO_5__RT__IMPL__CMP_METHOD_PTR_HPP_

#include <cstring>

#include <so_5/rt/h/agent.hpp>
#include <so_5/rt/h/message.hpp>
#include <so_5/rt/h/event_data.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// cmp_method_ptr
//

//! �������� ��������� �� ����� ������ ������.
/*!
	\return true ���� ��������� �� ������ �����,
	false ���� ��������� �� ������ �� �����.
*/

inline bool
cmp_method_ptr(
	const char * left_ordinal,
	size_t left_size,
	const char * right_ordinal,
	size_t right_size )
{
	return left_size == right_size &&
		0 == memcmp(
			left_ordinal,
			right_ordinal,
			left_size );
}

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
