#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define mask_b0  0x0001 //1    shift 0
#define mask_b1  0x0002 //2    shift 1
#define mask_b4  0x0010 //16   shift 4
#define mask_b7  0x0080 //128  shift 7
#define mask_b11 0x0800 //2048 shift 11
#define mask_b2  0x0004 //4    shift 2
#define mask_b3  0x0008 //8    shift 3
#define mask_b5  0x0020 //32   shift 5

// Structure stockant un triplé de 3 valeurs flottantes, utile pour
// stocker les probabilités
typedef struct{
   float l;
   float m;
   float r;
 }TRIPLE;

// Structure stockant la suite générée en sortie st et les suites des 3 LFSR 
typedef struct{
   unsigned* s0;
   unsigned* s1;
   unsigned* s2;
   unsigned* st;
 }SUITES;

// Structure stockant 3 clés K0, K1 et K2
typedef struct{
   unsigned K0;
   unsigned K1;
   unsigned K2;
 }CLES;

// Retourne en fonction des 3 bits en entrée et de la fonction de filtrage
// le bit de sortie correspondant  
unsigned Filtrage(unsigned x0, unsigned x1, unsigned x2, unsigned F){
	unsigned f7 = F & mask_b0;
	F = F>>1;
	unsigned f6 = F & mask_b0;
	F = F>>1;
	unsigned f5 = F & mask_b0;
	F = F>>1;
	unsigned f4 = F & mask_b0;
	F = F>>1;
	unsigned f3 = F & mask_b0;
	F = F>>1;
	unsigned f2 = F & mask_b0;
	F = F>>1;
	unsigned f1 = F & mask_b0;
	F = F>>1;
	unsigned f0 = F & mask_b0;
	if((x0 == 0x0000) && (x1 == 0x0000) && (x2 == 0x0000)){
		return f0 ;
	}
	else if((x0 == 0x0001) && (x1 == 0x0000) && (x2 == 0x0000)){
		return f1 ;
	}
	else if((x0 == 0x0000) && (x1 == 0x0001) && (x2 == 0x0000)){
		return f2 ;
	}
	else if((x0 == 0x0001) && (x1 == 0x0001) && (x2 == 0x0000)){
		return f3 ;
	}
	else if((x0 == 0x0000) && (x1 == 0x0000) && (x2 == 0x0001)){
		return f4 ;
	}
	else if((x0 == 0x0001) && (x1 == 0x0000) && (x2 == 0x0001)){
		return f5 ;
	}
	else if((x0 == 0x0000) && (x1 == 0x0001) && (x2 == 0x0001)){
		return f6 ;
	}
	else if((x0 == 0x0001) && (x1 == 0x0001) && (x2 == 0x0001)){
		return f7 ;
	}
	else;
}

