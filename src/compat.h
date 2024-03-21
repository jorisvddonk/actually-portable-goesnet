// GOESNET module compatibility defines, for easier porting of modules to Cosmopolitan libc!

// Compatibilty with Cosmopolitan libc:
#define _read read
#define _argc argc
#define _argv argv
#define _close close
#define _write write
#define strupr brtl_strupr
#define random brtl_random
#define srand brtl_srand

#include "brtl.h"

// Various noctis defines
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define qt_M_PI   4*M_PI/3
#define star_classes    12
#define planet_types    10
#define avgmoons        4
#define log2avgmoons	2
#define maxbodies	20 * avgmoons

char	outbuffer[40];
char	textbuffer[40];
char	parbuffer[160];
char	nullbuffer[128];
char	objectname[21];
char	subjectname[21];

char   nsync               = 1;		// 0
char   anti_rad           = 1;          // 1
char   pl_search          = 0;          // 2
char   field_amplificator = 0;          // 3
char   ilight             = 63;         // 4
char   ilightv            = 1;          // 5
char   charge             = 3;          // 6
char   revcontrols        = 0;          // 7
char   ap_targetting      = 0;          // 8
char   ap_targetted       = 0;          // 9
char   ip_targetting      = 0;          // 10
char   ip_targetted       = -1;		// 11
char   ip_reaching        = 0;          // 12
char   ip_reached         = 0;          // 13
char   ap_target_spin     = 0;          // 14
char   ap_target_r        = 0;          // 15
char   ap_target_g        = 0;          // 16
char   ap_target_b        = 0;          // 17
char   nearstar_spin      = 0;          // 18
char   nearstar_r         = 0;          // 19
char   nearstar_g         = 0;          // 20
char   nearstar_b         = 0;          // 21
char   gburst	          = 0;          // 22
char   menusalwayson      = 1;		// 23
char   depolarize  	  = 0;          // 24
int    sys                = 4;		// 25
int    pwr                = 15000;	// 27
int    dev_page           = 0;		// 29
int    ap_target_class    = 0;		// 31
int    f_ray_elapsed	  = 0;		// 33
int    nearstar_class     = 0;		// 35
int    nearstar_nop       = 0;		// 37
float  pos_x              = 0;		// 39
float  pos_y              = 0;		// 43
float  pos_z              = -500;	// 47
float  user_alfa          = 0;		// 51
float  user_beta          = 0;		// 55
float  navigation_beta    = 0;		// 59
float  ap_target_ray      = 0;		// 63
float  nearstar_ray       = 1000;	// 67
double dzat_x             = +3797120;
double dzat_y             = -4352112;
double dzat_z             = -925018;
double ap_target_x        = 0;
double ap_target_y        = 1E9;
double ap_target_z        = 0;
double nearstar_x         = 0;
double nearstar_y         = 1E9;
double nearstar_z         = 0;
double helptime           = 0;
double ip_target_initial_d= 0;
double requested_approach_coefficient = 1;
double current_approach_coefficient = 1;
double reaction_time      = 0.01;
char   fcs_status[11]     = "STANDBY";
int    fcs_status_delay   = 0;
int    psys               = 4;
double ap_target_initial_d= 0;
double requested_vimana_coefficient = 1;
double current_vimana_coefficient = 1;
double vimana_reaction_time = 0.01;
char   lithium_collector  = 0;
char   autoscreenoff	  = 0;
char   ap_reached         = 0;
int    lifter		  = 0;
double secs		  = 0;

void unfreeze();

double 	object_id = 12345;
char   	object_label[25];
double 	s_object_id = 12345;
char   	s_object_label[25];
double 	mblock_id = 12345;
char   	mblock_message[77];
double	subject_id = 12345;
double  idscale = 0.00001;
double laststar_x, laststar_y, laststar_z;
float  zrandom (int range) { return (random(range) - random(range)); }
double nearstar_identity;
int    nearstar_nob;
int8_t isthere(double star_id);

char msgbuffer[77];
char *divider = "&&&&&&&&&&&&&&&&&&&&&";

void msg (char *string);
void warn (char *text, int line);

int class_ray[star_classes] = { 5000, 15000, 300, 20000, 15000, 1000, 3000,
				2000, 4000, 1500, 30000, 250 };

