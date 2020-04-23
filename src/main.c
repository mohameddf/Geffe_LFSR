#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

typedef struct {
	 
	char L[3][17];  //Registres	
	char F[9];	    //Fonction

}LFSR;

typedef struct {
	int pos;
	float p;
	int ** gen_suite;
	int taille;
}LFSR_I;

//int * L0;
//int * L1;
//int * L2;

int t_verite[3][8] = {{0,1,0,1,0,1,0,1}, {0,0,1,1,0,0,1,1},{0,0,0,0,1,1,1,1}};
int xor_pos[3][4] = {{15,14,11,8}, {15,14,8,4}, {15,13,12,10}};	


/* 
	Affichage d'une suite de N bits
*/
void printSequence ( char * t, int * s, int n)	{
	printf("\n%s = ",t);
	for (int i = 0; i < n; i++)
		printf ("%d", s[i]);
	//printf ("");	
}

/* 
	* Fonction de filtrage
	* prend trois bits en entrée et retourne un bit en sortie
*/
int filteringF(char x0, char x1, char x2){
	return (x0*1) + (x1*2) + (x2*4);
}

/* 
	* Fonction de décalage
	* Décaler chaque LFSR
	* \param Geffe : Initialisation des trois LFSR 
	* \param i : Position de LFSR
	* \return Bit de sortie de la fonction de filtrage aprés décalage
*/
int shift ( LFSR * Geffe)	{

	char x0 = Geffe->L[0][15] - '0';
	char x1 = Geffe->L[1][15] - '0';
	char x2 = Geffe->L[2][15] - '0';

	//L0[i] = x0;
	//L1[i] = x1;
	//L2[i] = x2;

	int indice = filteringF ( x0, x1, x2);
	short tmp0 = (Geffe->L[0][15] - '0') ^ (Geffe->L[0][14] - '0') ^ (Geffe->L[0][11] - '0') ^ (Geffe->L[0][8] - '0'),
		  tmp1 = (Geffe->L[1][15] - '0') ^ (Geffe->L[1][14] - '0') ^ (Geffe->L[1][8] - '0') ^ (Geffe->L[1][4] - '0'),
		  tmp2 = (Geffe->L[2][15] - '0') ^ (Geffe->L[2][13] - '0') ^ (Geffe->L[2][12] - '0') ^ (Geffe->L[2][10] - '0');
	for (int i = 15; i > 0; i--)	{
		Geffe->L[0][i] = Geffe->L[0][i-1];
		Geffe->L[1][i] = Geffe->L[1][i-1];
		Geffe->L[2][i] = Geffe->L[2][i-1];
	}
	Geffe->L[0][0] = tmp0 + '0';
	Geffe->L[1][0] = tmp1 + '0';
	Geffe->L[2][0] = tmp2 + '0';

	return Geffe->F[indice] - '0';
}


/* 
	* Générer une suite de N bits pour un seul LFSR 
	* \param li_initial : initialisation de LFSR
	* \param n : nombre de bits à générer
	* \return : suite de N bits de la sortie de LFSR
*/
int * generate_li(char * li_initial, int n, int lfsr_i){

	int * LI = (int *) malloc (n * sizeof(int));

	for (int i = 0; i < n; i++)
	{
		LI[i] = li_initial[15] - '0';
		//L0
		short xor = (li_initial[xor_pos[lfsr_i][0]] - '0') ^ (li_initial[xor_pos[lfsr_i][1]] - '0') 
					^ (li_initial[xor_pos[lfsr_i][2]] - '0') ^ (li_initial[xor_pos[lfsr_i][3]] - '0');
		for (int i = 15; i > 0; i--)	{
			li_initial[i] = li_initial[i-1];
		}
		
		li_initial[0] = xor + '0';
		
	}
	return LI;
}

/*
	* Génération de la suite chiffrante S pour N bits
	* \param Geffe : 3 LFSR du générateur de type Geffe
	* \param n : nombre de bits à générer
	* \return la suite chiffrante de N bits

*/
int * generate(LFSR* Geffe, int n) { //char* bitL0, char* bitL1, char* bitL2){
	//fprintf ( stdout, "Generation de la suite chiffrante ");
	int * s = (int *) malloc ( n * sizeof (int));
	//L0 = (int *) malloc ( n * sizeof (int));
	//L1 = (int *) malloc ( n * sizeof (int));
	//L2 = (int *) malloc ( n * sizeof (int));
	if ( !s)	{
		fprintf ( stderr, "Erreur lors de l'allocation de la clé S\n");
		exit(1);
	}
	for (int i = 0; i < n; i++)	{
		s[i] = shift ( Geffe);
	}
	return s;
}


