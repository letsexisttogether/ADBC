#pragma once

#include "Core/DBConnection.hpp"

namespace ADBC
{
    template <class ... _Types>
    using QueryOutputPack = std::tuple<_Types&...>;

    template <class ... _Types>
    using QueryParamPack = std::tuple<_Types...>;

    template <class _QueryOutputPack, class _QueryParamPack>
    class Query
    {
    public:
        Query(std::string&& text, _QueryOutputPack outputs,
            _QueryParamPack params);

        auto GetText() const noexcept -> const std::string&;

        auto GetOutputPack() const noexcept -> const _QueryOutputPack&;
        auto GetParamPack() const noexcept -> const _QueryParamPack&;

    private:
        std::string m_Text{};
        _QueryOutputPack m_Outputs{};
        _QueryParamPack m_Params{};
    };

    template <class _QueryOutputPack, class _QueryParamPack>
    Query<_QueryOutputPack, _QueryParamPack>::Query(std::string&& text,
        _QueryOutputPack outputs, _QueryParamPack params)
        : m_Text{ std::move(text) }, m_Outputs{ std::move(outputs) },
        m_Params{ std::move(params) } {}

    template <class _QueryOutputPack, class _QueryParamPack>
    auto Query<_QueryOutputPack, _QueryParamPack>::GetText() const noexcept
        -> const std::string&
    {
        return m_Text;
    }

    template <class _QueryOutputPack, class _QueryParamPack>
    auto Query<_QueryOutputPack, _QueryParamPack>::GetOutputPack()
        const noexcept -> const _QueryOutputPack&
    {
        return m_Outputs;
    }

    template <class _QueryOutputPack, class _QueryParamPack>
    auto Query<_QueryOutputPack, _QueryParamPack>::GetParamPack()
        const noexcept -> const _QueryParamPack&
    {
        return m_Outputs;
    }
};
