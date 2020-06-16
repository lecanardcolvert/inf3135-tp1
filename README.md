---
title: "Travail pratique 1: Dessiner sur un canevas ASCII"
---

## Format Markdown (supprimer cette section avant la remise)

N'oubliez pas de bien exploiter le format Markdown.

Sauter une ligne pour changer de paragraphe.

Mettre les noms de fichier et bout de code courts entre apostrophes inversés.
Par exemple, si vous parlez du fichier `Makefile`.

Mettre les longs bouts de code dans des blocs de code (triples apostrophes
inversés). Par exemple, vous pouvez donner un exemple de commande comme suit:

```sh
$ make
$ ls -a
```

Utiliser des listes à puces ou des énumérations le plus possible (plus agréable
à lire). Par exemple, pour décrire le contenu du projet:

* `README.md`: ce fichier
* `Makefile`: permet d'automatiser la compilation
* etc.

Bien aérer le contenu du fichier source (`README.md`). Éviter les longues
lignes dans le fichier Markdown (par exemple, limiter à 80) pour une meilleure
lisibilité avec un éditeur de texte.

## Description

Décrivez ici le projet. Commencez d'abord par une description générale, puis
donnez ensuite des détails. 

Le logiciel canvascii.c permet de dessiner sur un canvas ASCII. Le canvas peut être initialisé ou lu sur l'entrée standard. Ensuite, il est possible de dessiner sur ce canvas. Le résultat est affiché ensuite sur la sortie standard.

=== À REMPLIR ===

Ce logiciel a été conçu et développé dans le cadre du cours [INF3135 Construction et maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135).

Le cours est enseigné par Alexandre Blondin Massé, professeur au [département d'informatique](http://info.uqam.ca/), à [l'Université du Québec à Montréal](https://uqam.ca/).

Le sujet du travail se trouve [ici](sujet.md).

## Auteur

Alexandre Hamel Bourdeau (HAMA12128907)

## Fonctionnement

Le logiciel est codé en language C. Il faut le compiler avant de pouvoir l'exécuter.

### Compilation

* Repérer le répertoire du projet
* Accéder à ce répertoire en utilisant le terminal
* Lancer la commande `make`

Exemple :

```sh
$ make
gcc -o canvascii -std=11 -Wall -Wextra canvascii.c
$
```

Le logiciel est compilé. Le fichier `canvascii` devrait apparaître dans le répertoire du projet. Vous pouvez vérifier à l'aide de la commande `ls`.

### Exécution

Dans le répertoire du projet, lancer la commande `./canvascii` suivi des options désirées, séparées par des espaces.

Les options du canvas sont celles-ci :

```text
-n HEIGHT,WIDTH     Crée un nouveau canvas de hauteur HEIGHT et de largeur WIDTH.
                    Doit être spécifié en premier, ou sinon le comportement n'est pas défini.
                    Ignore l'entrée (stdin).

-s                  Affiche le canvas, puis le lelogiciel termine.
-k                  Affiche le canvas en couleur, en replacant les pixels (de 0 à 7) par leur code ANSI correspondant.
```

Les options de dessin :
```text
-p CHAR                     Sélectionne un nouveau crayon. CHAR doit être de valeur 0, 1, 2, 3, 4, 5, 6 ou 7.
                            Le crayon par défaut est 7.

-h ROW                      Dessine une ligne horizontale sur ROW.

-v COL                      Dessine une ligne verticale sur COL.

-r ROW,COL,HEIGHT,WIDTH     Dessine un rectangle de hauteur HEIGHT et de largeur WIDTH.
                            Le coin en haut à gauche se trouve aux coordonnées (ROW, COL).
                            
-l ROW1,COL1,ROW2,COL2      Dessine un segment des coordonnées (ROW1, COL1) jusqu'à (ROW2, COL2) en utilisant l'algorithme de 
                            Bresenham.
                            
-c ROW,COL,RADIUS           Dessine un cercle. Le point du centre se trouve à (ROW, COL) et son rayon est défini par RADIUS.          
```
Lorsque aucune option n'est spécifiée, alors le guide d'utilisation est affiché à l'écran.

Exemple d'un dessin d'une ligne horizontale et d'une ligne verticale de différentes couleurs :

```sh
$ ./canvascii -n 5,5 -p 4 -h 2 -p 2 -v 2
..2..
..2..
44244
..2..
..2..
```

## Tests

Expliquez ici comment lancer la suite de tests automatiques avec la commande
`make test`, ainsi que le résultat que vous obtenez.

## Dépendances

Indiquez les dépendances du projet, avec lien officiel. Il faudrait au moins
mentionner GCC et [Bats](https://github.com/bats-core/bats-core). Utiliser une
liste à puces pour donner la liste des dépendances.

## Références

Indiquez ici les références que vous avez utilisées pour compléter le projet,
avec l'hyperlien vers la référence. Pas besoin de mentionner les diapositives
du cours, mais si vous avez eu recours à un site d'aide, un fragment de code ou
une discussion sur un forum, mentionnez-le.

## État du projet

Indiquez toutes les tâches qui ont été complétés en insérant un `X` entre les
crochets. Si une tâche n'a pas été complétée, expliquez pourquoi (lors de la
remise, vous pouvez supprimer ce paragraphe).

* [x] Le nom du dépôt GitLab est exactement `inf3135-ete2020-tp1` (Pénalité de
  **50%**).
* [x] L'URL du dépôt GitLab est exactement (remplacer `utilisateur` par votre
  nom identifiant GitLab) `https://gitlab.info.uqam.ca/utilisateur/inf3135-ete2020-tp1`
  (Pénalité de **50%**).
* [x] L'utilisateur `blondin_al` a accès au projet en mode *Developer*
  (Pénalité de **50%**).
* [x] Le dépôt GitLab est un *fork* de [ce
  dépôt](https://gitlab.info.uqam.ca/inf3135-ete2020/inf3135-ete2020-tp1)
  (Pénalité de **50%**).
* [x] Le dépôt GitLab est privé (Pénalité de **50%**).
* [x] Le dépôt contient au moins un fichier `.gitignore`.
* [x] Le fichier `Makefile` permet de compiler le projet lorsqu'on entre
  `make`. Il supporte les cibles `html`, `test` et `clean`.
* [ ] Le nombre de tests qui réussissent/échouent avec la `make test` est
  indiqué quelque part dans le fichier `README.md`.
* [ ] Les sections incomplètes de ce fichier (`README.md`) ont été complétées.
* [x] L'en-tête du fichier est documentée.
* [x] L'en-tête des déclarations des fonctions est documentée (*docstring*).
* [ ] Le programme ne contient pas de valeurs magiques.
