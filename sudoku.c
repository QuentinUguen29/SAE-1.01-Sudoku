#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3
#define TAILLE N*N
// Déclaration du type tGrille définissant une grille
typedef int tGrille[TAILLE][TAILLE];

// Déclaration des entêtes des fonctions/procédures
void chargerGrille(tGrille g);
bool estGrillePleine(tGrille g);
void afficherGrille(tGrille g);
bool possible(tGrille g, int numLigne, int numColonne, int valeur);
void saisir(int *adr_valeur);
bool estEntier(const char *chaine);
int nbElementsLigne(tGrille g, int lig);
int nbElementsColonne(tGrille g, int col);


void regles();
void bonjour();
void bravo();

//*************************************//
//        PROGRAMME PRINCIPAL          //
//*************************************//

int main() {
  // Définition des variables
  tGrille grille1;
  int numLigne, numColonne, valeur; // Modification ici
  char regle, restart;

  // Affichage du début
  bonjour();
  printf("Prêt à s'amuser ?\n");
  printf("Voulez-vous un récapitulatif des règles ? (O/N) : ");
  scanf(" %c", &regle);

  if (regle == 'O' || regle == 'o') {
    regles();
    printf("\nC'est parti ? (Appuyez sur une touche pour continuer)\n");
    getchar();
    getchar();
  }

  do {
    // Charger une nouvelle grille
    chargerGrille(grille1);
   
    // Tant que la grille n'est pas pleine
    while (!(estGrillePleine(grille1))) {
      printf("\n");
      afficherGrille(grille1);
      
      // Demande de la case
      printf("Indices de la case ?\n");
      printf("\t Ligne : ");
      // Demande de la ligne
      saisir(&numLigne);
      // Demande de la colonne
      printf("\t Colonne : ");
      saisir(&numColonne);

      // Vérifie si la case est vide
      if (grille1[numLigne - 1][numColonne - 1] != 0) {
        printf("IMPOSSIBLE, la case (%d, %d) n'est pas libre.\n", numLigne,
               numColonne);
      } else {
        printf("Valeur à insérer ? ");
        saisir(&valeur);
      }

      // Vérifier si la valeur peut être insérée
      if (possible(grille1, numLigne, numColonne, valeur)) {
        grille1[numLigne - 1][numColonne - 1] = valeur;
      } else if (grille1[numLigne - 1][numColonne - 1] != 0) {
        printf(" ");
      } else {
        printf("La valeur %d ne peut pas être insérée dans la case (%d, %d)\n",
               valeur, numLigne, numColonne);
      }
    }

    // Afficher la grille complétée
    afficherGrille(grille1);
    // Félicitations
    bravo();

    printf("Souhaitez-vous recommencer ? (O/N) : ");
    scanf(" %c", &restart);
  } while (restart == 'O' || restart == 'o');
  return EXIT_SUCCESS;
}

//*************************************//
//      Définition des fonctions       //
//*************************************//

// Charger la grille à partir d'un fichier
void chargerGrille(tGrille g) {
  char nomFichier[30];
  FILE *f;
  do {
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
      printf("\nERREUR sur le fichier %s\n", nomFichier);
    }
  } while (f == NULL);
  fread(g, sizeof(int), TAILLE * TAILLE, f);
  fclose(f);
}

// Afficher la grille
void afficherGrille(tGrille g) {
  // Affichage des n° de colonnes
  printf("     1  2  3   4  5  6   7  8  9\n");
  int colonne, ligne = 0;

  while (ligne < TAILLE) {
    // Affichage des n° de lignes
    if (ligne == 0 || ligne == N || ligne == 2*N) {
      printf("   +---------+---------+---------+\n");
    }
    colonne = 0;
    printf("%d  |", ligne + 1);
    while (colonne < TAILLE) {
      // Affichage des chiffres ou des points (si 0/ case vide)
      if (g[ligne][colonne] == 0) {
        printf(" . ");
      } else {
        printf(" %c ", g[ligne][colonne] + '0');
      }
      // Affichage des barres verticales entre les carrés
      if (colonne == N-1 || colonne == 2*N-1 || colonne == 3*N-1) {
        printf("|");
      }
      if (colonne == 3*N-1){
        printf(" (%d)", nbElementsLigne(g, ligne+1));

      }
      colonne++;
    }

    printf("\n");
    ligne++;
  }
  // Affichage de la dernière ligne
  printf("   +---------+---------+---------+\n");
  printf("    (%d)(%d)(%d) (%d)(%d)(%d) (%d)(%d)(%d)\n", nbElementsColonne(g, 1), nbElementsColonne(g, 2),nbElementsColonne(g, 3),nbElementsColonne(g, 4),nbElementsColonne(g, 5),nbElementsColonne(g, 6),nbElementsColonne(g, 7),nbElementsColonne(g, 8),nbElementsColonne(g, 9));
}

