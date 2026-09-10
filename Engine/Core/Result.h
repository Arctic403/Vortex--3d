#pragma once
#include <string>

namespace Vortex
{
template<typename T>
class Result
{
public:
    static Result Success(T value)
    {
        return Result(true, value, "");
    }

    static Result Failure(const std::string& error)
    {
        return Result(false, T{}, error);
    }

    bool IsSuccess() const { return success; }
    const T& Value() const { return value; }
    const std::string& Error() const { return error; }

private:
    Result(bool ok, T v, std::string e)
        : success(ok), value(v), error(e) {}

    bool success;
    T value;
    std::string error;
};
}
