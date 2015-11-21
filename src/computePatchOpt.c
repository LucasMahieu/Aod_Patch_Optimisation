/*
 *
 * ======================================================================
 *
 *       Filename:  comuptePatchOpt.c
 *
 *    Description: Programme qui va trouver le Patch de coût minimum 
 *
 *        Version:  1.0
 *        Created:  17.11.2015 13:43:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lucas MAHIEU (LM), lucas.mahieu@phelma.grenoble-inp.fr
 *        Company:  Grenoble_INP - 1A_PHELMA - 2A_ENSIMAG
 *
 * ======================================================================
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

int computePatchOpt(int, int);

typedef struct {
	int cout;
	char* cmd;
	int pereI;
	int pereJ;
} cellule;

cellule** mem = NULL;
FILE* fA;
FILE* fB;


int computePatchOpt_it(int n, int m)
{
	struct timeval tempsD, tempsF;
	if( gettimeofday(&tempsD,NULL)){
		printf("erreur GetTimeOfDay !!! ");
		return -1;
	}
	int i=0, j=0;
	int L=0;
	int min = 0, cout=0;
	//char* cmdPatch = NULL;
	char* toPrint = (char*)calloc(1,sizeof(*toPrint));
	char* tmpA = NULL;
	size_t lA = 0;
	char* tmpB = NULL;
	size_t lB = 0;
	ssize_t lAlen=0, lBlen=0;
	int cd = 10, cD=15, ca=0, cs=0;
	int pi=0,pj=0;
	int k=0, iD=0;
	int add=0, sub=0, del=0, Del=0;
	rewind(fB);
	for(j = 0; j <=m; j++) {
		if (j!=0){
			lBlen = getline(&tmpB, &lB, fB);
		}
		rewind(fA);
		iD=0;
		for(i = 0; i <=n; i++) {
			if(i!=0){
				lAlen = getline(&tmpA, &lA, fA);
			}
			L=0;
			if (i==0) {
				if (j==0) {
					mem[0][0].cout = 0;
				}
			//i=0 et j!=0 f(0,j) = sum(10+LkB) avec k=1..j
				else{
					rewind(fB);
					for (k=1; k<=j; k++) {
						lBlen = getline(&tmpB, &lB, fB);
						L += 10 + lBlen;
						asprintf(&toPrint,"+ %d\n%s",i,tmpB);
						pi = i;
						pj = j-1;
					}
					mem[0][j].cout = L; 
					mem[0][j].cmd = (char*)calloc(strlen(toPrint)+1,sizeof(char));
					strcpy(mem[0][j].cmd, toPrint);
					mem[0][j].pereI = pi; 
					mem[0][j].pereJ = pj; 
				}
			} 
	// i!=0 
			else {
			//Pour tout j=0, on Delete
				if (j==0) {
					if (i==1) {
						mem[i][0].cout = 10;
						asprintf(&toPrint,"d %d\n",i);
						pi = i-1;
						pj = j;
					}
				// j=0 et i!=1
					else{
						mem[i][0].cout = 15;
						asprintf(&toPrint,"D %d %d\n",1,i);
						pi = i-i;
						pj = j;
					}
					mem[i][0].cmd = (char*)calloc(strlen(toPrint)+1,sizeof(char));
					strcpy(mem[i][0].cmd, toPrint);
					mem[i][0].pereI = pi; 
					mem[i][0].pereJ = pj; 
				}
			// i!=0 et j!=0
				else {
					L = lBlen;
					if (lBlen == lAlen){
						if(strcmp(tmpA,tmpB) == 0){
							//Meme taille et meme chaine 
							cs = 0;
						}
						else{
							//Meme taille mais différents
							cs = 10 + L;
						}
					}
					else {
						//Taille différentes donc différent
						cs = 10 + L;
					}
					ca = 10 + L;

					add = mem[i][j-1].cout + ca;
					sub = mem[i-1][j-1].cout + cs;
					del = mem[i-1][j].cout + cd;
					Del = mem[iD][j].cout + cD;

					min = sub;
					if(cs == 0){
						asprintf(&toPrint,"");
						pi = i-1;
						pj = j-1;
					}
					else{
						asprintf(&toPrint,"= %d\n%s",i,tmpB);
						pi = i-1;
						pj = j-1;
					}
					if( add<=min ){
						min = add;
						asprintf(&toPrint,"+ %d\n%s",i,tmpB);
						pi = i;
						pj = j-1;

					}
					if( del<min ){
						min = del;
						asprintf(&toPrint,"d %d\n",i);
						pi = i-1;
						pj = j;
					}
					if( Del<min ){
						min = Del;
						asprintf(&toPrint,"D %d %d\n",iD+1,i-iD);
						pi = iD;
						pj = j;
					}
					mem[i][j].cout = min;
					mem[i][j].cmd = (char*)calloc(strlen(toPrint)+1,sizeof(char));
					strcpy(mem[i][j].cmd, toPrint);
					mem[i][j].pereI = pi;
					mem[i][j].pereJ = pj;
					if(min<=mem[iD][j].cout){
						iD=i;
					}
				}
			}
		}
	}
	i = i-1;
	j = j-1;
	cout = mem[i][j].cout;
	int prevPi=n, prevPj=m;
	int futurPi=-1, futurPj=-1;

	do{
		prevPi = mem[i][j].pereI;
		prevPj = mem[i][j].pereJ;
		mem[i][j].pereI = futurPi;
		mem[i][j].pereJ = futurPj;
		futurPi = i;
		futurPj = j;
		i = prevPi;
		j = prevPj;
	}while(futurPi>0 || futurPj>0);
	//free(tmpA);
	//free(tmpB);
	//if (toPrint){
		//free(toPrint);
	//}
	if( gettimeofday(&tempsF, NULL) ){
		printf(" erreur getTimeOfDay fin !! ");
	}
	else{
		suseconds_t timeE = tempsF.tv_sec*1000000 + tempsF.tv_usec;
		suseconds_t timeB = tempsD.tv_sec*1000000 + tempsD.tv_usec;
		int sec =(int)((timeE - timeB)/1000000);
		int usec =(int)((timeE - timeB)-1000000*sec);
		int min = 0;
		if (sec>=60){
			min = sec/60;
			sec = sec%60;
		}
		printf("#############################################################\n");
		printf("-------------- Patch Optimal généré en ----------------------\n");
		printf("-------------- %02dmin %02dsec %06dus -------------------------\n",min,sec,usec);
	}
	return cout;
}

int main ( int argc, char* argv[] )
{
	if( argc!=4 ){
		printf(" !!!! lancer: computepatchOpt source target patch_à_creer !!!!!\n");
		return EXIT_FAILURE;
	}
	printf("#############################################################\n");
	printf("#################  GENERATEUR DE PATCH OPTIMAL ##############\n");
	printf("#############################################################\n");
	printf("-------------- Entrée du Patch : %s\n",argv[1]);
	printf("-------------- Sortie du Patch : %s\n",argv[2]);
	int n = 0;
	int m = 0;
	int i = 0;
	int j = 0;

	struct timeval tempsDmain;
	struct timeval tempsFmain;
	if( gettimeofday(&tempsDmain,NULL)){
		printf("erreur GetTimeOfDay !!! ");
		return -1;
	}

	if((fA = fopen(argv[1], "r")) == NULL){
		printf("Erreur fopen f1");
		return -1;
	}
	if((fB = fopen(argv[2], "r")) == NULL){
		printf("Erreur fopen f2");
		return -1;
	}

	char tmp[500];
	// On calcule le nombre de lignes de A
	while (fgets(tmp,500, fA)!=NULL) {
		n++;
	}
	// On calcule le nombre de lignes de B
	while (fgets(tmp, 500, fB)!= NULL) {
		m++;
	}
	// On creer le taleau de memorisation
	if( (mem =(cellule**)calloc(n+1, sizeof(*mem))) == NULL) {
		printf("Erreur calloc");
		return -1;
	}
	for (i = 0; i < n+1; i++) {
		if( (mem[i] = (cellule*)calloc(m+1, sizeof(**mem))) == NULL) {
			printf("Erreur calloc");
			return -1;
		}
		for (j = 0;  j< m+1; j++) {
			mem[i][j].cout = 0;
			mem[i][j].cmd = (char*)calloc(1,sizeof(char));
			mem[i][j].pereI = 0;
			mem[i][j].pereJ = 0;
		}
	}

	printf("-------------- Coût du Patch :  %d \n",computePatchOpt_it(n,m));
	printf("#############################################################\n");
	printf("-------------- Patch en écriture veuillez patienter\n");

	FILE* p = NULL;
	p = fopen(argv[3],"w");
	int l=0, c=0,lTmp=l, cTmp=c;
	int compteur = 0;
	do {
		if (!(l==0 && c==0)){
			fputs(mem[l][c].cmd,p);
		}
		if(compteur>=m/100){
			compteur = 0;
			printf("#");
		}
		else{
		compteur++;
		}
		l = mem[lTmp][cTmp].pereI;
		c = mem[lTmp][cTmp].pereJ;
		cTmp = c;
		lTmp = l;
	}while(l>-1 && c>-1);
	printf(" 100%%\n");
	printf("-------------- Patch écrit dans '%s'\n",argv[3]);

	for (i = 1;  i<n+1; i++) {
		for (j = 1;  j<m+1; j++) {
			free(mem[i][j].cmd);
		}
		free(mem[i]);
	}
	free(mem);
	fclose(p);
	fclose(fA);
	fclose(fB);
	if( gettimeofday(&tempsFmain, NULL) ){
		printf(" erreur getTimeOfDay fin !! ");
	}
	else{
		suseconds_t timeFmain = tempsFmain.tv_sec*1000000 + tempsFmain.tv_usec;
		suseconds_t timeDmain = tempsDmain.tv_sec*1000000 + tempsDmain.tv_usec;
		int sec =(int)((timeFmain - timeDmain)/1000000);
		int usec =(int)((timeFmain - timeDmain)-1000000*sec);
		int min = 0;
		if (sec>=60){
			min = sec/60;
			sec = sec%60;
		}
		printf("#############################################################\n");
		printf("--------------- Temps total de l'opération ------------------\n");
		printf("--------------- %02dmin %02dsec %06dus ------------------------\n",min,sec,usec);
		printf("#############################################################\n");
	}
	return EXIT_SUCCESS;
}/* ----------  end of function main  ---------- */
