/*
	SObjectizer 5.
*/

#include <cstdlib>

#include <so_5/rt/impl/h/void_dispatcher.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

void_dispatcher_t::void_dispatcher_t()
{
}

void_dispatcher_t::~void_dispatcher_t()
{
}

ret_code_t
void_dispatcher_t::start()
{
	// ���� ����� �� ������ ���������� � �������� ������� ������.
	std::abort();
	return 0;
}

void
void_dispatcher_t::shutdown()
{
	// ���� ����� �� ������ ���������� � �������� ������� ������.
	std::abort();
}

void
void_dispatcher_t::wait()
{
	// ���� ����� �� ������ ���������� � �������� ������� ������.
	std::abort();
}

void
void_dispatcher_t::put_event_execution_request(
	const agent_ref_t &,
	unsigned int )
{
	// ����������.
}

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */
