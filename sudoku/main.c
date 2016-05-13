// Projet   : Sudoku/Hexadoku partiel 2015/2016 pour le projet de programmation
//          : en DEUST IOSI 1 - UVHC/ISTV (c) 2015-2016
// Auteur   : D. Duvivier
// Version  : 1.0.0
// Date     : 30/03/16
// Licence  : Exclusivement utilisable dans le cadre du Projet de programmation
//          : informatique en DEUST IOSI 1 - UVHC/ISTV (c) 2015-2016
// Limites  : Cette version ne teste aucun code de retour (de scanf & ...).
//          : Elle ne teste pas non plus la validité des paramètres...
//          : TODO: A vous de le faire !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//////////////////////////////////////////////////////////////////////////////
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !VERSION COMPLETE, SANS TEST SUR LES PARAMETRES ET LES CODES DE RETOUR ! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !!!! VERSION PARTIELLE DEDIÉE AU PROJET DE PROGRAMMATION (HEXADOKU) !!!! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
//////////////////////////////////////////////////////////////////////////////
//
// Le format du fichier lu est celui qui a été étudié en TD.
// Bien que seules des grilles de sudoku/hexadoku soient lues dans les fichiers
// pour ce programme. Le format utilisé permet de considérer différents types
// de jeux :
// - jeu de "Dames", "Échecs" ou autre jeu avec grille régulière
// - jeu de sudoku avec des régions (par exemple 3 lignes et 3 colonnes par
//   région et 9 * 9 cases au total, ou 4 lignes et 4 colonnes par
//   région et 16 * 16 cases au total)
// - jeu de type "PacMan" ou "Labyrinthe" ou "Casse-Briques" où la grille de
//   base est vide mais le "décor" est lu depuis le fichier en question
//
// Le format utilisé est donc le suivant pour les trois premières lignes de
// chaque fichier :
// nombre_de_lignes nombre_de_colonnes taille_region
// <caractere_vide_dans_fichier>
// caractere_vide_dans_grille s1 s2 s3 s4...
//
// ATTENTION : Le caractère "espace" (i.e. ' ') n'est pas autorisé dans les
// fichiers en tant que "symbole" puisqu'il est utilisé comme séparateur par
// scanf & cie (cf premiers TD de méthodologie).
// Nous utilisons donc le caractère "_" pour remplacer l'espace dans le
// fichier.
//
// Voici les explications détaillées :
// 1) Sur la première ligne, il y a trois valeurs numériques positives ou nulles :
//  - nombre_de_lignes   : nombre de lignes au total dans la grille
//  - nombre_de_colonnes : nombre de colonnes au total dans la grille
//  - taille_region      : taille de région
//    0 si la grille initiale est vide ("PacMan" ou "Labyrinthe" ou "Casse-Briques")
//    1 si la grille est régulière ("Dames", "Échecs")
//    2, 3, 4 ou 5 pour une grille avec régions 2*2, 3*3, 4*4 ou 5*5 ("Sudoku")
// 2) Sur la deuxième ligne, il y a 3 caractères :
//   - le caractère '<'
//   - le caractère caractere_vide_dans_fichier qui correspond à une case vide
//     dans la grille stockée dans le fichier (par exemple le caractère '.')
//   - le caractère '>'
// 3) Sur la troisième ligne, il y a AU MAXIMUM une suite de MAXCOLONNES caractères et le premier
//    caractère est le caractere_vide_dans_grille ou s0 (pour symbole N°0), le second caractère
//    est appelé s1 (pour symbole N°1), le troisième caractère est appelé s2 et ainsi de suite...
//    Le caractere_vide_dans_grille ou s0 correspond à une case vide affichée à l'écran dans la grille
//    (par exemple le caractère '_').
// Dans l'exemple ci-dessus, le symbole '.' lu dans le fichier est transformé en caractère '_',
// qui est lui même remplacé par un espace ' '
// dans la grille affichée pour éviter les problèmes avec les espaces, considérés comme des séparateurs
// dans les fichiers texte (i.e. si nous utilisons scanf & cie)...
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Taille (maxi) de la grille de jeu
#define MAXLIGNES   25
#define MAXCOLONNES 25

// Nombre maxi de symboles (caractères) différents dans la grille + espace
#define MAXSYMBOLES (MAXCOLONNES+1)

// Nombre de "symboles" par case. Pour des chiffres de 1 à 9 → MAXPLANS = 9
// TODO : Question : Est-ce MAXSYMBOLES ou MAXCOLONNES qu'il faut utiliser ci-dessous ?
#define MAXPLANS MAXSYMBOLES

// Caractère utilisé pour représenter un espace dans le fichier
#define SYMBOLESPACE '_'

// Valeurs du paramètre 0 pour AfficheErreur
#define STOPEXIT 1   // Mode "sauvage"
#define STOPRETURN 0 // Mode "return" + gestion des codes de retour

// La taille des régions est obtenue directement depuis la lecture du fichier
// contenant une grille

#define false 0
#define true (!0)

#define CARGRILLEVIDE ((symboles[0] == SYMBOLESPACE) ? ' ' : symboles[0])

// Type "Tgrille2D".
// Ci-dessous nous utilisons le type de base "caractère" pour faciliter
// l'affichage
typedef char TGrille2D[MAXLIGNES][MAXCOLONNES];

// Ci-dessous, la 3ème dimension est indépendante de la taille de grille.
// Elle correspond à un plan faisant le cumul des possibilités par case
// en guise de plan 0 (conformément à ce qui est précisé dans l'énoncé),
// suivi des "plans" pour chaque chiffre/symbole en allant de 1 à MAXPLANS :
typedef int TGrille3D[MAXLIGNES][MAXCOLONNES][MAXPLANS+1];

// Définition d'un type « pseudo-booléen » :
typedef short int bool;

// Définition d'un tableau de booléen (pour tester l'unicité des symboles en ligne, en colonne ou en région...) :
typedef bool TBool[MAXPLANS+1];

