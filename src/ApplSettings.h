#ifndef _APPLSETTINGS_H_
#define _APPLSETTINGS_H_


class ApplSettings
{
private:
    ApplSettings() = default;

public:
    // By default, show only summaries
    bool LoggingVerbose = true;
    bool LoggingSuperVerbose = true;

    static ApplSettings& instance()
    {
        static ApplSettings INSTANCE;
        return INSTANCE;
    }
};


#endif //_APPLSETTINGS_H_
