#pragma once

#include <memory>

struct tFileBaseImpl;

class tFileBase
{
	std::unique_ptr<tFileBaseImpl> pImpl;
public:
	virtual ~tFileBase() = default;
};