// Selon l'algorithme de résolution utilisé, il peut être intéressant pour éviter les boucles infinies, de
// « marquer » (par un booléen placé à vrai) chaque case déjà visitée par l'algorithme, ce qui induit la
// définition suivante :
// Grille de marques (si votre algorithme a besoin de déterminer quelle(s) case(s) a/ont déjà été « explorée(s) ») :
typedef bool TMarques[MAXLIGNES][MAXCOLONNES];

// Pour permettre un jeu « interactif » ou mémoriser une position et/ou un coup joué, il peut être très utile de
// définir les deux types suivants :
// Position sur la grille (cela peut être utile) :
typedef struct {
int ligne, colonne;
} TPosition;

// Un coup joué sur la grille :
typedef struct {
int ligne, colonne;
int symbole; // Numéro du symbole (1 est le premier symbole, 2 le second...)
} TCoupJoue;

// Pile des coups joués :
typedef TCoupJoue TTabCoupJoue[MAXCOLONNES*MAXLIGNES]; // Nombre de coups au maxi
typedef struct {
  int sommet; // Sommet de pile
  TTabCoupJoue tab; // Tableau contenant la pile
} TPileCoupsJoues;

//-----------------------------------------------------------------------------
// Calcule la racine entière du nombre passé en paramètre.
//
// Retourne 0 si le nombre passé en paramètre est inférieur à 4
// ou s'il n'est pas un carré.
// Retourne la racine carrée du nombre passé en paramètre sinon.
inline int RacineEntiere(const int NombreAuCarre)
{
  int RacineEntiere = sqrt(NombreAuCarre);
  if ((NombreAuCarre < 4) || (RacineEntiere * RacineEntiere != NombreAuCarre))
    return 0;
  else
    return RacineEntiere;
}

// Affiche une erreur sur le canal prévu à cet effet (stderr).
// Le paramètre "stoppersauvagement" indique si cette procédure stoppe
// "sauvagement" le programme en utilisant exit ou non :
// - si stoppersauvagement est différent de 0, on utilise exit()
// - si stoppersauvagement est égal à 0, on retourne -1
//   et exit n'est pas utilisé
int AfficheErreur(char *message, int stoppersauvagement)
{
  fprintf(stderr,"#\n# ERREUR: %s !\n#\n", message);
  if (stoppersauvagement != 0)
    exit(1);
  return -1;
}

// Initialise/"vide" la pile des coups joués
void initPileCoupsJoues(TPileCoupsJoues *pile)
{
  pile->sommet = -1; // Signale pile vide
}

// Position du sommet de pile de la pile des coups joués
int lireSommetPileCoupsJoues(TPileCoupsJoues pile)
{
  return (pile.sommet);
}

// Nombre de coups joués mémorisés en pile
int nombreCoupsJoues(TPileCoupsJoues pile)
{
  return (pile.sommet + 1);
}

// Retourne vrai si la pile est vide
// faux sinon
bool pileVide(TPileCoupsJoues pile)
{
  return pile.sommet < 0;
}

// Empile le (dernier) coup joué
// Mode préincrémenté
//
// ATTENTION : AUCUN CONTROLE EFFECTUE
//
void empileCoupJoue(TPileCoupsJoues *pile,
		    int ligne,
		    int colonne,
		    int symbole)
{
  // TODO: Vérifier si la pile n'est pas pleine
  // Vérification pile pleine = en cours !

    //if(pile->sommet == )
    //{
      pile->sommet++;
      pile->tab[pile->sommet] = (TCoupJoue){ligne, colonne, symbole};
    //}
}

// Dépile le (dernier) coup joué
// Mode post décrémenté
//
void depileCoupJoue(TPileCoupsJoues *pile,
		    int *ligne,
		    int *colonne,
		    int *symbole)
{
  // Vérification pile vide = fait !
  if(pile->sommet >= 0)
  {
      TCoupJoue coupjoue = pile->tab[pile->sommet];

      *ligne = coupjoue.ligne;
      *colonne = coupjoue.colonne;
      *symbole = coupjoue.symbole;
      pile->sommet--;
  }
  else
    puts("La pile est vide\n");
}

// Convertit un N° de plan (entre 1 et nbsymboles)
// en un caractère utilisable dans la grille de jeu
//
// Retourne '\0' si le numéro de plan est incorrect
//
char plan2char(const char *symboles, const int nbsymboles,
	       const int plan)
{
  if (plan < 1 ||  plan > nbsymboles)
    {
        return '0';
    }
    else
        return symboles[plan];
}

// Convertit un caractère (symbole situé dans la grille de jeu)
// en un N° de plan (entre 1 et GTaille)
// en un caractère utilisable dans la grille de jeu
//
// La suite des symboles n'est pas nécessairement triée, ce qui
// fait que cette fonction utilise une recherche séquentielle dans
// un tableau d'éléments non triés (non efficace).
//
// Retourne 0 si le caractère incorrect
//
// Remarque : Cette fonction peut être utilisée pour vérifier si le
// symbole saisi par l'utilisateur, lors de la saisie d'un coup à jouer,
// est correct (code de retour > 0) ou non (code de retour = 0)
//
int char2plan(const char * symboles, const int nbsymboles,
	      const char code)
{
    int p;
    for (p=1; p<=nbsymboles; p++)
    {
        if (code == symboles[p])
            return p;
    }
    return 0; //pas trouvé !
}

// Initialise la grille 2D en la remplissant de caractères passés via le paramètre s0.
// TODO: Ajouter un code de retour (si nécessaire)
void InitGrille2D(TGrille2D grille2D, char s0)
{
  int i, j;
  for (i=0; i<MAXLIGNES; i++)
    for (j=0; j<MAXCOLONNES; j++)
      grille2D[i][j] = s0;
}

