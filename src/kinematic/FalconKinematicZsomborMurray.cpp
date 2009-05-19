/***
 * @file FalconKinematicZsomborMurray.cpp
 * @brief IK and DK implementation for the Novint Falcon based on paper by
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.sourceforge.net/
 *
 */

#include "falcon/kinematic/FalconKinematicZsomborMurray.h"

namespace libnifalcon
{
	inline float clamp(float x, float a, float b)
	{
		return x < a ? a : (x > b ? b : x);
	}

	FalconKinematicZsomborMurray::FalconKinematicZsomborMurray(bool init_now)
	{
	}

	void FalconKinematicZsomborMurray::initialize()
	{
	}

	bool FalconKinematicZsomborMurray::getForces(const boost::array<double, 3> (&position), const boost::array<double, 3> (&cart_force), boost::array<int, 3> (&enc_force))
	{
		return true;
	}

	bool FalconKinematicZsomborMurray::getAngles(boost::array<double, 3> (&position), boost::array<double, 3> (&angles))
	{
		return true;
	}

	bool FalconKinematicZsomborMurray::getPosition(boost::array<int, 3> (&encoders), boost::array<double,3> (&position))
	{
		return true;
	}
}
