/*

	Noctis galactic map / DL command.
	GOES Net Module.

    Modified for compilation with Cosmopolitan libc
    Requires that STARMAP.BIN, GUIDE.BIN and CURRENT.BIN are either in ../data/ or in the current working directory.

    Compiling:
    cosmocc dl.c -o dl.com

    USAGE:
    ./dl.com FENIA
*/

const double MAX_OBJECTS_PER_STAR = 80;

#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/math.h"
#include "libc/calls/calls.h"
#include "libc/fmt/conv.h"
#include "libc/sysv/consts/o.h"
#include "fileops.h"
#include "compat.h"


//////////////////////////////////////////////////////////////////////////////

int	sts;
char	query;

int 	i, fh, gh;
int 	analyzed_sectors_range;



char find (char *starname)
{
	int p, n, ctc, found;
	ctc = strlen (starname);
	if (ctc > 20 || ctc <= 0) {
		msg ("INVALID OBJECT NAME.");
		return(0);
	}
	n = 0;
	found = 0;
	lseek (fh, 4, SEEK_SET);
	while (_read (fh, &s_object_id, 8) && _read (fh, &s_object_label, 24) == 24) {
		if (memcmp (&s_object_id, "Removed:", 8)) {
			if (!memcmp (s_object_label, starname, ctc)) {
				n++;
				memcpy (object_label, s_object_label, 24);
				object_id = s_object_id;
				memcpy (subjectname, object_label, 20);
				subject_id = object_id;
				if (object_label[21] == 'S') found = 1;
				if (object_label[21] == 'P') found = 2;
				p = 20;
				while (p >= 0) {
					if (s_object_label[p] != 32) {
						if (s_object_label[p] == starname[p])
							return (found);
						else
							break;
					}
					p--;
				}
			}
		}
	}
	if (!n)
		msg ("OBJECT NOT FOUND.");
	if (n > 1) {
		msg ("AMBIGUOUS SEARCH KEY:");
		msg ("PLEASE EXPAND NAME...");
		msg (divider);
		msg ("POSSIBLE RESULTS ARE:");
		msg (divider);
		lseek (fh, 4, SEEK_SET);
		while (_read (fh, &s_object_id, 8) && _read (fh, &s_object_label, 24) == 24) {
		if (memcmp (&s_object_id, "Removed:", 8) && !memcmp (s_object_label, starname, ctc)) {
			s_object_label[21] = 0;
			msg (s_object_label);
		}}
		msg (divider);
		found = 0;
	}
	return (found);
}

//////////////////////////////////////////////////////////////////////////////