int class_rayvar[star_classes] = { 2000, 10000, 200, 15000, 5000, 1000, 3000,
				   500, 5000, 10000, 1000, 10 };

int class_act[star_classes] = { 2, 4, 1, 6, 5, 10, 100, 1, 2, 1, 10, 1 };

char class_planets[star_classes] = { 12, 18, 8, 15, 20, 3, 0, 1, 7, 20, 2, 5 };

char   nearstar_p_type       [maxbodies];
int    nearstar_p_owner	     [maxbodies];
char   nearstar_p_moonid     [maxbodies];
double nearstar_p_ring       [maxbodies];
double nearstar_p_tilt       [maxbodies];
double nearstar_p_ray        [maxbodies];
double nearstar_p_orb_ray    [maxbodies];
double nearstar_p_orb_seed   [maxbodies];
double nearstar_p_orb_tilt   [maxbodies];
double nearstar_p_orb_orient [maxbodies];
double nearstar_p_orb_ecc    [maxbodies];

int planet_possiblemoons[] = { 1, 1, 2, 3, 2, 2, 18, 2, 3, 20, 20 };

const double planet_orb_scaling=  5.0;
const double avg_planet_sizing =  2.4;
const double moon_orb_scaling  = 12.8;
const double avg_moon_sizing   =  1.8;

double avg_planet_ray[] = { 0.007, 0.003, 0.010, 0.011, 0.010,
			    0.008, 0.064, 0.009, 0.012, 0.125,
			    5.000 };

char class_rgb[3*star_classes] = {
	63, 58, 40,
	30, 50, 63,
	63, 63, 63,
	63, 30, 20,
	63, 55, 32,
	32, 16, 10,
	32, 28, 24,
	10, 20, 63,
	63, 32, 16,
	48, 32, 63,
	40, 10, 10,
	00, 63, 63
};

void extract_ap_target_infos ();

const double deg = M_PI / 180;

// Implementations of common functions

void warn (char *text, int line)
{
    // no-op as we don't need to write to the framebuffer in this version of the dl module
}

void msg (char *string)
{
	int x;

	strcpy (msgbuffer, string);
	msgbuffer[21] = 0;
	printf (msgbuffer);

	x = strlen(msgbuffer);
	while (x < 21) {
		printf (" ");
		x++;
	}
}

void unfreeze() {
    /* Reading the previous situation */
	int fh = openSituation();

    read(fh, &nsync, 1);
    read(fh, &anti_rad, 1);
    read(fh, &pl_search, 1);
    read(fh, &field_amplificator, 1);
    read(fh, &ilight, 1);
    read(fh, &ilightv, 1);
    read(fh, &charge, 1);
    read(fh, &revcontrols, 1);
    read(fh, &ap_targetting, 1);
    read(fh, &ap_targetted, 1);
    read(fh, &ip_targetting, 1);
    read(fh, &ip_targetted, 1);
    read(fh, &ip_reaching, 1);
    read(fh, &ip_reached, 1);
    read(fh, &ap_target_spin, 1);
    read(fh, &ap_target_r, 1);
    read(fh, &ap_target_g, 1);
    read(fh, &ap_target_b, 1);
    read(fh, &nearstar_spin, 1);
    read(fh, &nearstar_r, 1);
    read(fh, &nearstar_g, 1);
    read(fh, &nearstar_b, 1);
    read(fh, &gburst, 1);
    read(fh, &menusalwayson, 1);
    read(fh, &depolarize, 1);
    read(fh, &sys, 2);
    read(fh, &pwr, 2);
    read(fh, &dev_page, 2);
    read(fh, &ap_target_class, 2);
    read(fh, &f_ray_elapsed, 2);
    read(fh, &nearstar_class, 2);
    read(fh, &nearstar_nop, 2);
    read(fh, &pos_x, 4);
    read(fh, &pos_y, 4);
    read(fh, &pos_z, 4);
    read(fh, &user_alfa, 4);
    read(fh, &user_beta, 4);
    read(fh, &navigation_beta, 4);
    read(fh, &ap_target_ray, 4);
    read(fh, &nearstar_ray, 4);
    read(fh, &dzat_x, 8);
    read(fh, &dzat_y, 8);
    read(fh, &dzat_z, 8);
    read(fh, &ap_target_x, 8);
    read(fh, &ap_target_y, 8);
    read(fh, &ap_target_z, 8);
    read(fh, &nearstar_x, 8);
    read(fh, &nearstar_y, 8);
    read(fh, &nearstar_z, 8);
    read(fh, &helptime, 8);
    read(fh, &ip_target_initial_d, 8);
    read(fh, &requested_approach_coefficient, 8);
    read(fh, &current_approach_coefficient, 8);
    read(fh, &reaction_time, 8);
    read(fh, &fcs_status, 11);
    read(fh, &fcs_status_delay, 2);
    read(fh, &psys, 2);
    read(fh, &ap_target_initial_d, 8);
    read(fh, &requested_vimana_coefficient, 8);
    read(fh, &current_vimana_coefficient, 8);
    read(fh, &vimana_reaction_time, 8);
    read(fh, &lithium_collector, 1);
    read(fh, &autoscreenoff, 1);
    read(fh, &ap_reached, 1);
    read(fh, &lifter, 2);
    read(fh, &secs, 8);
    //read(fh, &data, 1);
    //read(fh, &surlight, 1);
    //read(fh, &gnc_pos, 1);
    //read(fh, &goesfile_pos, 4);
    //read(fh, goesnet_command, 120);

    close(fh);
}

