#pragma once

#include <memory>

struct tFileBaseImpl;

class tFileBase
{
	enum class FILE_TYPE;
	std::unique_ptr<tFileBaseImpl> pImpl;
	virtual FILE_TYPE get_type_impl() const noexcept = 0;
public:
	enum class FILE_TYPE
	{
		REGULAR_FILE,
		HARD_LINK,
		SOFT_LINK
	};
	virtual ~tFileBase() = default;
	FILE_TYPE get_type () const
	{
		return get_type_impl();
	}
};