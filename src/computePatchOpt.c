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

int computePatchOpt(int, int);
int min_i_k(int*, int, int);

typedef struct {
	int cout;
	char cmd[100];
	int pereI;
	int pereJ;
} cellule;

cellule** mem = NULL;
FILE* fA;
FILE* fB;

int min_i_k(int* kD,int i, int j){
	int min = mem[i-1][j].cout;
	int tmp = 0;
	int k=2;
	*kD = 1;
		while((i-k)>=0){
		tmp = mem[i-k][j].cout; 
		if (tmp < min){
			min = tmp;
			*kD = k;
		}
		k++;
	}
	return min;
}
/*
int computePatchOpt (int i, int j)
{
    rewind(fA);
    rewind(fB);
	int ca=0,cs=0,cd=0,cD=0;
	int L = 0;
    int k = 0;
	int minD = 0;
	int min = 0;
	int add=0, del=0,sub=0;
	char tmpB[100];
	char tmpA[100];
	char toPrint[100];
	char cmdPatch[100] = "";
	int kD = 0;
	if ( memo[i][j] == INT_MAX){
		if (i==0 && j==0) {
			memo[i][j] = 0;
		}
		else if (i==0){
			for (k=1; k<=j; k++) {
				fgets(tmpB, 100, fB);
				L += 10 + strlen(tmpB);
			}
			memo[i][j] = L; 
		}
		else if ( j == 0 ) {
			if( i == 1 ) {
				memo[i][j] = 10;
			}
			else {
				memo[i][j] = 15;
			}
		}
		else {
			for ( k=0; k<i; k++) {
				fgets(tmpA, 100, fA);
			}
			for ( k=0; k<j; k++) {
				fgets(tmpB, 100, fB);
			}
			if ( strcmp( tmpA, tmpB ) == 0) {
				cs = 0;
			}
			else {
				L = strlen(tmpB);
				cs = 10 + L;
			}
			ca = 10 + L;
			cd = 10;
			cD = 15;
			add = computePatchOpt(i,j-1) + ca;
			sub = computePatchOpt(i-1,j-1) + cs;
			del = computePatchOpt(i-1,j) + cd;
			minD = min_i_k( &kD, i, j) + cD ;

			min = add;
			sprintf(toPrint,"+ %d\n%s",i,tmpA);
			strcpy(cmdPatch, toPrint);
			opt_choiceI[i][j] = i;
			opt_choiceJ[i][j] = j-1; 
			if (sub<=min) {
				min = sub;
				if (cs==0) {
					sprintf(toPrint,"");
					strcpy(cmdPatch, toPrint);
					opt_choiceI[i][j] = i-1; 
					opt_choiceJ[i][j] = j-1; 
				}
				else{
					sprintf(toPrint,"= %d\n%s",i,tmpB);
					strcpy(cmdPatch, toPrint);
					opt_choiceI[i][j] = i-1; 
					opt_choiceJ[i][j] = j-1; 
				}
			}
			if (del<min) {
				min = del;
				sprintf(toPrint,"d %d\n",i);
				strcpy(cmdPatch, toPrint);
				opt_choiceI[i][j] = i-1; 
				opt_choiceJ[i][j] = j; 
			}
			if (minD<min) {
				min = minD;
				sprintf(toPrint,"D %d %d\n",i,kD);
				strcpy(cmdPatch, toPrint);
				opt_choiceI[i][j] = i-kD; 
				opt_choiceJ[i][j] = j;
			}
			
			memo[i][j] = min;
		}
	}
	else{
		printf("%s",cmdPatch);
	}
	return memo[i][j];
}
*/