// Initialise/vide la grille des compteurs passée en paramètre
void InitGrille3D(TGrille3D grille3D)
{
  int i, j, k;
  for (i=0; i<MAXLIGNES; i++)
    for (j=0; j<MAXCOLONNES; j++)
      for (k=0; k<=MAXPLANS; k++)
	grille3D[i][j][k] = 0;
}

// Fonction interne (i.e. non visible dans l'API présentée à l'utilisateur).
// AfficherGrilleRegionsSeparation affiche une ligne de séparation pour mettre
// en forme le contenu de la grille comme un tableau
// de "lignes" lignes de "colonnes" caractères chacune,
// tailleregion est la taille de région (forcément > 1) :
//    2, 3, 4 ou 5 pour une grille avec régions
// TODO: Ajouter un code de retour (si nécessaire)
int AfficherGrilleRegionsSeparation(int lignes, int colonnes,
				    int tailleregion, int numeroligne)
{
  int j;

  if (numeroligne < 0 || numeroligne >= lignes)
    return AfficheErreur("AfficherGrilleRegionsSeparation: Numéro de ligne incorrect", STOPRETURN);

  printf("*"); // Bord gauche
  // La ligne de séparation change en fonction de la taille des régions
  // (en fonction du numéro de ligne)
  if ((numeroligne % tailleregion) != 0)
  {
    for (j=0; j<colonnes; j++)
    {
      if ((j % tailleregion) != 0)
	printf("+---");
      else
	printf("*---");
    }
    if ((colonnes % tailleregion) != 0)
      puts("|*");
    else
      puts("**");
  }
  else
  {
    for (j=0; j<colonnes; j++)
      printf("****");
    puts("**");
  }

  return 0;
}

// Fonction interne (i.e. non visible dans l'API présentée à l'utilisateur).
// AfficherGrilleRegionsDonnees affiche une ligne de donnees issues du
// contenu de la grille stockée dans un tableau
// de "lignes" lignes de "colonnes" caractères chacune,
// tailleregion est la taille de région (forcément > 1) :
//    2, 3, 4 ou 5 pour une grille avec régions
// TODO: Ajouter un code de retour (si nécessaire)
int AfficherGrilleRegionsDonnees(TGrille2D grille2D, int lignes, int colonnes,
				 int tailleregion, int numeroligne)
{
  int j;

  if (numeroligne < 0 || numeroligne >= lignes)
    return AfficheErreur("AfficherGrilleRegionsDonnees: Numéro de ligne incorrect", STOPRETURN);

  printf("*"); // Bord gauche
  for (j=0; j<colonnes; j++)
  {
    if ((j % tailleregion) != 0)
      printf("| %c ", grille2D[numeroligne][j]);
    else
      printf("* %c ", grille2D[numeroligne][j]);
  }
  if ((colonnes % tailleregion) != 0)
    puts("|*");
  else
    puts("**");

  return 0;
}

// AfficherGrille affiche le contenu de la grille comme un tableau
// de "lignes" lignes de "colonnes" caractères chacune,
// tailleregion est la taille de région (forcément > 1).
// TODO: Ajouter/gérer un code de retour (si nécessaire)
int AfficherGrille2D(TGrille2D grille2D, int lignes, int colonnes, int tailleregion)
{
  int i, j;

    // Première ligne au dessus de la grille:
    for (j=0; j<2+4*colonnes; j++)
      printf("*");
    puts("*");

    for (i=0; i<lignes; i++)
    {
      AfficherGrilleRegionsSeparation(lignes, colonnes, tailleregion, i);
      AfficherGrilleRegionsDonnees(grille2D, lignes, colonnes, tailleregion, i);
    } // for i

    // Dernières lignes en dessous de la grille:
    for (j=0; j<2+4*colonnes; j++)
      printf("*");
    puts("*");
    for (j=0; j<2+4*colonnes; j++)
      printf("*");
    puts("*");

  return 0;
}

// Lecture de la taille de la grille stockée dans le fichier
// dont le nom est passé en paramètre.
//
// Dans cette version, seules les grilles carrées sont autorisées
//
// Retourne 0 si :
// - le fichier n'existe pas,
// - la taille est < 4,
// - le fichier ne commence pas par deux entiers positifs,
// - ou si le nombre de colonnes est différent du nombre de lignes
// Retourne le nombre de lignes (et de colonnes) dans les autres cas.
//
// ATTENTION : Cette fonction ouvre et ferme le fichier.
//
int lireTailleGrille2D(const char *Nom)
{
  FILE *fgrille; // Fichier contenant la grille
  int l, c;

  if ((fgrille = fopen(Nom, "r")) == NULL)
    return 0; // Fichier introuvable !

  if (fscanf(fgrille, "%d %d", &l, &c) != 2) {
    fclose(fgrille);
    return 0; // Impossible de lire deux entiers dans le fichier
  }

  if ((l < 4) || (c < 4) || (l != c)) {
    fclose(fgrille);

    return 0; // Valeurs incorrectes ou grille non carrée
  }

  fclose(fgrille); // On ferme... !!!

  return l; // Comme l == c il est possible de retourner l'un ou l'autre
}


