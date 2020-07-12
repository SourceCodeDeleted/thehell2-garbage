#pragma once

namespace MPQAccess
{

class IMPQHandleContainer
{
public:
	virtual const HANDLE getHandle() const = 0;
};

}//namespace MPQAccess