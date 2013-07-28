/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ��� ����������� ���������.
*/

#if !defined( _SO_5__RT__STATE_HPP_ )
#define _SO_5__RT__STATE_HPP_

#include <string>
#include <map>
#include <set>

#include <so_5/h/declspec.hpp>

namespace so_5
{

namespace rt
{

class state_t;
class agent_t;

//
// state_base_t
//

//! ������� ����� ��� ���������, ������� ������
//! ����������� � ������ state_t ������������� �������.
class SO_5_TYPE state_base_t
{
	private:
		state_base_t();
		friend class state_t;
	public:
		virtual ~state_base_t();
};

//
// state_t
//

//! ����� ��� ������� ��������� �������.
class SO_5_TYPE state_t
	:
		private state_base_t
{
		state_t( const state_t & );
		void
		operator = ( const state_t & );
	public:
		explicit state_t(
			const agent_t * agent );
		state_t(
			const agent_t * agent,
			const std::string & state_name );
		virtual ~state_t();

		bool
		operator == ( const state_t & state ) const;

		//! �������� ��������� �������� ���������.
		const std::string &
		query_name() const;

		//! �������� �� ����� ���������� ���������?
		bool
		is_target( const agent_t * agent ) const;

	private:
		//! ��������� �� �����, ������� ������� ���� ����������.
		/*! ������ ��� �������� ����, ��� �����
			������ � ��������� ������� ��� �������.
		*/
		const agent_t * const m_target_agent;

		//! �������� ���������.
		const std::string m_state_name;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
