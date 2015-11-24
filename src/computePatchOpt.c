/**
 * \file computePatchOpt.c
 * \brief
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
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>



int computePatchOpt_it(int, int);
int pos_tab(int i, int j);
int CmpLine(char* p1, size_t pos1, char* p2, size_t pos2);
int GetLineA(char* p, size_t* pos);
int GetLineB(char* p, size_t* pos);
/**
 * \struct "cellule" qui contient pour chaque combinaison de ligne AiBj 
 * le cout min, 
 * le nom de la derniere cmd,
 *  et les coordonnées de la la cases qui à donné ce cout min.
 */
typedef struct {
	int cout;
	size_t l_cpy;
	int pereI;
	int pereJ;
} cellule;

/**
 * Variable globale: Pointeur qui servira à stocker toutes les cellules
 */
cellule* mem = NULL;
/**
 * Pointeur vers la zone mem fA
 */
int fA;
struct stat statsA;
int nb_ligne_A;
char* pA;
/**
 * Pointeur vers la zone mémoire de fB
 */
int fB;
char* pB;
struct stat statsB;
int nb_ligne_B;

/**
 * \fn int computePatchOpt_it (int n, int m)
 * Cette fonction permet de créer un patch de cout min = patch Optimal
 * qui transformera le fichier d'entré en le fichier de sortie
 *
 * \param[in] n nombre de ligne du fichier d'entrée
 * \param[in] m nombre de ligne du fichier de sortie
 * \return int cout du patch créer, ce cout est optimal
 */
int computePatchOpt_it(int n, int m)
{
	/*	struct timeval tempsD, tempsF;
		if( gettimeofday(&tempsD,NULL)){
		printf("erreur GetTimeOfDay !!! ");
		return -1;
		}
		*/
	//!Initialisation des variables
	//! -------------------------------
	int i=0, j=0;
	int min = 0, cout=0;
//	char* tmpA = NULL;
//	char* tmpB = NULL;
	size_t posB=0, posA=0;
	int lBlen=0, lAlen=0;
	int cd = 10, cD=15, ca=0, cs=0;
	int pi=0,pj=0;
	int iD=0;
	int add=0, sub=0, del=0, Del=0;
	int sumB=0;
	size_t ligneB=0, ligneA=0;
	//! -------------------------------
	for(j = 0; j <=m; j++) {
		if (j!=0) {
			ligneB=posB;
			lBlen = GetLineB(pB, &posB);
			sumB += 10 + lBlen;
		}
		iD=0;
		posA=0;
		lAlen=0;
		ligneA=0;
		for(i = 0; i <=n; i++) {
			if(i!=0){
				ligneA = posA;
				lAlen = GetLineA(pA, &posA);
			}
			// i=j=0	
			if (i==0) {
				if (j==0) {
					(*(mem+pos_tab(0,0))).cout = 0;
					(*(mem+pos_tab(0,0))).pereI = 0;
					(*(mem+pos_tab(0,0))).pereJ = 0;
				}
				//i=0 et j!=0  -> f(0,j) = sum(10+LkB) avec k=1..j, calculé à chaque getline
				else{
					pi = i;
					pj = j-1;
					//Maj du cout, de la cmd, et des peres pour retrouver le chemin
					(*(mem+pos_tab(0,j))).cout = sumB; 
					(*(mem+pos_tab(0,j))).l_cpy = ligneB;
					(*(mem+pos_tab(0,j))).pereI = pi; 
					(*(mem+pos_tab(0,j))).pereJ = pj; 
				}
			} 
			// i!=0 
			else {
				//Pour j=0, on Delete
				if (j==0) {
					// Si i=1, deletion coute 10
					if (i==1) {
						(*(mem+pos_tab(i,0))).cout = 10;
						pi = i-1;
						pj = j;
					}
					// j=0 et i!=1
					else{
						// Si i>1, deletion coute 15
						(*(mem+pos_tab(i,0))).cout = 15;
						pi = i-i;
						pj = j;
					}
					(*(mem+pos_tab(i,0))).pereI = pi; 
					(*(mem+pos_tab(i,0))).pereJ = pj; 
				}
				// i!=0 et j!=0
				else {
					// Pour comparer Li(A) et Lj(B) n regarde d'abords leur taille	
					if (lBlen == lAlen){
						if(CmpLine(pA,ligneA,pB,ligneB) == 1){
							//Même taille et même chaine 
							cs = 0;
						}
						else{
							//Même taille mais différents
							cs = 10 + lBlen;
						}
					}
					//Taille différentes donc différent
					else {
						cs = 10 + lBlen;
					}
					ca = 10 + lBlen;
					// Calcules du cout des 3 opérations possibles
					add = (*(mem+pos_tab(i,j-1))).cout + ca;
					sub = (*(mem+pos_tab(i-1,j-1))).cout + cs;
					del = (*(mem+pos_tab(i-1,j))).cout + cd;
					Del = (*(mem+pos_tab(iD,j))).cout + cD;
					// On selectionne l'opération de cout min
					// '= i\n%s' ou 'rien' si cs==0
					min = sub;
					pi = i-1;
					pj = j-1;
					// '+ i\n%s'
					if( add<=min ){
						min = add;
						pi = i;
						pj = j-1;
					}
					//'d i\n'
					if( del<min ){
						min = del;
						//if((ecrit = sprintf(toPrint,"d %d\n",i))==-1){
						pi = i-1;
						pj = j;
					}
					// 'D iD+1 i-iD\n'
					if( Del<min ){
						min = Del;
						//if((ecrit = sprintf(toPrint,"D %d %d\n",iD+1,i-iD))==-1){
						pi = iD;
						pj = j;
					}
					// Maj du cout, de la cmd, et de l'opération qui a donnée ce min
					(*(mem+pos_tab(i,j))).cout = min;
					(*(mem+pos_tab(i,j))).l_cpy = ligneB;
					(*(mem+pos_tab(i,j))).pereI = pi;
					(*(mem+pos_tab(i,j))).pereJ = pj;
					// On stock dans iD le min des cout de la colonne i
					if(min<=(*(mem+pos_tab(iD,j))).cout) {
						iD=i;
					}
				}
			}
		}
	}
	i = i-1;
	j = j-1;
	cout = (*(mem+pos_tab(i,j))).cout;
	int prevPi=n, prevPj=m;
	int futurPi=-1, futurPj=-1;

	do{
		prevPi = (*(mem+pos_tab(i,j))).pereI;
		prevPj = (*(mem+pos_tab(i,j))).pereJ;
		(*(mem+pos_tab(i,j))).pereI = futurPi;
		(*(mem+pos_tab(i,j))).pereJ = futurPj;
		futurPi = i;
		futurPj = j;
		i = prevPi;
		j = prevPj;
	}while(futurPi>0 || futurPj>0);
	// Cette zone commentée permet une bel affichage du résulta du programme 
	/*	if( gettimeofday(&tempsF, NULL) ){
		printf(" erreur getTimeOfDay fin !! ");
		}
		else{
		suseconds_t timeE = tempsF.tv_sec*1000000 + tempsF.tv_usec;
		suseconds_t timeB = tempsD.tv_sec*1000000 + tempsD.tv_usec;
		int sec =(int)((timeE - timeB)/1000000);
		int usec =(int)((timeE - timeB)-1000000*sec);
		int minutes = 0;
		if (sec>=60){
		min = sec/60;
		sec = sec%60;
		}
		printf("#############################################################\n");
		printf("-------------- Patch Optimal généré en ----------------------\n");
		printf("-------------- %02dminutes %02dsec %06dus -------------------------\n",min,sec,usec);
		}
		*/
//	free(tmpA);
//	free(tmpB);
	return cout;
}