/*
	* Initialisation d'un Générateur de type Geffe
	* \param path chemin de fichier des entrées
	* \param gen 3 LFSR pour initialiser
*/
void initialiserLFSR_file(const char* path,LFSR* gen){
	FILE* file = fopen(path,"r");
	if ( !file)	{
		printf ("Erreur à l'ouverture du fichier\n");
		exit(1);
	}
	char * buffer = NULL;
	ssize_t read;
	size_t len = 1;
	int i = 0;
   	printf("Initialisation du LFSR ");
    while ((read = getline(&buffer, &len, file)) != -1) {
		read = getline(&buffer, &len, file);
		// Gerer les valeurs k0, k1, k2
		if ( i < 3)	{  
		   strcpy(gen->L[i],buffer);
		   gen->L[i][16] = 0;
	   }
       //gerer derniere ligne (L)
       if(i == 3)	{
		   strncpy(gen->F,buffer,8);
		   gen->F[8] = 0;
	   }
       i++;
    }
//    timer ();
	free(buffer);
	fclose(file);
}

LFSR * initialiserLSFR(char * F, int * l0, int * l1, int * l2){
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	strncpy(Geffe->F, F, 8);
	for( int i = 0; i < 16 ; i++){
		Geffe->L[0][i] = l0[i] +'0';
	}
	for( int i = 0; i < 16 ; i++){
		Geffe->L[1][i] = l1[i]+'0';
	}
	for( int i = 0; i < 16 ; i++){
		Geffe->L[2][i] = l2[i] +'0';
	}

	return Geffe;

}

/*
	* Calculer la probabilité d'équivalence entre
		deux suite de bits
	* \return 
*/
float prob_equivalence(int * s, int * r, int n){
	int cpt = 0;
	for (int i = 0; i < n; i++)
	{
		if(s[i] == (r[i]))
		cpt++;
	}
	return (float)cpt/n;
}

/*
	* Générer toutes les possibilités pour N bits
	* \param path chemin de fichier pour générer
	* \param nbits nombre de bits
*/

