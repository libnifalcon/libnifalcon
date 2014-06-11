/***
 * @file FalconGripFourButton.h
 * @brief Hardware access definitions for the four-button grip that is packaged with the falcon
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONGRIPFOURBUTTON_H
#define FALCONGRIPFOURBUTTON_H

#include "falcon/core/FalconGrip.h"

namespace libnifalcon
{
/**
 * @class FalconGripFourButton
 * @ingroup GripClasses
 *
 * This class represents the default 4 button sphere grip that comes with the falcon.
 */	
	class FalconGripFourButton : public FalconGrip
	{
	public:
		/**
		 * Enumeration for the default 4 button grip that comes with the falcon.
		 * This grip has plus and minus buttonsr, one button in the center and 
		 * one pointing forward. For convenience also symbolic names are provided.
		 */
		enum
		{
			BUTTON_1 = 0x1,
			BUTTON_2 = 0x2,
			BUTTON_3 = 0x4,
			BUTTON_4 = 0x8,
		/* symbolic names */
			PLUS_BUTTON    = 0x1,
			FORWARD_BUTTON = 0x2,
			CENTER_BUTTON  = 0x4,
			MINUS_BUTTON   = 0x8
		} FourButtonGripButtons;
		
		/**
		 * Constructor
		 *
		 */
		FalconGripFourButton() : FalconGrip(4,0)
		{
		}

		/**
		 * Destructor
		 *
		 */
		~FalconGripFourButton()
		{
		}

		/**
		 * Parses data that the firmware retrieved, stores button status internally
		 *
		 * @param size Size of data buffer being passed in
		 * @param data Raw buffer of grip data, taken from data received from falcon
		 *
		 * @return True as long as size is 1, since we're still not sure how to parse whether or not we have the correct grip type
		 */
		bool runGripLoop(int size, const uint8_t* data)
		{
			//Assuming this to always be 1, since the data can easily be packed into a nibble
			if(size != 1)
			{
				return false;
			}
			m_digitalInputs = data[0] & ((1 << m_numDigitalInputs)-1);
			return true;
		}
	};
}


#endif