// LireGrille permet de lire une grille de jeu depuis un fichier.
// Le format du fichier lu doit permettre de considérer différents types de
// jeux comme expliqué en tête de ce programme.
// "Nom" est le nom du fichier contenant la grille ;
// "grille2D" est le tableau 2D qui doit recevoir la grille ;
// "lignes" et "colonnes" contiennent la taille de la grille lue ;
// "tailleregion" contient la taille de région lue depuis le fichier ;
// "carvide" est le caractère caractere_vide_dans_fichier qui correspond
// à une case vide dans la grille stockée dans le fichier ;
// "symboles" contient le tableau de caractères correspondant aux symboles
// autorisés dans la grille de jeu, avec le premier symbole "s0"
// qui correspond à une case vide affichée à l'écran dans la grille.
//
// TODO: Ajouter et gérer un code de retour (si nécessaire)
// TODO: Dans cette version, la fonction LireGrille est stoppée "sauvagement"
// via un "exit" dans la fonction AfficheErreur
// --> Utilisez AfficheErreur avec la valeur "0" pour second paramètre
// et récupérez proprement les codes de retour en informant au final
// l'utilisateur (i.e. LireGrille retourne -1 en cas d'erreur).
// N'oubliez pas de "fermer" le fichier si l'erreur se produit alors que
// le fichier a pu être ouvert !
int LireGrille2D(char *Nom, TGrille2D grille2D, int *lignes, int *colonnes,
	       int *tailleregion, char *carvide, char *symboles)
{
  FILE *fgrille;
  int l, c;
  char chainecarvide[4]; // Prévu pour 3 caractères + \0 pour lire carvide

  if((fgrille = fopen(Nom, "r")) == NULL)
    AfficheErreur("LireGrille: fichier introuvable", STOPEXIT);

  if(fscanf(fgrille, "%d %d %d", lignes, colonnes, tailleregion) != 3)
    AfficheErreur("LireGrille: lignes et/ou colonnes et/ou tailleregion introuvable(s) dans le fichier", STOPEXIT);

  // Une grille inférieure à 4*4 est incorrecte
  if (*lignes < 4 || *colonnes < 4 || *lignes > MAXLIGNES || *colonnes > MAXCOLONNES ||
      *tailleregion < 0 || *tailleregion > *lignes || *tailleregion > *colonnes)
    AfficheErreur("LireGrille: Valeur incorrecte de lignes et/ou colonnes et/ou taillerégion dans le fichier", STOPEXIT);

  if (*tailleregion > 1 && (((*lignes % *tailleregion) != 0) || ((*colonnes % *tailleregion) != 0)))
    AfficheErreur("LireGrille: lignes et/ou colonnes doivent être multiples de la taille de la région", STOPEXIT);

  // Ci-dessous l'utilisation de "%c" complique le code
  // --> Il est préférable de passer par "%s" et de récupérer le second caractère s'il existe.
  // --> Si vous connaissez fgets, c'est préférable à scanf car plus rapide,
  //     + permet de fixer la taille de chaine de caractères
  //     + de savoir combien de CARACTERES ont été lus
  if(fscanf(fgrille, "%3s", chainecarvide) != 1)
    AfficheErreur("LireGrille2D: <carvide> introuvable dans le fichier", STOPEXIT);

  if (strlen(chainecarvide) != 3)
    AfficheErreur("LireGrille2D: Format incorrect pour <carvide> dans le fichier", STOPEXIT);

  *carvide = chainecarvide[1]; // Récupère le second caractère dans <carvide>

  // --> Si vous connaissez fgets, c'est préférable à scanf car plus rapide,
  //     + permet de fixer la taille de chaine de caractères
  //     + de savoir combien de CARACTERES ont été lus
  //     Ici cela donnerait qqchose du genre :
  //     fgets(symboles, MAXSYMBOLES, fgrille);
  //
  // ATTENTION : Ci-dessous, il faudrait limiter la taille de chaine de caractères
  // à MAXSYMBOLE pour éviter problèmes de type "débordement de buffer"
  // --> Est-ce possible avec fscanf ?
  if(fscanf(fgrille, "%s", symboles) != 1)
    AfficheErreur("LireGrille2D: symboles introuvables dans le fichier", STOPEXIT);

  for (l=0; l<*lignes; l++)
  {
    for (c=0; c<*colonnes; c++)
    {
      if (feof(fgrille) || (fscanf(fgrille, " %c", &grille2D[l][c]) != 1))
	AfficheErreur("LireGrille: donnée manquante dans le fichier", STOPEXIT);
      // Premier remplacement : Remplacer carvide par s0 ?
      if (grille2D[l][c] == *carvide)
	grille2D[l][c] = symboles[0];
      // Deuxième remplacement : Remplacer SYMBOLESPACE par espace (' ') ?
      if (grille2D[l][c] == SYMBOLESPACE)
	grille2D[l][c] = ' ';
    }  // for c
  } // for l

  fclose(fgrille);
  return 0;
}

