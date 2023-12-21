/*
 * Copyright (c) 2023, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Port.h"
#include <AK/HashMap.h>
#include <AK/String.h>
#include <AK/StringView.h>
#include <AK/Types.h>
#include <AK/Variant.h>
#include <LibSemVer/SemVer.h>

constexpr StringView ports_database = "/usr/Ports/installed.db"sv;

class InstalledPort : public Port {
public:
    enum class Type {
        Auto,
        Manual,
    };
    static Optional<Type> type_from_string(StringView);

    static ErrorOr<HashMap<String, InstalledPort>> read_ports_database();
    static ErrorOr<void> for_each_by_type(HashMap<String, InstalledPort>&, Type type, Function<ErrorOr<void>(InstalledPort const&)> callback);

    InstalledPort(String const& name, String const& version, Type type)
        : Port(name, version)
        , m_type(type)
    {
    }

    Type type() const
    {
        return m_type;
    }
    StringView type_as_string_view() const
    {
        if (m_type == Type::Auto)
            return "Automatic"sv;
        if (m_type == Type::Manual)
            return "Manual"sv;
        VERIFY_NOT_REACHED();
    }

    ReadonlySpan<String> dependencies() const { return m_dependencies.span(); }

private:
    Type m_type;
    Vector<String> m_dependencies;
};
