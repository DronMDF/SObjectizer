/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������� ����� ��������� ��������� ������.
*/

#if !defined( _SO_5__RT__AGENT_STATE_LISTENER_HPP_ )
#define _SO_5__RT__AGENT_STATE_LISTENER_HPP_

#include <memory>

#include <cpp_util_2/h/nocopy.hpp>

#include <so_5/h/declspec.hpp>
#include <so_5/rt/h/state.hpp>

namespace so_5
{

namespace rt
{

class state_t;

//
// agent_state_listener_t
//

//!	������� ����� "����������" ��������� ������.
/*!
	� ��������� ������� ����� ���������� ������� ����� ���������
	������. ��������, ��� �������� ������� ����������� ����������.
	��� ���� ������� ������������� "���������" ��������� ������.

	������ ����� ���� ��������� ����� ���������� "����������".
	�����, ����� �������� ����� ������ ���������, � ������ so_change_state()
	������� � ������� �� ��� ����� changed().

	\note �����, ��� ���������� ��������� �������� ��������,
	����� ��������� �������� � ������������� ����� ���������
	� ����� �������� ������� ��� ���������.
*/
class SO_5_TYPE agent_state_listener_t
	:
		private cpp_util_2::nocopy_t
{
	public:
		virtual ~agent_state_listener_t();

		//! ���������� ����� �������� ����� ��������� ������.
		virtual void
		changed(
			//! �����, ��� ��������� ����������.
			agent_t & agent,
			//! ������� ��������� ������.
			const state_t & state ) = 0;
};

//! ����� ��������� �� ���������
typedef std::unique_ptr< agent_state_listener_t >
	agent_state_listener_unique_ptr_t;

//! ����� ������ �� agent_state_listener_t.
typedef std::shared_ptr< agent_state_listener_t >
	agent_state_listener_ref_t;

} /* namespace rt */

} /* namespace so_5 */

#endif
