#ifndef _PORTSSCANBUILTINPROFILES_H_
#define _PORTSSCANBUILTINPROFILES_H_

#include "PortsScanProfileType.h"
#include "utils/Singleton.h"

namespace Services { namespace PortsScanner
{

class PortsScanBuiltInProfiles: public Singleton<PortsScanBuiltInProfiles>
{
public:
    QList<PortsScanProfileType> GetAllProfiles()
    {
        QList<PortsScanProfileType> profiles;
        profiles.append(this->GetProfile_PlainNmapFast());
        profiles.append(this->GetProfile_PlainNmap());
        profiles.append(this->GetProfile_PlainNmapFastAllPorts());
        profiles.append(this->GetProfile_WebServers());
        profiles.append(this->GetProfile_WebServersFingerprint());
        profiles.append(this->GetProfile_IcsEssentials());
        profiles.append(this->GetProfile_IcsProtocols());

        return profiles;
    };

private:
    PortsScanProfileType GetProfile_PlainNmapFast()
    {
        QString targets = "ANY,ANY,,,,,Scan default nMap ports FAST,";

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "Plain nMap fast (default ports)";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_PlainNmap()
    {
        QString targets = "ANY,ANY,,,,-sC -sV -O,Scan default nMap ports,";

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "Plain nMap (default ports)";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_PlainNmapFastAllPorts()
    {
        QString targets = "ANY,ANY,,,,-p-,Scan all ports FAST,";

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "Plain nMap fast (all ports)";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_WebServers()
    {
        QString targets = "WEB SERVER,,\"66,80,81,443,445,457,1080,1100,1241,1352,1433,1434,1521,1944,2301,3000,3128,3306,4000,4001,4002,4100,5000,5432,5800,5801,5802,6346,6347,7001,7002,8080,8443,8888,30821\",\"66,80,81,443,445,457,1080,1100,1241,1352,1433,1434,1521,1944,2301,3000,3128,3306,4000,4001,4002,4100,5000,5432,5800,5801,5802,6346,6347,7001,7002,8080,8443,8888,30821\",,-sC -sV,Scan for webservers,";

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "Web Servers Detection";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_WebServersFingerprint()
    {
        QString targets = "WEB SERVER,,\"66,80,81,443,445,457,1080,1100,1241,1352,1433,1434,1521,1944,2301,3000,3128,3306,4000,4001,4002,4100,5000,5432,5800,5801,5802,6346,6347,7001,7002,8080,8443,8888,30821\",\"66,80,81,443,445,457,1080,1100,1241,1352,1433,1434,1521,1944,2301,3000,3128,3306,4000,4001,4002,4100,5000,5432,5800,5801,5802,6346,6347,7001,7002,8080,8443,8888,30821\",,\"--script=http-*\",Scan and fingerprint webservers,";

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "Web Servers Fingerprint";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_IcsEssentials()
    {
        // https://ctf-wiki.mahaloz.re/ics/discover/
        static QString targets = QString(QByteArray::fromBase64("SUNTLE1vZGJ1cyBUQ1AsNTAyLCxwb3J0OjUwMiwiLVBuIC0tc2NyaXB0PWJhbm5lcixtb2RidXMtZGlzY292ZXIsbW9kaWNvbi1pbmZvIC0tc2NyaXB0LWFyZ3M9J21vZGJ1cy1kaXNjb3Zlci5hZ2dyZXNzaXZlPXRydWUnIixNb2RidXMgVENQLApJQ1MsQ3JpbXNvbiB2Myw3ODksLHBvcnQ6Nzg5IHByb2R1Y3Q6XCJSZWQgTGlvbiBDb250cm9sc1wiLCItUG4gLS1zY3JpcHQ9YmFubmVyLGNyMy1maW5nZXJwcmludC5uc2UiLFJlZCBMaW9uIENyaW1zb24gVjMgUHJvdG9jb2wsCklDUyxJRUMgNjA4NzAtNS0xMDQsNzg5LCxwb3J0Ojc4OSwiLVBuIC1uIC1kIC0tc2NyaXB0PWJhbm5lcixpZWMtaWRlbnRpZnkubnNlIC0tc2NyaXB0LWFyZ3M9J2llYy1pZGVudGlmeS50aW1lb3V0PTUwMCciLElQQ09NTSBQcm90b2NvbHMsCklDUyxTaWVtZW5zIFM3LDEwMiwscG9ydDoxMDIsIi1QbiAtLXNjcmlwdD1iYW5uZXIsczctaW5mby5uc2UsczcxMjAwLWVudW1lcmF0ZS1vbGQubnNlIixTaWVtZW5zIFM3IEZhbWlseSwKSUNTLEJBQ25ldC9JUCwsNDc4MDgscG9ydDo0NzgwOCwiLVBuIC0tc2NyaXB0PWJhbm5lcixCQUNuZXQtZGlzY292ZXItZW51bWVyYXRlLm5zZSIsIkJBQ25ldC9JUCBwcm90b2NvbCIsCklDUyxETlAzLDIwMDAwLDIwMDAwLHBvcnQ6MjAwMDAsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixkbnAzLWVudW1lcmF0ZS5uc2UsZG5wMy1pbmZvLm5zZSIsRE5QMyBvbiBUQ1AvVURQIDIwMDAwLApJQ1MsRXRoZXJDQVQsLDM0OTgwLHBvcnQ6MzQ5ODAsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lciIsRXRoZXJDQVQgUHJvdG9jb2wsCklDUyxFdGhlcm5ldC9JUCw0NDgxOCwyMjIyLHBvcnQ6NDQ4MTgsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixlbmlwLWVudW1lcmF0ZSIsRXRoZXJuZXQvSVAsCklDUyxGTC1uZXQsLCI1NTAwMCw1NTAwMSw1NTAwMiw1NTAwMyIsInBvcnQ6NTUwMDAsNTUwMDEsNTUwMDIsNTUwMDMiLCItUG4gLXNDIC1zViAtLXNjcmlwdD1iYW5uZXIiLEZMLW5ldCwKSUNTLEZpZWxkYnVzLCIxMDg5LDEwOTAsMTA5MSIsIjEwODksMTA5MCwxMDkxIiwicG9ydDoxMDg5LDEwOTAsMTA5MSIsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lciIsRm91bmRhdGlvbiBGaWVsZGJ1cyBIU0UsCklDUyxPUEMgVUEgU2VydmVyLDQ4NDAsLHBvcnQ6NDg0MCwiLVBuIC1zQyAtc1YgLS1zY3JpcHQ9YmFubmVyIixPUEMgVUEgRGlzY292ZXJ5IFNlcnZlciAtIDQ4NDAsCklDUyxQUk9GSU5FVCwiMzQ5NjIsMzQ5NjMsMzQ5NjQiLCIzNDk2MiwzNDk2MywzNDk2NCIsInBvcnQ6MzQ5NjIsMzQ5NjMsMzQ5NjQiLCItUG4gLXNDIC1zViAtLXNjcmlwdD1iYW5uZXIiLFBST0ZJTkVULApJQ1MsRklOUyw5NjAwLDk2MDAscG9ydDo5NjAwLCItUG4gLXNDIC1zViAtLXNjcmlwdD1iYW5uZXIsb21yb250Y3AtaW5mby5uc2Usb21yb251ZHAtaW5mby5uc2UiLEZJTlMgKEZhY3RvcnkgSW50ZXJmYWNlIE5ldHdvcmsgU2VydmljZSksCklDUyxIQVJULUlQLDUwOTQsNTA5NCxwb3J0OjUwOTQsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixoYXJ0aXAubnNlIixIQVJULUlQIChIaWdod2F5IEFkZHJlc3NhYmxlIFJlbW90ZSBUcmFuc2R1Y2VyIG92ZXIgSVApLApJQ1MsTUVMU0VDLVEsNTAwNyw1MDA2LCJwb3J0OjUwMDYsNTAwNyIsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixtZWxzZWNxLWRpc2NvdmVyLm5zZSxtZWxzZWNxLWRpc2NvdmVyLm5zZSIsTUVMU0VDLVEgKE1pdHN1YmlzaGkgZWxlY3RyaWMpLApJQ1MsVHJpZGl1bSBGb3gsIjQ5MTEsMTkxMSIsLCJwb3J0OjE5MTEsNDkxMSIsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixmb3gtaW5mby5uc2UiLFRyaWRpdW0gRm94IFByb3RvY29sIGJ5IE5pYWdhcmEgQVgsCklDUyxQQ1dPUlgsMTk2MiwscG9ydDoxOTYyLCItUG4gLXNDIC1zViAtLXNjcmlwdD1iYW5uZXIscGN3b3J4LWluZm8iLFBDV29yeCBieSBQaG9lbml4IENvbnRhY3QsCklDUyxQcm9Db25PcywyMDU0NywsInBvcnQ6MjA1NDcgUExDIiwiLVBuIC1zQyAtc1YgLS1zY3JpcHQ9YmFubmVyLHByb2Nvbm9zLWluZm8ubnNlIixQcm9Db25PcywKSUNTLENvZGVzeXMsIjEyMDAsMjQ1NSIsLCJwb3J0OjI0NTUgb3BlcmF0aW5nIHN5c3RlbSIsIi1QbiAtc0MgLXNWIC0tc2NyaXB0PWJhbm5lcixjb2Rlc3lzLXYyLWRpc2NvdmVyLm5zZSIsQ29kZXN5cywKSUNTLEdFLVNSVFAsIjE4MjQ1LDE4MjQ2IiwsInBvcnQ6MTgyNDUsMTgyNDYgcHJvZHVjdDoiZ2VuZXJhbCBlbGVjdHJpYyIiLCItUG4gLXNDIC1zViAtLXNjcmlwdD1iYW5uZXIiLFNlcnZpY2UgUmVxdWVzdCBUcmFuc3BvcnQgUHJvdG9jb2wgKEdFLVNSVFApLA====="));

        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "ICS Essentials";
        profile.Targets = this->ParseTargetsFromString(targets);
        return profile;
    }

    PortsScanProfileType GetProfile_IcsProtocols()
    {
        QString targets = "";
        PortsScanProfileType profile;
        profile.BuiltInProfile = true;
        profile.ProfileName = "ICS Protocols";
        profile.Targets = this->GetProfile_IcsEssentials().Targets;
        profile.Targets.append(this->ParseTargetsFromString(targets));
        return profile;
    }

    QList<PortsScanTargetType> ParseTargetsFromString(const QString &input)
    {
        QList<PortsScanTargetType> output;
        for( const QString &target_str: input.split("\n") )
        {
            PortsScanTargetType target;
            if( target.ParseTargetFromString(target_str) )
            {
                output.append(target);
            }
        }
        return output;
    }

};

}} // Namespaces

#endif // _PORTSSCANBUILTINPROFILES_H_
