#pragma once
#include <optional>
#include <string>
#include <utility>

namespace Vortex
{
template<typename T>
class Result
{
public:
    static Result Success(T value)
    {
        return Result(std::move(value));
    }

    static Result Failure(std::string error)
    {
        return Result(FailureTag{}, std::move(error));
    }

    [[nodiscard]] bool IsSuccess() const noexcept { return value.has_value(); }
    explicit operator bool() const noexcept { return IsSuccess(); }

    T& Value() & { return value.value(); }
    const T& Value() const& { return value.value(); }
    T&& Value() && { return std::move(value).value(); }

    [[nodiscard]] const std::string& Error() const noexcept { return error; }

private:
    struct FailureTag {};

    explicit Result(T valueIn)
        : value(std::move(valueIn))
    {
    }

    Result(FailureTag, std::string errorIn)
        : error(std::move(errorIn))
    {
    }

    std::optional<T> value;
    std::string error;
};

template<>
class Result<void>
{
public:
    static Result Success() { return Result(true, {}); }
    static Result Failure(std::string error)
    {
        return Result(false, std::move(error));
    }

    [[nodiscard]] bool IsSuccess() const noexcept { return success; }
    explicit operator bool() const noexcept { return IsSuccess(); }
    [[nodiscard]] const std::string& Error() const noexcept { return error; }

private:
    Result(bool successIn, std::string errorIn)
        : success(successIn), error(std::move(errorIn))
    {
    }

    bool success = false;
    std::string error;
};
}
