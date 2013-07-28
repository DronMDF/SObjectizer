/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ���������� ���������� ��������� ������.

	���������� ���������� ��������� ������ ������ ���� ������
	���������� � ��� ������������� ���������� ���������
	����� ��������� �������� ����� �����������.

	�.�. �������� ����������� ����� ������������ ������ � ����� � ���,
	�� ������ ������� �� ��������� ���� �����:
	�� ������� ���������� ���������� ��� ����������� �����������,
	� �� ������� ��� ����������� ����������� ������ �� ������.
	��� ���� ��������� ������������ ����� ���������� agent_state_listener_t,
	������� �������� ��������� ����� ��������� �������������� �� ����������.
*/

#if !defined( _SO_5__RT__IMPL__STATE_LISTENER_CONTROLLER_HPP_ )
#define _SO_5__RT__IMPL__STATE_LISTENER_CONTROLLER_HPP_

#include <vector>

#include <so_5/rt/h/agent_state_listener.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// state_listener_controller
//

//! ���������� ���������� ��������� ������.
class state_listener_controller_t
{
	public:
		state_listener_controller_t()
		{}

		~state_listener_controller_t()
		{}

		//! ��������� ����� ���������.
		void
		changed(
			//! �����, ��� ��������� ����������.
			agent_t & agent,
			//! �������� ��������� ������.
			const state_t & state );

		//! �������� ���������,
		//! ����� ����� �������� �� ��������������.
		void
		so_add_nondestroyable_listener(
			agent_state_listener_t & state_listener );

		//! �������� ����������,
		//! �������� ������� ����������.
		void
		so_add_destroyable_listener(
			agent_state_listener_unique_ptr_t state_listener );

	private:
		//! �������� ���������.
		void
		add_listener(
			const agent_state_listener_ref_t & agent_state_listener_ref );

		//! ��� ������ ����������.
		typedef std::vector<
				agent_state_listener_ref_t >
			agent_state_listener_array_t;

		//! ��������� ��������� ������.
		agent_state_listener_array_t m_listeners;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
