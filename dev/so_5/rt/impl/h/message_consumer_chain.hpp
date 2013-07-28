/*
	SObjectizer 5.
*/
/*!
	\file
	\brief ����� ������� ������������ ���������.
*/
#if !defined( _SO_5__RT__IMPL__MESSAGE_CONSUMER_CHAIN_HPP_ )
#define _SO_5__RT__IMPL__MESSAGE_CONSUMER_CHAIN_HPP_

#include <memory>
#include <map>

#include <so_5/rt/impl/h/message_consumer_link.hpp>
#include <so_5/rt/h/type_wrapper.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

//
// message_consumer_chain_t
//

//! ����� ������� �������� ������� ������������ ���������.
class message_consumer_chain_t
{
	public:
		message_consumer_chain_t();
		virtual ~message_consumer_chain_t();

		//! �������� ����������� ��������� � ����� �������.
		/*!
			����� ���� ����� ������������ ����������� ���������,
			������� ����� �� ����� �������� � ����������� �������
			� ������ �������� � �����.
		*/
		void
		push(
			//! ��������� ����������� ���������.
			message_consumer_link_ref_t
				message_consumer_link );

		void
		pop_front();

		void
		pop_back();

		//! �������� ������ ������� ������������ ���������.
		inline message_consumer_link_ref_t &
		query_head()
		{
			return m_front_link;
		}

		inline bool
		is_empty() const
		{
			return 0 == m_front_link.get() && 0 == m_back_link.get();
		}

	private:
		//! ������ ������� �������.
		message_consumer_link_ref_t m_front_link;
		//! ��������� ������� �������.
		message_consumer_link_ref_t m_back_link;
};

//! ����� ��������� � ��������� ������ �� message_consumer_chain_t.
typedef std::shared_ptr< message_consumer_chain_t >
	message_consumer_chain_ref_t;

typedef std::map<
		type_wrapper_t,
		message_consumer_chain_ref_t >
	msg_type_to_consumer_chain_map_t;

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