// EcrireGrille2D permet d'écrire une grille2D de jeu dans un fichier.
// Le format du fichier écrit doit permettre de considérer différents types de
// jeux comme expliqué en tête de ce programme.
// "Nom" est le nom du fichier contenant la grille 2D ;
// "grille2D" est le tableau 2D qui doit recevoir la grille ;
// "lignes" et "colonnes" contiennent la taille de la grille lue ;
// "tailleregion" contient la taille de région lue depuis le fichier ;
// "carvide" est le caractère caractere_vide_dans_fichier qui correspond
// à une case vide dans la grille stockée dans le fichier ;
// "symboles" contient le tableau de caractères correspondant aux symboles
// autorisés dans la grille de jeu, avec le premier symbole "s0"
// qui correspond à une case vide affichée à l'écran dans la grille.
//
// TODO: Ajouter et gérer un code de retour (si nécessaire)
// TODO: Dans cette version, la fonction EcrireGrille est stoppée "sauvagement"
// via un "exit" dans la fonction AfficheErreur
// --> Utilisez AfficheErreur avec la valeur "0" pour second paramètre
// et récupérez proprement les codes de retour en informant au final
// l'utilisateur (i.e. LireGrille retourne -1 en cas d'erreur).
// N'oubliez pas de "fermer" le fichier si l'erreur se produit alors que
// le fichier a pu être ouvert !
int EcrireGrille2D(char *Nom, TGrille2D grille2D, int lignes, int colonnes,
		 int tailleregion, char carvide, char *symboles)
{
  FILE *fgrille;
  int l, c;
  char element; // Element en position [l](c] de la grille

  if((fgrille = fopen(Nom, "w")) == NULL)
    AfficheErreur("EcrireGrille2D: impossible de créer le fichier", STOPEXIT);

  if(fprintf(fgrille, "%d %d %d\n", lignes, colonnes, tailleregion) <= 0)
    AfficheErreur("EcrireGrille2D: impossible d'écrire lignes et/ou colonnes et/ou tailleregion dans le fichier", STOPEXIT);

  // Une grille inférieure à 4*4 est incorrecte
  if (lignes < 4 || colonnes < 4 || lignes > MAXLIGNES || colonnes > MAXCOLONNES ||
      tailleregion < 0 || tailleregion > lignes || tailleregion > colonnes)
    AfficheErreur("EcrireGrille2D: Valeur incorrecte de lignes et/ou colonnes et/ou taillerégion dans le fichier", STOPEXIT);

  if (tailleregion > 1 && (((lignes % tailleregion) != 0) || ((colonnes % tailleregion) != 0)))
    AfficheErreur("EcrireGrille2D: lignes et/ou colonnes doivent être multiples de la taille de la région", STOPEXIT);

  if(fprintf(fgrille, "<%c>\n", carvide) != 4)
    AfficheErreur("EcrireGrille2D: Impossible d'écrire <carvide> dans le fichier", STOPEXIT);

  // ATTENTION : Ci-dessous, il faudrait limiter la taille de chaine de caractères
  // à MAXSYMBOLE pour éviter problèmes de type "débordement de buffer"
  // --> Est-ce possible avec fprintf ?
  if(fprintf(fgrille, "%s\n", symboles) <= 0)
    AfficheErreur("EcrireGrille2D: Impossible d'écrire symboles dans le fichier", STOPEXIT);

  for (l=0; l<lignes; l++)
  {
    for (c=0; c<colonnes; c++)
    {
      // Comme pour la lecture du fichier où il a fallut faire des conversions
      // pour les caractères vides, il faut procéder à l'envers pour l'écriture
      // des fichiers contenant les grilles de jeu :
      element =  grille2D[l][c];
      // Premier remplacement : Remplacer espace (' ') par SYMBOLESPACE ?
      if (element == ' ')
        element = SYMBOLESPACE;
      // Deuxième remplacement : Remplacer s0 par carvide ?
      if (element == symboles[0])
	element = carvide;

      //
      if (c != colonnes-1)
      {
	// Nous ne sommes pas en dernière colonne --> Ajouter un espace
	if (fprintf(fgrille, "%c ", element) != 2)
	  AfficheErreur("EcrireGrille2D: Impossible d'écrire la grille de jeu dans le fichier", STOPEXIT);
      }
      else
      {
	// Nous sommes en dernière colonne --> Ne pas Ajouter d'espace, mais un retour chariot
	if (fprintf(fgrille, "%c\n", element) != 2)
	  AfficheErreur("EcrireGrille2D: Impossible d'écrire la grille de jeu dans le fichier", STOPEXIT);
      }
    } // for c
  } // for l

  fclose(fgrille);
  return 0;
}

// Affiche la grille relative au plan N°"plan".
// Le plan "0" contient le cumul des possibilités par case.
// Les plans 1 à 9 indiquent les possibilités de placement
// du symbole N°1 à nbsymboles.
void AfficheGrille3DPlan(TGrille3D grille3D, int nlignes, int ncolonnes, int plan)
{
  int l, c;

  for (c = 0; c < ncolonnes; c++)
    printf("---+");
  puts("");
  for (l = 0; l < nlignes; l++)
  {
    for (c = 0; c < ncolonnes; c++)
      printf("%2d |", grille3D[l][c][plan]);
    puts("");
    for (c = 0; c < ncolonnes; c++)
      printf("---+");
    puts("");
  }
  puts("");
}

// Retourne vrai (1) si le symbole N°"plan" peut être placé
// dans la ligne "ligne" sur la grille de jeu
// faux (0) sinon
// (plan doit être compris entre 1 et MAXPLANS)
bool symboleautoriseenligne(TGrille2D grille2D, int GTaille,
			    int ligne, int plan,
			    const char *symboles, const int nbsymboles)
{
    int i;
  char carplan; //N° plan

  carplan=plan2char(symboles,nbsymboles,plan);
  for(i=0;i<GTaille;i++)
  {
    if(grille2D[ligne][i] == carplan)
        return false;
  }

  return true;
}

// Retourne vrai (1) si le symbole N°"plan" peut être placé
// dans la colonne "colonne" sur la grille de jeu
// faux (0) sinon
// (plan doit être compris entre 1 et MAXPLANS)
bool symboleautoriseencolonne(TGrille2D grille2D, int GTaille,
			      int colonne, int plan,
			      const char *symboles, const int nbsymboles)
{
  int i;
  char carplan; //N° plan

  carplan=plan2char(symboles,nbsymboles,plan);
  for(i=0;i<GTaille;i++)
  {
    if(grille2D[i][colonne] ==carplan)
        return false;
  }

  return true;
}

