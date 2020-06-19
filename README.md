---
title: "Travail pratique 1: Dessiner sur un canevas ASCII"
---

## Description

Le logiciel canvascii.c permet de dessiner sur un canvas ASCII. Le canvas peut être initialisé ou lu sur l'entrée standard. Ensuite, il est possible de dessiner de plusieurs façons sur un canvas. Le résultat est affiché ensuite sur la sortie standard.

Pour plus de détails sur les fonctions de dessin, consulter la section [Exécution](#exécution).

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
* Lancer la commande `make`.

Exemple :

```sh
$ make
gcc -o canvascii -std=11 -Wall -Wextra canvascii.c
$ 
```

Le logiciel est compilé. Le fichier `canvascii` devrait apparaître dans le répertoire du projet. Vous pouvez vérifier à l'aide de la commande `ls`.

### Exécution

* Repérer le répertoire du projet
* Accéder au répertoire en utilisant le terminal
* Lancer la commande `./canvascii` suivi des options désirées, séparées par des espaces.

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

* Repérer le répertoire du projet
* Accéder au répertoire en utilisant le terminal
* Lancer la commande `make test`.

Exemple :

```sh
$ make test
bats check.bats
 ✓ With no argument, shows help
 ✓ Creating an empty 3x2 canvas
 ✓ Maximum allowed size is 40x80
 ✓ Loading and prints an empty 5x8 canvas
 ✓ Using all pens between 0 and 7
 ✓ Drawing horizontal line on 5x8 canvas with option -h
 ✓ Drawing vertical line on 5x8 canvas with option -v
 ✓ Drawing rectangle on 5x8 canvas with option -r
 ✓ Drawing line on 5x5 canvas with option -l
 ✓ Drawing circle on 5x8 canvas with option -c
 ✓ Combining multiple options
 ✓ Drawing non diagonal segment
 ✓ Drawing large circle
 ✓ Clipping line from (1,1) to (5,8)
 ✓ Clipping circle of radius 3 centered at (3,3)
 ✓ Option -k is recognized
 ✓ Forbidding character # in canvas
 ✓ Canvas of 41 lines is too high
 ✓ Canvas of 81 columns is too wide
 ✓ Width must be uniform for all lines
 ✓ Unrecognized option -a
 ✗ Option -n must be provided with values
   (in test file check.bats, line 186)
     `[ "$status" -eq 6 ]' failed with status 7
 ✗ Wrong value with option -p
   (in test file check.bats, line 194)
     `[ "${lines[0]}" = "Error: incorrect value with option -p" ]' failed with status 7
 ✓ Wrong value with option -h
 ✗ Wrong value with option -v
   (in test file check.bats, line 207)
     `[ "$status" -eq 7 ]' failed
 ✗ Wrong syntax with option -n
   (in test file check.bats, line 215)
     `[ "${lines[0]}" = "Error: incorrect value with option -n" ]' failed with status 7
 ✓ Wrong dimensions with option -n
 ✓ Negative value with option -h is forbidden
 ✓ Negative value with option -v is forbidden
 ✓ Negative positions with option -r are allowed
 ✓ Negative dimensions with option -r are forbidden
 ✓ Negative positions with option -l are allowed
 ✓ Negative positions with option -c are allowed
 ✓ Negative radius with option -c is forbidden

34 tests, 4 failures

make: *** [Makefile:15: test] Error 1
```

## Dépendances

* [GCC](https://gcc.gnu.org/)
* [Bats](https://github.com/bats-core/bats-core)
* [Pandoc](https://pandoc.org/)

## Références

Ce site m'a fourni un fragment de code qui m'a permis de lire une ligne sur `fgets` et d'ignorer le retour de ligne (`\n`) :

[https://www.siafoo.net/snippet/75/rev/2/embed](https://www.siafoo.net/snippet/75/rev/2/embed)

Ce site m'a aidé pour trouver la façon de bien formatter les codes de couleur ANSI :

[https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#background-colors](https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#background-colors)

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
* [x] Le nombre de tests qui réussissent/échouent avec la `make test` est
  indiqué quelque part dans le fichier `README.md`.
* [x] Les sections incomplètes de ce fichier (`README.md`) ont été complétées.
* [x] L'en-tête du fichier est documentée.
* [x] L'en-tête des déclarations des fonctions est documentée (*docstring*).
* [x] Le programme ne contient pas de valeurs magiques.
