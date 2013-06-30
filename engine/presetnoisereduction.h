/*
	Korg Pandora PX5D Editor for Linux
	Copyright (C) 2011-2012, Frederic Jaume <dev@exp-media.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PRESETNOISEREDUCTION_H
#define PRESETNOISEREDUCTION_H

#include <string>

class PresetNoiseReduction
{
public:
	PresetNoiseReduction();

	const char* getParamStringValue(unsigned int value);
	unsigned int getParameter() { return noiseReductionLevel; }
	bool getEnabled() { return noiseReductionLevel>0; }
	bool setParameter(unsigned int p);

	const char* getParamName() const;
	unsigned int getMinParam();
	unsigned int getMaxParam();

	std::string toString() const;

private:
	unsigned int noiseReductionLevel;
	unsigned int validateParameter(unsigned int value);
	static const char* commonValues[];

};

#endif // PRESETNOISEREDUCTION_H