// Retourne vrai (1) si le symbole N°"plan" peut être placé
// dans la région à laquelle appartient le couple "(ligne, colonne)"
// faux (0) sinon
// (plan doit être compris entre 1 et GTaille)
//
// ligne et colonne doivent être comprises entre 0 et GTaille-1
// TODO : A verifier !
//
bool symboleautoriseenregion(TGrille2D grille2D,
			     const unsigned int RTaille,
			     const unsigned int ligne,
			     const unsigned int colonne,
			     const unsigned int plan,
			     const char *symboles, const int nbsymboles)
{
  unsigned int rligne, rcolonne; // Position en haut à gauche de la région
  unsigned int ltmp, ctmp; // Calcul temporaire du multiple de RTaille
  unsigned int m, n; // ligne "modulo RTaille" et colonne "modulo RTaille"

  char carplan; // Caractère correspondant au plan N°plan

  carplan = plan2char(symboles, nbsymboles, plan);

  // si ligne est comprise entre 1 et GTaille
  // et si colonne est comprise entre 1 et GTaille.
  // Pour une grille 9*9 et des régions 3*3,
  // lorsque ligne ou colonne (lc) vaut 1 à GTaille,
  // rligne ou rcolonne (rlc) vaut :
  // lc = 1 --> rlc = 1 | lc-1 = 0 | (lc-1)/RTaille = 0 | 1+[(lc-1)/RTaille]*RTaille = 1 = rlc
  // lc = 2 --> rlc = 1 | lc-1 = 1 | (lc-1)/RTaille = 0 | 1+[(lc-1)/RTaille]*RTaille = 1 = rlc
  // lc = 3 --> rlc = 1 | lc-1 = 2 | (lc-1)/RTaille = 0 | 1+[(lc-1)/RTaille]*RTaille = 1 = rlc
  // lc = 4 --> rlc = 4 | lc-1 = 3 | (lc-1)/RTaille = 1 | 1+[(lc-1)/RTaille]*RTaille = 4 = rlc
  // lc = 5 --> rlc = 4 | lc-1 = 4 | (lc-1)/RTaille = 1 | 1+[(lc-1)/RTaille]*RTaille = 4 = rlc
  // lc = 6 --> rlc = 4 | lc-1 = 5 | (lc-1)/RTaille = 1 | 1+[(lc-1)/RTaille]*RTaille = 4 = rlc
  // lc = 7 --> rlc = 7 | lc-1 = 6 | (lc-1)/RTaille = 2 | 1+[(lc-1)/RTaille]*RTaille = 7 = rlc
  // lc = 8 --> rlc = 7 | lc-1 = 7 | (lc-1)/RTaille = 2 | 1+[(lc-1)/RTaille]*RTaille = 7 = rlc
  // lc = 9 --> rlc = 7 | lc-1 = 8 | (lc-1)/RTaille = 2 | 1+[(lc-1)/RTaille]*RTaille = 7 = rlc
  ltmp = ligne / RTaille;
  ctmp = colonne / RTaille;
  rligne = ltmp * RTaille;
  rcolonne = ctmp * RTaille;
  for (m=0; m<RTaille; m++)
    for (n=0; n<RTaille; n++)
      if (grille2D[rligne+m][rcolonne+n] == carplan)
	return false; // Inutile de continuer, le résultat est "faux"

  return true;
}

// Mise à jour des plans.
// Pour un sudoku "classique" (9x9) : plans 1 à 9 de la grille 3D pour énumérer
// les possibilités de placer les chiffres 1 à 9 (1 chiffre par plan "p").
// Pour les sudoku de taille > 9x9 les chiffres sont remplacés par des symboles
// (caractères) à raison d'un symbole associé à chaque plan.
//
// --> Pour chaque case de la grille, il faut vérifier si
// 1) la case est vide (sinon plus rien à placer -> plans 1 à 9 contiennent
//    "carvide" pour cette case)
// 2) le chiffre correspondant au plan p est autorisé dans la ligne courante
// 3) le chiffre correspondant au plan p est autorisé dans la colonne courante
// 4) le chiffre correspondant au plan p est autorisé dans la région courante
//
// Cette procédure met à jour les compteurs des plans 1 à 9.
// Elle met à jour le cumul des possibilités (plan 0).
//
// Cette procédure est très très peu efficace !!!
// Il y a moyen de l'améliorer et de l'accélérer fortement !!!
//
// ATTENTION au caractère vide : A l'extérieur "carvide" est le caractère vide
// DANS LE FICHIER, mais dans la grille c'est symboles[0]
// sauf si symboles[0] == SYMBOLESPACE auquel cas il est remplacé
// par un espace --> Ceci est géré par une macro
//
// TODO: Ne fonctionne qu'avec des régions 3*3 --> Généraliser comme pour Hexadoku/L2
//
void miseajourPlans(TGrille2D grille2D, TGrille3D grille3D,
		    int GTaille, int RTaille, char carvide,
		    const char *symboles, const int nbsymboles)
{
  int l, c, p; // Lignes, colonnes, plans

  for (l = 0; l < GTaille; l++)
  {
    for (c = 0; c < GTaille; c++)
    {
      grille3D[l][c][0] = 0; // RAZ cumul possibilités
      for (p = 1; p <= MAXPLANS; p++)
      {
	grille3D[l][c][p] =
	  ((grille2D[l][c] == carvide) &&
	   (symboleautoriseenligne(grille2D, GTaille, l, p, symboles, nbsymboles)) &&
	   (symboleautoriseencolonne(grille2D, GTaille, c, p, symboles, nbsymboles)) &&
	   (symboleautoriseenregion(grille2D, RTaille, l, c, p, symboles, nbsymboles)));
	grille3D[l][c][0] += grille3D[l][c][p]; // cumul possibilités par case
      }
    }
  }
}

// Affiche les possibilités de jeu restant pour chaque case
// (sans garantir que cela conduise à une solution globale !!!).
//
// ATTENTION :
// grille[0][0] correspond à la case en haut à droite de la grille de jeu,
// mais grille3D[0][0][p] correspond à cette [même] case
// dans le plan p de la grille3D
void afficheGrillePossibilites(TGrille3D grille3D,
				 const int GTaille,
				 const int RTaille,
				 const char *symboles, const int nbsymboles)
{
  int l, c; // ligne colonne
  int m, n; // ligne "modulo RTaille" et colonne "modulo RTaille"

  for (c = 0; c < GTaille; c++) {
    for (m = 0; m < RTaille-1; m++)
      printf("--");
    printf("-+");
  }
  puts("");
  for (l = 0; l < GTaille; l++)
  {
    for (m = 0; m < RTaille; m++)
    {
      for (c = 0; c < GTaille; c++) {
	// Il faut convertir chaque numéro de plan en symbole
	printf("%c", (grille3D[l][c][1+m*RTaille]==0?' ':plan2char(symboles, nbsymboles, 1+m*RTaille)));
	for (n = 1; n < RTaille; n++) {
	  // Il faut convertir chaque numéro de plan en symbole
	  printf(" %c", (grille3D[l][c][1+n+m*RTaille]==0?' ':plan2char(symboles, nbsymboles, 1+n+m*RTaille)));
	}
	printf("|");
      }
      puts("");
    }
    for (c = 0; c < GTaille; c++) {
      for (m = 0; m < RTaille-1; m++)
	printf("--");
      printf("-+");
    }
    puts("");
  }
  puts("");
}


