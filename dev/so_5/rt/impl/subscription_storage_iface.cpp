/*
 * SObjectizer-5
 */

/*!
 * \since v.5.5.3
 * \file
 * \brief An interface of subscription storage.
 */

#include <so_5/rt/impl/h/subscription_storage_iface.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

subscription_storage_t::subscription_storage_t( agent_t * owner )
	:	m_owner( owner )
	{}

subscription_storage_t::~subscription_storage_t()
	{}

agent_t *
subscription_storage_t::owner() const
	{
		return m_owner;
	}

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

