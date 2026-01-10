# Système de Gestion d'Établissement Scolaire

## 📋 Description

Système complet de gestion d'établissement scolaire développé en C++. Ce projet permet de gérer efficacement les étudiants, professeurs, matières, groupes, notes et relevés de notes avec persistance des données en JSON.

## ✨ Fonctionnalités Principales

### 👨‍🎓 Gestion des Étudiants
- Ajout, modification et suppression d'étudiants
- Recherche par ID ou nom
- Affichage de tous les étudiants
- Attribution automatique à des groupes (max 30 étudiants/groupe)
- Validation des IDs (doivent être positifs)

### 👨‍🏫 Gestion des Professeurs
- CRUD complet (Create, Read, Update, Delete)
- Recherche par ID ou nom
- Affectation aux matières
- Validation des données

### 📚 Gestion des Matières
- Ajout avec code unique, nom et coefficient
- Affectation de professeurs
- Modification et suppression
- Gestion des doublons

### 👥 Gestion des Groupes
- Création de groupes par filière
- Limite de 30 étudiants par groupe
- Ajout/suppression d'étudiants
- Modification des informations du groupe

### 📊 Gestion des Notes
- Ajout de notes (DS, TP, Examen)
- Validation automatique (0-20)
- Modification et suppression
- Calcul automatique des moyennes

### 📄 Relevés de Notes
- Génération de relevés par étudiant
- Ajout/modification de notes dans les relevés
- Calcul de moyenne générale
- Affichage formaté

### 📈 Statistiques
- Tri des étudiants par moyenne générale
- Export JSON des classements
- Visualisation des performances

## 🛠️ Technologies Utilisées

- **Langage** : C++17
- **Bibliothèques** :
  - `nlohmann/json` - Manipulation JSON
  - STL (Standard Template Library)
  - Smart pointers (`shared_ptr`)

## 📁 Structure du Projet

```
projet/
├── main.cpp              # Fichier principal
├── Exceptions.h          # Gestion des exceptions personnalisées
├── json.hpp              # Bibliothèque JSON (nlohmann)
├── data.json             # Sauvegarde des données
├── etudiants_par_moyenne.json  # Export des classements
└── groupes.json          # Sauvegarde des groupes
```

## 🚀 Installation et Compilation

### Prérequis
- Compilateur C++17 ou supérieur (g++, clang++, MSVC)
- Bibliothèque nlohmann/json

### Installation de nlohmann/json

**Option 1 : Téléchargement direct**
```bash
wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
```

**Option 2 : Via gestionnaire de paquets**
```bash
# Ubuntu/Debian
sudo apt-get install nlohmann-json3-dev

# macOS
brew install nlohmann-json
```

### Compilation

**Linux/macOS :**
```bash
g++ -std=c++17 main.cpp -o gestion_etablissement
```

**Windows (MinGW) :**
```bash
g++ -std=c++17 main.cpp -o gestion_etablissement.exe
```

**Windows (MSVC) :**
```bash
cl /EHsc /std:c++17 main.cpp
```

## 💻 Utilisation

### Lancement du Programme
```bash
./gestion_etablissement
```

### Menu Principal
```
=== Menu Principal ===
1. Gestion des étudiants
2. Gestion des professeurs
3. Gestion des groupes
4. Gestion des matières
5. Gestion des relevés de notes
7. Gestion des notes
8. Afficher les étudiants triés par moyenne
0. Quitter
```

### Exemple d'Utilisation

**1. Ajouter un étudiant**
- Choisir option 1 → 1
- Entrer nom, prénom, ID, filière, niveau
- Choisir ou créer un groupe

**2. Ajouter une note**
- Choisir option 7 → 1
- Entrer ID note, ID étudiant, valeur (0-20)
- Spécifier code cours et type d'examen

**3. Consulter les moyennes**
- Choisir option 8
- Les étudiants sont triés du meilleur au moins bon
- Export automatique en JSON

## 📊 Gestion des Données

### Persistance
Les données sont automatiquement sauvegardées dans `data.json` à la fermeture du programme et rechargées au démarrage.

### Structure JSON
```json
{
  "etudiants": [...],
  "professeurs": [...],
  "matieres": [...],
  "notes": [...],
  "groupes": [...],
  "releves": [...]
}
```

### Emplacement des Fichiers
Modifiez les chemins dans le code si nécessaire :
```cpp
// Ligne ~1337
const string cheminFichier = "C:/Users/kanbo/Desktop/cpp_projet/data.json";
```

## 🔧 Architecture du Code

### Classes Principales

**Hiérarchie des Personnes**
```
personne (abstract)
├── etudiant
├── Professeur
└── Admnistratif
```

**Gestionnaires**
- `GestionEtudiants` - Gestion des étudiants
- `GestionProf` - Gestion des professeurs
- `GestionGroupe` - Gestion des groupes
- `GestionMatiere` - Gestion des matières
- `GestionNotes` - Gestion des notes
- `GestionReleveDeNotes` - Gestion des relevés

**Classe Centrale**
- `Etablissement` - Orchestre tous les gestionnaires

### Exceptions Personnalisées (Exceptions.h)
- `NoteInvalide` - Note hors de [0, 20]
- `MatiereExistant` - Code matière en doublon
- `MatiereIntrouvable` - Matière non trouvée
- `ReleveExistant` / `ReleveIntrouvable`
- `GroupeExistant`
- `ProfesseurIntrouvable`

## 🎯 Fonctionnalités Avancées

### Validation des Données
- IDs positifs uniquement
- Notes entre 0 et 20
- Codes matières uniques
- Limite de 30 étudiants par groupe

### Recherche
- Par ID (accès direct)
- Par nom (retourne une liste)
- Par filière pour les groupes

### Calculs Automatiques
- Moyenne par étudiant
- Moyenne générale de l'établissement
- Tri par performance

## 🐛 Gestion des Erreurs

Le programme utilise :
- **Exceptions** pour les erreurs métier
- **Try-catch** pour la robustesse
- **Validation** à la saisie

Exemple :
```cpp
try {
    etudiant e("Doe", "John", -5); // ID négatif
} catch (const invalid_argument& e) {
    cout << e.what() << endl; // "ID doit être positif !"
}
```

## 📝 Notes Importantes

1. **Groupes** : Maximum 30 étudiants par groupe
2. **Notes** : Valeurs entre 0 et 20 uniquement
3. **IDs** : Doivent être uniques et positifs
4. **Sauvegarde** : Automatique à la fermeture

## 🔜 Améliorations Possibles

- [ ] Interface graphique (Qt/GTK)
- [ ] Base de données SQL
- [ ] Génération de bulletins PDF
- [ ] Statistiques avancées (graphiques)
- [ ] Gestion des absences
- [ ] Planning des cours
- [ ] Authentification multi-utilisateurs

## 👥 Contributeurs

Projet développé dans le cadre d'un cours de programmation orientée objet en C++.

## 📄 Licence

Ce projet est libre d'utilisation à des fins éducatives.

---

**Bon usage ! 🎓**