/**
 * Avance la position du nombre de char lu
 * retourne le nombre de char lu
 */
int GetLineA(char* p, size_t* pos)
{
	char c;
	int len=0;
	while((*pos<statsA.st_size)&&((c=*(p+(*pos))) != '\n')){
		(*pos)++;
		len++;
	}
	if((*pos)>=statsA.st_size){
		return -1;
	}
	(*pos)+=1;
	return len+1;
}
int GetLineB(char* p, size_t* pos)
{
	char c;
	int len=0;
	while((*pos<statsB.st_size)&&((c=*(p+(*pos))) != '\n')){
		(*pos)++;
		len++;
	}
	if((*pos)>=statsB.st_size){
		return -1;
	}
	(*pos)+=1;
	return len+1;
}

/**
 * Compare la ligne en pos1 de p1, et la ligne en pos2 de p2
 * \return 0 si les lignes sont differentes
 * \return 1 si les lignes sont identiques
 */
int CmpLine(char* p1, size_t pos1, char* p2, size_t pos2)
{
	char c1,c2;
	size_t i=0;
	while( ((c1=*(p1+pos1+i))!='\n') && ((c2=*(p2+pos2+i))!='\n') ){
		i++;
		if(c1!=c2){
			return 0;
		}
	}
	if(*(p1+pos1+i)=='\n'){
		if(*(p2+pos2+i)=='\n'){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}

}

int pos_tab(int i, int j)
{
	return (i+j*(nb_ligne_A+1));
}



/**
 * \fn int main ( int argc, char* argv[] )
 * \param argc nombre de parametre
 * \param argv[] liste de chaine de char
 *
 */
int main ( int argc, char* argv[] )
{
	if( argc!=3 ){
		printf(" !!!! lancer: computepatchOpt F1 F2 !!!!\n");
		return EXIT_FAILURE;
	}
	// Permet un affichage graphique du résultat 
	/*	if( argc!=4 ){
		printf(" !!!! lancer: computepatchOpt source target patch_à_creer !!!!!\n");
		return EXIT_FAILURE;
		}
		*/
	//	printf("#############################################################\n");
	//	printf("#################  GENERATEUR DE PATCH OPTIMAL ##############\n");
	//	printf("#############################################################\n");
	//	printf("-------------- Entrée du Patch : %s\n",argv[1]);
	//	printf("-------------- Sortie du Patch : %s\n",argv[2]);
	int n = 0;
	int m = 0;

/*	   struct timeval tempsDmain;
	   struct timeval tempsFmain;
	   if( gettimeofday(&tempsDmain,NULL)){
	   printf("erreur GetTimeOfDay !!! ");
	   return -1;
	   }
	   */
	

	if(stat(argv[1], &statsA)){
		printf("Erreur stat f1");
		return -1;
	}
	if(stat(argv[2], &statsB)){
		printf("Erreur stat f2");
		return -1;
	}
	if((fA = open(argv[1], O_RDONLY)) < 1){
		printf("Erreur open f1");
		return -1;
	}
	if((fB = open(argv[2], O_RDONLY)) < 1){
		printf("Erreur open f2");
		return -1;
	}
	
	
	if(!(pA = (char*)mmap(NULL, statsA.st_size, PROT_READ, MAP_PRIVATE, fA,0 ))){
		printf("Erreur mmap f1");
		close(fA);
		return -1;
	}
	if(!(pB = (char*)mmap(NULL, statsB.st_size, PROT_READ, MAP_PRIVATE, fB, 0))){
		printf("Erreur mmap f2");
		close(fB);
		return -1;
	}
// Calcul du nombre de ligne de fA et fB
	size_t ligneA=0, ligneB=0;
	while( GetLineA(pA,&ligneA)!=-1){
		n++;
	}
	nb_ligne_A = n;
	while((GetLineB(pB,&ligneB))!=-1){
		m++;
	}
	nb_ligne_B = m;

	mem = (cellule*)malloc((n+1)*(m+1)*sizeof(*mem));
	


	int cout = computePatchOpt_it(n,m);
	printf("%d\n",cout);
	//	printf("-------------- Coût du Patch :  %d \n",computePatchOpt_it(n,m));
	//	printf("#############################################################\n");
	//	printf("-------------- Patch en écriture veuillez patienter\n");

	//	FILE* p = NULL;
	//	p = fopen(argv[3],"w");
	//	int compteur = 0;

	// Dans cette partie, on créer le patch dans le fichier de sortie ou dans stdin
	int l=0, c=0,lTmp=l, cTmp=c;
	int pi=0, pj=0;
//	char* str=NULL;
	char car;
	size_t i=0;
	do{
		pi = (*(mem+pos_tab(l,c))).pereI;
		pj = (*(mem+pos_tab(l,c))).pereJ;
		//fputs(mem[l][c].cmd,p);
		// Cas de la SUBTITUTION		
		if( (pi==l+1) && (pj==c+1) ){
			// Al = Bc -> cout = 0
			if((*(mem+pos_tab(l,c))).cout == (*(mem+pos_tab(pi,pj))).cout){
				//On doit recopier Al sur la sortie -> le patch ne fait rien
			}
			//On doit substituer la ligne Al par Bc
			else{
				printf("= %d\n",pi);
				while( (car=*(pB+(*(mem+pos_tab(pi,pj))).l_cpy+i))!='\n'){
					i++;
					printf("%c",car);
				}
				printf("\n");
				i=0;
			}
		}
		//Cas de l'addition
		else if( (pi==l) && (pj==c+1) ){
			printf("+ %d\n",pi);
			while( (car=*(pB+(*(mem+pos_tab(pi,pj))).l_cpy+i))!='\n'){
				i++;
				printf("%c",car);
			}
			printf("\n");
			i=0;
		}
		//Cas de la deletion simple
		else if( (pi==l+1) && (pj==c)){
			printf("d %d\n",pi);	
		}
		//Cas de la deletion multiple
		else{
			printf("D %d %d\n",l+1,pi-l);
		}
		//		if(compteur>=m/100){
		//			compteur = 0;
		//			printf("#");
		//		}
		//		else{
		//		compteur++;
		//		}
		l = (*(mem+pos_tab(lTmp,cTmp))).pereI;
		c = (*(mem+pos_tab(lTmp,cTmp))).pereJ;
		cTmp = c;
		lTmp = l;
	}while(l<n || c<m);


	//	printf(" 100%%\n");
	//	printf("-------------- Patch écrit dans '%s'\n",argv[3]);
	munmap(pA,statsA.st_size);
	munmap(pB,statsB.st_size);
	free(mem);
	close(fA);
	close(fB);
	//Permet l'affiche graphique du résultat et du temps d'exécution
	/*	fclose(p);
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
		*/
	return EXIT_SUCCESS;
}/* ----------  end of function main  ---------- */