// Produit et affiche les n 1ères valeurs des suites générées par les LFSR L0, L1, L2 et  la sortie S
SUITES generateur(unsigned F,unsigned k0, unsigned k1,unsigned k2 ,int n){
	int i=0;
	float px0 =0;
	float px1 =0;
	float px2 =0;
	unsigned t0[n];
	unsigned t1[n];
	unsigned t2[n];
	unsigned ts[n];
	while(i<n){
		//L0
		unsigned k0_b0 = (k0 & mask_b0);
		unsigned k0_b1 = (k0 & mask_b1)>>1;
		unsigned k0_b4 = (k0 & mask_b4)>>4;
		unsigned k0_b7 = (k0 & mask_b7)>>7;
		unsigned k0_back = k0_b0^k0_b1^k0_b4^k0_b7;
		if(k0_back & 0x1){
			k0 = k0 | 0x8000;
		}
		k0 = k0>>1;
		//L1
		unsigned k1_b0 = (k1 & mask_b0);
		unsigned k1_b1 = (k1 & mask_b1)>>1;
		unsigned k1_b7 = (k1 & mask_b7)>>7;
		unsigned k1_b11 = (k1 & mask_b11)>>11;
		unsigned k1_back = k1_b0^k1_b1^k1_b7^k1_b11;
		if(k1_back & 0x1){
			k1 = k1 | 0x8000;
		}
		k1 = k1>>1;
		//L2
		unsigned k2_b0 = (k2 & mask_b0);
		unsigned k2_b2 = (k2 & mask_b2)>>2;
		unsigned k2_b3 = (k2 & mask_b3)>>3;
		unsigned k2_b5 = (k2 & mask_b5)>>5;
		unsigned k2_back = k2_b0^k2_b2^k2_b3^k2_b5;
		if(k2_back & 0x1){
			k2 = k2 | 0x8000;
		}
		k2 = k2>>1;
		//Filtrage
		unsigned x0 = k0_b0;	
		unsigned x1 = k1_b0;	
		unsigned x2 = k2_b0;	
		unsigned sortie = Filtrage(x0,x1,x2,F);
		t0[i] = x0;
		t1[i] = x1;
		t2[i] = x2;
		ts[i] = sortie;
		if(x0 == sortie){
			px0++;
		}
		if(x1 == sortie){
			px1++;
		}
		if(x2 == sortie){
			px2++;
		}
		printf("Tour %d, x0: %2X; x1 %2X; x2: %2X; S: %2X\n",i,t0[i],t1[i],t2[i],ts[i]);
		i++;
	}
	SUITES suites = {t0, t1, t2, ts};
	px0 = px0 / n;
	px1 = px1 / n;
	px2 = px2 / n;
	TRIPLE proba = {px0 , px1, px2};
	// Afficher les correlations 
	// Mettre en commentaire les 3 lignes du dessous 
	// avant d'effectuer la recherche des clés, sinon
	// trop d'informations sont affichées
	printf("Correlation en x0 après génération de la suite: %f\n",proba.l); 
	printf("Correlation en x1 après génération de la suite: %f\n",proba.m);
	printf("Correlation en x2 après génération de la suite: %f\n",proba.r);
	return suites;
} 

// Calcule à partir des valeurs de retour d'une fonction de filtrage F
// en hexadécimal les correlations entre E/S de sa table de vérité
void table_verite(unsigned F){
	unsigned filtre = F;
	unsigned x0;
	unsigned x1;
	unsigned x2;
	float px0 = 0;
	float px1 = 0;
	float px2 = 0;
	//val 0: 000
	x0 = 0x01;
	x1 = 0x01;
	x2 = 0x01;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 1: 100
	x0 = 0x00;
	x1 = 0x01;
	x2 = 0x01;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 2: 010
	x0 = 0x01;
	x1 = 0x00;
	x2 = 0x01;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 3: 110
	x0 = 0x00;
	x1 = 0x00;
	x2 = 0x01;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 4: 001
	x0 = 0x01;
	x1 = 0x01;
	x2 = 0x00;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 5: 101
	x0 = 0x00;
	x1 = 0x01;
	x2 = 0x00;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 6: 011
	x0 = 0x01;
	x1 = 0x00;
	x2 = 0x00;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	//val 7: 111
	x0 = 0x00;
	x1 = 0x00;
	x2 = 0x00;
	if(x0 == (F & mask_b0)){
			px0++;
	}
	if(x1 == (F & mask_b0)){
			px1++;
	}
	if(x2 == (F & mask_b0)){
			px2++;
	}
	F = F>>1;
	px0 = px0/8;
	px1 = px1/8;
	px2 = px2/8;
	printf("Correlation en x0 %f\n", px0);
	printf("Correlation en x1 %f\n", px1);
	printf("Correlation en x2 %f\n", px2);
}

// Affiche toutes les tables de vérité de 0x00 à 0xFF
void all_table_verite(){
	unsigned z =0x00;
	while(z<0xFF){
		printf("Filtre: %2X\n",z);
		table_verite(z);
		z = z+0x01;
	}
}

