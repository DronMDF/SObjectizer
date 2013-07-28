/*
	SObjectizer 5
*/

/*!
	\file
	\brief ���� ������.
*/

#if !defined( _SO_5__RET_CODE_HPP_ )
#define _SO_5__RET_CODE_HPP_

#include <so_5/h/declspec.hpp>

#include <iostream>

namespace so_5
{

//! ��� �������� �������� API-������� � �������.
typedef int ret_code_t;

//
// ����������� ���� ������
//

//! ������ ������� so_environment.
const int rc_environment_error = 1;

//! \name ������ ������� ������.
//! \{

//! ������� � ������������ ��������� ������.
const int rc_agent_unknown_state = 10;

//! ����� ��� �������� � ����� SO.
const int rc_agent_is_already_bind_to_env = 11;

//! ����� ��� �������� � ����������.
const int rc_agent_is_already_bind_to_disp = 12;

//! ��������� � �������� ������ �� ������.
const int rc_named_disp_not_found = 13;

//! ����� �� �������� � ����������.
const int rc_agent_has_no_cooperation = 14;

//! ����� �� ����� ���� �������� � ��������� ����.
const int rc_agent_incompatible_type_conversion = 15;

//! \}

//! ������ ���������� �������.
//! \{

//! ������� ������� ���������.
const int rc_zero_ptr_to_coop = 20;

//! ���������� � ����� ������ ��� ����������������.
const int rc_coop_with_specified_name_is_already_registered = 21;

//! ���������� �������� ������ ������ �� ����� ��� �������� � ����������.
const int rc_coop_has_references_to_null_agents_or_binders = 22;

//! ���������������� ���������� � �������� ������ �� �������.
const int rc_coop_has_not_found_among_registered_coop = 23;

//! ���������� �� ������ ���� ����������������.
const int rc_coop_define_agent_failed = 24;

//! \}


//! \name ������ ����������.
//! \{

//! �� ������� ��������� ���������.
const int rc_disp_start_failed = 30;

//! �� ������� ������� ���������.
const int rc_disp_create_failed = 31;

//! �������� ������ � ��� �������� ��� �� ���������.
const int rc_disp_type_mismatch = 32;

//! \}

//! \name ������ ����������� ������������
//! ������� � ������������� ���������.
//! \{

//! ����������� ����������� ������� �� ��� ���������, mbox �
//! ��������� ��� ������� ��� ��������������� ����������.
const int rc_evt_handler_already_provided = 40;

//! ������������� ����������� �� ��� ���������, mbox �
//! ���������, �������� ��� � ������ ������������������.
const int rc_no_event_handler_provided = 41;

//! ������������� ����������� �� ��� ���������, mbox �
//! ���������, �������� ���� � ������ ���������, �� ��� ����������
//! ������ ������ ��������� � ����������.
const int rc_event_handler_match_error = 42;

//! ����� �� �������� ���������� ���������,
//! ������� ������� ��� �����������.
const int rc_agent_is_not_the_state_owner = 43;

//! ����������� ������������ ������� �� ��� ���������, mbox �
//! ��������� ��� ������� ��� ��������������� ����������.
const int rc_intercept_handler_already_provided = 44;

//! ������������� ������������ �� ��� ���������, mbox �
//! ���������, �������� ��� � ������ ������������������.
const int rc_no_interception_handler_provided = 45;

//! ������������� ������������ �� ��� ���������, mbox �
//! ���������, �������� ���� � ������ ���������, �� ��� ����������
//! ������ ������ ��������� � ����������.
const int rc_interception_handler_match_error = 46;
//! \}

//! \name ������ mbox-��.
//! \{

//! ����������� ������������ mbox-� � ������, ������� ��� ����.
const int rc_mbox_duplicating_name = 80;

//! �� ������� ����� ����������� mbox.
const int rc_mbox_unable_to_find_mbox = 81;
//! \}

//! \name ������ ������ ���������� ��� ������������� ���������.
//! \{

//! �� ������� ������������� ��������� �������.
const int rc_unable_to_schedule_timer_act = 90;
//! \}

//! \name ������ ������ c� �����.
//! \{

//! ���� �� �������� � ����� SObjectizer.
const int rc_layer_not_binded_to_so_env = 100;

//! ������� �������� �������������� ����
//! �� �������� ���������.
const int rc_trying_to_add_nullptr_extra_layer = 101;

//! ������� �������� �������������� ����,
//! ������� ��� ���������� � ������ ����� �� ���������.
const int rc_trying_to_add_extra_layer_that_already_exists_in_default_list = 102;

//! ������� �������� �������������� ����,
//! ������� ��� ���������� � ������ �������������� �����.
const int rc_trying_to_add_extra_layer_that_already_exists_in_extra_list = 103;

//! ������� ���������������� ���� ������ ��������.
const int rc_unable_to_start_extra_layer = 104;

//! ���� ��������� ���� �� ����������.
const int rc_layer_does_not_exist = 105;
//! \}

//! \name ������ ������ �����.
//! \{

//! ������ ��� ( mbox-�, ���������� ).
const int rc_empty_name = 500;

const int rc_unexpected_error = 0xFFFFFF;
//! \}

} /* namespace so_5 */

#endif