int computePatchOpt_it(int n, int m) {
	int i=0, j=0;
	int L=0;
	int min = 0;
	char cmdPatch[100] = {};
	char toPrint[100] = {};
	char tmpA[100] = {};
	char tmpB[100] = {};
	int cd = 10;
	int cD = 15;
	int ca = 0;
	int cs = 0;
	int pi=0,pj=0;
	int k=0, kD=0;
	int add=0, sub=0, del=0, Del=0;
	rewind(fA);
	for (i = 0; i <= n; i++) {
		if(i!=0){
			fgets(tmpA, 100, fA);
		}
		rewind(fB);
		for (j = 0; j <= m; j++) {
			L=0;
			if (j!=0){
				fgets(tmpB, 100, fB);
			}
			if (i==0) {
				if (j==0) {
					mem[0][0].cout = 0;
				}
			//i=0 et j!=0
				else{
					rewind(fB);
					for (k=1; k<=j; k++) {
						fgets(tmpB, 100, fB);
						L += 10 + strlen(tmpB);
						sprintf(toPrint,"+ %d\n%s",i,tmpB);
						strcpy(cmdPatch, toPrint);
						pi = i;
						pj = j-1;
					}
					mem[0][j].cout = L; 
					strcpy(mem[0][j].cmd, cmdPatch); 
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
						sprintf(toPrint,"d %d\n",i);
						strcpy(cmdPatch, toPrint);
						pi = i-1;
						pj = j;
					}
				// j=0 et i!=1
					else{
						mem[i][0].cout = 15;
						sprintf(toPrint,"D %d %d\n",i,i);
						strcpy(cmdPatch, toPrint);
						pi = i-i;
						pj = j;
					}
					strcpy(mem[i][0].cmd, cmdPatch); 
					mem[i][0].pereI = pi; 
					mem[i][0].pereJ = pj; 
				}
			// i!=0 et j!=0
				else {
					L = strlen(tmpB);
					if (strcmp(tmpA,tmpB) == 0) {
						cs = 0;
					}
					else {
						cs = 10 + L;
					}
					ca = 10 + L;

					add = mem[i][j-1].cout + ca;
					sub = mem[i-1][j-1].cout + cs;
					del = mem[i-1][j].cout + cd;
					Del = min_i_k( &kD, i, j) + cD;

					min = sub;
					if(cs == 0){
						sprintf(toPrint,"");
						strcpy(cmdPatch, toPrint);
						pi = i-1;
						pj = j-1;
					}
					else{
						sprintf(toPrint,"= %d\n%s",i,tmpB);
						strcpy(cmdPatch, toPrint);
						pi = i-1;
						pj = j-1;
					}
					if( add<=min ){
						min = add;
						sprintf(toPrint,"+ %d\n%s",i,tmpB);
						strcpy(cmdPatch, toPrint);
						pi = i;
						pj = j-1;

					}
					if( del<min ){
						min = del;
						sprintf(toPrint,"d %d\n",i);
						strcpy(cmdPatch, toPrint);
						pi = i-1;
						pj = j;
					}
					if( Del<min ){
						min = Del;
						sprintf(toPrint,"D %d %d\n",i-kD+1,kD);
						strcpy(cmdPatch, toPrint);
						pi = i-kD;
						pj = j;
					}
					mem[i][j].cout = min;
					strcpy(mem[i][j].cmd, cmdPatch);
					mem[i][j].pereI = pi;
					mem[i][j].pereJ = pj;
				}
			}
		}
	}
	return mem[i-1][j-1].cout;
}

int main ( int argc, char* argv[] ){

	int n = 0;
	int m = 0;
	int i = 0;
	int j = 0;
	
	if( argc!=3 ){
		printf(" !!!! lancer: computepatchOpt source target !!!!!\n");
		return EXIT_FAILURE;
	}

	if((fA = fopen(argv[1], "r")) == NULL){
		printf("Erreur fopen f1");
		return -1;
	}
	if((fB = fopen(argv[2], "r")) == NULL){
		printf("Erreur fopen f2");
		return -1;
	}
	char tmp[100];
	// On calcule le nombre de lignes de A
	while (fgets(tmp,100, fA)!=NULL) {
		n++;
	}
	// On calcule le nombre de lignes de B
	while (fgets(tmp, 100, fB)!= NULL) {
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
			mem[i][j].pereI = 0;
			mem[i][j].pereJ = 0;
		}
	}

	printf("Cout = %d\n",computePatchOpt_it(n,m));
	FILE* p = NULL;
	//FILE* pinv = NULL;
	p = fopen("patch01.txt","w");
	//pinv = fopen("pinv.txt","w");
	char** p_inv;
	p_inv = (char**)calloc(m+1,sizeof(*p_inv));
	for (j=0; j<m+1; j++){
		p_inv[j] = (char*)calloc(100,sizeof(char));
	}
	int l=n, c=m,lTmp=l, cTmp=c;
	int k=m;
	while(l!=0 || c!=0) {
		strcpy( p_inv[k], mem[l][c].cmd );
		//printf("%s", mem[l][c].cmd);
		l = mem[lTmp][cTmp].pereI;
		c = mem[lTmp][cTmp].pereJ;
		cTmp = c;
		lTmp = l;
		k--;
	}
	for (k=0; k<m+1; k++){
		fputs(p_inv[k],p);
	}


	for (j = 0;  j<n+1; j++) {
		free(mem[j]);
	}
	free(p_inv);
	free(mem);
	fclose(p);
	//fclose(pinv);
	fclose(fA);
	fclose(fB);
	return EXIT_SUCCESS;
}/* ----------  end of function main  ---------- */
