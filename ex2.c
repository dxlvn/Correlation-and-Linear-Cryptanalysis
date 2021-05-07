#include <stdio.h>
#include <stdlib.h>
#define mask_b31  0x80000000 
#define mask_b0  0x00000001 

// Structure stockant un couple de valeurs hexadécimales
typedef struct{
   unsigned left;
   unsigned right;
 }COUPLE;

// Implémentation de l'opération "<<<7" qui applique une rotation de 7 bits
// vers la gauche sur la branche gauche  
unsigned rotation_7(unsigned x){
	int i = 0;
	while(i<7){
	unsigned old_b31 = (x & mask_b31)>>31;
	x = x<<1;
	if(old_b31 & 0x1){
		x = x | 0x00000001;
	}
	i++;
	}
	return x;
}

// Opération inverse (vers la droite) de la fonction rotation_7
unsigned rotation_inverse_7(unsigned x){
	int i = 0;
	while(i<7){
	unsigned old_b0 = (x & mask_b0);
	x = x>>1;
	if(old_b0 & 0x1){
		x = x | 0x80000000;
	}
	i++;
	}
	return x;
}

// Fonction effectuant un tour du chiffrement Ek0,k1 et retourant
// le couple chiffré obtenu 
COUPLE tour(COUPLE x,unsigned k0, unsigned k1){
	unsigned xl = x.left;
	unsigned xr = x.right;
	printf("xl vaut %2X\n", xl);
	printf("xr vaut %2X\n", xr);
	unsigned res1,res2,x_r,x_l;
	res1 = xl^xr; //a
	res1 = rotation_7(res1); //b
	res1 = res1^k0; //c
	res2 = res1^xr; //d
	res2 = rotation_7(res2); //e
	res2 = res2^k1; //f
	x_l = res1; //g
	x_r = res2; //h
	COUPLE res = {x_l,x_r};
	printf("x_l vaut %2X\n", x_l);
	printf("x_r vaut %2X\n", x_r);
	return res;
}

// Fonction effectuant n tour du chiffrement Ek0,k1 et retourant
// le couple chiffré obtenu 
COUPLE tour_n(COUPLE x,unsigned k0, unsigned k1,int n){
	int i=0;
	COUPLE res = {x.left, x.right};
	while(i<n){
		res = tour(res,k0,k1);
		i++;
		printf("Tour %d, Left: %2X, Right: %2X\n",i,res.left, res.right);
	}
	return res;
}

// Fonction permettant de calculer la clé k1 par des transformations linéaires
unsigned find_k1(unsigned xl_1, unsigned xr_1, unsigned xr_0){
	unsigned res = xl_1^xr_0;
	res = rotation_7(res);
	res = res^xr_1;
	return res;
}

// Fonction permettant de calculer la clé k0 par des transformations linéaires
unsigned find_k0(unsigned xl_0, unsigned xr_0, unsigned xr_1, unsigned k1){
	unsigned res1,res2,res;
	res1 = xr_1^k1;
	res1 = rotation_inverse_7(res1);
	res1 = res1^xr_0;
	res2 = xl_0^xr_0;
	res2 = rotation_7(res2);
	res = res1^res2;
	return res;
}

// Fonction permettant de retrouver les clés k0 et k1
COUPLE find_keys(unsigned xl0, unsigned xr0,unsigned xl1,unsigned xr1){
	unsigned k0,k1;
	//COUPLE res;
	k1 = find_k1(xl1,xr1,xr0);
	k0 = find_k0(xl0,xr0,xr1,k1);
	COUPLE res= {k0,k1};
	printf("k0 vaut %2X\n", res.left);
	printf("k1 vaut %2X\n", res.right);
	return res;
}

int main(){ 
	unsigned x0l = 0x45019824;
	unsigned x0r = 0x51023321;
	unsigned k0 = 0x01020304;
	unsigned k1 = 0x98765432; 
	unsigned x1l = 0xD7818E;
	unsigned x1r = 0x72AF039A;
	COUPLE C = {x0l,x0r};
	tour(C,k0,k1);
	find_keys(x0l,x0r,x1l,x1r);
	tour_n(C,k0,k1,12);

    return 0;
}