// Vérifier si la grille est pleine
bool estGrillePleine(tGrille g) {
  bool complet = true;
  // Parcours du tableau
  for (int lig = 0; lig < TAILLE && complet; lig++) {
    for (int col = 0; col < TAILLE && complet; col++) {
      // Vérifie si la case est vide
      if (g[lig][col] == 0) {
        complet = false;
      }
    }
  }
  return complet;
}

// Saisir une valeur ENTIERE
void saisir(int *adr_valeur) {
  char resultat[10];  // Modifier la taille selon vos besoins
  // Vérifie que la saisie soit un ENTIER compris en 1 et 9
  while (scanf("%9s", resultat) != 1 || !estEntier(resultat) || (*adr_valeur = atoi(resultat)) < 1 || *adr_valeur > 9) {
    // Permet d'attendre que le caractère \n soit saisi
    while (getchar() != '\n');
    printf("Erreur, veuillez saisir un \033[1mENTIER\033[0m compris entre 1 et %d : ", TAILLE);
  }
}

// Fonction pour vérifier si une chaîne représente un entier
bool estEntier(const char *chaine) {
  // Vérifie chaque caractère dans la chaîne
  for (int i = 0; chaine[i] != '\0'; i++) {
    // Si le caractère n'est pas un chiffre
    if (chaine[i] < '0' || chaine[i] > '9') {
      return false; // Ce n'est pas un entier
    }
  }
  return true; // Tous les caractères sont des chiffres, c'est un entier
}

// Vérifier si une valeur peut être placée dans une case
bool possible(tGrille g, int ligne, int colonne, int valeur) {
  bool possible = false;
  int debutLigne = N * ((ligne - 1) / N);
  int debutColonne = N * ((colonne - 1) / N);

  // Vérification de la ligne
  for (int l = 0; l < TAILLE; l++) {
    if (g[l][colonne - 1] == valeur) {
      possible = true;
      break;
    }
  }

  // Vérification de la colonne
  for (int c = 0; c < TAILLE && !possible; c++) {
    if (g[ligne - 1][c] == valeur) {
      possible = true;
      break;
    }
  }

  // Vérification du carré
  for (int l = debutLigne; l < debutLigne + N && !possible; l++) {
    for (int c = debutColonne; c < debutColonne + N; c++) {
      if (g[l][c] == valeur) {
        possible = true;
        break;
      }
    }
  }
  return !possible;
}

int nbElementsLigne(tGrille g, int lig){
  int nbElt = 0;
  for (int c = 0; c < TAILLE; c++) {
    if (g[lig - 1][c] != 0) {
      nbElt++;
    }
  }
  return nbElt;
}


int nbElementsColonne(tGrille g, int col){
  int nbElt = 0;
  for (int c = 0; c < TAILLE; c++) {
    if (g[c][col-1] != 0) {
      nbElt++;
    }
  }
  return nbElt;
}






// Afficher les règles du jeu
void regles() {
  printf("\033[1m\033[4mVoici les règles du jeu :\033[0m\n");
  printf("1. Le Sudoku se joue sur une grille de 9x9 cases, divisée en neuf "
         "régions de 3x3 cases.\n");
  printf("2. L'objectif est de remplir chaque case de la grille avec un "
         "chiffre, de 1 à 9.\n");
  printf("3. Aucun chiffre ne peut être répété dans : \n\t- une même ligne "
         "horizontale.\n\t- une même colonne verticale.\n\t- une même région "
         "de 3x3 cases.\n");
  printf("4. Utilisez la logique pour placer les chiffres dans les cases "
         "vides "
         "en fonction des chiffres déjà présents.\n");
  printf("5. Si une case peut contenir plusieurs chiffres possibles, "
         "essayez-les un par un pour voir lequel convient en fonction des "
         "autres chiffres dans la grille.\n");
  printf("6. Soyez patient et persévérez pour remplir la grille en respectant "
         "les règles.\n");
  printf("Le Sudoku est un jeu de logique passionnant qui mettra vos "
         "compétences "
         "à l'épreuve. Amusez-vous bien en résolvant les énigmes de Sudoku "
         "!\n");
}

// Afficher un message de bienvenue
void bonjour() {
  printf(" ____              _                  \n");
  printf("|  _ \\            (_)                 \n");
  printf("| |_) | ___  _ __  _  ___  _   _ _ __ \n");
  printf("|  _ < / _ \\| '_ \\| |/ _ \\| | | | '__|\n");
  printf("| |_) | (_) | | | | | (_) | |_| | |   \n");
  printf("|____/ \\___/|_| |_| |\\___/ \\__,_|_|   \n");
  printf("                 _/ |                 \n");
  printf("                |__/                  \n");
}

// Afficher un message de félicitations
void bravo() {
  printf("\n██████  ██████   █████  ██    ██  ██████  \n");
  printf("██   ██ ██   ██ ██   ██ ██    ██ ██    ██ \n");
  printf("██████  ██████  ███████ ██    ██ ██    ██ \n");
  printf("██   ██ ██   ██ ██   ██  ██  ██  ██    ██ \n");
  printf("██████  ██   ██ ██   ██   ████    ██████  \n");
}