void generate_bits(const char* path, int nbits){
	printf("Génération de toutes les combinaisons sur %d bits\n", nbits);
	FILE  * file = fopen (path, "w");
	__int64_t n = pow(2,nbits);
	__int64_t tmp = 0;
	for (__int64_t c = 0; c < n; c++)
	{
		for (int i = sizeof(char)*nbits -1; i > -1; i--)
		{
			tmp = c & (1<<i);
			fprintf(file, "%c", (tmp == 0 ? '0': '1'));
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

/* 
	* Vérifier pour chaque possibilité la probabilité d'equivalence et afficher si
	* \param path : chemin de fichier qui contient toutes les possiblités de N bits
	* \param s : suite chiffante de N bits à tester
	* \param n : la taille de la suite de bits
*/
void verifier_cor(const char * path,int * s, int n, LFSR_I * lfsr){
	FILE* file = fopen(path,"r");
	int * result = NULL;
	//char tab_tmp [16];
	if ( !file)	{
		printf ("Erreur à l'ouverture du fichier des combinaisons sur 16 bits\n");
		generate_bits(path, 16);
		file = fopen(path,"r");
	}
	char * buffer = NULL;
	size_t len = 1;
	lfsr->gen_suite = NULL;
	int taille = 1;
	lfsr->gen_suite = malloc(sizeof(int*));
	lfsr->gen_suite[0] = malloc(16*sizeof(int *));
   	//printf("Attaque par correlation\n");
    while ((getline(&buffer, &len, file)) != -1) {
    	//strncpy(tab_tmp, buffer,16);
    	result = generate_li(buffer, n, lfsr->pos);
		float p = prob_equivalence(s, result, n);
		float min = lfsr->p-0.1;
		float max = lfsr->p+0.1;
		//printf("%.2f -- %.2f\n", min, max); 
		if(p > min  && p  < max){
			for(int i = 15; i >= 0; i--){
					lfsr->gen_suite[taille-1][15-i] = result[i]; 
					//printf("%d", lfsr.gen_suite[taille-1][i]);
			}
			//printf("LFSR[%d] taille = %d\n", lfsr->pos, taille);
			taille ++;
		 	lfsr->gen_suite =realloc(lfsr->gen_suite,taille * sizeof(int*));
		  	lfsr->gen_suite[taille-1] = malloc(16*sizeof(int));
			
		}
		free(result);
    }
    lfsr-> taille = taille -1 ;

	free(buffer);
	fclose(file);
}



/* 
	* Calculer correlation entre chaque sortie de LFSR 
	et la sortie de la fonction F
*/
LFSR_I * calculer_correlation_f(char * F){

	LFSR_I  * lfsr = (LFSR_I *) malloc ( 3 * sizeof (LFSR_I));
	printf("-----------------------------------------------------\n");
	printf("F = %s  ", F);
	int F_int[8];
	int cpt0 = 0;
	int cpt1 = 0;
	for(int i = 0;i< 8; i++){
		if(F[i] -'0' == 0)
			cpt0++;
		else cpt1++;
		F_int[i] = F[i] -'0';
	}
	if(cpt0 == cpt1)
		printf("[Equilibré] "); 
	for(int i= 0; i < 3; i++){
		lfsr[i].pos = i;
		float p = prob_equivalence(F_int, t_verite[i], 8);
		lfsr[i].p = p ;
		printf("L[%d] = %.2f\t", i,p);
	}
	printf("\n-----------------------------------------------------\n");
	return lfsr;
}

void calculer_correlation_t_f(){
	FILE * file = NULL;
	file = fopen("gen_8bits", "r");
	if (!file)	{
		printf ("Erreur à l'ouverture du fichier de combinaison sur 8 bits\n");
		generate_bits("gen_8bits",8);
		file = fopen("gen_8bits", "r");
	}
	char * buffer = NULL;
	size_t len = 1;
	while (getline(&buffer, &len, file) != -1){
		calculer_correlation_f(buffer);
	}
	free(buffer);
	fclose(file);
}

void decrypt(char * F, int * s, int n){
	
	LFSR_I * tab_lfsr = NULL;
	LFSR* Geffe = NULL;
	
	tab_lfsr =  calculer_correlation_f(F);
	// Vérifier corrélation de chaque LFSR
	
	for (int i = 0; i < 3; ++i)
	{
		if(tab_lfsr[i].p == 0.5) {
			printf("Attaque par recherche exhaustive n'est pas implementé\n");
			exit(1);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		verifier_cor("gen_16bits", s, n, &tab_lfsr[i]);

	}

	int row0 = tab_lfsr[0].taille;
	int row1 = tab_lfsr[1].taille;
	int row2 = tab_lfsr[2].taille;

	printf("\nAttaque par corrélation...\n\n");
	printf("Clés candidates\t");
	printf("K0=%d, K1=%d, K2=%d\n", row0, row1, row2);
	printf("Recherche exhaustive...\n");
	int * gen;
	for (int i = 0; i < row0; i++)
	{

		for (int j = 0; j < row1; j++)
		{
			for (int k = 0; k < row2; k++)
			{	

				Geffe = initialiserLSFR(F, tab_lfsr[0].gen_suite[i],
										tab_lfsr[1].gen_suite[j],
										tab_lfsr[2].gen_suite[k]);
				
				gen = generate(Geffe, n);
				float p = prob_equivalence(s, gen, n);
				if(p == 1){
						printSequence("K0", tab_lfsr[0].gen_suite[i],16);
						printSequence("K1", tab_lfsr[1].gen_suite[j],16);
						printSequence("K2", tab_lfsr[2].gen_suite[k],16);
						printf("\nFelicitation\n ");
						printSequence("S", s, n);
						printf("\n");
						free(gen);
						free(Geffe);
						free(tab_lfsr);
						return;
					}
				free(gen);
				free(Geffe);
				}

		}
	}


	free(tab_lfsr);


}


void checkArgs ( int argc, char** argv)	{
	if(argc == 4){
		int n = atoi(argv[3]);
	if (strcmp(argv[1], "-c") == 0)
	{
		LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
		initialiserLFSR_file(argv[2],Geffe);
	
		int * s = generate ( Geffe, n);
		printSequence ("S ",  s, n);
  		printf("\n\n");
  		free(Geffe);
  		free ( s);
  		exit(0);
	}
	if (strcmp(argv[1], "-d") == 0){
	 	FILE* entre = fopen(argv[2], "r");
	 	if(entre){
	 		char* buffer = NULL;
	 		char F[8];
	 		int* S = (int*) malloc(n* sizeof(int));
	 		size_t size = 1;
	 		getline(&buffer, &size, entre); 
	 		strncpy(F,buffer, 8);
	 		getline(&buffer, &size, entre); 
	 		
	 		for (int j = 0; j < n; j++)
	 		{
	 			S[j] = buffer[j] - '0';
	 		}
	 		decrypt(F, S, n);
	 		free(buffer);
	 		free(S);
	 		fclose(entre);
	 		exit(0);

	 	}else{
	 		fprintf(stderr, "-d: Erreur dans l'ouverture du fichier\n");
	 		exit(1);
	 	}
	}
	if (strcmp(argv[1], "-g") == 0)	generate_bits(argv[2], n);
	
	}
	if((strcmp(argv[1], "-F") == 0)){
		if(argc == 2) calculer_correlation_t_f();
		if(argc == 3) calculer_correlation_f(argv[2]);

	}
	else{
		fprintf(stdout, "Liste des commandes Flow:\n\t -c [fichier] [n]: Génére une suite chiffrante de N bits \n\t -d [fichier] [n]: Attaque par correlation pour générer K \n\t -g [fichier] [n] : Génére dans fichier toutes les combinaisons possbile sur n bits\n");
		exit(1);
	}
}



int main(int argc, char *argv[])
{	

	checkArgs (argc, argv);
	return 0;
}