int8_t isthere(double star_id) { // from Noctis-IV-LR: https://github.com/dgcole/noctis-iv-lr/
    int8_t visible_sectors = 9;
    int32_t sect_x, sect_y, sect_z;
    int32_t k, advance = 100000;
    double sidlow  = star_id - idscale;
    double sidhigh = star_id + idscale;
    uint8_t sx, sy, sz;
    double laststar_id;
    sect_x = (dzat_x - visible_sectors * 50000) / 100000;
    sect_x *= 100000;
    sect_y = (dzat_y - visible_sectors * 50000) / 100000;
    sect_y *= 100000;
    sect_z = (dzat_z - visible_sectors * 50000) / 100000;
    sect_z *= 100000;

    if (field_amplificator) {
        visible_sectors = 14;
    }

    k = 100000 * visible_sectors;

    for (sx = visible_sectors; sx > 0; sx--, sect_y -= k, sect_x += advance) {
        for (sy = visible_sectors; sy > 0; sy--, sect_z -= k, sect_y += advance) {
            for (sz = visible_sectors; sz > 0; sz--, sect_z += advance) {
                // TODO; Cleanup, rename properly. No teletubby names.
                int32_t eax = sect_x;
                int32_t edx = sect_z;

                eax += edx;
                int32_t ecx = eax;
                edx         = eax;
                edx &= 0x0001FFFF;

                edx += sect_x;
                edx -= 0xC350;
                laststar_x = edx;

                int64_t result = (int64_t) edx * (int64_t) eax;
                eax            = result & 0xFFFFFFFF;
                edx            = result >> 32;

                edx += eax;
                ecx += edx;
                edx &= 0x0001FFFF;

                edx += sect_y;
                edx -= 0xC350;
                laststar_y = edx;
                eax        = ecx;

                result = (int64_t) edx * (int64_t) eax;
                eax    = result & 0xFFFFFFFF;
                edx    = result >> 32;

                edx += eax;
                edx &= 0x0001FFFF;

                edx += sect_z;
                edx -= 0xC350;
                laststar_z = edx;

                laststar_x = round(laststar_x);
                laststar_y = round(laststar_y);
                laststar_z = round(laststar_z);

                laststar_id = (laststar_x * idscale) * (laststar_y * idscale) * (laststar_z * idscale);

                if (laststar_id > sidlow && laststar_id < sidhigh) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void extract_ap_target_infos ()
{
	srand (ap_target_x/100000*ap_target_y/100000*ap_target_z/100000);

	ap_target_class = random (star_classes);
	ap_target_ray = ((float)class_ray[ap_target_class] + (float)random(class_rayvar[ap_target_class])) * 0.001;

	ap_target_r = class_rgb[3*ap_target_class+0];
	ap_target_g = class_rgb[3*ap_target_class+1];
	ap_target_b = class_rgb[3*ap_target_class+2];

	ap_target_spin = 0;
	if (ap_target_class==11) ap_target_spin = random (30) + 1;
	if (ap_target_class==7) ap_target_spin = random (12) + 1;
	if (ap_target_class==2) ap_target_spin = random (4) + 1;
}