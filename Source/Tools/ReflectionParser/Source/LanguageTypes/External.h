#pragma once

#include "LanguageTypes/Class.h"

class External : public Class
{
public:
    External(const Cursor &cursor);

    bool ShouldCompile(void) const override;
};