// Joue un coup sur la grille et le mémorise dans la pile
//
// Le symbole joué est représenté par son N°plan (attention [1..GTaille]).
// ligne et colonne doivent être comprises entre 0 et GTaille-1)
//
// ATTENTION : AUCUN CONTROLE EFFECTUE
//
// Appelle systématiquement miseajourPlans() même si ce n'est pas efficace
//
void jouerUnCoup(TGrille2D grille2D, TGrille3D grille3D,
		 const int GTaille, const int RTaille,
		 const char *symboles, const int nbsymboles,
		 const int ligne, const int colonne,
		 const int plan,
		 TPileCoupsJoues *pile)
{
  // Ci-dessous nous ajoutons 1 à ligne et colonne pour que l'utilisateurs
  // "voit" les lignes et les colonnes numérotées à partir de 1
  printf("jouerUnCoup: place %d en (%d, %d)\n", plan, ligne+1, colonne+1);

  // Placer le symbole sur la grille de jeu
  grille2D[ligne][colonne] = plan2char(symboles, nbsymboles, plan);
  empileCoupJoue(pile, ligne, colonne, plan); // "Mémorise le coup joué"
  miseajourPlans(grille2D, grille3D, GTaille, RTaille, CARGRILLEVIDE,
		 symboles, nbsymboles);
}

// Annule le dernier coup sur la grille et le supprime de la pile
//
// ATTENTION : CETTE PROCEDURE REQUIERT UN CALCUL COMPLET DES COMPTEURS
// DES POSSIBILITES DE JEU
// Ceci n'est pas effectué dans/par la procédure "annuleruncoup" pour permettre
// d'accélerer les annulations "en cascade".
// Il n'est pas possible de procéder simplement pour mettre à jour les
// compteurs de manière locale car l'annulation d'un coup implique la
// possibilité de rejouer ce symbole dans la région contenant la position
// courante (ligne, colonne), et il faut que toutes les contraintes/compteurs
// soient remises à jour pour réautoriser UNIQUEMENT EN CERTAINES POSITIONS
// de la région concernée à l'issue de l'annulation, CQFD.
//
// ATTENTION : AUCUN CONTROLE EFFECTUE
//
// Appelle systématiquement miseajourPlans() même si ce n'est pas efficace
//
void annulerUnCoup(TGrille2D grille2D, TGrille3D grille3D,
		   const int GTaille, const int RTaille,
		   const char *symboles, const int nbsymboles,
		   TPileCoupsJoues *pile)
{
  int ligne, colonne, plan;

  depileCoupJoue(pile, &ligne, &colonne, &plan); // Dépile dernier coup joué
  // Ci-dessous nous ajoutons 1 à ligne et colonne pour que l'utilisateurs
  // "voit" les lignes et les colonnes numérotées à partir de 1
  printf("annulerUnCoup: annule le placement de %d en (%d, %d)\n",
	 plan, ligne+1, colonne+1);
  grille2D[ligne][colonne] = CARGRILLEVIDE; // Enlève le symbole de la grille de jeu
  miseajourPlans(grille2D, grille3D, GTaille, RTaille, CARGRILLEVIDE,
		 symboles, nbsymboles);
}

// Saisit un coup à jouer
//
// TODO : Vérifier que les lignes colonnes sont correctement saisies en récupérant le code de retour de scanf...
//
void saisirUnCoup(const int GTaille, const char *symboles, const int nbsymboles,
		  int *ligne, int *colonne, int *plan)
{
  char symbole[2]; // Symbole saisi sous forme d'une chaine de 2 caractères

  do {
    puts("Veuillez entre le coup à jouer sous la forme : ligne (>=1) colonne (>=1) symbole");
    scanf("%d %d %1s", ligne, colonne, symbole);

    //Vérification numéro ligne
    if(*ligne < 1 || *ligne > GTaille)
    {
        puts("Numéro de ligne invalide ! recommencez");
        scanf("%d %d %1s", ligne, colonne, symbole);
    }

    //Vérification numéro colonne
    if(*colonne < 1 || *colonne > GTaille)
    {
        puts("Numéro de colonne invalide ! recommencez");
        scanf("%d %d %1s", ligne, colonne, symbole);
    }

    if(symbole[0] <= 0 || symbole[0] > 'G' )
    {
        puts("Symbole invalide ! recommencez");
        scanf("%d %d %1s", ligne, colonne, symbole);

        // TODO /!\ BUG SAISIE G$ par exemple fait bug le programme
    }

    // TODO : Récupérez et traitez en conséquence le code de retour de scanf
    // TODO : Indiquez à l'utilisateur la raison du refus éventuel du coup joué
  } while (*ligne < 1 || *ligne > GTaille || *colonne < 1 || *colonne > GTaille ||
	   char2plan(symboles, nbsymboles, symbole[0]) <= 0);

  // En ce point la saisie est OK a priori --> Calculons le N° de symbole/plan
  *plan = char2plan(symboles, nbsymboles, symbole[0]);
}

// Vérifie si la grille est complête ou non.
// Cette fonction est nécessaire car le plan 0 de la grille3D peut être rempli
// de 0 dans deux cas : soit il n'y a plus aucune possibilité de jeu, mais la
// grille (2D) n'est pas pleine en raison de l'impossibilité de complêter la
// grille (grille infaisable) ou la grille (2D) est pleine et en ce cas la
// résolution est terminée (grille faisable, et trouvée !).
// Cette fonction détecte le cas où la grille est terminée & faisable.
//
bool grille2DComplete(TGrille2D grille2D,
		    const int GTaille, const char *symboles)
{
  int l, c, cumul = 0;

  for (l = 0; l < GTaille; l++)
    for (c = 0; c < GTaille; c++)
      if (grille2D[l][c] != CARGRILLEVIDE)
	cumul++;
  return cumul == GTaille * GTaille;
}

