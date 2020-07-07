# Évaluation du travail pratique 1

## Identification de l'étudiant

* Nom d'utilisateur: hamel_bourdeau.alexandre
* Nom et prénom: Hamel Bourdeau Alexandre
* Code permanent: HAMA12128907

## Total: 82.5/100

## Fonctionnalité 42.5/50

Nombre de tests échoués: 5 (-7.5 points)

## Qualité du code 14/15

* Bien en général
* Remplacer `fprintf(stdout` par `printf(`
* Éviter les lignes vides sans raison

## Documentation: 12/15

* Les *docstrings* doivent commencer par 2 `*`
* Vous n'étiez pas supposés d'utiliser de fichier `.h`. Et il n'est pas fait
  correctement. Par exemple, les *docstrings* auraient dû être situées dans ce
  fichier et il manque les gardes d'inclusion multiple.
* La balise `@param` doit être répétée pour chaque paramètre
* `@returns` -> `@return`
* Le README est très bien. Ne pas oublier de mettre tous les fichiers entre
  apostrophes inversées. Et attention au vocabulaire: `canvascii.c` n'est pas
  un logiciel, c'est un fichier source C. Limiter la longueur des lignes à 80
  caractères.

## Makefile 4/5

* Manque les dépendances pour la cible "test"

## Utilisation de Git 10/15

* Pourrait être mieux
* Faire plusieurs petits *commits* pour mieux voir l'évolution du projet. Par
  exemple, `Ajout de plusieurs fonctions de dessin`, `Plusieurs modifications
  : importation, docstring`
* Plusieurs messages vagues: `Update README.md` (plusieurs fois), `Changements
  mineurs`, `Modifications mineures`, ...
* Tous les messages devraient commencer par un verbe

## Annexe A: rapport Bats

```text
1..35
ok 1 With no argument, shows help
ok 2 Creating an empty 3x2 canvas
ok 3 Maximum allowed size is 40x80
ok 4 Loading and prints an empty 5x8 canvas
ok 5 Using all pens between 0 and 7
ok 6 Drawing horizontal line on 5x8 canvas with option -h
ok 7 Drawing vertical line on 5x8 canvas with option -v
ok 8 Drawing rectangle on 5x8 canvas with option -r
ok 9 Drawing line on 5x5 canvas with option -l
ok 10 Drawing circle on 5x8 canvas with option -c
ok 11 Combining multiple options
ok 12 Drawing non diagonal segment
ok 13 Drawing large circle
ok 14 Clipping line from (1,1) to (5,8)
ok 15 Clipping circle of radius 3 centered at (3,3)
ok 16 Option -k is recognized
ok 17 Forbidding character # in canvas
not ok 18 Forbidding character : in canvas
# (in test file check.bats, line 160)
#   `[ "${lines[0]}" = "Error: wrong pixel value :" ]' failed
ok 19 Canvas of 41 lines is too high
ok 20 Canvas of 81 columns is too wide
ok 21 Width must be uniform for all lines
ok 22 Unrecognized option -a
not ok 23 Option -n must be provided with values
# (in test file check.bats, line 194)
#   `[ "$status" -eq 6 ]' failed with status 7
not ok 24 Wrong value with option -p
# (in test file check.bats, line 202)
#   `[ "${lines[0]}" = "Error: incorrect value with option -p" ]' failed with status 7
ok 25 Wrong value with option -h
not ok 26 Wrong value with option -v
# (in test file check.bats, line 215)
#   `[ "$status" -eq 7 ]' failed
not ok 27 Wrong syntax with option -n
# (in test file check.bats, line 223)
#   `[ "${lines[0]}" = "Error: incorrect value with option -n" ]' failed with status 7
ok 28 Wrong dimensions with option -n
ok 29 Negative value with option -h is forbidden
ok 30 Negative value with option -v is forbidden
ok 31 Negative positions with option -r are allowed
ok 32 Negative dimensions with option -r are forbidden
ok 33 Negative positions with option -l are allowed
ok 34 Negative positions with option -c are allowed
ok 35 Negative radius with option -c is forbidden
```

