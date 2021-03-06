/*
 * SObjectizer-5
 */

/*!
 * \file
 * \since v.5.5.1
 * \brief Implementation of free functions send/send_delayed.
 */

#pragma once

#include <so_5/rt/h/mbox.hpp>
#include <so_5/rt/h/environment.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

	/*
	 * This is helpers for so_5::send implementation.
	 */

	template< class MESSAGE, bool IS_SIGNAL >
	struct instantiator_and_sender_t
		{
			void
			send( const so_5::rt::mbox_t & to )
				{
					std::unique_ptr< MESSAGE > msg( new MESSAGE() );

					to->deliver_message( std::move( msg ) );
				}

			void
			send_delayed(
				so_5::rt::environment_t & env,
				const so_5::rt::mbox_t & to,
				std::chrono::steady_clock::duration pause )
				{
					std::unique_ptr< MESSAGE > msg( new MESSAGE() );

					env.single_timer( std::move( msg ), to, pause );
				}

			timer_id_t
			send_periodic(
				so_5::rt::environment_t & env,
				const so_5::rt::mbox_t & to,
				std::chrono::steady_clock::duration pause,
				std::chrono::steady_clock::duration period )
				{
					std::unique_ptr< MESSAGE > msg( new MESSAGE() );

					return env.schedule_timer(
							std::move( msg ), to, pause, period );
				}
		};

	template< class MESSAGE >
	struct instantiator_and_sender_t< MESSAGE, true >
		{
			void
			send( const so_5::rt::mbox_t & to )
				{
					to->deliver_signal< MESSAGE >();
				}

			void
			send_delayed(
				so_5::rt::environment_t & env,
				const so_5::rt::mbox_t & to,
				std::chrono::steady_clock::duration pause )
				{
					env.single_timer< MESSAGE >( to, pause );
				}

			timer_id_t
			send_periodic(
				so_5::rt::environment_t & env,
				const so_5::rt::mbox_t & to,
				std::chrono::steady_clock::duration pause,
				std::chrono::steady_clock::duration period )
				{
					return env.schedule_timer< MESSAGE >( to, pause, period );
				}
		};

} /* namespace impl */

} /* namespace rt */

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a message.
 */
template< typename MESSAGE, typename... ARGS >
void
send( const so_5::rt::mbox_t & to, ARGS&&... args )
	{
		std::unique_ptr< MESSAGE > msg(
				new MESSAGE( std::forward<ARGS>(args)... ) );

		to->deliver_message( std::move( msg ) );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for sending a signal.
 */
template< typename MESSAGE >
void
send( const so_5::rt::mbox_t & to )
	{
		so_5::rt::impl::instantiator_and_sender_t<
				MESSAGE,
				so_5::rt::is_signal< MESSAGE >::value > helper;

		helper.send( to );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a message to
 * the agent's direct mbox.
 */
template< typename MESSAGE, typename... ARGS >
void
send_to_agent( const so_5::rt::agent_t & receiver, ARGS&&... args )
	{
		send< MESSAGE >( receiver.so_direct_mbox(), std::forward<ARGS>(args)... );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for sending a signal to the agent's direct mbox.
 */
template< typename MESSAGE >
void
send_to_agent( const so_5::rt::agent_t & receiver )
	{
		send< MESSAGE >( receiver.so_direct_mbox() );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a delayed message.
 */
template< typename MESSAGE, typename... ARGS >
void
send_delayed(
	//! An environment to be used for timer.
	so_5::rt::environment_t & env,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		std::unique_ptr< MESSAGE > msg(
				new MESSAGE( std::forward<ARGS>(args)... ) );

		env.single_timer( std::move( msg ), to, pause );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a delayed message.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE, typename... ARGS >
void
send_delayed(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		send_delayed< MESSAGE >( agent.so_environment(), to, pause,
				std::forward< ARGS >(args)... );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a delayed message
 * to the agent's direct mbox.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE, typename... ARGS >
void
send_delayed_to_agent(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		send_delayed< MESSAGE >(
				agent.so_environment(),
				agent.so_direct_mbox(),
				pause,
				std::forward< ARGS >(args)... );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a delayed signal.
 */
template< typename MESSAGE >
void
send_delayed(
	//! An environment to be used for timer.
	so_5::rt::environment_t & env,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause )
	{
		so_5::rt::impl::instantiator_and_sender_t<
				MESSAGE,
				so_5::rt::is_signal< MESSAGE >::value > helper;

		helper.send_delayed( env, to, pause );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a delayed signal.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE >
void
send_delayed(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause )
	{
		send_delayed< MESSAGE >( agent.so_environment(), to, pause );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a delayed signal to
 * the agent's direct mbox.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE >
void
send_delayed_to_agent(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause )
	{
		send_delayed< MESSAGE >(
				agent.so_environment(),
				agent.so_direct_mbox(),
				pause );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a periodic message.
 */
template< typename MESSAGE, typename... ARGS >
timer_id_t
send_periodic(
	//! An environment to be used for timer.
	so_5::rt::environment_t & env,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		std::unique_ptr< MESSAGE > msg(
				new MESSAGE( std::forward<ARGS>(args)... ) );

		return env.schedule_timer( std::move( msg ), to, pause, period );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a periodic message.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE, typename... ARGS >
timer_id_t
send_periodic(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		return send_periodic< MESSAGE >(
				agent.so_environment(),
				to,
				pause,
				period,
				std::forward< ARGS >(args)... );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for creating and delivering a periodic message
 * to the agent's direct mbox.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE, typename... ARGS >
timer_id_t
send_periodic_to_agent(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period,
	//! Message constructor parameters.
	ARGS&&... args )
	{
		return send_periodic< MESSAGE >(
				agent.so_environment(),
				agent.so_direct_mbox(),
				pause,
				period,
				std::forward< ARGS >(args)... );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a periodic signal.
 */
template< typename MESSAGE >
timer_id_t
send_periodic(
	//! An environment to be used for timer.
	so_5::rt::environment_t & env,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period )
	{
		so_5::rt::impl::instantiator_and_sender_t<
				MESSAGE,
				so_5::rt::is_signal< MESSAGE >::value > helper;

		return helper.send_periodic( env, to, pause, period );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a periodic signal.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE >
timer_id_t
send_periodic(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Mbox for the message to be sent to.
	const so_5::rt::mbox_t & to,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period )
	{
		return send_periodic< MESSAGE >(
				agent.so_environment(),
				to,
				pause,
				period );
	}

/*!
 * \since v.5.5.1
 * \brief A utility function for delivering a periodic signal to
 * the agent's direct mbox.
 *
 * Gets the Environment from the agent specified.
 */
template< typename MESSAGE >
timer_id_t
send_periodic_to_agent(
	//! An agent whos environment must be used.
	so_5::rt::agent_t & agent,
	//! Pause for message delaying.
	std::chrono::steady_clock::duration pause,
	//! Period of message repetitions.
	std::chrono::steady_clock::duration period )
	{
		return send_periodic< MESSAGE >(
				agent.so_environment(),
				agent.so_direct_mbox(),
				pause,
				period );
	}

} /* namespace so_5 */

