########## Commandes du Makefile #########

Pour lancer l'exercice 1: make run1

Pour lancer l'exercice 2: make run2

Pour supprimer les exécutables : make clean

################## STRUCTURE de ex1.c ######################

------Structures de données utilisées

--------Structure stockant un triplé de 3 valeurs flottantes, utile pour stocker les probabilités---------
typedef struct{
   float l;
   float m;
   float r;
 }TRIPLE;

---------Structure stockant la suite générée en sortie st et les suites des 3 LFSR---------
typedef struct{
   unsigned* s0;
   unsigned* s1;
   unsigned* s2;
   unsigned* st;
 }SUITES;

----------Structure stockant 3 clés K0, K1 et K2-----------
typedef struct{
   unsigned K0;
   unsigned K1;
   unsigned K2;
 }CLES;


--------unsigned Filtrage(unsigned x0, unsigned x1, unsigned x2, unsigned F);--------
Retourne en fonction des 3 bits en entrée et de la fonction de filtrage
le bit de sortie correspondant  


--------SUITES generateur(unsigned F,unsigned k0, unsigned k1,unsigned k2 ,int n);--------
Produit et affiche les n 1ères valeurs des suites générées par les LFSR L0, L1, L2 et  la sortie S


--------void table_verite(unsigned F);---------
Calcule à partir des valeurs de retour d'une fonction de filtrage F
en hexadécimal les correlations entre E/S de sa table de vérité


--------void all_table_verite();---------
Affiche toutes les tables de vérité de 0x00 à 0xFF

--------void correlations_suites(SUITES s,int n);--------
Affiche les correlations entre les 3 entrées et la sortie 
à partir d'une SUITE s générée par un générateur


--------float correlations_2_suites(unsigned* x, unsigned* s ,int n);------
Retourne le pourcentage de correlation uniquement entre 2 suites
utile pour retrouver les clés

--------unsigned trouve_k2(unsigned* s, int n, unsigned F, float p)--------
Effectue la recherche exhaustive de la clé k2 à partir de la suite de sortie s
et de la probabilité de correlation p


-------unsigned trouve_k1(unsigned* s, int n, unsigned F, float p);--------
Effectue la recherche exhaustive de la clé k1 à partir de la suite de sortie s
et de la probabilité de correlation p


-------unsigned trouve_k0(unsigned* s, int n, unsigned F, float p);--------
Effectue la recherche exhaustive de la clé k0 à partir de la suite de sortie s
et de la probabilité de correlation p



################## STRUCTURE de ex2.c ######################


-------------Structure stockant un couple de valeurs hexadécimales-----------
typedef struct{
   unsigned left;
   unsigned right;
 }COUPLE;


--------unsigned rotation_7(unsigned x);----------
Implémentation de l'opération "<<<7" qui applique une rotation de 7 bits
vers la gauche sur la branche gauche  


--------unsigned rotation_inverse_7(unsigned x);--------
Opération inverse (vers la droite) de la fonction rotation_7



--------COUPLE tour(COUPLE x,unsigned k0, unsigned k1);---------
Fonction effectuant un tour du chiffrement Ek0,k1 et retourant
le couple chiffré obtenu 


Fonction effectuant n tour du chiffrement Ek0,k1 et retrouvant
le couple chiffré obtenu 
--------COUPLE tour_n(COUPLE x,unsigned k0, unsigned k1,int n);---------


---------unsigned find_k1(unsigned xl_1, unsigned xr_1, unsigned xr_0);-----------
Fonction permettant de calculer la clé k1 par des transformations linéaires


--------unsigned find_k0(unsigned xl_0, unsigned xr_0, unsigned xr_1, unsigned k1);--------
Fonction permettant de calculer la clé k0 par des transformations linéaires


--------COUPLE find_keys(unsigned xl0, unsigned xr0,unsigned xl1,unsigned xr1);---------
Fonction permettant de retrouver les clés k0 et k1