///////////////////////////////MENU SELECTION NIVEAU ... //////////////////////

void choix_menu()
{
    printf ( "choix 1 : Jouer un coup\n") ;
    printf ( "choix 2 : He\n") ;
    printf ( "choix 3 : \n") ;
    printf ( "choix 4 : \n") ;
    printf ( "choix 0 : fin\n") ;

}

int menu()
{
    int c;
        do{
            choix_menu();
            scanf("%d",&c);
            switch(c)
            {
                case 1:
                        saisirUnCoup(GTaille, symboles, nbsymboles, &ligne, &colonne, &plan);
                        jouerUnCoup(grille2D, grille3D, GTaille, RTaille,
                        symboles, nbsymboles,
                        ligne, colonne, plan,
                        &pile);
                        AfficherGrille2D(grille2D, lignes, colonnes, tailleregion);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 0: printf("fin\n");
            }
        }while(c!=0);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////// PROGRAMME PRINCIPAL /////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  TGrille2D grille2D;   // Grille de jeu (2D)
  TGrille3D grille3D;   // Grille 3D
  TPileCoupsJoues pile; // Pile des coups joués
  int lignes, colonnes; // Nombre lignes et de colonnes dans la grille de jeu
  int nbsymboles;       // Nombre symboles non vides (i.e. plans dans la grille de jeu)
  int tailleregion;     // Taille des régions dans la grille de jeu
  char carvide;         // Caractère idenfiant case vide dans grille stocké dans fichier
  char symboles[MAXSYMBOLES+1]; // Symboles autorisés dans la grille de jeu

                                // (+1 : pour stocker \0 car lu comme une chaine de caractères)
  int ligne, colonne; // Ligne et colonne pour le coup saisi
  int plan;                // Variable de boucle pour énumérer les plans 1 à MAXPLANS
  int GTaille = 0; // Taille de la grille (4, 9, 16, 25...)
  int RTaille = 0; // Taille d'une région (2, 3,  4,  5...)
  //char *nomgrille="grille09x09_00.dat";
  if (argc != 2)
    AfficheErreur("hexadoku: nom de fichier grille manquant (en argument sur la ligne de commande)", STOPEXIT);

  GTaille = lireTailleGrille2D(argv[1]);
  if (GTaille == 0)
    AfficheErreur("hexadoku: fichier grille introuvable ou taille de grille incorrecte (en argument sur la ligne de commande)", STOPEXIT);

menu();

  RTaille = RacineEntiere(GTaille);
  printf("# Sudoku de taille %ux%u - %d régions de taille %dx%d\n",
	 GTaille, GTaille, GTaille, RTaille, RTaille);

  // TODO: Vérifier si la taille de la grille ne dépasse pas 25

  InitGrille2D(grille2D, '.');
  InitGrille3D(grille3D);
  initPileCoupsJoues(&pile);

  puts("\nHexadoku :");
  puts("------------");

  LireGrille2D(argv[1], grille2D, &lignes, &colonnes,
	     &tailleregion, &carvide, symboles);
  nbsymboles = strlen(symboles) - 1; // -1 pour décompter symbole vide/N°0
  printf("Car vide : <%c>\n", carvide);
  printf("Symboles : <%s>\n", symboles);
  printf("Régions  : %d x %d\n", tailleregion, tailleregion);
  printf("NSymboles: %d\n\n", nbsymboles);
  AfficherGrille2D(grille2D, lignes, colonnes, tailleregion);
  EcrireGrille2D("sudoku_bak.dat", grille2D, lignes, colonnes,
	       tailleregion, carvide, symboles);

  miseajourPlans(grille2D, grille3D, GTaille, RTaille, CARGRILLEVIDE,
		 symboles, nbsymboles);

 /* for (plan=1; plan<=nbsymboles; plan++)
  {
    printf("Possibilités de placement du symbole %d :\n\n", plan);

    AfficheGrille3DPlan(grille3D, lignes, colonnes, plan);
  }

  afficheGrillePossibilites(grille3D, GTaille, RTaille,
		      symboles, nbsymboles);*/

  do
  {
     saisirUnCoup(GTaille, symboles, nbsymboles, &ligne, &colonne, &plan);

    // Vérifier si le coup est valide et le jouer si oui, l'invalider sinon
    ligne--;
    colonne--;

    // Le test est le même que dans miseajourPlans()
    if ((grille2D[ligne][colonne] == CARGRILLEVIDE) &&
	(symboleautoriseenligne(grille2D, GTaille, ligne, plan, symboles, nbsymboles)) &&
	(symboleautoriseencolonne(grille2D, GTaille, colonne, plan, symboles, nbsymboles)) &&
	(symboleautoriseenregion(grille2D, RTaille, ligne, colonne, plan, symboles, nbsymboles)))
    {
        puts("Coup valide");
      // grille2D[ligne][colonne] = plan2char(symboles, nbsymboles, plan);

      jouerUnCoup(grille2D, grille3D, GTaille, RTaille,
		  symboles, nbsymboles,
		  ligne, colonne, plan,
		  &pile);
		  AfficherGrille2D(grille2D, lignes, colonnes, tailleregion);
    }
    else
    {
      puts("Coup invalide, veuillez recommencer");
    }

    //afficheGrillePossibilites(grille3D, GTaille, RTaille,
		//	      symboles, nbsymboles);

  } while (!grille2DComplete(grille2D, GTaille, symboles));

  puts("\nGrille complète, bien joué !");
  AfficherGrille2D(grille2D, lignes, colonnes, tailleregion);

  puts("\nAnnulation des coups joués, en sens inverse :");
  while (!pileVide(pile))
  {
    annulerUnCoup(grille2D, grille3D, GTaille, RTaille,
		  symboles, nbsymboles, &pile);
  }

  return 0;
}
