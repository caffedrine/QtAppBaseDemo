#include "TldCountry.h"

namespace Services { namespace TldCountry
{

    static QMap<QString, QString> CountriesList =
            {
                    {"AF",           ".af"},
                    {"AX",           ".ax"},
                    {"AL",           ".al"},
                    {"DZ",           ".dz"},
                    {"AS",           ".as"},
                    {"AD",           ".ad"},
                    {"AO",           ".ao"},
                    {"AI",           ".ai"},
                    {"AQ",           ".aq"},
                    {"AG",           ".ag"},
                    {"AR",           ".ar"},
                    {"AM",           ".am"},
                    {"AW",           ".aw"},
                    {"AC",           ".ac"},
                    {"AU",           ".au"},
                    {"AT",           ".at"},
                    {"AZ",           ".az"},
                    {"BS",           ".bs"},
                    {"BH",           ".bh"},
                    {"BD",           ".bd"},
                    {"BB",           ".bb"},
                    {"BY",           ".by"},
                    {"BE",           ".be"},
                    {"BZ",           ".bz"},
                    {"BJ",           ".bj"},
                    {"BM",           ".bm"},
                    {"BT",           ".bt"},
                    {"BO",           ".bo"},
                    {"BQ",           ".bq"},
                    {"NL",           ".nl"},
                    {"BA",           ".ba"},
                    {"BW",           ".bw"},
                    {"BV",           ".bv"},
                    {"BR",           ".br"},
                    {"IO",           ".io"},
                    {"VG",           ".vg"},
                    {"Unknown code", ".bn"},
                    {"BG",           ".bg"},
                    {"BF",           ".bf"},
                    {"Unknown code", ".mm"},
                    {"BI",           ".bi"},
                    {"KH",           ".kh"},
                    {"CM",           ".cm"},
                    {"CA",           ".ca"},
                    {"Unknown code", ".cv"},
                    {"Unknown code", ".cat"},
                    {"KY",           ".ky"},
                    {"CF",           ".cf"},
                    {"TD",           ".td"},
                    {"CL",           ".cl"},
                    {"CN",           "People’s Republic of"},
                    {"CX",           ".cx"},
                    {"CC",           ".cc"},
                    {"CO",           ".co"},
                    {"KM",           ".km"},
                    {"CG",           "Democratic Republic of the (Congo-Kinshasa)"},
                    {"CG",           "Republic of the (Congo-Brazzaville)"},
                    {"CK",           ".ck"},
                    {"CR",           ".cr"},
                    {"Unknown code", ".ci"},
                    {"HR",           ".hr"},
                    {"CU",           ".cu"},
                    {"CW",           ".cw"},
                    {"CY",           ".cy"},
                    {"Unknown code", ".cz"},
                    {"DK",           ".dk"},
                    {"DJ",           ".dj"},
                    {"DM",           ".dm"},
                    {"DO",           ".do"},
                    {"Unknown code", ".tl"},
                    {"Unknown code", ".tp"},
                    {"EC",           ".ec"},
                    {"EG",           ".eg"},
                    {"SV",           ".sv"},
                    {"GQ",           ".gq"},
                    {"ER",           ".er"},
                    {"EE",           ".ee"},
                    {"ET",           ".et"},
                    {"Unknown code", ".eu"},
                    {"Unknown code", ".fk"},
                    {"Unknown code", ".fo"},
                    {"Unknown code", ".fm"},
                    {"FJ",           ".fj"},
                    {"FI",           ".fi"},
                    {"FR",           ".fr"},
                    {"GF",           ".gf"},
                    {"PF",           ".pf"},
                    {"Unknown code", ".tf"},
                    {"Unknown code", ".ga"},
                    {"Unknown code", ".gal"},
                    {"GM",           ".gm"},
                    {"Unknown code", ".ps"},
                    {"GE",           ".ge"},
                    {"DE",           ".de"},
                    {"GH",           ".gh"},
                    {"GI",           ".gi"},
                    {"GR",           ".gr"},
                    {"GL",           ".gl"},
                    {"GD",           ".gd"},
                    {"GP",           ".gp"},
                    {"GU",           ".gu"},
                    {"GT",           ".gt"},
                    {"GG",           ".gg"},
                    {"GN",           ".gn"},
                    {"GW",           ".gw"},
                    {"GY",           ".gy"},
                    {"HT",           ".ht"},
                    {"HM",           ".hm"},
                    {"HN",           ".hn"},
                    {"HK",           ".hk"},
                    {"HU",           ".hu"},
                    {"IS",           ".is"},
                    {"IN",           ".in"},
                    {"ID",           ".id"},
                    {"Unknown code", ".ir"},
                    {"IQ",           ".iq"},
                    {"IE",           ".ie"},
                    {"IM",           ".im"},
                    {"IL",           ".il"},
                    {"IT",           ".it"},
                    {"JM",           ".jm"},
                    {"JP",           ".jp"},
                    {"JE",           ".je"},
                    {"JO",           ".jo"},
                    {"KZ",           ".kz"},
                    {"KE",           ".ke"},
                    {"KI",           ".ki"},
                    {"KW",           ".kw"},
                    {"KG",           ".kg"},
                    {"Unknown code", ".la"},
                    {"LV",           ".lv"},
                    {"LB",           ".lb"},
                    {"LS",           ".ls"},
                    {"LR",           ".lr"},
                    {"LY",           ".ly"},
                    {"LI",           ".li"},
                    {"LT",           ".lt"},
                    {"LU",           ".lu"},
                    {"Unknown code", ".mo"},
                    {"Unknown code", "Republic of (the former Yugoslav Republic of Macedonia"},
                    {"MG",           ".mg"},
                    {"MW",           ".mw"},
                    {"MY",           ".my"},
                    {"MV",           ".mv"},
                    {"ML",           ".ml"},
                    {"MT",           ".mt"},
                    {"MH",           ".mh"},
                    {"MQ",           ".mq"},
                    {"MR",           ".mr"},
                    {"MU",           ".mu"},
                    {"YT",           ".yt"},
                    {"MX",           ".mx"},
                    {"Unknown code", ".md"},
                    {"MC",           ".mc"},
                    {"MN",           ".mn"},
                    {"ME",           ".me"},
                    {"MS",           ".ms"},
                    {"MA",           ".ma"},
                    {"MZ",           ".mz"},
                    {"MM",           ".mm"},
                    {"NA",           ".na"},
                    {"NR",           ".nr"},
                    {"NP",           ".np"},
                    {"NL",           ".nl"},
                    {"NC",           ".nc"},
                    {"NZ",           ".nz"},
                    {"NI",           ".ni"},
                    {"NE",           ".ne"},
                    {"NG",           ".ng"},
                    {"NU",           ".nu"},
                    {"NF",           ".nf"},
                    {"Unknown code", "self-declared state)"},
                    {"Unknown code", "self-declared state)"},
                    {"Unknown code", ".kp"},
                    {"MP",           ".mp"},
                    {"NO",           ".no"},
                    {"OM",           ".om"},
                    {"PK",           ".pk"},
                    {"PW",           ".pw"},
                    {"Unknown code", ".ps"},
                    {"PA",           ".pa"},
                    {"PG",           ".pg"},
                    {"PY",           ".py"},
                    {"PE",           ".pe"},
                    {"PH",           ".ph"},
                    {"Unknown code", ".pn"},
                    {"PL",           ".pl"},
                    {"PT",           ".pt"},
                    {"PR",           ".pr"},
                    {"QA",           ".qa"},
                    {"RO",           ".ro"},
                    {"Unknown code", ".ru"},
                    {"RW",           ".rw"},
                    {"Unknown code", ".re"},
                    {"Unknown code", ".bq"},
                    {"Unknown code", ".an"},
                    {"Unknown code", ".bl"},
                    {"Unknown code", ".gp"},
                    {"Unknown code", ".fr"},
                    {"Unknown code", ".sh"},
                    {"KN",           ".kn"},
                    {"LC",           ".lc"},
                    {"Unknown code", ".mf"},
                    {"Unknown code", ".gp"},
                    {"Unknown code", ".fr"},
                    {"Unknown code", ".pm"},
                    {"VC",           ".vc"},
                    {"WS",           ".ws"},
                    {"SM",           ".sm"},
                    {"Unknown code", ".st"},
                    {"SA",           ".sa"},
                    {"SN",           ".sn"},
                    {"RS",           ".rs"},
                    {"SC",           ".sc"},
                    {"SL",           ".sl"},
                    {"SG",           ".sg"},
                    {"Unknown code", ".bq"},
                    {"Unknown code", ".an"},
                    {"Unknown code", ".nl"},
                    {"Unknown code", ".sx"},
                    {"Unknown code", ".an"},
                    {"SK",           ".sk"},
                    {"SI",           ".si"},
                    {"SB",           ".sb"},
                    {"SO",           ".so"},
                    {"Unknown code", ".so"},
                    {"ZA",           ".za"},
                    {"GS",           ".gs"},
                    {"Unknown code", ".kr"},
                    {"SS",           ".ss"},
                    {"ES",           ".es"},
                    {"LK",           ".lk"},
                    {"SD",           ".sd"},
                    {"SR",           ".sr"},
                    {"Unknown code", ".sj"},
                    {"Unknown code", ".sz"},
                    {"SE",           ".se"},
                    {"CH",           ".ch"},
                    {"Unknown code", ".sy"},
                    {"Unknown code", ".tw"},
                    {"TJ",           ".tj"},
                    {"Unknown code", ".tz"},
                    {"TH",           ".th"},
                    {"TG",           ".tg"},
                    {"TK",           ".tk"},
                    {"TO",           ".to"},
                    {"Unknown code", ".tt"},
                    {"TN",           ".tn"},
                    {"TR",           ".tr"},
                    {"TM",           ".tm"},
                    {"TC",           ".tc"},
                    {"TV",           ".tv"},
                    {"UG",           ".ug"},
                    {"UA",           ".ua"},
                    {"Unknown code", ".ae"},
                    {"Unknown code", ".uk"},
                    {"Unknown code", ".us"},
                    {"Unknown code", ".vi"},
                    {"UY",           ".uy"},
                    {"UZ",           ".uz"},
                    {"VU",           ".vu"},
                    {"Unknown code", ".va"},
                    {"Unknown code", ".ve"},
                    {"Unknown code", ".vn"},
                    {"WF",           ".wf"},
                    {"EH",           ".eh"},
                    {"EH",           ".ma"},
                    {"YE",           ".ye"},
                    {"ZM",           ".zm"},
                    {"ZW",           ".zw"},
            };

    QString GetCountryByTLD(QString tld)
    {
        QMap<QString, QString>::iterator i;

        for(i = CountriesList.begin(); i != CountriesList.end(); ++i)
        {
            if(i.value() == tld)
                return i.key();
        }

        return "";
    }

    QString GetTldByCountry(QString country_tld)
    {
        if(CountriesList.find(country_tld) == CountriesList.end())
            return "";
        return CountriesList.value(country_tld);
    }
}}  // Namespaces
