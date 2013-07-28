/*
	MBAPI 4.
*/

/*!
	\file
	\brief ��������� ��������� handshake ����� mbapi-��������.
*/

#if !defined( _MBAPI_4__DEFS__HANDSHAKING_PARAMS_HPP_ )
#define _MBAPI_4__DEFS__HANDSHAKING_PARAMS_HPP_

#include <mbapi_4/defs/h/declspec.hpp>

namespace mbapi_4
{

//
// handshaking_params_t
//

/*!
	\brief ��������� ��������� handshake ����� ���������� mbapi_4 ��������.
*/
class MBAPI_4_DEFS_TYPE handshaking_params_t
{
	public:

		/*!
			��������� ����������.
		*/
		handshaking_params_t(
			//! �������� �� ���������� ������.
			/*!
				�� ��������� ���������� ���������.
			*/
			bool is_compression_enabled = false );

		/*!
			\name ������ ���������/���������� ����������.
		*/
		//! \{
		bool
		is_compression_enabled() const;

		handshaking_params_t &
		enable_compression();

		handshaking_params_t &
		disable_compression();
		//! \}

	private :
		//! ������� �������������/������������� ����������.
		bool m_is_compression_enabled;
};

} /* namespace mbapi_4 */

#endif
