#include "correlation.h"

#include <cmath>

#define SAMPLEPERBIT (SAMPLERATE / BITRATE)

#define FREQ_MARK  1200.0
#define FREQ_SPACE 2200.0


static uint8_t ptr=0;

static double data[SAMPLEPERBIT];
static double corr_mark_i[SAMPLEPERBIT]; /* sin 1200 Hz */
static double corr_mark_q[SAMPLEPERBIT]; /* cos 1200 Hz */
static double corr_space_i[SAMPLEPERBIT]; /* sin 2200 Hz */
static double corr_space_q[SAMPLEPERBIT]; /* cos 2200 Hz */

/*********************************************************************************************************************/
void afsk_init() {
/*********************************************************************************************************************/
    
    
        uint8_t i;
    
        for (i=0;i<SAMPLEPERBIT;i++)
        {
            corr_mark_i[i] = cos(2*M_PI*i/static_cast<double>(SAMPLEPERBIT)*FREQ_MARK/BITRATE);
            corr_mark_q[i] = sin(2*M_PI*i/static_cast<double>(SAMPLEPERBIT)*FREQ_MARK/BITRATE);
            corr_space_i[i] = cos(2*M_PI*i/static_cast<double>(SAMPLEPERBIT)*FREQ_SPACE/BITRATE);
            corr_space_q[i] = sin(2*M_PI*i/static_cast<double>(SAMPLEPERBIT)*FREQ_SPACE/BITRATE);
        }
        
    }
    

/*********************************************************************************************************************/
std::tuple<double, double, double, double> afsk_demod(double curr_sample) {
/*********************************************************************************************************************/
    
        uint8_t i = 0;
        double d = 0.0;
        double out_mark_i=0,out_mark_q=0,out_space_i=0,out_space_q=0;
    
        data[ptr]=curr_sample;
    
        ptr = (ptr+1)%SAMPLEPERBIT; /* % : Modulo */
    
        for(i=0;i<SAMPLEPERBIT;i++)
        {
    
            d = data[(ptr+i)%SAMPLEPERBIT];
            out_mark_i += d*corr_mark_i[i];
            out_mark_q += d*corr_mark_q[i];
            out_space_i += d*corr_space_i[i];
            out_space_q += d*corr_space_q[i];
        }
        
        
         return {out_mark_i, out_mark_q, out_space_i, out_space_q};
        /* return  (out_space_i>>12)*(out_space_i>>12)+
                (out_space_q>>12)*(out_space_q>>12)-
                (out_mark_i>>12)*(out_mark_i>>12)-
                (out_mark_q>>12)*(out_mark_q>>12);		*/	  /// wszedzie bylo 11
    
    }