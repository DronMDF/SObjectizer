/*
	SObjectizer 5.
*/

/*!
 * \since v.5.3.0
 * \file 
 * \brief Detection of compiler version and absence of various features.
 */

#if !defined( SO_5__COMPILER_FEATURES_HPP )
#define SO_5__COMPILER_FEATURES_HPP

#if defined( _MSC_VER ) && ( _MSC_VER <= 1700 )
	// Visual C++ 2012 and earlier doesn't support variadic templates.
	#define SO_5_NO_VARIADIC_TEMPLATES

	#if _MSC_VER == 1700
		// Please see details at:
		// http://connect.microsoft.com/VisualStudio/feedback/details/761829/c-11-unexpected-behavior-for-std-future-wait-for-and-std-packaged-task
		//
		// Because of it is dangerous to use wait_for() with
		// synchronous service calls.
		#define SO_5_STD_FUTURE_WAIT_FOR_ALWAYS_DEFFERED
	#endif
#endif 

#endif

