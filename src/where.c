/*

	Noctis galactic guide / WHERE command.
	GOES Net Module.

    Modified for compilation with Cosmopolitan libc
    Requires that STARMAP.BIN is in ../data/ or in the current working directory.

    Compiling:
    cosmocc where.c -o where.com

    USAGE:
    ./where.com SURICRASIA
*/

#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/math.h"
#include "libc/calls/calls.h"
#include "libc/fmt/conv.h"
#include "libc/sysv/consts/o.h"
#include "compat.h"


int 	i, fh;

char	outbuffer[40];
char	textbuffer[40];
char	parbuffer[160];
char	nullbuffer[128];
char	objectname[21];
char	subjectname[21];
char	parentname[21];

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
				//
				if (object_label[21] == 'P') {
					subject_id -= (object_label[22]-'0') * 10;
					subject_id -= (object_label[23]-'0');
				}
				//
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

int main(int argc, char *argv[])
{
	char query;

	if (_argc<2) {
		msg ("________USAGE________");
		msg ("WHERE PLANETNAME");
		msg ("^^^^^^^^^^^^^^^^^^^^^");
		msg ("PLEASE RUN AGAIN,");
		msg ("SPECIFYING PARAMETERS");
		return;
	}
	else {
		msg ("  GOES GALACTIC MAP  ");
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

	strcpy (objectname, parbuffer);
	query = find (objectname);
	if (query) {
		if (query==1) {
			msg ("THIS OBJECT IS A STAR");
			msg ("AND ITS POSITION CAN");
			msg ("BE DETERMINED USING");
			msg ("THE 'PAR' MODULE.");
		}
		else {
			lseek (fh, 4, SEEK_SET);
			while (_read (fh, &s_object_id, 8) && _read (fh, &s_object_label, 24) == 24) {
				if (memcmp (&s_object_id, "Removed:", 8)) {
					if (s_object_id >= subject_id - idscale && s_object_id <= subject_id + idscale) {
						s_object_label[20] = 0;
						msg (subjectname);
						msg ("IS PART OF THE");
						msg (s_object_label);
						msg ("SYSTEM.");
						goto gotit;
					}
				}
			}
			msg ("UNABLE  TO  DETERMINE");
			msg ("THIS PLANET'S  PARENT");
			msg ("STAR;  PROBABLY, THAT");
			msg ("STAR ISN'T CATALOGUED");
		    gotit:
		}
	}

	_close (fh);
}