void prepare_nearstar ()
{
	int    n, c, q, r, s, t;
	double key_radius;

	nearstar_class = ap_target_class;
	nearstar_x = ap_target_x;
	nearstar_y = ap_target_y;
	nearstar_z = ap_target_z;
	nearstar_ray = ap_target_ray;
	nearstar_spin = ap_target_spin;
	nearstar_r = ap_target_r;
	nearstar_g = ap_target_g;
	nearstar_b = ap_target_b;

	nearstar_identity = nearstar_x/100000*nearstar_y/100000*nearstar_z/100000;

	srand ((long)nearstar_x%10000*(long)nearstar_y%10000*(long)nearstar_z%10000);

	nearstar_nop = random (class_planets[nearstar_class] + 1);

	for (n=0; n<nearstar_nop; n++) {
		nearstar_p_owner[n]	 = -1;
		nearstar_p_orb_orient[n] = (double) deg * (double) random (360);
		nearstar_p_orb_seed[n]   = 3 * (n*n+1) * nearstar_ray + (float) random (300 * nearstar_ray) / 100;
		nearstar_p_tilt[n]       = zrandom (10*nearstar_p_orb_seed[n]) / 500;
		nearstar_p_orb_tilt[n]   = zrandom (10*nearstar_p_orb_seed[n]) / 5000;
		nearstar_p_orb_ecc[n]    = 1 - (double) random (nearstar_p_orb_seed[n] + 10*fabs(nearstar_p_orb_tilt[n])) / 2000;
		nearstar_p_ray[n]        = (double) random (nearstar_p_orb_seed[n]) * 0.001 + 0.01;
		nearstar_p_ring[n]	 = zrandom (nearstar_p_ray[n]) * (1 + (double) random (1000) / 100);
		if (nearstar_class != 8)
			nearstar_p_type[n] = random (planet_types);
		else {
			if (random(2)) {
				nearstar_p_type[n] = 10;
				nearstar_p_orb_tilt[n] *= 100;
			}
			else
				nearstar_p_type[n] = random (planet_types);
		}
		if (nearstar_class==2||nearstar_class==7||nearstar_class==15)
			nearstar_p_orb_seed[n] *= 10;
	}

	if (!nearstar_class) {
		if (random(4)==2) nearstar_p_type[2] = 3;
		if (random(4)==2) nearstar_p_type[3] = 3;
		if (random(4)==2) nearstar_p_type[4] = 3;
	}

	for (n=0; n<nearstar_nop; n++) {
		switch (nearstar_class) {
			case 2: while (nearstar_p_type[n]==3)
					nearstar_p_type[n] = random (10);
				break;
			case 5: while (nearstar_p_type[n]==6||
				       nearstar_p_type[n]==9)
					nearstar_p_type[n] = random (10);
				break;
			case 7:	nearstar_p_type[n] = 9;
				break;
			case 9: while (nearstar_p_type[n]!=0&&
				       nearstar_p_type[n]!=6&&
				       nearstar_p_type[n]!=9)
					nearstar_p_type[n] = random (10);
				break;
			case 11:while (nearstar_p_type[n]!=1&&
				       nearstar_p_type[n]!=7)
					nearstar_p_type[n] = random (10);
		}
	}

	for (n=0; n<nearstar_nop; n++) {
		switch (nearstar_p_type[n]) {
			case 0:
				if (random(8))
					nearstar_p_type[n] ++;
				break;
			case 3:
				if ((n<2)||(n>6)||(nearstar_class&&random(4))) {
					if (random(2))
						nearstar_p_type[n]++;
					else
						nearstar_p_type[n]--;
				}
				break;
			case 7:
				if (n<7) {
					if (random(2))
						nearstar_p_type[n] --;
					else
						nearstar_p_type[n] -= 2;
				}
				break;
		}
	}

	nearstar_nob = nearstar_nop;

	if (nearstar_class==2||nearstar_class==7||nearstar_class==15)
		goto no_moons;

	for (n=0; n<nearstar_nop; n++) {
		// (t=) Numero di satelliti per pianeta.
		s = nearstar_p_type[n];
		if (n < 2) {
			t = 0;
			if (s == 10)
				t = random (3);
		}
		else
			t = random (planet_possiblemoons[s] + 1);
		if (nearstar_nob + t > maxbodies)
			t = maxbodies - nearstar_nob;
		for (c=0; c<t; c++) {
			q 			 = nearstar_nob + c;
			nearstar_p_owner[q]	 = n;
			nearstar_p_moonid[q]	 = c;
			nearstar_p_orb_orient[q] = (double) deg * (double) random (360);
			nearstar_p_orb_seed[q]   = (c*c+4) * nearstar_p_ray[n] + (float) zrandom (300 * nearstar_p_ray[n]) / 100;
			nearstar_p_tilt[q]       = zrandom (10*nearstar_p_orb_seed[q]) / 50;
			nearstar_p_orb_tilt[q]   = zrandom (10*nearstar_p_orb_seed[q]) / 500;
			nearstar_p_orb_ecc[q]    = 1 - (double) random (nearstar_p_orb_seed[q] + 10*fabs(nearstar_p_orb_tilt[q])) / 2000;
			nearstar_p_ray[q]        = (double) random (nearstar_p_orb_seed[n]) * 0.05 + 0.1;
			nearstar_p_ring[q]	 = 0;
			nearstar_p_type[q]       = random (planet_types);
			r = nearstar_p_type[q];
			if (r==9 && s != 10) r = 2;
			if (r==6 && s < 9) r = 5;
			if (n > 7 && random(c)) r = 7;
			if (n > 9 && random(c)) r = 7;
			if (r==2 || r==3 || r==4 || r==8) {
				if (s != 6 && s < 9)
					r = 1;
			}
			if (r==3 && s < 9) {
				if (n > 7)
					r = 7;
				if (nearstar_class && random(4))
					r = 5;
				if (nearstar_class == 2 ||
				    nearstar_class == 7 ||
				    nearstar_class == 11)
					r = 8;
			}
			if (r==7 && n <= 5) r = 1;
			if ((nearstar_class==2||nearstar_class==5||
			     nearstar_class==7||nearstar_class==11)
			     && random(n)) r = 7;
			nearstar_p_type[q] = r;
		}
		nearstar_nob += t;
	}

no_moons:
	key_radius = nearstar_ray * planet_orb_scaling;
	if (nearstar_class == 8) key_radius *= 2;
	if (nearstar_class == 2) key_radius *= 16;
	if (nearstar_class == 7) key_radius *= 18;
	if (nearstar_class == 11) key_radius *= 20;
	for (n=0; n<nearstar_nop; n++) {
		nearstar_p_ray[n] = avg_planet_ray[nearstar_p_type[n]]
				  + avg_planet_ray[nearstar_p_type[n]] * zrandom (100) / 200;
		nearstar_p_ray[n] *= avg_planet_sizing;
		nearstar_p_orb_ray[n] = key_radius + key_radius * zrandom (100) / 500;
		nearstar_p_orb_ray[n] += key_radius * avg_planet_ray[nearstar_p_type[n]];
		if (n < 8)
			key_radius += nearstar_p_orb_ray[n];
		else
			key_radius += 0.22 * nearstar_p_orb_ray[n];
	}

	n = nearstar_nop;
	while (n < nearstar_nob) {
		q = 0;
		c = nearstar_p_owner[n];
		key_radius = nearstar_p_ray[c] * moon_orb_scaling;
		while (n<nearstar_nob && nearstar_p_owner[n] == c) {
			nearstar_p_ray[n] = avg_planet_ray[nearstar_p_type[n]]
					  + avg_planet_ray[nearstar_p_type[n]] * zrandom (100) / 200;
			nearstar_p_ray[n] *= avg_moon_sizing;
			nearstar_p_orb_ray[n] = key_radius + key_radius * zrandom (100) / 250;
			nearstar_p_orb_ray[n] += key_radius * avg_planet_ray[nearstar_p_type[n]];
			if (q < 2) key_radius += nearstar_p_orb_ray[n];
			if (q >= 2 && q < 8) key_radius += 0.12 * nearstar_p_orb_ray[n];
			if (q >= 8) key_radius += 0.025 * nearstar_p_orb_ray[n];
			q++;
			n++;
		}
	}

	for (n = 0; n < nearstar_nop; n++) {
		nearstar_p_ring[n] = 0.75 * nearstar_p_ray[n] * (2 + random(3));
		s = nearstar_p_type[n];
		if (s != 6 && s != 9) {
			if (random(5))
				nearstar_p_ring[n] = 0;
		}
		else {
			if (random(2))
				nearstar_p_ring[n] = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

long labposit[80];
char labprogr[80];

long labposit2[80];
char labprogr2[80];

long fpart (double id, char *label)
{
	long	fpart, no;
	
	int	attempts = 0;
	double	multiplier = 1E9;

	if (label[21] == 'P') {
		no  = label[23] - '0';
		no += 10 * (label[22] - '0');
		id -= no;
	}

	id = fabs(id);

	do {
		
		double lid = floor(id);
		fpart = multiplier * ((double)id - (double)lid);
		multiplier *= 1000;
		attempts++;
	}while (fpart < 1E6 && attempts < 44) ; 
	
	return (fpart);
}

long notesabout (double id)
{
	long ncount = 0;
	if (gh == -1) return (0);

	lseek (gh, 4, SEEK_SET);
	while (_read (gh, &mblock_id, 8) && _read (gh, &mblock_message, 76) == 76) {
		if (memcmp (&mblock_id, "Removed:", 8)) {
			if (mblock_id >= id - idscale && mblock_id <= id + idscale)
				ncount++;
		}
	}

	return (ncount);
}

void listplanets ()
{
	int	n, n2, b1, b2, b3;
	long	b4, ncount;

	int 	planet_nr, pcount;
	int	planet_id, mcount;

	char 	least = 0;

	double 	star_id;
	long	fp_star_check;
	long	fp_object_check;

	if (query == 2) {
		planet_nr  = object_label[23] - '0';
		planet_nr += 10 * (object_label[22] - '0');
		star_id    = object_id - planet_nr;
	}
	else
		star_id = object_id;

	if (!isthere (star_id)) {
		msg (subjectname);
		msg ("IS OUT OF RANGE");
		return;
	}
	else
		fp_star_check = fpart (star_id, "F-PARTTESTSTARLABEL Sxx");



	warn ("ORGANIZING TREE...", 1);

	ap_target_x = laststar_x;
	ap_target_y = laststar_y;
	ap_target_z = laststar_z;
	extract_ap_target_infos ();
	prepare_nearstar ();

	
	sprintf (textbuffer, "*%s", subjectname);
	msg (textbuffer);

	if (query == 1) {
		ncount = notesabout (star_id);
		if (ncount > 0) {
			sprintf (textbuffer, "]   (%ld NOTES)", ncount);
			msg (textbuffer);
		}
		pcount = 0;
		lseek (fh, 4, SEEK_SET);
		while (_read (fh, &object_id, 8) && _read (fh, &object_label, 24) == 24) {
			fp_object_check = fpart (object_id, object_label);
			if (memcmp (&object_id, "Removed:", 8) && fp_object_check == fp_star_check && object_id > star_id + 1 - idscale && object_id <= star_id + MAX_OBJECTS_PER_STAR + idscale) {
			
			planet_nr  = object_label[23] - '0';
			planet_nr += 10 * (object_label[22] - '0');
			if (nearstar_p_owner[planet_nr - 1] == -1) {
				labposit[pcount] = lseek(fh, 0, SEEK_CUR) - 32;
				labprogr[pcount] = planet_nr;
				least = 1;
				pcount++;
			}}
		}
		do {
			b1 = 0;
			b2 = 0;
			while (b2 < pcount - 1) {
				if (labprogr[b2] > labprogr[b2+1]) {
					b4 = labposit[b2];
					labposit[b2] = labposit[b2+1];
					labposit[b2+1] = b4;
					b3 = labprogr[b2];
					labprogr[b2] = labprogr[b2+1];
					labprogr[b2+1] = b3;
					b1 = 1;
				}
				b2++;
			}
		} while (b1);
		n = 0;
		while (pcount) {
			lseek (fh, labposit[n], SEEK_SET);
			_read (fh, &object_id, 8);
			_read (fh, &object_label, 24);
			planet_nr  = object_label[23] - '0';
			planet_nr += 10 * (object_label[22] - '0');
			pcount--;
			if (pcount) {
				sprintf (textbuffer, "$%02d&%s", planet_nr, object_label);
				msg (textbuffer);
				ncount = notesabout (object_id);
				if (ncount > 0) {
					sprintf (textbuffer, "]   (%ld NOTES)", ncount);
					msg (textbuffer);
				}
			}
			else {
				sprintf (textbuffer, "[%02d&%s", planet_nr, object_label);
				msg (textbuffer);
				ncount = notesabout (object_id);
				if (ncount > 0) {
					sprintf (textbuffer, "    (%ld NOTES)", ncount);
					msg (textbuffer);
				}
			}
			planet_id  = object_label[23] - '0';
			planet_id += 10 * (object_label[22] - '0');
			mcount = 0;
			lseek (fh, 4, SEEK_SET);
			while (_read (fh, &object_id, 8) && _read (fh, &object_label, 24) == 24) {
				fp_object_check = fpart (object_id, object_label);
				if (memcmp (&object_id, "Removed:", 8) && fp_object_check == fp_star_check && object_id > star_id + 1 - idscale && object_id <= star_id + MAX_OBJECTS_PER_STAR + idscale) {
				planet_nr  = object_label[23] - '0';
				planet_nr += 10 * (object_label[22] - '0');
				if (nearstar_p_owner[planet_nr - 1] == planet_id - 1) {
					labposit2[mcount] = lseek(fh, 0, SEEK_CUR) - 32;
					labprogr2[mcount] = nearstar_p_moonid[planet_nr - 1];
					mcount++;
				}}
			}
			do {
				b1 = 0;
				b2 = 0;
				while (b2 < mcount - 1) {
					if (labprogr2[b2] > labprogr2[b2+1]) {
						b4 = labposit2[b2];
						labposit2[b2] = labposit2[b2+1];
						labposit2[b2+1] = b4;
						b3 = labprogr2[b2];
						labprogr2[b2] = labprogr2[b2+1];
						labprogr2[b2+1] = b3;
						b1 = 1;
					}
					b2++;
				}
			} while (b1);
			n2 = 0;
			while (mcount) {
				lseek (fh, labposit2[n2], SEEK_SET);
				_read (fh, &object_id, 8);
				_read (fh, &object_label, 24);
				planet_nr  = object_label[23] - '0';
				planet_nr += 10 * (object_label[22] - '0');
				mcount--;
				if (pcount) {
					if (mcount) {
						sprintf (textbuffer, "] $%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
						msg (textbuffer);
						ncount = notesabout (object_id);
						if (ncount > 0) {
							sprintf (textbuffer, "] ]   (%ld NOTES)", ncount);
							msg (textbuffer);
						}
					}
					else {
						sprintf (textbuffer, "] [%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
						msg (textbuffer);
						ncount = notesabout (object_id);
						if (ncount > 0) {
							sprintf (textbuffer, "]     (%ld NOTES)", ncount);
							msg (textbuffer);
						}
					}
				}
				else {
					if (mcount) {
						sprintf (textbuffer, "  $%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
						msg (textbuffer);
						ncount = notesabout (object_id);
						if (ncount > 0) {
							sprintf (textbuffer, "  ]   (%ld NOTES)", ncount);
							msg (textbuffer);
						}
					}
					else {
						sprintf (textbuffer, "  [%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
						msg (textbuffer);
						ncount = notesabout (object_id);
						if (ncount > 0) {
							sprintf (textbuffer, "      (%ld NOTES)", ncount);
							msg (textbuffer);
						}
					}
				}
				n2++;
			}
			n++;
		}
		if (least) {
			msg (divider);
			msg ("PLANETS LISTING END.");
		}
		else
			msg ("NO KNOWN PLANETS.");
	}

	if (query == 2) {
		ncount = notesabout (object_id);
		if (ncount > 0) {
			sprintf (textbuffer, "]   (%ld NOTES)", ncount);
			msg (textbuffer);
		}
		planet_id  = object_label[23] - '0';
		planet_id += 10 * (object_label[22] - '0');
		mcount = 0;
		lseek (fh, 4, SEEK_SET);
		while (_read (fh, &object_id, 8) && _read (fh, &object_label, 24) == 24) {
			fp_object_check = fpart (object_id, object_label);
			if (memcmp (&object_id, "Removed:", 8) && fp_object_check == fp_star_check && object_id > star_id + 1 - idscale && object_id <= star_id + MAX_OBJECTS_PER_STAR + idscale) {
			planet_nr  = object_label[23] - '0';
			planet_nr += 10 * (object_label[22] - '0');
			if (nearstar_p_owner[planet_nr - 1] == planet_id - 1) {
				labposit2[mcount] = lseek(fh, 0, SEEK_CUR) - 32;
				labprogr2[mcount] = nearstar_p_moonid[planet_nr - 1];
				least = 1;
				mcount++;
			}}
		}
		do {
			b1 = 0;
			b2 = 0;
			while (b2 < mcount - 1) {
				if (labprogr2[b2] > labprogr2[b2+1]) {
					b4 = labposit2[b2];
					labposit2[b2] = labposit2[b2+1];
					labposit2[b2+1] = b4;
					b3 = labprogr2[b2];
					labprogr2[b2] = labprogr2[b2+1];
					labprogr2[b2+1] = b3;
					b1 = 1;
				}
				b2++;
			}
		} while (b1);
		n2 = 0;
		while (mcount) {
			lseek (fh, labposit2[n2], SEEK_SET);
			_read (fh, &object_id, 8);
			_read (fh, &object_label, 24);
			planet_nr  = object_label[23] - '0';
			planet_nr += 10 * (object_label[22] - '0');
			mcount--;
			if (mcount) {
				sprintf (textbuffer, "$%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
				msg (textbuffer);
				ncount = notesabout (object_id);
				if (ncount > 0) {
					sprintf (textbuffer, "]   (%ld NOTES)", ncount);
					msg (textbuffer);
				}
			}
			else {
				sprintf (textbuffer, "[%02d&%s", nearstar_p_moonid[planet_nr - 1] + 1, object_label);
				msg (textbuffer);
				ncount = notesabout (object_id);
				if (ncount > 0) {
					sprintf (textbuffer, "    (%ld NOTES)", ncount);
					msg (textbuffer);
				}
			}
			n2++;
		}
		if (least) {
			msg (divider);
			msg ("MOONS LISTING END.");
		}
		else
			msg ("NO KNOWN MOONS.");
	}
}

double ap_target_id = 12345;

int main(int argc, char *argv[])
{
	unfreeze ();
	if (_argc<2 && ap_targetted != 1) {
		msg ("________USAGE________");
		msg ("DL OBJECTNAME");
		msg ("DL OBJECTNAME:RANGE");
		msg ("^^^^^^^^^^^^^^^^^^^^^");
		msg ("PLEASE RUN AGAIN,");
		msg ("SPECIFYING PARAMETERS");
		msg (divider);
		msg ("(DL REVISION 6011/29)");
		return;
	}
	else {
		msg ("DEPENDENCIES LISTING:");
		msg (divider);
	}

	gh = openGuide();

	fh = openStarmap();
	if (fh == -1) {
		msg ("STARMAP NOT AVAILABLE");
		return;
	}

	if (_argc<2) {
		ap_target_id = ap_target_x / 100000 * ap_target_y / 100000 * ap_target_z / 100000;
		lseek (fh, 4, SEEK_SET);
		while (_read (fh, &object_id, 8) && _read (fh, &object_label, 24) == 24) {
			if (object_id > ap_target_id - idscale && object_id < ap_target_id + idscale) {
				object_label[20] = 0;
				strcpy (objectname, object_label);
				analyzed_sectors_range = 100;
				goto no_arguments;
			}
		}
		msg ("CURRENT REMOTE TARGET");
		msg ("CANNOT BE FOUND.");
		return;
	}

	i = 2;
	strcpy (parbuffer, _argv[1]);
	while (i < _argc) {
		strcat (parbuffer, " ");
		strcat (parbuffer, _argv[i]);
		i++;
	}

	i = 0;
	while (parbuffer[i]) {
		if (parbuffer[i] == '_')
			parbuffer[i] = 32;
		i++;
	}

	i = 0;
	while (i < 21 && parbuffer[i] != ':' && parbuffer[i] != 0) {
		objectname[i] = parbuffer[i];
		i++;
	}

	if (parbuffer[i] != ':')
		analyzed_sectors_range = 100;
	else {
		sts = atoi (parbuffer + i + 1);
		if (sts <= 2 || sts > 10000)
			analyzed_sectors_range = 100;
		else
			analyzed_sectors_range = sts;
	}

	if (sts > 100) warn ("SCANNING THE GALAXY: ESC TO STOP", -1);

	strupr (parbuffer);
	strupr (objectname);
	objectname[i] = 0;

    no_arguments:
	query = find (objectname);
	if (query) {
		if (query==1) msg ("SUBJECT: STAR;");
		if (query==2) msg ("SUBJECT: PLANET;");
		sprintf (outbuffer, "NAME: %s", subjectname);
		msg (outbuffer);
		msg ("_____________________");
		listplanets ();
	}

	if (gh != -1) _close (gh);
	_close (fh);
}