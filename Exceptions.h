#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class EtudiantExistant : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cet etudiant existe deja dans l'etablissement.";
    }
};

class EtudiantIntrouvable : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cet etudiant n a pas ete trouve dans l'etablissement.";
    }
};

class ProfesseurExistant : public std::exception {
public:
    const char* what() const noexcept override {
        return "Ce professeur existe deja dans l'etablissement.";
    }
};

class ProfesseurIntrouvable : public std::exception {
public:
    const char* what() const noexcept override {
        return "Ce professeur n a pas ete trouvé dans l'etablissement.";
    }
};

class MatiereExistant : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cette matiere existe deja dans le systeme.";
    }
};

class MatiereIntrouvable : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cette matiere n'a pas ete trouvée dans le systeme.";
    }
};

class NoteInvalide : public std::exception {
public:
    const char* what() const noexcept override {
        return "Note impossible à ajouter.";
    }
};

class ReleveIntrouvable : public std::exception {
public:
    const char* what() const noexcept override {
        return "Le releve de notes n a pas ete trouve.";
    }
};
class ReleveExistant : public std::exception {
public:
    const char* what() const noexcept override {
        return "Un relevé de notes existe deja pour cet etudiant.";
    }
};

class GroupeExistant : public std::exception {
public:
    const char* what() const noexcept override {
        return "Ce groupe existe deja dans l etablissement.";
    }
};

class GroupeIntrouvable : public std::exception {
public:
    const char* what() const noexcept override {
        return "Ce groupe n'a pas ete trouve dans l etablissement.";
    }
};

#endif
