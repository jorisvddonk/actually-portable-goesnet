/*

	Noctis galactic guide / PAR command.
	GOES Net Module.

*/

#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/math.h"
#include "libc/calls/calls.h"
#include "libc/fmt/conv.h"
#include "libc/sysv/consts/o.h"
#include "compat.h"

//////////////////////////////////////////////////////////////////////////////

int	sts;
char	query;

int 	i, fh, gh;
int 	analyzed_sectors_range;

char	outbuffer[40];
char	textbuffer[40];
char	parbuffer[160];
char	nullbuffer[128];
char	objectname[21];
char	subjectname[21];

double	mblock_subject;
char	mblock_message[77];

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

double 	laststar_x, laststar_y, laststar_z;

void calc_parsis_for ()
{
	double 	star_id;
	int 	planet_nr;

	if (object_label[21] == 'S') {
		star_id = object_id;
		if (isthere (star_id)) {
			sprintf (textbuffer, "X=%1.0f", laststar_x);
			msg (textbuffer);
			sprintf (textbuffer, "Y=%1.0f", -laststar_y);
			msg (textbuffer);
			sprintf (textbuffer, "Z=%1.0f", laststar_z);
			msg (textbuffer);
		}
		else {
			msg (object_label);
			msg ("IS OUT OF RANGE");
		}
	}
	else {
		planet_nr  = object_label[23] - '0';
		planet_nr += 10 * (object_label[22] - '0');
		star_id    = object_id - planet_nr;
		if (isthere (star_id)) {
			sprintf (textbuffer, "X=%1.0f", laststar_x);
			msg (textbuffer);
			sprintf (textbuffer, "Y=%1.0f", -laststar_y);
			msg (textbuffer);
			sprintf (textbuffer, "Z=%1.0f", laststar_z);
			msg (textbuffer);
		}
		else {
			msg (object_label);
			msg ("IS OUT OF RANGE");
		}
	}
}

int main(int argc, char *argv[])
{
	if (_argc<2) {
		msg ("________USAGE________");
		msg ("PAR OBJECTNAME");
		msg ("PAR OBJECTNAME:RANGE");
		msg ("^^^^^^^^^^^^^^^^^^^^^");
		msg ("PLEASE RUN AGAIN,");
		msg ("SPECIFYING PARAMETERS");
		msg (divider);
		msg ("(PAR REVISION 6011/2)");
		return;
	}
	else {
		msg ("GOES STARMAP ANALYSIS");
		msg (divider);
	}

	fh = _open (file, O_RDONLY);
	if (fh == -1) {
		fh = _open (file_cwd, O_RDONLY);
		if (fh == -1) {
			msg ("STARMAP NOT AVAILABLE");
			return;
		}
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

	unfreeze ();
	if (sts > 100) warn ("SCANNING THE GALAXY: ESC TO STOP", -1);

	strupr (parbuffer);
	strupr (objectname);
	objectname[i] = 0;
	query = find (objectname);
	if (query) {
		if (query==1) msg ("SUBJECT: STAR;");
		if (query==2) msg ("SUBJECT: PLANET;");
		sprintf (outbuffer, "NAME: %s", subjectname);
		msg (outbuffer);
		calc_parsis_for ();
	}

	_close (fh);
}