#include "libc/runtime/runtime.h"

char   *situation_file  = "..//data//CURRENT.BIN";
char   *situation_file_cwd  = "CURRENT.BIN";
char	*file = "..//data//STARMAP.BIN";
char    *file_cwd = "STARMAP.BIN";
char	*guide = "..//data//GUIDE.BIN";
char	*guide_cwd = "GUIDE.BIN";
char    *comm = "..//data//COMM.BIN";
char    *comm_cwd = "COMM.BIN";

int openStarmap();
int openGuide();
int createComm();
int openSituation();

int openStarmap()
{
    char *pth = getenv("NOCTIS_DATA_DIR");
    int fh = -1;
    if (pth != NULL) {
        int pth_len = strlen(pth);
        int file_len = strlen(file_cwd);
        char* pth2 = malloc((pth_len + 2 + file_len) * sizeof(pth));
        strcpy(pth2, strdup(pth));
        pth2[pth_len] = '/';
        for (int i = 0; i < file_len; i++) {
            pth2[pth_len+1+i] = file_cwd[i];
        }
        pth2[pth_len+1+file_len] = 0;
        fh = open (pth2, O_RDONLY);
        free(pth2);
    }
    if (fh == -1) {
        fh = open (file_cwd, O_RDONLY);
    }
	if (fh == -1) {
		fh = open (file, O_RDONLY);
	}
    return fh;
}

int openGuide()
{
    char *pth = getenv("NOCTIS_DATA_DIR");
    int gh = -1;
    if (pth != NULL) {
        int pth_len = strlen(pth);
        int guide_len = strlen(guide_cwd);
        char* pth2 = malloc((pth_len + 2 + guide_len) * sizeof(pth));
        strcpy(pth2, strdup(pth));
        pth2[pth_len] = '/';
        for (int i = 0; i < guide_len; i++) {
            pth2[pth_len+1+i] = guide_cwd[i];
        }
        pth2[pth_len+1+guide_len] = 0;
        gh = open (pth2, O_RDONLY);
        free(pth2);
    }
    if (gh == -1) {
        gh = open (guide_cwd, O_RDONLY);
    }
	if (gh == -1) {
		gh = open (guide, O_RDONLY);
	}
    return gh;
}

int openSituation()
{
    char *pth = getenv("NOCTIS_DATA_DIR");
    int sh = -1;
    if (pth != NULL) {
        int pth_len = strlen(pth);
        int situation_file_len = strlen(situation_file_cwd);
        char* pth2 = malloc((pth_len + 2 + situation_file_len) * sizeof(pth));
        strcpy(pth2, strdup(pth));
        pth2[pth_len] = '/';
        for (int i = 0; i < situation_file_len; i++) {
            pth2[pth_len+1+i] = situation_file_cwd[i];
        }
        pth2[pth_len+1+situation_file_len] = 0;
        sh = open (pth2, O_RDONLY);
        free(pth2);
    }
    if (sh == -1) {
        sh = open (situation_file_cwd, O_RDONLY);
    }
	if (sh == -1) {
		sh = open (situation_file, O_RDONLY);
	}
    return sh;
}


int createComm()
{
    char *pth = getenv("NOCTIS_DATA_DIR");
    int ch = -1;
    if (pth != NULL) {
        int pth_len = strlen(pth);
        int comm_len = strlen(comm_cwd);
        char* pth2 = malloc((pth_len + 2 + comm_len) * sizeof(pth));
        strcpy(pth2, strdup(pth));
        pth2[pth_len] = '/';
        for (int i = 0; i < comm_len; i++) {
            pth2[pth_len+1+i] = comm_cwd[i];
        }
        pth2[pth_len+1+comm_len] = 0;
        ch = creat (pth2, 0644);
        free(pth2);
    }
    if (ch == -1) {
        ch = creat (comm_cwd, 0644);
    }
	if (ch == -1) {
		ch = creat (comm, 0644);
	}
    return ch;
}