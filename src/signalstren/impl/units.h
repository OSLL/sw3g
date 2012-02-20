#ifndef UNITS_H
#define UNITS_H

#include <map>

#include "core/unit.h"

using namespace std;

namespace fine {
    namespace impl {
        namespace units {
            /**
              * Power in %
              */
            static unit UNIT_POWER_IN_PERCENTS("Power (%)");

            /**
              * Power in decibels above milliwatt.
              * Table-convertible to power in %
              */
            static table_convertible_unit UNIT_POWER_IN_dBm("Power (dBm)");

            /**
              * Converts power in dBm to power in % using converter.txt file.
              * Allowed range of values (in dBm): -113..-10.
              */
            static int convert(int num) {
                char string[100];
                int n=0;
                strcpy(string,"grep -w '\\");
                char ch1[6];
                sprintf(ch1,"%d",num);
                strcat(string,ch1);
                strcat(string,"' converter.txt > value.out");
                system(string);
                FILE *f1;
                char str1[100];

                f1 = fopen("value.out","r");
                fscanf(f1,"%[^\n]s",str1);
                int i=0,j=0;
                while(str1[i]!='='&&str1[i]!='\0')
                    i++;
                i++;
                i++;
                while(str1[i]!='\0')
                {
                    ch1[j]=str1[i];
                    j++;i++;
                }
                ch1[j]='\0';
                n = atoi(ch1);
                fclose(f1);
                return n;
            }

            /**
              * Initialization function for UNIT_POWER_IN_dBm
              */
            static bool pow_in_dbm_init() {
                map<double, double> percents_to_dbm;
                for (int dbm = -113; dbm <= -10; ++dbm) {
                    percents_to_dbm.insert(make_pair(dbm, convert(dbm)));
                }

                UNIT_POWER_IN_dBm.add_conversion_table(UNIT_POWER_IN_PERCENTS, percents_to_dbm);

                return true;
            }

            static bool __pow_in_dbm_init = pow_in_dbm_init();
        }
    }
}

#endif // UNITS_H