// Affiche les correlations entre les 3 entrées et la sortie 
// à partir d'une SUITE s générée par un générateur
void correlations_suites(SUITES s,int n){
	int i=0;
	float cor0 = 0;
	float cor1 = 0;
	float cor2 = 0;

	while(i<n){
		if(s.s0[i] == s.st[i]){
			cor0++;
		}
		if(s.s1[i] == s.st[i]){
			cor1++;
		}
		if(s.s2[i] == s.st[i]){
			cor2++;
		}
		i++;
	}
	cor0 = cor0/n;
	cor1 = cor1/n;
	cor2 = cor2/n;
	printf("Correlation en x0 après vérification: %f\n",cor0);
	printf("Correlation en x1 après vérification: %f\n",cor1);
	printf("Correlation en x2 après vérification: %f\n",cor2);
}

// Retourne le pourcentage de correlation uniquement entre 2 suites
// utile pour retrouver les clés
float correlations_2_suites(unsigned* x, unsigned* s ,int n){
	int i=0;
	float cpt = 0;
	while(i<n){
		if(x[i] == s[i]){
			cpt++;
		}
		i++;
	}
	cpt = cpt/n;
	return cpt;
}

// Effectue la recherche exhaustive de la clé k2 à partir de la suite de sortie s
// et de la probabilité de correlation p
unsigned trouve_k2(unsigned* s, int n, unsigned F, float p){
	unsigned z = 0x0000;
	unsigned k2;
	float c = 1;
	float y = 1;
	while(z<0xFFFF){
		SUITES gen = generateur(F,0x0000,0x0000,z,n);
		unsigned* tab = gen.s2;
		c = correlations_2_suites(tab,s,n);
		if(fabs(c-p) < y){ 
			y = fabs(c-p);
			k2 = z;
		}
		z = z+0x0001;
	}
	printf("k2: %2X\n",k2);
	return k2;
}

// Effectue la recherche exhaustive de la clé k1 à partir de la suite de sortie s
// et de la probabilité de correlation p
unsigned trouve_k1(unsigned* s, int n, unsigned F, float p){
	unsigned z = 0x0000;
	unsigned k1;
	float c = 1;
	float y = 1;
	while(z<0xFFFF){
		SUITES gen = generateur(F,0x0000,z,0x0000,n);
		unsigned* tab = gen.s1;
		c = correlations_2_suites(tab,s,n);
		if(fabs(c-p) < y){ 
			y = fabs(c-p);
			k1 = z;
		}
		z = z+0x0001;
	}
	printf("k1: %2X\n",k1);
	return k1;
}

// Effectue la recherche exhaustive de la clé k0 à partir de la suite de sortie s
// et de la probabilité de correlation p
unsigned trouve_k0(unsigned* s, int n, unsigned F, float p){
	unsigned z = 0x0000;
	unsigned k0;
	float c = 1;
	float y = 1;
	while(z<0xFFFF){
		SUITES gen = generateur(F,z,0x0000,0x0000,n);
		unsigned* tab = gen.s0;
		c = correlations_2_suites(tab,s,n);
		if(fabs(c-p) < y){ 
			y = fabs(c-p);
			k0 = z;
		}
		z = z+0x0001;
	}
	printf("k0: %2X\n",k0);
	return k0;
}


int main(){
	//********* LES LIGNES COMMENTÉES SONT PRÉSENTES POUR DES TESTS ************
	// Clés
	unsigned k0 = 0xD1F3;
	unsigned k1 = 0xC794;
	unsigned k2 = 0x38BA;
	// Nombre de bits de la suite chiffrante à générer
	int n = 512;  // Plus la valeur de n est grande, plus on est précis ( 150 valeur moyenne)
	// Fonction de filtrage 
	unsigned F = 0x8E;

	SUITES S = generateur(F,k0,k1,k2,n);
	unsigned* st = S.st; // Suite générée en sortie
	correlations_suites(S,n);

	//table_verite(F);
	//all_table_verite();

	// Mettre en commentaires les lignes 135,147,148 et 149 avant de
	// tester les lignes ci dessous
	float p2 = 0.75;
	float p1 = 0.25;
	float p0 = 0.25;
	// trouve_k2(st,n,F,p2);  
	// trouve_k1(st,n,F,p1);  
	// trouve_k0(st,n,F,p0); 

    return 0;
}