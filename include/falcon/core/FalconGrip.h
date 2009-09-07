/***
 * @file FalconGrip.h
 * @brief Base class for grip definition classes
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef FALCONGRIP_H
#define FALCONGRIP_H

#include <cstdlib>
#include "falcon/core/FalconCore.h"

namespace libnifalcon
{
/**
 * @class FalconGrip
 * @ingroup CoreClasses
 * @ingroup GripClasses
 *
 * Grip classes define how we deal with whatever grip might appear on the falcon. The base class tries to be as generic as possible,
 * allowing for up to 32 digital inputs and 128 analog inputs. 
 */
	
	class FalconGrip : public FalconCore
	{
	public:

		enum {
			FALCON_GRIP_INDEX_OUT_OF_RANGE = 4000 /**< Returned if button index requested is out of range for the current grip */
		};


		/**
		 * Constructor. Defines the grip capabilities.
		 *
		 * @param digital_inputs Number of digital inputs on the grip
		 * @param analog_inputs Number of analog inputs on the grip
		 *
		 */
		FalconGrip(int32_t digital_inputs, int32_t analog_inputs) :
			m_numDigitalInputs(digital_inputs),
			m_numAnalogInputs(analog_inputs),
			m_digitalInputs(0)
		{
		}

		/**
		 * Destructor
		 *
		 *
		 */
		virtual ~FalconGrip()
		{
		}

		/**
		 * Parses input/output for grip related data (button states, etc...)
		 *
		 * @param size Amount of data in the buffer being passed in
		 * @param data Buffer of button data
		 *
		 * @return true if parsing successful, false otherwise
		 */
		virtual bool runGripLoop(int size, const uint8_t* data) = 0;

		/**
		 * Returns the number of digital inputs available on the current grip
		 *
		 *
		 * @return Number of digital inputs available on the grip
		 */
		unsigned int getNumDigitalInputs() const { return m_numDigitalInputs; }

		/**
		 * Returns the number of analog inputs available on the current grip
		 *
		 *
		 * @return Number of analog inputs available on the grip
		 */
		unsigned int getNumAnalogInputs() const { return m_numAnalogInputs; }

		/**
		 * Returns the value of the requested digital input
		 *
		 * @param index Index of the digital input to return
		 *
		 * @return true if digital input requested is high, false otherwise (or if index is out of range)
		 */
		bool getDigitalInput(unsigned int index)
		{
			if(index > m_numDigitalInputs)
			{
				m_errorCode = FALCON_GRIP_INDEX_OUT_OF_RANGE;
				return false;
			}
			return ((m_digitalInputs & (1 << index)) > 0);
		}

		/**
		 * Returns the bitfield used to store all digital inputs
		 *
		 * @return Bitfield of digital inputs
		 */
		unsigned int getDigitalInputs() const { return m_digitalInputs; }

		/**
		 * Returns the value of the requested analog input
		 *
		 * @param index Index of the analog input to return
		 *
		 * @return Value of analog input requested, (0 if index is out of range)
		 */
		int getAnalogInput(int index)
		{
			if(index > m_numAnalogInputs)
			{
				m_errorCode = FALCON_GRIP_INDEX_OUT_OF_RANGE;
				return 0;
			}
			return m_analogInputs[index];
		}
	protected:
		unsigned int m_numDigitalInputs; /**< Number of digital inputs available on the grip */
		unsigned int m_numAnalogInputs; /**< Number of analog inputs available on the grip */
		//I think assuming 32 digital inputs and 128 analog is enough
		unsigned int m_digitalInputs; /**< Bitfield to hold digital input values */
		int m_analogInputs[128]; /**< Array of analog input values */
	};
}

#endif
